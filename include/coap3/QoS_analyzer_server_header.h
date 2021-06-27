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
#include <coap3/QoS_organizer_analyzer_port.h>

#define hexchar_to_dec(c) ((c) & 0x40 ? ((c) & 0x0F) + 9 : ((c) & 0x0F))

static coap_context_t * setup_server_context (void);
static void init_resources (coap_context_t *ctx);
static void hnd_post_reg(coap_resource_t *resource,coap_session_t *session, coap_pdu_t *request, coap_string_t *query, coap_pdu_t *response);
static void hnd_unknown_put(coap_resource_t *resource, coap_session_t *session,
const coap_pdu_t *request, const coap_string_t *query, coap_pdu_t *response);
int cmdline_input(char *text, coap_string_t *buf);
int check_segment(const uint8_t *s, size_t length);
void decode_segment(const uint8_t *seg, size_t length, unsigned char *buf);
void
free_xmit_data_analyzer_server(coap_session_t *session COAP_UNUSED, void *app_ptr);


static coap_pdu_t *
coap_new_request_analyzer_server(coap_context_t *ctx,
                 coap_session_t *session,
                 method_t m,
                 coap_optlist_t **options,
                 unsigned char *data,
                 size_t length);

void
decode_segment(const uint8_t *seg, size_t length, unsigned char *buf) {

  while (length--) {

    if (*seg == '%') {
      *buf = (hexchar_to_dec(seg[1]) << 4) + hexchar_to_dec(seg[2]);

      seg += 2; length -= 2;
    } else {
      *buf = *seg;
    }

    ++buf; ++seg;
  }
}

int
check_segment(const uint8_t *s, size_t length) {

  int n = 0;

  while (length) {
    if (*s == '%') {
      if (length < 2 || !(isxdigit(s[1]) && isxdigit(s[2])))
        return -1;

      s += 2;
      length -= 2;
    }

    ++s; ++n; --length;
  }

  return n;
}


int cmdline_input(char *text, coap_string_t *buf) {
  int len;
  printf("test point\n");
  printf("strlen(test) : %d\n", strlen(text));
  len = check_segment((unsigned char *)text, strlen(text));
  printf("len : %d\n", len);
  if (len < 0)
    return 0;

  buf->s = (unsigned char *)coap_malloc(len);
  if (!buf->s)
  {
    return 0;
  }
    
  printf("len : %d\n", len);
  buf->length = len;
  printf("buf->length : %d\n", len);
  decode_segment((unsigned char *)text, strlen(text), buf->s);
  return 1;
}
static void hnd_post_unknown(coap_resource_t *resource, coap_session_t *session,
const coap_pdu_t *request, const coap_string_t *query, coap_pdu_t *response){
  printf("enter into hnd_post_unknow\n");
}


static void hnd_unknown_put(coap_resource_t *resource, coap_session_t *session,
const coap_pdu_t *request, const coap_string_t *query, coap_pdu_t *response) {
  /* Remove (void) definition if variable is used */
  (void)resource;
  coap_pdu_code_t req_code = coap_pdu_get_code(request);
  coap_resource_t *r;
  coap_string_t *uri_path;
  /* get the uri_path - which will get used by coap_resource_init() */
  uri_path = coap_get_uri_path(request);
  printf("uri : %s\n", uri_path);
  if (!uri_path) {
    coap_pdu_set_code(response, COAP_RESPONSE_CODE_NOT_FOUND);
    return;
  }
  /* Check if new URI Path is valid */
  // if (!check_url_fn (uri_path, req_code)) {
  //   coap_pdu_set_code(response, COAP_RESPONSE_CODE_NOT_FOUND);
  //   coap_delete_string(uri_path);
  //   return;
  // }
  /*
   * Create a resource to handle the new URI
   * uri_path will get deleted when the resource is removed
   */
  // r = coap_resource_init((coap_str_const_t*)uri_path,
  //       COAP_RESOURCE_FLAGS_RELEASE_URI | COAP_RESOURCE_FLAGS_NOTIFY_NON);
  // coap_register_handler(r, COAP_REQUEST_PUT, hnd_put);
  // coap_register_handler(r, COAP_REQUEST_DELETE, hnd_delete);
  // /* We possibly want to Observe the GETs */
  // coap_resource_set_get_observable(r, 1);
  // coap_register_handler(r, COAP_REQUEST_GET, hnd_get);
  // coap_add_resource(coap_session_get_context(session), r);
  /* Do the PUT for this first call */
  // hnd_put(r, session, request, query, response);
  return;
}
void
free_xmit_data_analyzer_server(coap_session_t *session COAP_UNUSED, void *app_ptr) {
  coap_free(app_ptr);
  return;
}

static coap_pdu_t *
coap_new_request_analyzer_server(coap_context_t *ctx,
                 coap_session_t *session,
                 method_t m,
                 coap_optlist_t **options,
                 unsigned char *data,
                 size_t length) {
  coap_pdu_t *pdu;
  (void)ctx;
  unsigned char msgtype = COAP_MESSAGE_CON; /* usually, requests are sent confirmable */
  if (!(pdu = coap_new_pdu(msgtype, m, session)))
    return NULL;
  // if ( !coap_add_token(pdu, the_token.length, the_token.s)) {
  //   coap_log(LOG_DEBUG, "cannot add token to request\n");
  // }
  if (options)
    coap_add_optlist_pdu(pdu, options);
  if (length) {
    /* Let the underlying libcoap decide how this data should be sent */
    coap_add_data_large_request(session, pdu, length, data, free_xmit_data_analyzer_server, data);
  }
  return pdu;
}
static coap_context_t * setup_server_context (void) {
  coap_endpoint_t *endpoint;
  coap_address_t listen_addr;
  coap_context_t *context = coap_new_context(NULL);
  if (!context)
    return NULL;
  coap_address_init(&listen_addr);
  listen_addr.addr.sa.sa_family = AF_INET;
  listen_addr.addr.sin.sin_port = htons (analyzer_server_port);
  // endpoint = coap_new_endpoint(context, &listen_addr, COAP_PROTO_UDP);
  endpoint = coap_new_endpoint(context, &listen_addr, COAP_PROTO_TCP);
  coap_context_set_keepalive(context, 0);
  if (!endpoint) {
    coap_free_context(context);
    return NULL;
  }
  /* Initialize resources - See coap_resource(3) init_resources() example */

  return context;
}
static void init_resources (coap_context_t *ctx) {
  coap_resource_t *r;
  r = coap_resource_init(coap_make_str_const("rd"), 0);
  coap_register_handler(r, COAP_REQUEST_POST, hnd_post_reg);
  coap_add_resource(ctx, r);

// r = coap_resource_init(coap_make_str_const("time"), COAP_RESOURCE_FLAGS_NOTIFY_CON);
// coap_register_handler(r, COAP_REQUEST_GET, hnd_get_time);
// coap_add_resource(ctx, r);

  // coap_resource_t *test;
  // test = coap_resource_unknown_init(hnd_unknown_put);
  // coap_register_handler(test, COAP_REQUEST_POST, hnd_post_unknown);
  // coap_add_resource(ctx, test);

}

static void
hnd_post_reg(
coap_resource_t *resource,
coap_session_t *session, 
coap_pdu_t *request, 
coap_string_t *query, 
coap_pdu_t *response) {
  int length_of_data;
  uint8_t *data;
  coap_get_data(request,&length_of_data, &data);
  char *check_data = data;
  for(int i = 0; i < length_of_data; i++)
  {
    printf("%c ", *check_data);
    check_data++;
  }
  printf("\n");
  //转发到乐山
  uint8_t uri_path[] = "rd";
  coap_optlist_t *analyzer_client_request_option = 
  coap_new_optlist(COAP_OPTION_URI_PATH, sizeof(uri_path)-1, uri_path);
  int number_of_query = 0;
  if(query->length != 0)
  {
    number_of_query++;
    uint8_t *query_string_begin = query->s;
    uint8_t *query_string_end = query->s; 
    int option_len = 0;
    for(int i = 0; i < query->length; i++)
    {
      if(*query_string_end == '&')
      {
        coap_optlist_t *tem_opt = coap_new_optlist(COAP_OPTION_URI_QUERY, option_len, query_string_begin);
        coap_insert_optlist(&analyzer_client_request_option, tem_opt);
        printf("option len : %d\n", analyzer_client_request_option->length);
        uint8_t *check_option = tem_opt -> data;
        int check_len = tem_opt -> length;
        for(int i = 0; i < check_len; i++)
        {
          printf("%c ", *check_option);
          check_option++;
        }
        printf("\n------------------\n");
        option_len = 0;
        query_string_end++;
        query_string_begin = query_string_end;
        number_of_query++;
      }
      else
      {
        query_string_end++;
        option_len++;
      }
    }
    coap_optlist_t *tem_opt = coap_new_optlist(COAP_OPTION_URI_QUERY, option_len, query_string_begin);
    coap_insert_optlist(&analyzer_client_request_option, tem_opt);
    printf("option len : %d\n", analyzer_client_request_option->length);
    uint8_t *check_option = tem_opt -> data;
    int check_len = tem_opt -> length;
    for(int i = 0; i < check_len; i++)
    {
      printf("%c ", *check_option);
      check_option++;
    }
    printf("\n------------------\n");

  }
  printf("num of query : %d\n", number_of_query);



  coap_pdu_t  *pdu;
  method_t method = COAP_REQUEST_POST;
  coap_string_t payload_test;
  cmdline_input(data, &payload_test);
  pdu = coap_new_request_analyzer_server(analyzer_client_ctx, 
  analyzer_client_session, method, &analyzer_client_request_option, 
  payload_test.s, payload_test.length);
  int mid_con = coap_send_large(analyzer_client_session, pdu);

  int get_ack = 0;
  while (get_ack == 0)
  {
    pthread_mutex_lock(&analyzer_mutex);
    struct coap_pdu_t_node * tem = analyzer_client_head;
    while (tem != NULL)
    {
      if(coap_pdu_get_mid(tem->received) == mid_con)
      {
        get_ack = 1;
        printf("find ack message!! id is : %d\n", mid_con);
        coap_pdu_set_code(response, coap_pdu_get_code(tem->received));
        coap_opt_iterator_t opt_iter;
        coap_opt_t *option;
        coap_option_iterator_init(tem->received, &opt_iter, COAP_OPT_ALL);
        while ((option = coap_option_next(&opt_iter)))
        {
          coap_add_option(response, opt_iter.number,
          coap_opt_length(option),
          coap_opt_value(option));
        }
        len_analyer_received--;
        break;
      }
      else
      {
        tem = tem->next;
      }
      /* code */
    }
    pthread_mutex_unlock(&analyzer_mutex);
  }
  printf("end of post \n");
}




