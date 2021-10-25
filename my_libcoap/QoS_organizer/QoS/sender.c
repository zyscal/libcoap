#include "sender.h"
#include <unistd.h>
void simple_sender() {
    while (true)
    {
        coap_pdu_t* pdu;
        uint8_t *payload;
        int dataLength;
        // 避免超时应当先看ACK队列
        pthread_mutex_lock(&organizer_DL_ACK_queue_mutex);
        pdu = GetAndDelACKQueueFront(&Qorganizer_DLACKQueue, &payload, &dataLength);
        pthread_mutex_unlock(&organizer_DL_ACK_queue_mutex);
        if(pdu != NULL) {
            printf("ack queue has something \n");
            coap_add_data(pdu, dataLength, payload);
            int mid = coap_send_large(organizer_client_session, pdu);
            free(payload);
            continue;
        }
        // 看注册队列
        pdu = GetAndDelRegQueueFront();
        if(pdu != NULL){
            // sleep(5000);
            int mid = coap_send_large(organizer_client_session, pdu);
            continue;
        }

        // 更新消息
        pthread_mutex_lock(&UdpMessageMutex);
        pdu = GetAndDelUpdateQueueFront();
        pthread_mutex_unlock(&UdpMessageMutex);
        if(pdu != NULL) {
            int mid = coap_send_large(organizer_client_session, pdu);
            continue;
        }

        // 看信息上报non队列
        pthread_mutex_lock(&nonMessageMutex);
        pdu = GetAndDelNONQueueFront(&payload, &dataLength);
        pthread_mutex_unlock(&nonMessageMutex);
        if(pdu != NULL) {
            printf("non queue has something\n");
            coap_add_data(pdu, dataLength, payload);
            int mid = coap_send_large(organizer_client_session, pdu);
            free(payload);
            continue;
        }

    }
}