#include "sender.h"
#include <unistd.h>
void simple_sender() {
    while (true)
    {
        // 首先看注册队列
        coap_pdu_t* pdu = GetAndDelRegQueueFront();
        if(pdu != NULL){
            printf("there is sonmething in regQueue\n");
            printf("token length : %d\n", pdu -> token_length);
            for(int i = 0; i < pdu->used_size - (pdu->data - pdu->token); i++){
                printf("%c", pdu->data[i]);
            }
            printf("\n");
            // sleep(5000);
            int mid = coap_send_large(organizer_client_session, pdu);
            printf("organizer send reg mid is : %d\n", mid);
        }
    }
}