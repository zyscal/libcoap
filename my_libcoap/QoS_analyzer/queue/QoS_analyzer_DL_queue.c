#include "QoS_analyzer_DL_queue.h"

int InsertDLMsg(coap_pdu_t *pdu, coap_session_t *session, DLQueue** Head, pthread_mutex_t *mutex)
{
    pthread_mutex_lock(mutex);
    if(*Head == NULL) {
        *Head = (DLQueue *) malloc (sizeof(DLQueue));
        (*Head)->data = pdu;
        (*Head)->session = session;
        (*Head)->next = NULL;
        pthread_mutex_unlock(mutex);
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
    pFront->data = pdu;
    pFront->session = session;
    pFront->next->next = NULL;
    pthread_mutex_unlock(mutex);
    return count + 1; 
}

coap_pdu_t *GetAndDelDLQueueFront(DLQueue** Head,coap_session_t **session_pointer, pthread_mutex_t *mutex)
{
    pthread_mutex_lock(mutex);
    if(*Head == NULL) {
        pthread_mutex_unlock(mutex); 
        return NULL;
    }
    DLQueue *p = *Head;
    *Head = (*Head)->next;
    coap_pdu_t *front_pdu = p->data;
    *session_pointer = p->session;
    free(p);
    pthread_mutex_unlock(mutex); 
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

bool InsertMid(coap_mid_t mid, coap_bin_const_t token) {
    if(midListHead == NULL) {
        // 队列头部为空
        midListHead = (midList *) malloc(sizeof(midList));
        midListHead->next = NULL;
        midListHead->mid = mid;
        midListHead->tokenLength = token.length;
        midListHead->token = (uint8_t *)malloc(sizeof(uint8_t) * token.length);
        for(int i = 0 ; i < token.length; i++) {
            (midListHead->token)[i] = (token.s)[i];
        }
        return true;
    } else {
        midList *p = midListHead;
        while (p != NULL)
        {
            // mid已经存在
            if(p->mid == mid) {
                if(tokenSame(p->token, p->tokenLength, token.s, token.length)) {
                    return false;
                } else { // 更新旧token
                    p->tokenLength = token.length;
                    for(int i = 0 ; i < token.length; i++) {
                        (p->token)[i] = (token.s)[i];
                    }
                }
            } else {
                p = p->next;
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
        newMidList->next = midListHead->next;
        midListHead = newMidList;
        return true;
    }

}

coap_mid_t findMidByToken(coap_bin_const_t token) {
    if(midListHead == NULL) {
        return -1;
    }
    if(tokenSame(midListHead->token, midListHead->tokenLength, token.s, token.length)){
        // 头结点匹配
        midList *p = midListHead;
        midListHead = midListHead->next;
        int mid = p->mid;
        free(p->token);
        free(p);
        return mid;
    }
    midList *pBack = midListHead->next;
    midList *pFront = midListHead;
    while (pBack != NULL)
    {
        if(tokenSame(pBack->token, pBack->tokenLength, token.s, token.length)) {
            int mid = pBack->mid;
            pFront->next = pBack->next;
            free(pBack->token);
            free(pBack);
            return mid;
        }
        /* code */
    }
    return -1;
}