#include "sender.h"
#include <unistd.h>
void simple_sender() {
    while (true)
    {
        // 首先看注册队列
        coap_pdu_t* pdu = GetAndDelRegQueueFront();
        if(pdu != NULL){
            // sleep(5000);
            int mid = coap_send_large(organizer_client_session, pdu);
        }
    }
}