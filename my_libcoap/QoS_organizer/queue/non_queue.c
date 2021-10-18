#include "non_queue.h"
coap_pdu_t *InsertNONMsg(coap_pdu_t *non, coap_session_t *session, uint8_t *payload, int dataLength) {
    coap_pdu_t  *newNon = coap_pdu_duplicate(non, session, non->token_length, non->token, NULL);
    printf("before set mid : %d\n",coap_pdu_get_mid(non));
    coap_pdu_set_mid(newNon, coap_pdu_get_mid(non));
    if(nonMessageHead == NULL) {
        // 队头没有消息
        nonMessageHead = (nonQueue *) malloc(sizeof(nonQueue));
        nonMessageHead->next = NULL;
        nonMessageHead->data = newNon;
        nonMessageHead->Length = dataLength;
        nonMessageHead->payload = (uint8_t *) malloc (sizeof(uint8_t) * dataLength);
        for(int i = 0; i < dataLength; i++) {
            (nonMessageHead->payload)[i] = payload[i];
        }
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
        pFront->next->Length = dataLength;
        pFront->next->payload = (uint8_t *) malloc (sizeof(uint8_t) * dataLength);
        for(int i = 0; i < dataLength; i++) {
            (pFront->next->payload)[i] = payload[i];
        }
    }
    return newNon;
}

coap_pdu_t* GetAndDelNONQueueFront(uint8_t **payload, int *dataLength) {
    if(nonMessageHead == NULL) {
        return NULL;
    }
    nonQueue *head = nonMessageHead;
    nonMessageHead = nonMessageHead->next;
    *dataLength = head->Length;
    *payload = (uint8_t *) malloc (sizeof(uint8_t) * head->Length);
    for(int i = 0; i < head->Length; i++) {
       (*payload)[i] = (head->payload)[i]; 
    }
    coap_pdu_t *pdu = head->data;
    free(head->payload);
    free(head);
    return pdu;
}