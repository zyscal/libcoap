#include "non_queue.h"
coap_pdu_t *InsertNONMsg(coap_pdu_t *non, coap_session_t *session) {
    coap_pdu_t  *newNon = coap_pdu_duplicate(non, session, non->token_length, non->token, NULL);
    coap_pdu_set_mid(newNon, coap_pdu_get_mid(non));
    if(nonMessageHead == NULL) {
        // 队头没有消息
        nonMessageHead = (nonQueue *) malloc(sizeof(nonQueue));
        nonMessageHead->next = NULL;
        nonMessageHead->data = newNon;
    } else {
        // 将消息插入到队列末端
        nonQueue *pFront = nonMessageHead;
        nonQueue *pBack = pFront->next;
        while (pBack != NULL)
        {
            pBack = pBack->next;
            pFront = pFront->next;
        }
        pFront->next = (nonQueue *) malloc (sizeof(nonQueue));
        pFront->next->next = NULL;
        pFront->next->data = newNon;
    }
    return newNon;
}

coap_pdu_t* GetAndDelNONQueueFront() {
    if(nonMessageHead == NULL) {
        return false;
    }
    coap_pdu_t *head = nonMessageHead;
    nonMessageHead->next = nonMessageHead->next->next;
    return head;
}