#ifndef QoS_A_C_H
#define QoS_A_C_H
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
#include "../QoS_organizer_analyzer_port.h"
#include "sessions/session_list.h"
#include "queue/QoS_analyzer_DL_queue.h"


void init_analyzer_client_resources();
void free_xmit_data_analyzer_client(coap_session_t *session COAP_UNUSED, void *app_ptr);
static coap_session_t* 
open_session(
  coap_context_t *ctx,
  coap_proto_t proto,
  coap_address_t *bind_addr,
  coap_address_t *dst,
  const uint8_t *identity,
  size_t identity_len,
  const uint8_t *key,
  size_t key_len
);
coap_session_t *get_session(
  coap_context_t *ctx,
  const char *local_addr,
  const char *local_port,
  coap_proto_t proto,
  coap_address_t *dst,
  const uint8_t *identity,
  size_t identity_len,
  const uint8_t *key,
  size_t key_len
);
int resolve_address(const coap_str_const_t *server, struct sockaddr *dst);
static coap_pdu_t *
coap_new_request_analyzer_client(coap_context_t *ctx,
                 coap_session_t *session,
                 method_t m,
                 coap_optlist_t **options,
                 unsigned char *data,
                 size_t length);
void handle_response(const coap_pdu_t *received, struct coap_pdu_t_node** pointer);

coap_response_t
message_handler(coap_session_t *session COAP_UNUSED,
                const coap_pdu_t *sent,
                const coap_pdu_t *received,
                const coap_mid_t id COAP_UNUSED);
static coap_optlist_t *optlist = NULL;
static coap_string_t payload = { 0, NULL };

static unsigned char _token_data[8];

static char *root_ca_file = NULL; /* List of trusted Root CAs in PEM */
static char *ca_file = NULL;   /* CA for cert_file - for cert checking in PEM,
                                  DER or PKCS11 URI */
static char *cert_file = NULL; /* certificate and optional private key in PEM,
                                  or PKCS11 URI*/


#endif