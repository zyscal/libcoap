#include "wakcoapToLibcoap.h"
void free_xmit_data_organizer_server_tem(coap_session_t *session COAP_UNUSED, void *app_ptr) {
  coap_free(app_ptr);
  return;
}

int check_segment_wakcoapToLibcap(const uint8_t *s, size_t length) {
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
int cmdline_input_wakcoapToLibcoap(char *text, coap_string_t *buf) {
  int len;
  printf("size of test : %d\n", strlen(text));
  len = check_segment_wakcoapToLibcap((unsigned char *)text, strlen(text));
  if (len < 0)
    return 0;
  buf->s = (unsigned char *)coap_malloc(len);
  if (!buf->s)
  {
    return 0;
  }
  buf->length = len;
  printf("buf->length : %d\n", len);
  decode_segment((unsigned char *)text, strlen(text), buf->s);
  return 1;
}

void wakaamaCoapToLibcoapReg(coap_packet_t_wakaama *message, int InternalID, coap_pdu_t **pdu) {
    // type
    coap_pdu_type_t type = COAP_MESSAGE_CON;
    // code
    uint8_t code = message->code;
    // pdu
    *pdu = coap_new_pdu(type, code, organizer_client_session);
    
    // token
    coap_bin_const_t token;
    token.length = message->token_len;
    token.s = message->token;
    coap_add_token(*pdu, token.length, token.s);

    // CONTENT_TYPE
    unsigned char buf[4];
    coap_add_option(*pdu, COAP_OPTION_CONTENT_TYPE,
                  coap_encode_var_safe(buf, sizeof(buf),
                                       COAP_MEDIATYPE_APPLICATION_LINK_FORMAT),
                                       buf);

    // uri_path
    uint8_t uri_path[] = "rd";
    coap_optlist_t *organizer_client_request_option = 
    coap_new_optlist(COAP_OPTION_URI_PATH, sizeof(uri_path)-1, uri_path);
    // uri_query
    multi_option_t * uri_query_head = message->uri_query;
    // 添加InternalID
    uint8_t InternalIDToChar[20];
    sprintf(InternalIDToChar, "ID=%d", InternalID);
    coap_optlist_t *tem_opt = coap_new_optlist(COAP_OPTION_URI_QUERY, strlen(InternalIDToChar)
        , InternalIDToChar);
    coap_insert_optlist(&organizer_client_request_option, tem_opt);
    // 添加其他的uri_query
    while (uri_query_head != NULL)
    {
        coap_optlist_t *tem_opt = coap_new_optlist(COAP_OPTION_URI_QUERY, uri_query_head->len
        , uri_query_head->data);
        coap_insert_optlist(&organizer_client_request_option, tem_opt);
        uri_query_head = uri_query_head->next;
    }
    coap_add_optlist_pdu(*pdu, &organizer_client_request_option);
    // payload
    coap_string_t payload;
    cmdline_input_wakcoapToLibcoap(message->payload, &payload);
    coap_add_data_large_request(organizer_client_session, *pdu, payload.length, payload.s,
    free_xmit_data_organizer_server_tem, payload.s);
}