#include "QoS_analyzer_client_header.h"
#include "queue/ACK_queue.h"
coap_response_t
message_handler(coap_session_t *session,
                const coap_pdu_t *sent,
                const coap_pdu_t *received,
                const coap_mid_t id COAP_UNUSED)
{
  if(coap_pdu_get_type(received) == COAP_MESSAGE_ACK ||
  coap_pdu_get_type(received) == COAP_MESSAGE_RST)
  { // ACK 数据，应当放到ACK队列中
    coap_pdu_t *ack_pdu = InsertACKMsg(received, session, &ULACKQueue);
  } 
  coap_response_t ans;
  return ans;
}

void free_xmit_data_analyzer_client(coap_session_t *session COAP_UNUSED, void *app_ptr) {
  coap_free(app_ptr);
  return;
}




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
) {
  coap_session_t *session;
  // if (proto == COAP_PROTO_DTLS || proto == COAP_PROTO_TLS) {
  //   /* Encrypted session */
  //   if (root_ca_file || ca_file || cert_file) {
  //     /* Setup PKI session */
  //     coap_dtls_pki_t *dtls_pki = setup_pki(ctx);
  //     session = coap_new_client_session_pki(ctx, bind_addr, dst, proto, dtls_pki);
  //   }
  //   else if (identity || key) {
  //     /* Setup PSK session */
  //     coap_dtls_cpsk_t *dtls_psk = setup_psk(identity, identity_len,
  //                                              key, key_len);
  //     session = coap_new_client_session_psk2(ctx, bind_addr, dst, proto,
  //                                          dtls_psk);
  //   }
  //   else {
  //     /* No PKI or PSK defined, as encrypted, use PKI */
  //     coap_dtls_pki_t *dtls_pki = setup_pki(ctx);
  //     session = coap_new_client_session_pki(ctx, bind_addr, dst, proto, dtls_pki);
  //   }
  // }
  // else {
    /* Non-encrypted session */
    session = coap_new_client_session(ctx, bind_addr, dst, proto);
  // }
  return session;
}

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
) {
  coap_session_t *session = NULL;

  if ( local_addr ) {
    int s;
    struct addrinfo hints;
    struct addrinfo *result = NULL, *rp;

    memset( &hints, 0, sizeof( struct addrinfo ) );

    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = COAP_PROTO_RELIABLE(proto) ? SOCK_STREAM : SOCK_DGRAM; /* Coap uses UDP */
    hints.ai_flags = AI_PASSIVE | AI_NUMERICHOST | AI_NUMERICSERV | AI_ALL;

    s = getaddrinfo( local_addr, local_port, &hints, &result );
    if ( s != 0 ) {
      fprintf( stderr, "getaddrinfo: %s\n", gai_strerror( s ) );
      return NULL;
    }

    /* iterate through results until success */
    for ( rp = result; rp != NULL; rp = rp->ai_next ) {
      coap_address_t bind_addr;
      if ( rp->ai_addrlen <= (socklen_t)sizeof( bind_addr.addr ) ) {
        coap_address_init( &bind_addr );
        bind_addr.size = (socklen_t)rp->ai_addrlen;
        memcpy( &bind_addr.addr, rp->ai_addr, rp->ai_addrlen );
        session = open_session(ctx, proto, &bind_addr, dst,
                               identity, identity_len, key, key_len);
        if ( session )
          break;
      }
    }
    freeaddrinfo( result );
  } else {
    printf("local_addr is null\n");
    // session = open_session(ctx, proto, NULL, dst,
    //                            identity, identity_len, key, key_len);
  }
  return session;
}

int resolve_address(const coap_str_const_t *server, struct sockaddr *dst) {
  struct addrinfo *res, *ainfo;
  struct addrinfo hints;
  static char addrstr[256];
  int error, len=-1;

  memset(addrstr, 0, sizeof(addrstr));
  if (server->length)
    memcpy(addrstr, server->s, server->length);
  else
    memcpy(addrstr, "localhost", 9);

  memset ((char *)&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_family = AF_UNSPEC;

  error = getaddrinfo(addrstr, NULL, &hints, &res);

  if (error != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
    return error;
  }

  for (ainfo = res; ainfo != NULL; ainfo = ainfo->ai_next) {
    switch (ainfo->ai_family) {
    case AF_INET6:
    case AF_INET:
      len = (int)ainfo->ai_addrlen;
      memcpy(dst, ainfo->ai_addr, len);
      goto finish;
    default:
      ;
    }
  }
 finish:
  freeaddrinfo(res);
  return len;
}

static coap_pdu_t *
coap_new_request_analyzer_client(coap_context_t *ctx,
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
    coap_add_data_large_request(session, pdu, length, data, free_xmit_data_analyzer_client, data);
  }
  return pdu;
}


void hnd_unknown_put(coap_resource_t *resource, coap_session_t *session,
const coap_pdu_t *request, const coap_string_t *query, coap_pdu_t *response) {
    printf("enter into hnd_unknown_put");
}
void hnd_post_unknown(coap_resource_t *resource,
              coap_session_t *session,
              const coap_pdu_t *request,
              const coap_string_t *query COAP_UNUSED,
              coap_pdu_t *response) {
    printf("enter into hnd_post_unknown");
              }

void hnd_get_unknown(coap_resource_t *resource, coap_session_t *session,
  const coap_pdu_t *request, const coap_string_t *query,
  coap_pdu_t *response) 
{
  bool checkObserve = false;
  printf("enter into hnd_get\n");
  int length = request->used_size - request->token_length - request->body_length;
  uint8_t *queryBegin = request->token + request->token_length;
  // for(int i = 0 ; i < length; i++){
  //   printf("%c %d\n", queryBegin[i], queryBegin[i]);
  // }
  // printf("---------\n");
  if (queryBegin[0] == 96) {
    printf("this is observe\n");
    checkObserve = true;
  }
  // 获取code，type, mid
  coap_pdu_code_t code = coap_pdu_get_code(request);
  printf("code is : %d\n",code);
  coap_pdu_type_t type = coap_pdu_get_type(request);
  printf("type is : %d\n");
  // 便利opt，从中拿到GlobalID
  coap_opt_iterator_t opt_iter;
  coap_opt_t *option;
  coap_option_iterator_init(request, &opt_iter, COAP_OPT_ALL);
  if(checkObserve) { // 如果是observe事件，则第一个opt值为96,跳过该opt
    coap_option_next(&opt_iter);
  }
  // GlobalID用于定位，lengthofglobal用于查找长度， find用于判定
  uint8_t* GlobalID;
  int LengthOfGlobalID = 0;
  // organizer 的 session
  coap_session_t *organizerSession = NULL;
  while ((option = coap_option_next(&opt_iter))) {
    int Length = coap_opt_length(option);
    uint8_t *opt = coap_opt_value(option);
    char firstChar = opt[0];
    if(firstChar >= '0' && firstChar <= '9') {
      // 是普通地址
    } else if((firstChar >= 'a' && firstChar <= 'z') || (firstChar >= 'A' && firstChar <= 'Z')){
      // 初步判断是GlobalID
      GlobalID = opt;
      LengthOfGlobalID = Length;
      for(int i = 0; i < Length; i++) {
        printf("%c", opt[i]);
      }
      printf("\n");
      organizerSession = findSessionByGlobalID(GlobalID, Length);
    }
  }
  if(organizerSession != NULL) {
    printf("organizer session is not null \n");
    printf("session type : %d\n", organizerSession->type);
  } else {
    printf("organizer session failed\n");
  }
  // 创建下行pdu
  // coap_session_t *p = analyzer_server_ctx->session;
  // while (p != NULL)
  // {
  //   if(p == organizerSession) {
  //     break;
  //   }
  //   else {
  //     p = p->next;
  //   }
  //   /* code */
  // }
  // if(p == NULL) {
  //   printf("p is NULL\n\n");
  // }
  

  // coap_pdu_t *pdu = coap_new_pdu(type, code, p);
  // if(pdu != NULL) {
  //   printf("create DL pdu success\n");
  // }
}


    void hnd_delete_unknown(coap_resource_t *resource,
              coap_session_t *session,
              const coap_pdu_t *request,
              const coap_string_t *query COAP_UNUSED,
              coap_pdu_t *response) {
    printf("enter into hnd_delete_unknown");
              }
void init_analyzer_client_resources() {
  coap_resource_t *r;
  r = coap_resource_unknown_init(hnd_unknown_put);
  coap_register_handler(r, COAP_REQUEST_POST, hnd_post_unknown);
  coap_register_handler(r, COAP_REQUEST_GET, hnd_get_unknown);
  coap_register_handler(r, COAP_REQUEST_DELETE, hnd_delete_unknown);
  coap_add_resource(analyzer_client_ctx, r);
}