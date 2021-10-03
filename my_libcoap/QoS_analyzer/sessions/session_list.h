#ifndef ANJAY_NODE
#define ANJAY_NODE
#include <coap3/coap.h>
struct anjay_node {
    int InternalID;
    int GlobalIDSize;
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

// 第一个边缘节点不存放信息，从next开始。。
organizer_node organizer_node_head;
organizer_node *handle_organizer(coap_session_t *session);
organizer_node *insert_organizer(coap_session_t *session);
anjay_node* handle_anjay_node(coap_session_t *session, 
int InternalID, uint8_t *GlobalID, int LengthOfGlobalID);
void update_GlobalID(anjay_node* client, char *GlobalID, int GlobalIDSize);
#endif