#include "QoS_organizer_server_header.h"

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


coap_context_t * setup_server_context (void) {
  coap_endpoint_t *endpoint;
  coap_address_t listen_addr;
  coap_context_t *context = coap_new_context(NULL);
  if (!context)
    return NULL;
  coap_address_init(&listen_addr);
  listen_addr.addr.sa.sa_family = AF_INET;
  listen_addr.addr.sin.sin_port = htons (organizer_server_port);

  endpoint = coap_new_endpoint(context, &listen_addr, COAP_PROTO_UDP);
  if (!endpoint) {
    coap_free_context(context);
    return NULL;
  }
  return context;
}

void init_organizer_server_resources (coap_context_t *ctx) {
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
void
free_xmit_data_organizer_server(coap_session_t *session COAP_UNUSED, void *app_ptr) {
  coap_free(app_ptr);
  return;
}
static coap_pdu_t *
coap_new_request_organizer_server(coap_context_t *ctx,
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
    coap_add_data_large_request(session, pdu, length, data, free_xmit_data_organizer_server, data);
  }
  return pdu;
}

int check_token(coap_bin_const_t a, coap_bin_const_t b)
{
  if(a.length != b.length)
  {
    return 0;
  }
  for(int i = 0; i < a.length; i++)
  {
    if(a.s[i] != b.s[i])
    {
      return 0;
    }
  }
  return 1;
}
int check_segment_o_s(const uint8_t *s, size_t length) {
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
int cmdline_input_o_s(char *text, coap_string_t *buf) {
  int len;
  len = check_segment_o_s((unsigned char *)text, strlen(text));
  if (len < 0)
    return 0;
  buf->s = (unsigned char *)coap_malloc(len);
  if (!buf->s)
  {
    return 0;
  }
  buf->length = len;
  decode_segment((unsigned char *)text, strlen(text), buf->s);
  return 1;
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
  uint8_t uri_path[] = "rd";
  coap_get_data(request, &length_of_data, &data);
  uint8_t * check_data = data;
  coap_optlist_t *organizer_client_request_option = 
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
        coap_insert_optlist(&organizer_client_request_option, tem_opt);
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
    coap_insert_optlist(&organizer_client_request_option, tem_opt);
  }

  //转发到analyzer
  // 创建新的pdu
  coap_pdu_t  *pdu = coap_new_pdu(coap_pdu_get_type(request),
  coap_pdu_get_code(request), organizer_client_session);
  // 获取请求中tocken
  coap_bin_const_t token =   coap_pdu_get_token(request);
  // 添加tocken
  coap_add_token(pdu, token.length, token.s);
  // 添加optlist
  coap_add_optlist_pdu(pdu, &organizer_client_request_option);
  // 添加payload
  coap_string_t payload_test;
  cmdline_input_o_s(data, &payload_test);
  coap_add_data_large_request(organizer_client_session, 
  pdu, payload_test.length, payload_test.s, free_xmit_data_organizer_server, payload_test.s);
  // 发送
  coap_mid_t mid_con = coap_send_large(organizer_client_session, pdu);


  int get_ack = 0;
  // while (get_ack == 0)
  // {
  //   pthread_mutex_lock(&organizer_mutex);
  //   struct coap_pdu_t_node * tem = organizer_client_head;
  //   while (tem != NULL)
  //   {
  //     if(check_token(coap_pdu_get_token(tem->received), coap_pdu_get_token(request)))
  //     {
  //       get_ack = 1;
  //       printf("find tcp ack!!\n");
  //       coap_pdu_set_code(response, coap_pdu_get_code(tem->received));
  //       coap_opt_iterator_t opt_iter;
  //       coap_opt_t *option;
  //       coap_option_iterator_init(tem->received, &opt_iter, COAP_OPT_ALL);
  //       while ((option = coap_option_next(&opt_iter)))
  //       {
  //         coap_add_option(response, opt_iter.number,
  //         coap_opt_length(option),
  //         coap_opt_value(option));
  //       }
  //       len_analyer_received--;
  //       break;
  //     }
  //     else
  //     {
  //       tem = tem->next;
  //     }
  //     /* code */
  //   }
  //   pthread_mutex_unlock(&organizer_mutex);
  // }
}





