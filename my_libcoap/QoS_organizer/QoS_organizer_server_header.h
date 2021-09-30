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
#define hexchar_to_dec(c) ((c) & 0x40 ? ((c) & 0x0F) + 9 : ((c) & 0x0F))
coap_context_t * setup_server_context (void);
void init_resources (coap_context_t *ctx);
static void hnd_post_reg(coap_resource_t *resource,coap_session_t *session, coap_pdu_t *request, coap_string_t *query, coap_pdu_t *response);
static void hnd_unknown_put(coap_resource_t *resource, coap_session_t *session,
const coap_pdu_t *request, const coap_string_t *query, coap_pdu_t *response);
static coap_pdu_t *coap_new_request_organizer_server(coap_context_t *ctx,
                 coap_session_t *session,
                 method_t m,
                 coap_optlist_t **options,
                 unsigned char *data,
                 size_t length);
void decode_segment(const uint8_t *seg, size_t length, unsigned char *buf);
static void hnd_post_unknown(coap_resource_t *resource, coap_session_t *session,
const coap_pdu_t *request, const coap_string_t *query, coap_pdu_t *response);
int check_token(coap_bin_const_t a, coap_bin_const_t b);




