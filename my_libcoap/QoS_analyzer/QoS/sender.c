#include "sender.h"



void analyzer_DL_simple_sender() {
    while (1)
    {
        coap_session_t *session;
        pthread_mutex_lock(&analyzer_DL_queue_mutex);
        coap_pdu_t *pdu = GetAndDelDLQueueFront(&DLQueueHead, &session);
        pthread_mutex_unlock(&analyzer_DL_queue_mutex);

        if(pdu == NULL) {
            continue;
        }
        coap_mid_t mid_con = coap_send_large(session, pdu);
    }
}

