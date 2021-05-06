#include<coap3/coap.h>
#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#define INDEX "This is an example server using libcoap\n"
typedef unsigned char method_t;
static unsigned char _token_data[8];
coap_binary_t the_token = { 0, _token_data };
static char *root_ca_file = NULL; /* List of trusted Root CAs in PEM */
static char *ca_file = NULL;   /* CA for cert_file - for cert checking in PEM,
                                  DER or PKCS11 URI */
static char *cert_file = NULL; /* certificate and optional private key in PEM,
                                  or PKCS11 URI*/

unsigned char msgtype = COAP_MESSAGE_CON; /* usually, requests are sent confirmable */
static coap_context_t *
setup_server_context (void) {
  coap_endpoint_t *endpoint;
  coap_address_t listen_addr;
  coap_context_t *context = coap_new_context(NULL);

  if (!context)
    return NULL;

  coap_address_init(&listen_addr);
  listen_addr.addr.sa.sa_family = AF_INET;
  listen_addr.addr.sin.sin_port = htons (5683);

  endpoint = coap_new_endpoint(context, &listen_addr, COAP_PROTO_UDP);
  if (!endpoint) {
    coap_free_context(context);
    return NULL;
  }

  /* Initialize resources - See coap_resource(3) init_resources() example */

  return context;
}
static void
hnd_post_time(
coap_resource_t *resource,
coap_session_t *session, 
coap_pdu_t *request, 
coap_string_t *query, 
coap_pdu_t *response) {


  // coap_pdu_set_type(response, COAP_MESSAGE_ACK);



  coap_pdu_set_code(response, COAP_RESPONSE_CODE_CREATED);

	const uint8_t* response_data = "Hello World!";
	// response->code = COAP_RESPONSE_CODE(68);
	// // coap_add_option(response, COAP_OPTION_CONTENT_TYPE, coap_encode_var_bytes(buf, COAP_MEDIATYPE_TEXT_PLAIN), buf);
	coap_add_data  (response, strlen(response_data), (uint8_t *)response_data);



  printf("end of post \n");


  // /* .. code .. */

  // coap_pdu_set_code(response, COAP_RESPONSE_CODE_CHANGED);
}




static void
hnd_get_time(coap_context_t *ctx, coap_resource_t *resource,
coap_session_t *session, coap_pdu_t *request, coap_binary_t *token,
coap_string_t *query, coap_pdu_t *response) {
  /* Remove (void) definition if variable is used */
  (void)ctx;
  (void)resource;
  (void)session;
  (void)request;
  (void)token;
  (void)query;
  (void)response;

  /* .. code .. */

  coap_pdu_set_code(response, COAP_RESPONSE_CODE_CONTENT);
}

static void init_resources (coap_context_t *ctx) {
  coap_resource_t *r;
  r = coap_resource_init(coap_make_str_const("rd"), 0);
  coap_register_handler(r, COAP_REQUEST_POST, hnd_post_time);
  	coap_add_resource(ctx, r);


  r = coap_resource_init(coap_make_str_const("time"),
                         COAP_RESOURCE_FLAGS_NOTIFY_CON);
  coap_register_handler(r, COAP_REQUEST_GET, hnd_get_time);

  	coap_add_resource(ctx, r);
}

static void
free_xmit_data(coap_session_t *session COAP_UNUSED, void *app_ptr) {
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
  server.addr.sin.sin_port = htons (5683);

  session = coap_new_client_session(context, NULL, &server, COAP_PROTO_TCP);
  if (!session) {
    coap_free_context(context);
    return NULL;
  }
  /* The context is in session->context */
  return session;
}

static coap_pdu_t * coap_new_request(coap_session_t *session,
                 method_t m,
                 coap_optlist_t **options,
                 unsigned char *data,
                 size_t length) {
  coap_pdu_t *pdu;

  if (!(pdu = coap_new_pdu(msgtype, m, session)))
    return NULL;

  if ( !coap_add_token(pdu, the_token.length, the_token.s)) {
    coap_log(LOG_DEBUG, "cannot add token to request\n");
  }

  if (options)
    coap_add_optlist_pdu(pdu, options);

  if (length) {
    /* Let the underlying libcoap decide how this data should be sent */
    coap_add_data_large_request(session, pdu, length, data,
                                free_xmit_data, data);
  }

  return pdu;
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

  if (proto == COAP_PROTO_DTLS || proto == COAP_PROTO_TLS) {
    /* Encrypted session */
    if (root_ca_file || ca_file || cert_file) {
      /* Setup PKI session */
      coap_dtls_pki_t *dtls_pki = setup_pki(ctx);
      session = coap_new_client_session_pki(ctx, bind_addr, dst, proto, dtls_pki);
    }
    else if (identity || key) {
      /* Setup PSK session */
      coap_dtls_cpsk_t *dtls_psk = setup_psk(identity, identity_len,
                                               key, key_len);
      session = coap_new_client_session_psk2(ctx, bind_addr, dst, proto,
                                           dtls_psk);
    }
    else {
      /* No PKI or PSK defined, as encrypted, use PKI */
      coap_dtls_pki_t *dtls_pki = setup_pki(ctx);
      session = coap_new_client_session_pki(ctx, bind_addr, dst, proto, dtls_pki);
    }
  }
  else {
    /* Non-encrypted session */
    session = coap_new_client_session(ctx, bind_addr, dst, proto);
  }
  return session;
}
static coap_session_t *get_session(
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
    session = open_session(ctx, proto, NULL, dst,
                               identity, identity_len, key, key_len);
  }
  return session;
}

int main()
{
    // coap_context_t* context = setup_server_context();
    // init_resources(context);
    // uint32_t wait_ms = COAP_RESOURCE_CHECK_TIME * 1000;
    // while (1)
    // {
    //   int result = coap_run_once(context, wait_ms);
    //   if (result < 0)
    //   {
    //     printf("before break");
    //     break;
    //   }
    //   else
    //   {
    //     printf("there is something recv, result : %d\n", result);
    //   }
      
    // }
    // printf("out of while \n");


  coap_context_t  *ctx  = coap_new_context( NULL );

  coap_address_t dst;
  coap_address_init(&dst);
  dst.addr.sin.sin_port = htons(0);
  dst.addr.sin.sin_addr.s_addr = inet_addr("192.168.3.24");



  coap_session_t *session = get_session(
    ctx,
    NULL, 
    0,
    COAP_PROTO_TCP,
    &dst,
    NULL,
    0,
    NULL,
    0
  );
  // coap_binary_t the_token = { 0, _token_data };
  // coap_session_init_token(session, )
  unsigned char *a = "12345678";
  // coap_pdu_t  *pdu = coap_new_request(session, 1, NULL, a, sizeof(a));

  pdu = coap_new_pdu(COAP_MESSAGE_CON, m, session)
  coap_pdu_t *pdu = coap_new_pdu(COAP_MESSAGE_CON, COAP_REQUEST_POST, )
  uint32_t wait_ms = COAP_RESOURCE_CHECK_TIME * 1000;
  while(1)
  {
      int result = coap_io_process(ctx, 2000);
  }



}