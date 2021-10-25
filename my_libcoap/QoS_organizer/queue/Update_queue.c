#include "Update_queue.h"

int InsertUpdateMessage(coap_pdu_t *pdu) {
    if(updateQueueHead == NULL) {
        updateQueueHead = (update_queue *) malloc (sizeof(update_queue));
        updateQueueHead->next = NULL;
        updateQueueHead->data = pdu;
        return 1;
    }
    int count = 1;
    update_queue *p = updateQueueHead;
    while (p->next != NULL)
    {
        p = p->next;
        count++;
    }
    p->next = (update_queue *) malloc (sizeof(update_queue));
    p->next->data = pdu;
    p->next->next = NULL;
    return count + 1;
}
coap_pdu_t * GetAndDelUpdateQueueFront() {
    if(updateQueueHead == NULL) {
        return NULL;
    }
    update_queue *p = updateQueueHead;
    updateQueueHead = updateQueueHead->next;
    coap_pdu_t *UpdateMes = p->data;
    free(p);
    return UpdateMes;
}

