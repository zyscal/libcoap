#include "QoS_organizer_server_header.h"



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








