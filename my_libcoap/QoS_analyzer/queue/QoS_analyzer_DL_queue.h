#ifndef Q_A_DL_Q
#define Q_A_DL_Q
#include <coap3/coap.h>
#include <stdio.h>
#include <stdbool.h>
#include "../sessions/session_list.h"
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
    // 如果是重传包需要在这个自加1
    int count;
    struct midList *next;
};

struct observeList
{
    uint8_t *token;
    uint8_t tokenLength;
    anjay_node* anjay;
    struct observeList *next;
};

typedef struct DLQueue DLQueue;
typedef struct midList midList;
typedef struct observeList observeList;
// 下行消息队列头部
DLQueue *DLQueueHead;

// midList链表头
midList *midListHead;

// observeListHead 观测队列头
observeList *observeListHead;

// InsertMid 如果当前mid不存在则建立映射关系，返回true；如果存在token不同则表示，之前的数据丢失，重新建立映射关系，返回true
// 否则返回false，表示该数据是重传包
int InsertMid(coap_mid_t mid, coap_bin_const_t token);

// findMidByTokenAndDel 通过token值找到对应的mid值
coap_mid_t findMidByTokenAndDel(coap_bin_const_t token);

// InsertObserveList 当产生新的观测事件时候，需要在观测表中添加新的观测事件
void InsertObserveList(coap_bin_const_t token, uint8_t *GlobalID, int LengthOfGlobalID);

// findAndUpdateMidByToken non消息使用该方法，通过token找到observelist中anjay对象，从anjay中拿出下一个mid,同时将mid更新
coap_mid_t findAndUpdateMidByToken (coap_bin_const_t token);

// findMidByTokenNotDel 通过token值找到对应的mid值,但是不删除
coap_mid_t findMidByTokenNotDel(coap_bin_const_t token);

// InsertDLMsg 向某个队列中插入某个下行pdu, 返回当前队列中数量
int InsertDLMsg(coap_pdu_t *pdu, coap_session_t *session, DLQueue** Head);

// GetAndDelDLQueueFront 从某个队列中取出头部pdu和session
coap_pdu_t *GetAndDelDLQueueFront(DLQueue** Head,coap_session_t **session_pointer);

// 下行队列排他锁
pthread_mutex_t analyzer_DL_queue_mutex;

// mid队列排他锁
pthread_mutex_t analyzer_midList_mutex;

// 观测表的锁
pthread_mutex_t analyzer_observeList_mutex;
#endif

