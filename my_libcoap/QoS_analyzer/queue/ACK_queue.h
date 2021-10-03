#ifndef ANALYZER_ACK_QUEUE
#define ANALYZER_ACK_QUEUE

#include <coap3/coap.h>
#include "../../QoS_organizer_analyzer_port.h"
#include <stdbool.h>
struct ACKQueue
{
    struct ACKQueue *next;
    struct coap_pdu_t *data;
};
typedef struct ACKQueue ACKQueue;
ACKQueue *analyzer_ack_queue_head;
coap_pdu_t* InsertACKMsg(coap_pdu_t *pdu, coap_session_t *session);
coap_pdu_t* GetAndDelACKQueueFront(coap_mid_t send_mid);
pthread_mutex_t analyzer_ack_queue_mutex;



// ACKQueue *organizer_client_head;
#endif