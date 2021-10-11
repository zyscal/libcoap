#include "QoS_organizer_client_header.h"
#include <coap3/coap.h> 
#include "liblwm2m.h"


void
free_xmit_data_organizer_client(coap_session_t *session COAP_UNUSED, void *app_ptr) {
  coap_free(app_ptr);
  return;
}
#define hexchar_to_dec(c) ((c) & 0x40 ? ((c) & 0x0F) + 9 : ((c) & 0x0F))

int check_segment_Q_O_C(const uint8_t *s, size_t length) {
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
void decode_segment_Q_O_C(const uint8_t *seg, size_t length, unsigned char *buf) {
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

int cmdline_input_Q_O_C(char *text, coap_string_t *buf) {
  int len;
  len = check_segment_Q_O_C((unsigned char *)text, strlen(text));
  if (len < 0)
    return 0;
  buf->s = (unsigned char *)coap_malloc(len);
  if (!buf->s)
  {
    return 0;
  }
  buf->length = len;
  decode_segment_Q_O_C((unsigned char *)text, strlen(text), buf->s);
  return 1;
}

void prv_result_callback(uint16_t clientID,lwm2m_uri_t * uriP,int status,
  lwm2m_media_type_t format,  uint8_t * data,int dataLength,void * userData)
{
  printf("---------------------\n");
  printf("clientID is : %d\n", clientID);
  printf("objectId : %d\n", uriP->objectId);
  printf("instanceId : %d\n", uriP->instanceId);
  printf("resourceId : %d\n", uriP->resourceId);
  printf("resourceInstanceId : %d\n", uriP->resourceInstanceId);
  printf("status : %d\n", status);
  printf("format : %d\n", format);
  printf("COAP_RESPONSE_CODE_CONTENT is : %d\n", COAP_RESPONSE_CODE_CONTENT);
  printf("data : %d\n", dataLength);
  for(int i = 0; i < dataLength; i++) {
    printf("%c", data[i]);
  }
  printf("\n");

  printf("------------------\n");
  // 组装ACK，将ACK加入到队列中
  coap_pdu_t *ACK = (coap_pdu_t *)userData;
  // code
  coap_pdu_set_code(ACK, status);

  if(format == 0) {
    coap_add_option(ACK, COAP_OPTION_CONTENT_FORMAT, 0, NULL);
  }
  else if(format <= 255) {
    uint8_t content_type[1];
    content_type[0] = format;
    coap_add_option(ACK, COAP_OPTION_CONTENT_FORMAT, 1, content_type);
  } else {
    uint8_t content_type[2];
    uint16_t formatToInt = format;
    content_type[0] = formatToInt >> 8;
    content_type[1] = formatToInt << 8 >> 8;
    coap_add_option(ACK, COAP_OPTION_CONTENT_FORMAT, 2, content_type);
  }
  // 加入到队列中
  pthread_mutex_lock(&organizer_DL_ACK_queue_mutex);
  int num = InsertACKMsg(ACK, organizer_client_ctx, &Qorganizer_DLACKQueue, &organizer_DL_ACK_queue_mutex, data, dataLength);
  pthread_mutex_unlock(&organizer_DL_ACK_queue_mutex);
  printf("after insert ,队列中数据数量为：%d, mid is :  %d\n\n", num, ACK->mid);
}

void handle_uri(int *uri, char a) {
  if(*uri == 65535) {
    *uri = 0;
  }
  *uri *= 10;
  *uri += a - '0';
}
void hnd_read(coap_resource_t *resource,coap_session_t *session, 
coap_pdu_t *request, coap_string_t *query, coap_pdu_t *response) {
  uint8_t clientID = 0;
  int queryID = 1;
  int objectId = 65535;
  int instanceId = 65535;
  int resourceId = 65535;
  int resourceInstanceId = 65535;
  for(int i = 0; i < query->length; i++) {
    char a = query->s[i];
    if(queryID == 1 && a >= '0' && a <= '9') { // 首先开始寻找客户端ID
      clientID *= 10;
      clientID += a - '0';
    } else if(a == '&') { // 开始处理下一个query
      queryID++;
    } else if(queryID == 2) { // objectId
      handle_uri(&objectId, a);
    } else if(queryID == 3) {
      handle_uri(&instanceId, a);
    } else if(queryID == 4) {
      handle_uri(&resourceId, a);
    } else if(queryID == 5) {
      handle_uri(&resourceInstanceId, a);
    }
  }
  lwm2m_uri_t uri;
  uri.objectId = objectId;
  uri.instanceId = instanceId;
  uri.resourceId = resourceId;
  uri.resourceInstanceId = resourceInstanceId;

  // ACCEPT 类型，默认为text为0
  coap_opt_iterator_t opt_iter;
  coap_opt_t *option;
  int sumdelta = 0;
  coap_option_iterator_init(request, &opt_iter, COAP_OPT_ALL);
  uint16_t type = 0;
  while (option = coap_option_next(&opt_iter))
  {
    sumdelta += *option >> 4;
    if(sumdelta == 17) {
      uint8_t *value = coap_opt_value(option);
      int length = coap_opt_length(option);
      if (length == 0) {
        // text 长度为0 不做修改
      } else {
        for(int i = 0 ; i < length; i++) {
          type <<= 8;
          type |= value[i];
        }
      }
    }
  }
  printf("type 1is : %d\n", type);
  // 配置accept参数
  lwm2m_client_t * targetP = (lwm2m_client_t *)lwm2m_list_find((lwm2m_list_t *)lwm2mH->clientList, clientID);
  targetP->format = type;
  // 创建ACK
  coap_pdu_t *ACK = coap_new_pdu(COAP_MESSAGE_ACK, 0, organizer_client_session);
  // 组装token
  coap_bin_const_t token = coap_pdu_get_token(request);
  coap_add_token(ACK, token.length, token.s);
  // mid
  coap_pdu_set_mid(ACK, coap_pdu_get_mid(request));
  printf("request mid is : %d\n", request->mid);

  lwm2m_dm_read(lwm2mH, clientID, &uri, prv_result_callback, ACK);

  uint8_t *payload;
  uint8_t LengthOfPayload;
  // 从队列中取出ACK
  while(1) {
    pthread_mutex_lock(&organizer_DL_ACK_queue_mutex);
    coap_pdu_t *p = GetAndDelACKQueueFront(coap_pdu_get_mid(request), &Qorganizer_DLACKQueue, &organizer_DL_ACK_queue_mutex,
    &payload, &LengthOfPayload);
    pthread_mutex_unlock(&organizer_DL_ACK_queue_mutex);
    if(p == NULL) {
      continue;
    }
    printf("find ACK !!!!!\n\n");
    // code
    coap_pdu_set_code(response, coap_pdu_get_code(p));
    // token
    coap_add_token(response, token.length, token.s);
    // data
    printf("type 3is : %d\n", targetP->format);
    coap_add_data_large_response(resource, organizer_client_session, request,
    response, query, targetP->format, -1, 0, LengthOfPayload, payload, NULL, NULL);

    uint8_t* checkdata;
    int lengthofcheckdata;
    coap_get_data(response, &lengthofcheckdata, &checkdata);
    printf("lengthofcheckdata is : %d\n", lengthofcheckdata);
    printf("data is : \n");
    for(int i = 0; i < lengthofcheckdata; i++){
      printf("%c", checkdata[i]);
    }
    printf("\n");
    // 此处应当释放ACK消息 和 payload
    free(payload);
    coap_delete_pdu(ACK);
    break;
  }
}



void init_organizer_client_resources (coap_context_t *ctx) {
  coap_resource_t *r;
  r = coap_resource_init(coap_make_str_const("read"), 0);
  coap_register_handler(r, COAP_REQUEST_GET, hnd_read);
  coap_add_resource(ctx, r);
}











coap_response_t
message_handler(coap_session_t *session COAP_UNUSED,
                const coap_pdu_t *sent,
                const coap_pdu_t *received,
                const coap_mid_t id COAP_UNUSED)
{
  coap_pdu_code_t code = coap_pdu_get_code(received);
  switch (code)
  {
  case COAP_RESPONSE_CODE_CREATED: { // 注册事件的返回，因为基于TCP无法匹配对应的CON，所以需要通过CODE进行判断，sent为NULL
    int i = 1; // option计数
    int InternalID = 0; // 存储内部ID
    coap_opt_iterator_t opt_iter;
    coap_opt_t *option;
    coap_option_iterator_init(received, &opt_iter, COAP_OPT_ALL);
    while ((option = coap_option_next(&opt_iter)))
    {
      int Length = coap_opt_length(option);
      uint8_t *GlobalID = coap_opt_value(option);
      if(i == 1){
        // 寻找InternalID
        for(int i = 0; i < Length; i++){
          InternalID *= 10;
          InternalID += GlobalID[i] - '0';
        }
      } else { // 寻找Clientlist，保存GlobalID
        lwm2m_client_t * targetP;
        for (targetP = lwm2mH->clientList ; targetP != NULL ; targetP = targetP->next)
        {
            if(targetP->internalID == InternalID) {
              targetP->GlobalID = GlobalID;
              targetP->LengthOfGlobalID = Length;
              break;
            }
        }
      }
      i++;
    }
    break;
  }
  default:
    break;
  }
  return COAP_RESPONSE_OK;
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
