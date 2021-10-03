#include <coap3/coap.h>
#include <coap3/coap_pdu_internal.h>
#include <stdio.h>
#include "QoS_organizer_priority_queue.h"

bool IsRegQueueEmpty(){
  if (head == NULL) {
    return true;
  }
  return false;
}

coap_pdu_t* GetAndDelRegQueueFront() {
  pthread_mutex_lock(&organizer_reg_queue_mutex); 
  if(head == NULL) {
    pthread_mutex_unlock(&organizer_reg_queue_mutex); 
    return NULL;
  }
  struct regQueue* FrontRegMsg = head;
  head = head->next;
  coap_pdu_t* front_pdu = FrontRegMsg->msg;
  free(FrontRegMsg);
  pthread_mutex_unlock(&organizer_reg_queue_mutex); 
  return front_pdu;
}

uint8_t InsertRegMsg(coap_pdu_t* NewPdu) {
  pthread_mutex_lock(&organizer_reg_queue_mutex); 
  if (head == NULL) {
    head = (struct regQueue*)malloc(sizeof(struct regQueue));
    head->msg = NewPdu;
    head->next = NULL;
    pthread_mutex_unlock(&organizer_reg_queue_mutex); 

    return 1;
  }
  int count = 2;
  struct regQueue* pBack = head->next;
  struct regQueue* pFront = head;
  while(pBack != NULL){
    count++;
    pBack = pBack->next;
    pFront = pFront->next;
  }
  pFront->next = (struct regQueue*) malloc (sizeof(struct regQueue));
  pFront->next->msg = NewPdu;
  pFront->next->next = NULL;
  pthread_mutex_unlock(&organizer_reg_queue_mutex); 

  return count;
}
 
