#include <stdio.h>
#include <coap3/coap.h>
#include "../queue/QoS_organizer_priority_queue.h"

// 最简单的发送模型,多个队列轮询发送,首先查询注册队列
void simple_sender();
