#include "ACK_queue.h"

coap_pdu_t* InsertACKMsg(coap_pdu_t *pdu, coap_session_t *session, ACKQueue** analyzerACKQueueHead){
    pthread_mutex_lock(&analyzer_UL_ACK_queue_mutex);
    coap_pdu_t *new_pdu = coap_pdu_duplicate(pdu, session, pdu->token_length,
    pdu->token, NULL);
    new_pdu->mid = coap_pdu_get_mid(pdu);
    if(*analyzerACKQueueHead == NULL) {
        // 队列为空
        *analyzerACKQueueHead = (ACKQueue *) malloc(sizeof(ACKQueue));
        (*analyzerACKQueueHead)->data = new_pdu;
        (*analyzerACKQueueHead)->next = NULL;
    } else {
        // ACK队列中已经存在多个数据，应当插入在队列尾端，取消息从队列头取
        ACKQueue* p = *analyzerACKQueueHead;
        while (p->next != NULL)
        {
            p = p->next;
            /* code */
        }
        p->next = (ACKQueue *) malloc(sizeof(ACKQueue));
        p->next->data = new_pdu;
        p->next->next = NULL;
    }
    pthread_mutex_unlock(&analyzer_UL_ACK_queue_mutex);
    return new_pdu;
}

// same 比较两个pdu中的mid是否匹配
bool same(coap_mid_t send_mid, coap_pdu_t *receive){
    return send_mid == coap_pdu_get_mid(receive);
}

// 从ACK队列中找打匹配send的ACK，从队列中删除节点并返回。
coap_pdu_t* GetAndDelACKQueueFront(coap_mid_t send_mid, ACKQueue** analyzerACKQueueHead){
    pthread_mutex_lock(&analyzer_UL_ACK_queue_mutex);
    if(*analyzerACKQueueHead == NULL) {
        // ACK队列为空
        pthread_mutex_unlock(&analyzer_UL_ACK_queue_mutex);
        return NULL;
    }
    if(same(send_mid, (*analyzerACKQueueHead)->data)) {
        // ACK队列头部匹配
        coap_pdu_t* p = (*analyzerACKQueueHead)->data;
        *analyzerACKQueueHead = (*analyzerACKQueueHead)->next;
        pthread_mutex_unlock(&analyzer_UL_ACK_queue_mutex);
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
            pthread_mutex_unlock(&analyzer_UL_ACK_queue_mutex);
            return ack;
        }
        back = back->next;
        front = front->next;
    }
    pthread_mutex_unlock(&analyzer_UL_ACK_queue_mutex);
    return NULL;
}