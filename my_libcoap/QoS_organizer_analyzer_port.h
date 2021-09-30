#ifndef QoS_top_header
#define QoS_top_header

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
#include<coap3/coap_pdu_internal.h>
#include<pthread.h>
#include <errno.h>

typedef unsigned char method_t;

#define leshan_server_port 5683

#define analyzer_server_port 5800

#define analyzer_client_port 5900
#define analyzer_client_port_str "5900"

#define organizer_server_port 6000

#define organizer_client_port 5700
#define organizer_client_port_str "5700"
  

extern coap_context_t *organizer_client_ctx;
extern coap_context_t *organizer_server_ctx ;
extern coap_session_t *organizer_client_session;

extern coap_context_t *analyzer_client_ctx ;
extern coap_context_t *analyzer_server_ctx ;
extern coap_session_t *analyzer_client_session ;

struct coap_pdu_t_node
{
    struct coap_pdu_t_node *next;
    struct coap_pdu_t *received;
    /* data */
};
typedef struct coap_pdu_t_node coap_pdu_t_node;
coap_pdu_t_node *analyzer_client_head;
coap_pdu_t_node *organizer_client_head;

extern int len_analyer_received;
extern int len_organizer_received;
extern pthread_mutex_t analyzer_mutex;
extern pthread_mutex_t organizer_mutex;


#endif