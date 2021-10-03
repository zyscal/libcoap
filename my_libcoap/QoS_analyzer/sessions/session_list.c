#include "session_list.h"

anjay_node* insert_anjay_node(organizer_node* Gateway, 
int InternalID, uint8_t* GlobalID, int LengthOfGlobalID) {
    // organizer 下anjay节点列表为空，则添加在头部
    if(Gateway->anjay_client_node == NULL) {
        anjay_node* anjay_node_head = (anjay_node*) malloc(sizeof(anjay_node));
        anjay_node_head->InternalID = InternalID;
        anjay_node_head->GlobalID = (uint8_t*)malloc(LengthOfGlobalID * sizeof(uint8_t));
        for(int i = 0; i < LengthOfGlobalID; i++){
            anjay_node_head->GlobalID[i] = GlobalID[i];
        }
        anjay_node_head->next = NULL;
        return anjay_node_head;
    }
    // 列表中存在anjay客户端，寻找InternalID是否已经存在
    anjay_node* p = Gateway->anjay_client_node;
    while (p != NULL)
    {
        if(p->InternalID == InternalID){ // InternalID已经存在
            // 更新GlobalID
            p->GlobalID = (uint8_t*)malloc(LengthOfGlobalID * sizeof(uint8_t));
            for(int i = 0; i < LengthOfGlobalID; i++){
                p->GlobalID[i] = GlobalID[i];
            }
            return p;
        } else {
            p = p->next;
        }
    }
    // 边缘下InternalID首次注册，需要建立新的,插入到队列前面
    if(p == NULL) {
        p = Gateway->anjay_client_node;
        anjay_node* new_anjay_node = (anjay_node*)malloc(sizeof(anjay_node));
        new_anjay_node->InternalID = InternalID;
        new_anjay_node->GlobalID = (uint8_t*)malloc(LengthOfGlobalID * sizeof(uint8_t));
        for(int i = 0 ; i < LengthOfGlobalID; i++){
            new_anjay_node->GlobalID[i] = GlobalID[i];
        }
        new_anjay_node->next = p;
        Gateway->anjay_client_node = new_anjay_node;
        return new_anjay_node;
    }
    return NULL; 
}

organizer_node *insert_organizer(coap_session_t *session){
    organizer_node *new_organizer = (organizer_node*) malloc(sizeof(organizer_node));
    new_organizer->session = session;
    new_organizer->anjay_client_node = NULL;
    new_organizer->next = organizer_node_head.next;
    organizer_node_head.next = new_organizer;
    return new_organizer;
}

organizer_node *handle_organizer(coap_session_t *session){
    organizer_node *p = organizer_node_head.next;
    // 表中寻找session
    while(p != NULL){
        if(p->session == session){
            return p;
        } else {
            p = p->next;
        }
    }
    // 没有找到对应的边缘，在开始添加一个新的
    return insert_organizer(session);
}

anjay_node* handle_anjay_node(coap_session_t *session, 
int InternalID, uint8_t *GlobalID, int LengthOfGlobalID){
    // 表中找到对应的organizer
    organizer_node *p = handle_organizer(session);
    // 将anjay加入到对应的organzier_node下
    return insert_anjay_node(p, InternalID, GlobalID, LengthOfGlobalID);
}
void update_GlobalID(anjay_node* client, char *GlobalID, int GlobalIDSize){
    client->GlobalID = (char*) malloc(sizeof(char) * GlobalIDSize);
    client->GlobalIDSize = GlobalIDSize;
    for(int i = 0; i < GlobalIDSize; i++){
        client->GlobalID[i] = GlobalID;
    }
}