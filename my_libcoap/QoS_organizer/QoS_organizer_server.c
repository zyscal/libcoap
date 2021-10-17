#include "QoS_organizer_server_header.h"



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

void
free_xmit_data_organizer_server(coap_session_t *session COAP_UNUSED, void *app_ptr) {
  coap_free(app_ptr);
  return;
}
// static coap_pdu_t *
// coap_new_request_organizer_server(coap_context_t *ctx,
//                  coap_session_t *session,
//                  method_t m,
//                  coap_optlist_t **options,
//                  unsigned char *data,
//                  size_t length) {
//   coap_pdu_t *pdu;
//   (void)ctx;
//   unsigned char msgtype = COAP_MESSAGE_CON; /* usually, requests are sent confirmable */
//   if (!(pdu = coap_new_pdu(msgtype, m, session)))
//     return NULL;
//   // if ( !coap_add_token(pdu, the_token.length, the_token.s)) {
//   //   coap_log(LOG_DEBUG, "cannot add token to request\n");
//   // }

//   if (options)
//     coap_add_optlist_pdu(pdu, options);

//   if (length) {
//     /* Let the underlying libcoap decide how this data should be sent */
//     coap_add_data_large_request(session, pdu, length, data, free_xmit_data_organizer_server, data);
//   }
//   return pdu;
// }

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





