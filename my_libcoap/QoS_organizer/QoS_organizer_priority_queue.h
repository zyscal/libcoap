#ifndef QoS_O_QUEUE
#define QoS_O_QUEUE
#include <coap3/coap.h>

struct regQueue
{
  coap_pdu_t *msg;
  struct regQueue *next;
  uint8_t level;
};
struct regQueue *head = NULL;
bool IsRegQueueEmpty();
coap_pdu_t* GetAndDelRegQueueFront();
uint8_t InsertRegMsg(coap_pdu_t* NewPdu);



#endif
