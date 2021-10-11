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
// ULACKQueue 上行数据的ACK队列，举例：analyzer中server接受到reg消息后阻塞等待ULACKQueue中的ACK。
ACKQueue *ULACKQueue;
// DLACKQueue 下行数据的ACK队列，举例：analyzer中client接收下行的read后阻塞等待DLACKQueue中的ACK。
ACKQueue *DLACKQueue;
// 用于向某个ACK队列中插入消息
coap_pdu_t* InsertACKMsg(coap_pdu_t *pdu, coap_session_t *session, ACKQueue** analyzerACKQueueHead, pthread_mutex_t *mutex);
// 用于通过mid从某个队列中取消息，并将队列中节点删除
coap_pdu_t* GetAndDelACKQueueFront(coap_mid_t send_mid, ACKQueue** analyzerACKQueueHead, pthread_mutex_t *mutex);
// 上行数据的ACK排他锁
pthread_mutex_t analyzer_UL_ACK_queue_mutex;
// 下行数据的ACK排他锁
pthread_mutex_t analyzer_DL_ACK_queue_mutex;

#endif