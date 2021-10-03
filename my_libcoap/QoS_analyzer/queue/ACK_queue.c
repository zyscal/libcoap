#include "ACK_queue.h"

coap_pdu_t* InsertACKMsg(coap_pdu_t *pdu, coap_session_t *session){
    pthread_mutex_lock(&analyzer_ack_queue_mutex);
    coap_pdu_t *new_pdu = coap_pdu_duplicate(pdu, session, pdu->token_length,
    pdu->token, NULL);
    new_pdu->mid = coap_pdu_get_mid(pdu);
    if(analyzer_ack_queue_head == NULL) {
        // 队列为空
        analyzer_ack_queue_head = (ACKQueue *) malloc(sizeof(ACKQueue));
        analyzer_ack_queue_head->data = new_pdu;
        analyzer_ack_queue_head->next = NULL;
    } else {
        // ACK队列中已经存在多个数据，应当插入在队列尾端，取消息从队列头取
        ACKQueue* p = analyzer_ack_queue_head;
        while (p->next != NULL)
        {
            p = p->next;
            /* code */
        }
        p->next = (ACKQueue *) malloc(sizeof(ACKQueue));
        p->next->data = new_pdu;
        p->next->next = NULL;
    }
    pthread_mutex_unlock(&analyzer_ack_queue_mutex);
    return new_pdu;
}

// same 比较两个pdu中的mid是否匹配
bool same(coap_mid_t send_mid, coap_pdu_t *receive){
    return send_mid == coap_pdu_get_mid(receive);
}

// 从ACK队列中找打匹配send的ACK，从队列中删除节点并返回。
coap_pdu_t* GetAndDelACKQueueFront(coap_mid_t send_mid){
    pthread_mutex_lock(&analyzer_ack_queue_mutex);
    if(analyzer_ack_queue_head == NULL) {
        // ACK队列为空
        pthread_mutex_unlock(&analyzer_ack_queue_mutex);
        return NULL;
    }
    if(same(send_mid, analyzer_ack_queue_head->data)) {
        // ACK队列头部匹配
        coap_pdu_t* p = analyzer_ack_queue_head->data;
        analyzer_ack_queue_head = analyzer_ack_queue_head->next;
        pthread_mutex_unlock(&analyzer_ack_queue_mutex);
        return p;
    }
    // 从头节点的下一个开始匹配
    ACKQueue* back = analyzer_ack_queue_head->next;
    ACKQueue* front = analyzer_ack_queue_head;
    while (back != NULL)
    {
        if(same(send_mid, back->data)) { // 匹配到第一个则返回
            front->next = back->next;
            coap_pdu_t* ack = back->data;
            free(back);
            pthread_mutex_unlock(&analyzer_ack_queue_mutex);
            return ack;
        }
        back = back->next;
        front = front->next;
    }
    pthread_mutex_unlock(&analyzer_ack_queue_mutex);
    return NULL;
}