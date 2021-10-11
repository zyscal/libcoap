#ifndef Q_A_DL_Q
#define Q_A_DL_Q
#include <coap3/coap.h>
#include <stdio.h>
struct DLQueue
{
    struct DLQueue *next;
    coap_pdu_t *data;
    coap_session_t *session;
    uint8_t level;
    /* data */
};

typedef struct DLQueue DLQueue;

// 下行消息队列头部
DLQueue *DLQueueHead;

// InsertDLMsg 向某个队列中插入某个下行pdu, 返回当前队列中数量
int InsertDLMsg(coap_pdu_t *pdu, coap_session_t *session, DLQueue** Head, pthread_mutex_t *mutex);

// GetAndDelDLQueueFront 从某个队列中取出头部pdu和session
coap_pdu_t *GetAndDelDLQueueFront(DLQueue** Head,coap_session_t **session_pointer, pthread_mutex_t *mutex);

// 下行队列排他锁
pthread_mutex_t analyzer_DL_queue_mutex;

#endif

