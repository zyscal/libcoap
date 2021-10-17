#ifndef Q_A_DL_Q
#define Q_A_DL_Q
#include <coap3/coap.h>
#include <stdio.h>
#include <stdbool.h>
struct DLQueue
{
    struct DLQueue *next;
    coap_pdu_t *data;
    coap_session_t *session;
    uint8_t level;
};
// midList 基于tcp的coap会丢失mid信息，需要通过token寻找对应的mid值
struct midList
{
    coap_mid_t mid;
    uint8_t *token;
    int tokenLength;
    struct midList *next;
};

typedef struct DLQueue DLQueue;
typedef struct midList midList;
// 下行消息队列头部
DLQueue *DLQueueHead;

// midList链表头
midList *midListHead;

// InsertMid 如果当前mid不存在则建立映射关系，返回true；如果存在token不同则表示，之前的数据丢失，重新建立映射关系，返回true
// 否则返回false，表示该数据是重传包
bool InsertMid(coap_mid_t mid, coap_bin_const_t token);

// findMidByToken 通过token值找到对应的mid值
coap_mid_t findMidByToken(coap_bin_const_t token);

// InsertDLMsg 向某个队列中插入某个下行pdu, 返回当前队列中数量
int InsertDLMsg(coap_pdu_t *pdu, coap_session_t *session, DLQueue** Head, pthread_mutex_t *mutex);

// GetAndDelDLQueueFront 从某个队列中取出头部pdu和session
coap_pdu_t *GetAndDelDLQueueFront(DLQueue** Head,coap_session_t **session_pointer, pthread_mutex_t *mutex);

// 下行队列排他锁
pthread_mutex_t analyzer_DL_queue_mutex;

// mid队列排他锁
pthread_mutex_t analyzer_midList_mutex;

#endif

