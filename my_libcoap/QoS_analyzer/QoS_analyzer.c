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
coap_context_t *analyzer_client_ctx;
coap_context_t *analyzer_server_ctx;
coap_session_t *analyzer_client_session;
pthread_mutex_t analyzer_mutex;
int len_analyer_received;
void* analyzer_client(void* arg)
{
      
    printf("enter into analyzer_client\n");
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
    printf("create analyzer_client_session success!\n");
  }
  coap_register_response_handler(analyzer_client_ctx, message_handler);


  uint32_t wait_ms = COAP_RESOURCE_CHECK_TIME * 1000;
  while (1)
  {

    int result = coap_io_process(analyzer_client_ctx, wait_ms);
    if (result < 0)
    {
      printf("before break");
      break;
    }
    else
    {
      // printf("there is something recv, result : %d\n", result);
    }
    
  }
  printf("test point\n");
}


int main()
{
    pthread_mutex_init(&analyzer_mutex,NULL);
    pthread_t tids[0];
    int i = pthread_create(&tids[0], NULL, analyzer_client, NULL);
    if(i == 0)
    {
        printf("thread analyzer_client success!\n");
    }
    else{
        printf("thread analyzer_client failed!\n");
    }


    analyzer_server_ctx = setup_server_context();
    init_resources(analyzer_server_ctx);

    uint32_t wait_ms = COAP_RESOURCE_CHECK_TIME * 1000;
    while (1) 
    {
      int result = coap_run_once(analyzer_server_ctx, wait_ms);
      if (result < 0)
      {
        printf("before break");
        break;
      }
      else
      {
        // printf("there is something recv, result : %d\n", result);
      }
      
    }
    printf("out of while \n");

      /////////////////////
      //   coap_context_t  *ctx = NULL;
      //   ctx = coap_new_context( NULL );
      //   coap_session_t *session = NULL;
      //   coap_address_t dst;
      //   static coap_str_const_t server;
      //   char server_ip[] = "192.168.3.24";
      //   server.s = server_ip;
      //   server.length = sizeof(server_ip);
      //   int res = resolve_address(&server, &dst.addr.sa);
      //   dst.size = res;
      //   printf("dst.size : %d\n", res);
      //   dst.addr.sin.sin_port = htons(5683);
      //   printf("test\n");
      //   coap_context_set_keepalive(ctx, 0);
      //   coap_context_set_block_mode(ctx, COAP_BLOCK_USE_LIBCOAP);
      //   session = get_session(
      //   ctx,
      //   "192.168.3.24", "",
      //   COAP_PROTO_TCP,
      //   &dst,
      //   NULL,
      //   0,
      //   NULL,
      //   0
      // );
      //   if ( !session ) {
      //   printf("cannot create client session\n");
      // }
      // coap_pdu_t  *pdu;
      // method_t method = COAP_REQUEST_POST;
      // pdu = coap_new_request(ctx, session, method, &optlist, payload.s, payload.length);
      ///////////////////////



}