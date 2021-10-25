#include "ACK_queue.h"

int InsertACKMsg(coap_pdu_t *pdu, coap_session_t *session, ACKQueue** analyzerACKQueueHead)
{
    coap_pdu_t *new_pdu = coap_pdu_duplicate(pdu, session, pdu->token_length,
    pdu->token, NULL);
    coap_pdu_set_mid(new_pdu,coap_pdu_get_mid(pdu)) ;
    int count = 1;
    if((*analyzerACKQueueHead) == NULL) {
        // 队列为空
        *analyzerACKQueueHead = (ACKQueue *) malloc(sizeof(ACKQueue));
        (*analyzerACKQueueHead)->data = new_pdu;
        (*analyzerACKQueueHead)->next = NULL;
        (*analyzerACKQueueHead)->session = session;
        return 1;
    } else {
        // ACK队列中已经存在多个数据，应当插入在队列尾端，取消息从队列头取
        ACKQueue* p = *analyzerACKQueueHead;
        while (p->next != NULL)
        {
            p = p->next;
            count++;
            /* code */
        }
        p->next = (ACKQueue *) malloc(sizeof(ACKQueue));
        p->next->data = new_pdu;
        p->next->next = NULL;
        p->next->session = session;
    }
    return count + 1;
}

// same 比较两个pdu中的mid是否匹配
bool same(coap_mid_t send_mid, coap_pdu_t *receive){
    return send_mid == coap_pdu_get_mid(receive);
}

// 从ACK队列中找打匹配send的ACK，从队列中删除节点并返回。
coap_pdu_t* GetAndDelACKQueueByMid(coap_mid_t send_mid, ACKQueue** analyzerACKQueueHead)
{
    if(*analyzerACKQueueHead == NULL) {
        // ACK队列为空
        return NULL;
    }
    if(same(send_mid, (*analyzerACKQueueHead)->data)) {
        // ACK队列头部匹配
        coap_pdu_t* p = (*analyzerACKQueueHead)->data;
        *analyzerACKQueueHead = (*analyzerACKQueueHead)->next;
        return p;
    }
    // 从头节点的下一个开始匹配
    ACKQueue* back = (*analyzerACKQueueHead)->next;
    ACKQueue* front = *analyzerACKQueueHead;
    while (back != NULL)
    {
        if(same(send_mid, back->data)) { // 匹配到第一个则返回
            front->next = back->next;
            coap_pdu_t* ack = back->data;
            free(back);
            return ack;
        }
        back = back->next;
        front = front->next;
    }
    return NULL;
}

coap_pdu_t * GetAndDelACKQueueFront(ACKQueue** analyzerACKQueueHead, coap_session_t **session) {
    if (*analyzerACKQueueHead == NULL) {
        return NULL;
    }
    ACKQueue *p = *analyzerACKQueueHead;
    *analyzerACKQueueHead = (*analyzerACKQueueHead)->next;
    coap_pdu_t *message = p->data;
    *session = p->session;
    free(p);
    return message;
}