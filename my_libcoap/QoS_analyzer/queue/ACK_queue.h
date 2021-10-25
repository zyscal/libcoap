#ifndef ANALYZER_ACK_QUEUE
#define ANALYZER_ACK_QUEUE
#include <coap3/coap.h>
#include "../../QoS_organizer_analyzer_port.h"
#include <stdbool.h>
struct ACKQueue
{
    struct ACKQueue *next;
    coap_session_t* session;
    struct coap_pdu_t *data;
};
typedef struct ACKQueue ACKQueue;
// ULACKQueue 上行数据的ACK队列，举例：analyzer中server接受到reg消息后阻塞等待ULACKQueue中的ACK。
ACKQueue *ULACKQueue;
// DLACKQueue 需要向下行发送到ACK队列
ACKQueue *DLACKQueue;
// ULACKUNHANDLED 用于analyer内部使用，接受Globalid用。
ACKQueue *ULACKUnhandledQueue;
// 用于向某个ACK队列中插入消息
int InsertACKMsg(coap_pdu_t *pdu, coap_session_t *session, ACKQueue** analyzerACKQueueHead);
// 用于通过mid从某个队列中取消息，并将队列中节点删除
coap_pdu_t* GetAndDelACKQueueByMid(coap_mid_t send_mid, ACKQueue** analyzerACKQueueHead);
// 从ACK队列中弹出头部pdu
coap_pdu_t * GetAndDelACKQueueFront(ACKQueue** analyzerACKQueueHead, coap_session_t **session);


// 上行数据的ACK排他锁
pthread_mutex_t analyzer_UL_ACK_queue_mutex;
// 下行数据的ACK排他锁
pthread_mutex_t analyzer_DL_ACK_queue_mutex;
// 下行数据的ACK排他锁
pthread_mutex_t analyzer_UL_ACKUNHSNDLED_queue_mutex;

#endif