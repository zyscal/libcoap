#include "ACK_queue.h"

int InsertACKMsg(coap_pdu_t *pdu, coap_session_t *session, ACKQueue** organizerACKQueueHead, pthread_mutex_t *mutex,
uint8_t *payload, int Length)
{
    // pthread_mutex_lock(mutex);
    coap_pdu_t *new_pdu = coap_pdu_duplicate(pdu, session, pdu->token_length,
    pdu->token, NULL);
    new_pdu->mid = coap_pdu_get_mid(pdu);
    int count = 1;
    if(*organizerACKQueueHead == NULL) {
        // 队列为空
        *organizerACKQueueHead = (ACKQueue *) malloc(sizeof(ACKQueue));
        (*organizerACKQueueHead)->data = new_pdu;
        (*organizerACKQueueHead)->next = NULL;
        (*organizerACKQueueHead)->payload = (uint8_t *) malloc(sizeof(uint8_t) * (Length + 1));
        (*organizerACKQueueHead)->Length = Length;
        for(int i = 0; i < Length; i++) {
            (*organizerACKQueueHead)->payload[i] = payload[i];
        }

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
        p->next->payload = (uint8_t *)malloc(sizeof(uint8_t) * (Length + 1));
        p->next->Length = Length;
        for(int i = 0; i < Length; i++){
            p->next->payload[i] = payload[i];
        }
        p->next->next = NULL;
    }
    // pthread_mutex_unlock(mutex);
    return count;
}

// same 比较两个pdu中的mid是否匹配
bool same(coap_mid_t send_mid, coap_pdu_t *receive){
    return send_mid == coap_pdu_get_mid(receive);
}

// 从ACK队列中找打匹配send的ACK，从队列中删除节点并返回。
coap_pdu_t* GetAndDelACKQueueFront(coap_mid_t send_mid, ACKQueue** organizerACKQueueHead, pthread_mutex_t *mutex, 
uint8_t **payload, int *Length)
{
    // pthread_mutex_lock(mutex);
    if(*organizerACKQueueHead == NULL) {
        // ACK队列为空
        // pthread_mutex_unlock(mutex);
        return NULL;
    }
    if(same(send_mid, (*organizerACKQueueHead)->data)) {

        // ACK队列头部匹配
        coap_pdu_t* p = (*organizerACKQueueHead)->data;
        *payload = (uint8_t *)malloc(sizeof(uint8_t) * ((*organizerACKQueueHead)->Length + 1));
        for(int i = 0 ; i < (*organizerACKQueueHead)->Length; i++) {
            (*payload)[i] = (*organizerACKQueueHead)->payload[i];
        }

        *Length = (*organizerACKQueueHead)->Length;
        free((*organizerACKQueueHead)->payload);
        ACKQueue *head = *organizerACKQueueHead;
        *organizerACKQueueHead = (*organizerACKQueueHead)->next;
        free(head);
        // pthread_mutex_unlock(mutex);
        return p;
        // coap_pdu_t *p = (coap_pdu_t *)malloc(sizeof(coap_pdu_t));
        // return p;
    }
    // 从头节点的下一个开始匹配
    ACKQueue* back = (*organizerACKQueueHead)->next;
    ACKQueue* front = *organizerACKQueueHead;
    while (back != NULL)
    {
        if(same(send_mid, back->data)) { // 匹配到第一个则返回
            front->next = back->next;
            coap_pdu_t* ack = back->data;
            *payload = (uint8_t *)malloc(sizeof(uint8_t) * (back->Length + 1));
            for(int i = 0 ; i < back->Length; i++) {
                *payload[i] = back->payload[i];
            }
            *Length = back->Length;
            free(back);
            // pthread_mutex_unlock(mutex);
            return ack;
        }
        back = back->next;
        front = front->next;
    }
    // pthread_mutex_unlock(mutex);
    return NULL;
}