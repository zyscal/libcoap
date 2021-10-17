#include "sender.h"
#include <unistd.h>
void simple_sender() {
    while (true)
    {
        coap_pdu_t* pdu;
        // 避免超时应当先看ACK队列

        // 看注册队列
        pdu = GetAndDelRegQueueFront();
        if(pdu != NULL){
            // sleep(5000);
            int mid = coap_send_large(organizer_client_session, pdu);
            continue;
        }

        // 看信息上报non队列
        pthread_mutex_lock(&nonMessageMutex);
        pdu = GetAndDelNONQueueFront();
        pthread_mutex_unlock(&nonMessageMutex);
        if(pdu != NULL) {
            int mid = coap_send_large(organizer_client_session, pdu);
            continue;
        }
    }
}