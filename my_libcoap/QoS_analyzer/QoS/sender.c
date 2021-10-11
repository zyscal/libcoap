#include "sender.h"



void analyzer_DL_simple_sender() {
    while (1)
    {
        coap_session_t *session;
        coap_pdu_t *pdu = GetAndDelDLQueueFront(&DLQueueHead,
         &session, &analyzer_DL_queue_mutex);
        if(pdu == NULL) {
            continue;
        }
        coap_mid_t mid_con = coap_send_large(session, pdu);
    }
}

