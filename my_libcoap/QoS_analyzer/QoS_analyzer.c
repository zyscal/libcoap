#include "QoS_analyzer_client_header.h"
#include "QoS_analyzer_server_header.h"

#include<coap3/coap.h>
#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include "queue/QoS_analyzer_DL_queue.h"
coap_context_t *analyzer_client_ctx;
coap_context_t *analyzer_server_ctx;
coap_session_t *analyzer_client_session;
int len_analyer_received;
extern pthread_mutex_t analyzer_UL_ACK_queue_mutex;
extern pthread_mutex_t analyzer_DL_ACK_queue_mutex;
extern pthread_mutex_t analyzer_DL_queue_mutex;
extern pthread_mutex_t analyzer_midList_mutex;

void* analyzer_server_sender(void* arg) {
    printf("enter into analyzer_server_sender\n");
    analyzer_DL_simple_sender();
}


void* analyzer_client(void* arg)
{
    analyzer_client_ctx = coap_new_context( NULL );
    coap_address_t dst;
    static coap_str_const_t server;
    char server_ip[] = "192.168.3.24";
    server.s = server_ip;
    server.length = sizeof(server_ip);
    int res = resolve_address(&server, &dst.addr.sa);
    dst.size = res;
    dst.addr.sin.sin_port = htons(leshan_server_port);
    coap_context_set_keepalive(analyzer_client_ctx, 0);
    coap_context_set_block_mode(analyzer_client_ctx, COAP_BLOCK_USE_LIBCOAP);

    analyzer_client_session = get_session(
    analyzer_client_ctx,
    "192.168.3.24", analyzer_client_port_str,
    COAP_PROTO_UDP,
    &dst,
    NULL,
    0,
    NULL,
    0
  );
    if ( !analyzer_client_session ) {
    printf("cannot create client analyzer_client_session\n");
  } else {
  }
  coap_register_response_handler(analyzer_client_ctx, message_handler);
  // 为客户端创建资源
  init_analyzer_client_resources();
  uint32_t wait_ms = COAP_RESOURCE_CHECK_TIME * 1000;

  // 创建发送线程
    // 开启轮轮询发送线程
  pthread_t thread;
  int result = pthread_create(&thread, NULL, analyzer_server_sender, NULL);
  if(result == 0) {
  } else {
      printf("failed to create analyzer DL sender thread\n");
  }

  while (1)
  {
    int result = coap_io_process(analyzer_client_ctx, wait_ms);
    if (result < 0)
    {
      break;
    }
    else
    {
      // printf("there is something recv, result : %d\n", result);
    }
    
  }
}


int main()
{
    pthread_mutex_init(&analyzer_UL_ACK_queue_mutex,NULL);
    pthread_mutex_init(&analyzer_DL_ACK_queue_mutex,NULL);
    pthread_mutex_init(&analyzer_DL_queue_mutex,NULL);
    pthread_mutex_init(&analyzer_midList_mutex, NULL);
    pthread_mutex_init(&organizerNodeMutex, NULL);
    pthread_mutex_init(&analyzer_observeList_mutex, NULL);

    
    pthread_t tids[0];
    int i = pthread_create(&tids[0], NULL, analyzer_client, NULL);
    if(i == 0)
    {
    }
    else{
        printf("thread analyzer_client failed!\n");
    }
    analyzer_server_ctx = setup_server_context();
    init_analyzer_server_resources(analyzer_server_ctx);

    uint32_t wait_ms = COAP_RESOURCE_CHECK_TIME * 1000;
    while (1) 
    {
      int result = coap_run_once(analyzer_server_ctx, wait_ms);
      if (result < 0)
      {
        break;
      }
      else
      {
        // printf("there is something recv, result : %d\n", result);
      }
      
    }
}