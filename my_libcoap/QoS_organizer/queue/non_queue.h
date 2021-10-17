#ifndef ANALYZER_NON_QUEUE
#define ANALYZER_NON_QUEUE
#include <coap3/coap.h>
#include <pthread.h>
#include "../../QoS_organizer_analyzer_port.h"
struct nonQueue
{
    coap_pdu_t *data;
    struct nonQueue *next;
};
typedef struct nonQueue nonQueue;

nonQueue *nonMessageHead;

pthread_mutex_t nonMessageMutex;
// InsertNONMsg 将一条non消息加入队列中
coap_pdu_t *InsertNONMsg(coap_pdu_t *non, coap_session_t *session);
// GetAndDelNONQueueFront 从non队列中头部取出一条消息
coap_pdu_t* GetAndDelNONQueueFront();
#endif