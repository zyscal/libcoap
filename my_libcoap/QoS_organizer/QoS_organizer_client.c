#include "QoS_organizer_client_header.h"
#include <coap3/coap.h>


// void handle_ACK(const coap_pdu_t *received, struct coap_pdu_t_node** pointer)
// {
//   printf("enter into organizer response\n");
//   if(*pointer == NULL)
//   {
//       printf("before malloc size of head : %d\n", sizeof(*pointer));
//       *pointer = (coap_pdu_t_node *)malloc(sizeof(coap_pdu_t_node));

//   }
//   printf("after malloc size of head : %d\n", sizeof(*pointer));
//   printf("received->used_size : %d\n", received->used_size);
//   printf("received->alloc_size : %d\n", received->alloc_size);
//   printf("received->max_size : %d\n", received->max_size);
//   printf("received->hdr_size : %d\n", received->hdr_size);
//   printf("received->max_hdr_size : %d\n", received->max_hdr_size);
//   printf("received->token_length : %d\n",received->token_length);
//   coap_bin_const_t token = coap_pdu_get_token(received);
//   coap_pdu_code_t code = coap_pdu_get_code(received);


//   printf("code : %d\n", code);
//   printf("token len : %d\n", token.length);
//   size_t data_len;
//   uint8_t *data;
//   coap_get_data(received, &data_len, &data);
//   printf("the ack data len : %d\n", data_len);

//   (*pointer)->received = coap_new_pdu(COAP_MESSAGE_ACK, code, organizer_client_session);

//   coap_add_token((*pointer)->received, token.length, token.s);
//   (*pointer)->received->lg_xmit = received->lg_xmit;

//   coap_opt_iterator_t opt_iter;
//   coap_opt_t *option;
//   coap_option_iterator_init(received, &opt_iter, COAP_OPT_ALL);
  
//   while ((option = coap_option_next(&opt_iter)))
//   {
//     printf("-----------------\n");
//     // printf("option number is : %d\n", option->)
//     coap_add_option((*pointer)->received, opt_iter.number,
//       coap_opt_length(option), coap_opt_value(option));
//   }
//   if(data_len != 0)
//   {
//     coap_add_data((*pointer)->received, &data_len, &data);
//   }

//   len_organizer_received ++;

// }
 
coap_response_t
message_handler(coap_session_t *session COAP_UNUSED,
                const coap_pdu_t *sent,
                const coap_pdu_t *received,
                const coap_mid_t id COAP_UNUSED)
{
  // printf("enter into coap_response_t\n");

  // pthread_mutex_lock(&organizer_mutex);
  // struct coap_pdu_t_node *tem = organizer_client_head;
  // if (tem == NULL)
  // {
  //   handle_ACK(received, &organizer_client_head);
  // }
  // else
  // {
  //   printf("two con message acks\n");
  //   for(int i = 1; i < len_organizer_received; i++)
  //   {
  //     tem = tem->next;
  //   }
  //   handle_ACK(received, &(tem->next));
  // }
  // pthread_mutex_unlock(&organizer_mutex);
  // printf("after unlock\n");
  
  return COAP_RESPONSE_OK;
}
 
 void free_xmit_data_organizer_client(coap_session_t *session COAP_UNUSED, void *app_ptr) {
  coap_free(app_ptr);
  return;
}
static coap_session_t *setup_client_session (struct in_addr ip_address) {
  coap_session_t *session;
  coap_address_t server;
  /* See coap_context(3) */
  coap_context_t *context = coap_new_context(NULL);

  if (!context)
    return NULL;

  coap_address_init(&server);
  server.addr.sa.sa_family = AF_INET;
  server.addr.sin.sin_addr = ip_address;
  server.addr.sin.sin_port = htons (analyzer_server_port);

  session = coap_new_client_session(context, NULL, &server, COAP_PROTO_TCP);
  if (!session) {
    coap_free_context(context);
    return NULL;
  }
  /* The context is in session->context */
  return session;
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
    printf("enter into get session\n");
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
coap_new_request_organizer_client(coap_context_t *ctx,
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


  if (options)
    coap_add_optlist_pdu(pdu, options);

  if (length) {
    /* Let the underlying libcoap decide how this data should be sent */
    coap_add_data_large_request(session, pdu, length, data, free_xmit_data_organizer_client, data);
  }
  return pdu;
}
