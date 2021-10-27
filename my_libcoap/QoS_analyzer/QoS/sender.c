#include "sender.h"



void analyzer_DL_simple_sender() {
    while (1)
    {
        coap_session_t *session = NULL;
        coap_pdu_t *pdu = NULL;
        coap_mid_t mid;
        // 检查下行的ACK
        pthread_mutex_lock(&analyzer_UL_ACK_queue_mutex);
        pdu = GetAndDelACKQueueFront(&ULACKQueue, &session);
        pthread_mutex_unlock(&analyzer_UL_ACK_queue_mutex);
        if(pdu != NULL) {
            mid = coap_send_large(session, pdu);
            continue;
        }

        // 检查是否存在下行请求数据
        pthread_mutex_lock(&analyzer_DL_queue_mutex);
        pdu = GetAndDelDLQueueFront(&DLQueueHead, &session);
        pthread_mutex_unlock(&analyzer_DL_queue_mutex);
        if(pdu != NULL) {
            mid = coap_send_large(session, pdu);
            continue;
        }
    }
}

