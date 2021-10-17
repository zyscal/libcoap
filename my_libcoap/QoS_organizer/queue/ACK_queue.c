#include "ACK_queue.h"

int InsertACKMsg(coap_pdu_t *pdu, coap_session_t *session, ACKQueue** organizerACKQueueHead, uint8_t *payload, int Length)
{
    coap_pdu_t *new_pdu = coap_pdu_duplicate(pdu, session, pdu->token_length,
    pdu->token, NULL);
    new_pdu->mid = coap_pdu_get_mid(pdu);
    int count = 1;
    if(*organizerACKQueueHead == NULL) {
        // 队列为空
        *organizerACKQueueHead = (ACKQueue *) malloc(sizeof(ACKQueue));
        (*organizerACKQueueHead)->data = new_pdu;
        (*organizerACKQueueHead)->next = NULL;
        (*organizerACKQueueHead)->payload = (uint8_t *) malloc(sizeof(uint8_t) * (Length));
        (*organizerACKQueueHead)->Length = Length;
        for(int i = 0; i < Length; i++) {
            ((*organizerACKQueueHead)->payload)[i] = payload[i];
        }
        return 1;
    } else {
        // ACK队列中已经存在多个数据，应当插入在队列尾端，取消息从队列头取
        ACKQueue* p = *organizerACKQueueHead;
        while (p->next != NULL)
        {
            count++;
            p = p->next;
            /* code */
        }
        p->next = (ACKQueue *) malloc(sizeof(ACKQueue));
        p->next->data = new_pdu;
        p->next->payload = (uint8_t *)malloc(sizeof(uint8_t) * (Length));
        p->next->Length = Length;
        for(int i = 0; i < Length; i++){
            (p->next->payload)[i] = payload[i];
        }
        p->next->next = NULL;
    }
    return count + 1;
}



// sameToken 比较两个token是否相同
bool sameToken(coap_bin_const_t token1, coap_bin_const_t token2) {
    if(token1.length != token2.length) {
        return false;
    }
    for(int i = 0; i < token1.length; i++) {
        if((token1.s)[i] != (token2.s)[i]) {
            return false;
        }
    }
    return true;
}

bool cmpTokenByPdu(coap_bin_const_t token, coap_pdu_t *pdu) {
    return sameToken(token, coap_pdu_get_token(pdu));
}

// sameMID 比较两个pdu中的mid是否匹配
bool sameMID(coap_mid_t send_mid, coap_pdu_t *receive){
    return send_mid == coap_pdu_get_mid(receive);
}

// 从ACK队列中找打匹配send的ACK，从队列中删除节点并返回。
coap_pdu_t* GetAndDelACKQueueFront(coap_bin_const_t token, ACKQueue** organizerACKQueueHead, uint8_t **payload, int *Length)
{
    if(*organizerACKQueueHead == NULL) {
        // ACK队列为空
        return NULL;
    }
    if(cmpTokenByPdu(token, (*organizerACKQueueHead)->data)) {
        // ACK队列头部匹配
        coap_pdu_t* p = (*organizerACKQueueHead)->data;
        *payload = (uint8_t *)malloc(sizeof(uint8_t) * ((*organizerACKQueueHead)->Length));
        for(int i = 0 ; i < (*organizerACKQueueHead)->Length; i++) {
            (*payload)[i] = (*organizerACKQueueHead)->payload[i];
        }

        *Length = (*organizerACKQueueHead)->Length;
        free((*organizerACKQueueHead)->payload);
        ACKQueue *head = *organizerACKQueueHead;
        *organizerACKQueueHead = (*organizerACKQueueHead)->next;
        free(head);
        return p;
    }
    // 从头节点的下一个开始匹配
    ACKQueue* back = (*organizerACKQueueHead)->next;
    ACKQueue* front = *organizerACKQueueHead;
    while (back != NULL)
    {
        if(cmpTokenByPdu(token, back->data)) { // 匹配到第一个则返回
            front->next = back->next;
            coap_pdu_t* ack = back->data;
            *payload = (uint8_t *)malloc(sizeof(uint8_t) * (back->Length));
            for(int i = 0 ; i < back->Length; i++) {
                *payload[i] = back->payload[i];
            }
            *Length = back->Length;
            free(back);
            return ack;
        }
        back = back->next;
        front = front->next;
    }
    return NULL;
}