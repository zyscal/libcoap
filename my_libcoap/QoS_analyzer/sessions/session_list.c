#include "session_list.h"

anjay_node* insert_anjay_node(organizer_node* Gateway, 
int InternalID, uint8_t* GlobalID, int LengthOfGlobalID, int mid) {
    // organizer 下anjay节点列表为空，则添加在头部
    if(Gateway->anjay_client_node == NULL) {
        // printf("当前organizer为空\n");
        Gateway->anjay_client_node = (anjay_node*) malloc(sizeof(anjay_node));
        Gateway->anjay_client_node->InternalID = InternalID;
        Gateway->anjay_client_node->GlobalIDSize = LengthOfGlobalID;
        Gateway->anjay_client_node->GlobalID = (uint8_t*)malloc(LengthOfGlobalID * sizeof(uint8_t));
        for(int i = 0; i < LengthOfGlobalID; i++){
            Gateway->anjay_client_node->GlobalID[i] = GlobalID[i];
        }
        Gateway->anjay_client_node->MID = mid;
        Gateway->anjay_client_node->next = NULL;
        return Gateway->anjay_client_node;
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
            p->GlobalIDSize = LengthOfGlobalID;
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
        new_anjay_node->GlobalIDSize = LengthOfGlobalID;
        new_anjay_node->GlobalID = (uint8_t*)malloc(LengthOfGlobalID * sizeof(uint8_t));
        for(int i = 0 ; i < LengthOfGlobalID; i++){
            new_anjay_node->GlobalID[i] = GlobalID[i];
        }
        new_anjay_node->MID = mid;
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
            // 找到organizer
            // printf("找到对应的organizer\n");
            return p;
        } else {
            p = p->next;
        }
    }
    // 没有找到对应的边缘，在开始添加一个新的
    // printf("没有找到准备创建新的organizer\n");
    return insert_organizer(session);
}

anjay_node* handle_anjay_node(coap_session_t *session, 
int InternalID, uint8_t *GlobalID, int LengthOfGlobalID, int mid){
    // 表中找到对应的organizer
    organizer_node *p = handle_organizer(session);

    // 将anjay加入到对应的organzier_node下
    return insert_anjay_node(p, InternalID, GlobalID, LengthOfGlobalID, mid);
}
void update_GlobalID(anjay_node* client, char *GlobalID, int GlobalIDSize){
    client->GlobalID = (char*) malloc(sizeof(char) * GlobalIDSize);
    client->GlobalIDSize = GlobalIDSize;
    for(int i = 0; i < GlobalIDSize; i++){
        client->GlobalID[i] = GlobalID;
    }
}
// GlobalIDIsSame 判断当前的anjay节点中的GlobalID是否匹配
bool GlobalIDIsSame(uint8_t *GlobalID, int Length, anjay_node* anjay_client_node) {
    if(Length != anjay_client_node->GlobalIDSize) {
        return false;
    }
    for(int i = 0; i < Length; i++) {
        if(GlobalID[i] != anjay_client_node->GlobalID[i]) {
            return false;
        }
    }
    return true;
}
// OrganizerIsContain 判断当前的organizer中是否存在这个GlobalID,如果存在返回InternalID
int OrganizerIsContain(organizer_node* organizer, uint8_t *GlobalID, int Length, coap_session_t **ansSession) {
    anjay_node* p = organizer->anjay_client_node;
    while (p != NULL)
    {
        if(GlobalIDIsSame(GlobalID, Length, p)) {
            *ansSession = organizer->session;
            return p->InternalID;
        }
        p = p->next;
        /* code */
    }
    return 0;
}

// findSessionAndInternalIDByGlobalID 通过GlobalID找到对应的organizer session
int findSessionAndInternalIDByGlobalID(uint8_t *GlobalID, int Length, coap_session_t **ansSession) {
    organizer_node *p = organizer_node_head.next;
    while (p != NULL)
    {
        int InternalID = OrganizerIsContain(p, GlobalID, Length, ansSession);
        if(*ansSession != NULL) {
            return InternalID;
        }
        p = p->next;
    }
    return 0;
}

anjay_node * findAnjayByGlobalID(int LengthOFGlobalID, uint8_t *GlobalID) {
    organizer_node *p = organizer_node_head.next;
    while (p != NULL)
    {
        anjay_node *q = p->anjay_client_node;
        while (q != NULL)
        {
            if(GlobalIDIsSame(GlobalID, LengthOFGlobalID, q)) {
                return q;
            }
            q = q->next;
        }
        p = p->next;
    }
    return NULL;
}
