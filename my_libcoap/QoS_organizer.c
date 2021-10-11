// #include <coap3/QoS_organizer_server_header.h>
// #include <coap3/QoS_organizer_client_header.h>
// #include <coap3/QoS_organizer_analyzer_port.h>
// #include <coap3/coap.h>
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <ctype.h>
// #include <signal.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include<netdb.h>
// #include <netinet/in.h>
// #include <pthread.h>
// #include <errno.h>

// void* organizer_client(void* arg)
// {
//     organizer_client_ctx = coap_new_context( NULL );
//     coap_address_t dst;
//     static coap_str_const_t server;
//     char server_ip[] = "192.168.3.24";
//     server.s = server_ip;
//     server.length = sizeof(server_ip);
//     int res = resolve_address(&server, &dst.addr.sa);
//     dst.size = res;
//     printf("dst.size : %d\n", res);
//     dst.addr.sin.sin_port = htons(analyzer_server_port);
//     printf("test\n");
//     coap_context_set_keepalive(organizer_client_ctx, 0);
//     coap_context_set_block_mode(organizer_client_ctx, COAP_BLOCK_USE_LIBCOAP);

//     organizer_client_session = get_session(
//     organizer_client_ctx,
//     "192.168.3.24", organizer_client_port_str,
//     COAP_PROTO_TCP,
//     // COAP_PROTO_UDP,
//     &dst,
//     NULL,
//     0,
//     NULL,
//     0
//   );
//     if ( !organizer_client_session ) {
//     printf("cannot create client organizer_client_session\n");
//   }
//   coap_register_response_handler(organizer_client_ctx, message_handler);
//   // coap_register_option(organizer_client_ctx, COAP_OPTION_URI_PATH);
//   // coap_pdu_t  *pdu;
//   // method_t method = COAP_REQUEST_POST;
//   // coap_string_t payload_test;
//   // char *test_payload_char = "testtes111111111111111111111111";
//   // cmdline_input(test_payload_char, &payload_test);
//   // printf("payload_test size : %d\n", payload_test.length);
//   // unsigned char uri_path[] = "rd";
//   // coap_optlist_t *organizer_client_request_option = 
//   // coap_new_optlist(COAP_OPTION_URI_PATH, sizeof(uri_path) - 1, uri_path);
//   // printf("end of optlist\n"); 
//   // printf("opt num : %d\n", organizer_client_request_option->number);
//   // pdu = coap_new_request_organizer_server(organizer_client_ctx, 
//   // organizer_client_session, method, &organizer_client_request_option, 
//   // payload_test.s, payload_test.length);
//   // coap_send_large(organizer_client_session, pdu);
//   uint32_t wait_ms = COAP_RESOURCE_CHECK_TIME * 1000;
//   while (1)
//   {
//     int result = coap_io_process(organizer_client_ctx, wait_ms);
//     if (result < 0)
//     {
//       printf("before break");
//       break;
//     }
//     else
//     {
//       // printf("there is something recv, result : %d\n", result);
//     }
//   }
//   printf("test point\n");
// }




// int main()
// {
//     pthread_t tids[0];
//     int i = pthread_create(&tids[0], NULL, organizer_client, NULL);
//     if(i == 0)
//     {
//         printf("thread organizer_client success!\n");
//     }
//     else{
//         printf("thread organizer_client failed!\n");
//     }
    
//   //organizer首先与analyzer建立连接
//   organizer_server_ctx = setup_server_context();
//   init_resources(organizer_server_ctx);
//   uint32_t wait_ms = COAP_RESOURCE_CHECK_TIME * 1000;
//   while (1)
//   {
//     int result = coap_run_once(organizer_server_ctx, wait_ms);
//     if (result < 0)
//     {
//       printf("before break");
//       break;
//     }
//     else
//     {
//       // printf("there is something recv, result : %d\n", result);
//     }
//   }
//   printf("out of while \n");
//     ///////////////

// }