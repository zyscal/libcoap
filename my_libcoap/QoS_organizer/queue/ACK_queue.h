#ifndef ORGANIZER_ACK_QUEUE
#define ORGANIZER_ACK_QUEUE
#include <coap3/coap.h>
#include "../../QoS_organizer_analyzer_port.h"
#include <stdbool.h>
struct ACKQueue
{
    struct ACKQueue *next;
    struct coap_pdu_t *data;
    uint8_t *payload;
    int Length;
};
typedef struct ACKQueue ACKQueue;

// Qorganizer_DLACKQueue 下行数据的ACK队列，举例：organizer将read方法阻塞转发到anjay，将下行数据的ACK添加到本队列中
ACKQueue *Qorganizer_DLACKQueue;

// 用于向某个ACK队列中插入消息
int InsertACKMsg(coap_pdu_t *pdu, coap_session_t *session, ACKQueue** organizerACKQueueHead, uint8_t *payload, int Length);
// 用于通过mid从某个队列中取消息，并将队列中节点删除
coap_pdu_t* GetAndDelACKQueueFront(coap_bin_const_t token, ACKQueue** organizerACKQueueHead, uint8_t **payload, int *Length);

// 下行数据的ACK排他锁
pthread_mutex_t organizer_DL_ACK_queue_mutex;

#endif