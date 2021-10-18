#ifndef ANJAY_NODE
#define ANJAY_NODE
#include <coap3/coap.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
struct anjay_node {
    int InternalID;
    int GlobalIDSize;
    int MID;
    char *GlobalID;
    struct anjay_node* next;
};
typedef struct anjay_node anjay_node;
struct organizer_node
{
    coap_session_t *session; // 边缘侧会话
    struct anjay_node* anjay_client_node;
    struct organizer_node* next;
};
typedef struct organizer_node organizer_node;

pthread_mutex_t organizerNodeMutex;

// 第一个边缘节点不存放信息，从next开始。。
organizer_node organizer_node_head;
anjay_node* handle_anjay_node(coap_session_t *session, int InternalID, uint8_t *GlobalID, int LengthOfGlobalID, int mid);
int findSessionAndInternalIDByGlobalID(uint8_t *GlobalID, int Length, coap_session_t **ansSession);
anjay_node * findAnjayByGlobalID(int LengthOFGlobalID, uint8_t *GlobalID);
#endif