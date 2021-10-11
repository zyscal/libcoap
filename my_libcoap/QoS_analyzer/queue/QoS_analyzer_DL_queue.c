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
