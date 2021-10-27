#include "QoS_analyzer_DL_queue.h"

int InsertDLMsg(coap_pdu_t *pdu, coap_session_t *session, DLQueue** Head)
{
    if(*Head == NULL) {
        *Head = (DLQueue *) malloc (sizeof(DLQueue));
        (*Head)->data = pdu;
        (*Head)->session = session;
        (*Head)->next = NULL;
        return 1;
    }
    int count = 1;
    DLQueue *pBack = (*Head)->next;
    DLQueue *pFront = *Head;
    while (pBack != NULL)
    {
        count++;
        pBack = pBack->next;
        pFront = pFront->next;
    }
    pFront->next = (DLQueue *) malloc(sizeof(DLQueue));
    pFront->next->data = pdu;
    pFront->next->session = session;
    pFront->next->next = NULL;
    return count + 1; 
}

coap_pdu_t *GetAndDelDLQueueFront(DLQueue** Head,coap_session_t **session_pointer)
{
    if(*Head == NULL) {
        return NULL;
    }
    DLQueue *p = *Head;
    *Head = (*Head)->next;
    coap_pdu_t *front_pdu = p->data;
    *session_pointer = p->session;
    free(p);
    return front_pdu;
}

bool tokenSame(uint8_t* token1, int length1, uint8_t *token2, int length2) {
    if (length1 != length2)
    {
        return false;
        /* code */
    }
    for(int i = 0 ; i < length1; i++) {
        if(token1[i] != token2[i]) {
            return false;
        }
    }
    return true;
}

int InsertMid(coap_mid_t mid, coap_bin_const_t token) {
    int count = 0;
    if(midListHead == NULL) {
        // 队列头部为空
        midListHead = (midList *) malloc(sizeof(midList));
        midListHead->next = NULL;
        midListHead->mid = mid;
        midListHead->tokenLength = token.length;
        midListHead->token = (uint8_t *)malloc(sizeof(uint8_t) * token.length);
        midListHead->count = 1;
        for(int i = 0 ; i < token.length; i++) {
            (midListHead->token)[i] = (token.s)[i];
        }
        return 1;
    } else {
        midList *p = midListHead;
        while (p != NULL)
        {
            // mid已经存在,考虑到重传问题
            if(p->mid == mid) {
                if(tokenSame(p->token, p->tokenLength, token.s, token.length)) {
                    p->count++;
                    printf("重传\n");
                    return -1;
                } else { // 更新旧token
                    p->tokenLength = token.length;
                    free(p->token);
                    p->token = (uint8_t *) malloc (sizeof(uint8_t) * token.length);
                    for(int i = 0 ; i < token.length; i++) {
                        (p->token)[i] = (token.s)[i];
                    }
                    p->count = 1;
                    printf("token 更新\n");
                    return -1;
                }
            } else {
                p = p->next;
                count++;
            }
        }
        // mid不存在，在头部insert一个就好了
        midList *newMidList = (midList *)malloc(sizeof(midList));
        newMidList->mid = mid;
        newMidList->tokenLength = token.length;
        newMidList->token = (uint8_t *) malloc(sizeof(uint8_t) * token.length);
        for(int i = 0 ; i < token.length; i++) {
            (newMidList->token)[i] = (token.s)[i];
        }
        newMidList->count = 1;
        newMidList->next = midListHead;
        midListHead = newMidList;
        return count + 1;
    }

}

coap_mid_t findMidByTokenNotDel(coap_bin_const_t token) {
    if(midListHead == NULL) {
        return -1;
    }
    midList *p = midListHead;
    while (p != NULL)
    {
        if(tokenSame(p->token, p->tokenLength, token.s, token.length)) {
            p->mid;
            return p->mid;
        }
        p = p->next;
    }
    // 没有找到
    return -1;
    
}


coap_mid_t findMidByTokenAndDel(coap_bin_const_t token) {
    if(midListHead == NULL) {
        printf("midList为空\n");
        return -1;
    }
    if(tokenSame(midListHead->token, midListHead->tokenLength, token.s, token.length)){
        // 头结点匹配
        if(midListHead->count == 1) {
            midList *p = midListHead;
            midListHead = midListHead->next;
            int mid = p->mid;
            free(p->token);
            free(p);
            return mid;
        } else {
            midListHead->count--;
            return midListHead->mid;
        }
    }
    midList *pBack = midListHead->next;
    midList *pFront = midListHead;
    while (pBack != NULL)
    {
        if(tokenSame(pBack->token, pBack->tokenLength, token.s, token.length)) {
            if(pBack->count == 1) {
                int mid = pBack->mid;
                pFront->next = pBack->next;
                free(pBack->token);
                free(pBack);
                return mid;
            }
            else
            {
                pBack->count--;
                return pBack->mid;
            }
        }
        pBack = pBack->next;
        pFront = pFront->next;
    }
    printf("midList没有找到匹配\n");
    return -1;
}

void InsertObserveList(coap_bin_const_t token, uint8_t *GlobalID, int LengthOfGlobalID) {
    observeList *p = (observeList *) malloc (sizeof(observeList));
    p->tokenLength = token.length;
    p->token = (uint8_t *) malloc (sizeof(uint8_t) * token.length);
    p->next = NULL;
    for(int i = 0; i < token.length; i++) {
        (p->token)[i] = (token.s)[i];
    }
    anjay_node *anjay = findAnjayByGlobalID(LengthOfGlobalID, GlobalID);
    if(anjay != NULL) {
        p->anjay = anjay;
    } else {
        printf("failed to find anjay by globalID\n");
    }

    if (observeListHead == NULL)
    {
        observeListHead = p;
        return;
    }
    observeList *q = observeListHead;
    while (q->next != NULL)
    {
        // 判断当前token
        q = q->next;
    }
    q->next = p;
}

coap_mid_t findAndUpdateMidByToken (coap_bin_const_t token) {
    if (observeListHead == NULL)
    {
        return -1;
    }
    observeList *p = observeListHead;
    while (p != NULL)
    {
        if(tokenSame(token.s, token.length, p->token, p->tokenLength)) {
            // 匹配后从anjay中拿下一个mid并更新
            p->anjay->MID++;
            return p->anjay->MID;
        }
        p = p->next;
    }
    return -1;
}