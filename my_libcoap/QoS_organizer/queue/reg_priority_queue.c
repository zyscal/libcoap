#include <coap3/coap.h>
#include <coap_pdu_internal.h>
#include <stdio.h>

bool IsRegQueueEmpty(){
  if (head == NULL) {
    return true;
  }
  return false;
}

coap_pdu_t* GetAndDelRegQueueFront() {
  if(head == NULL) {
    return NULL;
  }
  regQueue* FrontRegMsg = head;
  head = head->next;
  return FrontRegMsg->msg;
}

uint8_t InsertRegMsg(coap_pdu_t* NewPdu) {
  if (head == NULL) {
    head = (regQueue*)malloc(sizeof(regQueue));
    head->msg = NewPdu;
    head->next = NULL;
    return 1;
  }
  int count = 2;
  regQueue* pBack = head->next;
  regQueue* pFront = head;
  while(pBack != NULL){
    count++;
    pBack = pBack->next;
    pFront = pFront->next;
  }
  pFront->next = (regQueue*) malloc (sizeof(regQueue));
  pFront->next->msg = NewPdu;
  pFront->next->next = NULL;
  return count;
}
 
