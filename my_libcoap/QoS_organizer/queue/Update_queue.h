#ifndef ORGANIZER_UPD_QUEUE
#define ORGANIZER_UPD_QUEUE


#include <stdio.h>
#include <coap3/coap.h>
#include <pthread.h>
struct update_queue
{
    coap_pdu_t *data;
    struct update_queue *next;
};

typedef struct update_queue update_queue;

// updateQueueHead 更新消息的队列头部
update_queue *updateQueueHead;

// 互斥锁
pthread_mutex_t UdpMessageMutex;

// InsertUpdateMessage 向更新队列中添加一条新消息
int InsertUpdateMessage(coap_pdu_t *pdu);

// GetAndDelUpdateQueueFront 从更新队列中取出第一条消息
coap_pdu_t * GetAndDelUpdateQueueFront();

#endif
