#ifndef ORGANIZER_REG_QUEUE
#define ORGANIZER_REG_QUEUE
#include <coap3/coap.h>
#include <stdbool.h>
#include "../../QoS_organizer_analyzer_port.h"
struct regQueue
{
  coap_pdu_t *msg;
  struct regQueue *next;
  uint8_t level;
};
extern struct regQueue *head;
bool IsRegQueueEmpty();
coap_pdu_t* GetAndDelRegQueueFront();
uint8_t InsertRegMsg(coap_pdu_t* NewPdu);
#endif
