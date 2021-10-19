#include "QoS_analyzer_client_header.h"
#include "queue/ACK_queue.h"
#include "coap3/option.h"
coap_response_t
message_handler(coap_session_t *session,
                const coap_pdu_t *sent,
                const coap_pdu_t *received,
                const coap_mid_t id COAP_UNUSED)
{
  if(coap_pdu_get_type(received) == COAP_MESSAGE_ACK ||
  coap_pdu_get_type(received) == COAP_MESSAGE_RST)
  { // ACK 数据，应当放到ACK队列中
    coap_pdu_t *ack_pdu = InsertACKMsg(received, session, &ULACKQueue, &analyzer_UL_ACK_queue_mutex);
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
  //---------------------------
  // 拿GlobalID, 找对应的session
  // checkObserve 为0表示非观测事件，为1表示观测，2表示取消观测
  int checkObserve = 0;
  int length = request->used_size - request->token_length - request->body_length;
  uint8_t *queryBegin = request->token + request->token_length;
  if (queryBegin[0] == 96) {
    printf("this is observe\n");
    checkObserve = 1;
  } else if (queryBegin[0] == 97) {
    checkObserve = 2;
  }
  // 获取code，type, mid
  coap_pdu_code_t code = coap_pdu_get_code(request);
  coap_pdu_type_t type = coap_pdu_get_type(request);
  coap_mid_t mid = coap_pdu_get_mid(request);

  // 便利opt，从中拿到GlobalID
  coap_opt_iterator_t opt_iter;
  coap_opt_t *option;
  coap_option_iterator_init(request, &opt_iter, COAP_OPT_ALL);
  if(checkObserve == 1) { // 如果是observe事件，则第一个opt值为96,跳过该opt
    coap_option_next(&opt_iter);
  }
  // GlobalID用于定位，lengthofglobal用于查找长度
  uint8_t* GlobalID;
  int LengthOfGlobalID = 0;
  // InternalID
  int InternalID = 0;
  // organizer 的 session
  coap_session_t *organizerSession = NULL;
  while ((option = coap_option_next(&opt_iter))) {
    int Length = coap_opt_length(option);
    uint8_t *opt = coap_opt_value(option);
    char firstChar = opt[0];
    if(Length >= 6){
      GlobalID = opt;
      LengthOfGlobalID = Length;
      InternalID = findSessionAndInternalIDByGlobalID(GlobalID, Length, &organizerSession);
      break;
    }
  }
  if(organizerSession == NULL){
    printf("organizer session failed\n");
  }  
//-------------------------------------------
// 处理下行转发数据
  // 创建pdu
  coap_pdu_t *pdu = coap_new_pdu(type, code, organizerSession);
  if(pdu == NULL) {
    printf("failed to create pdu\n");
  }
  // 添加mid
  coap_pdu_set_mid(pdu, mid);
  // 获取请求中tocken，并将token与mid映射关系加入表中
  coap_bin_const_t token = coap_pdu_get_token(request);
  pthread_mutex_lock(&analyzer_midList_mutex);
  printf("插入midList的结果是：%d, 当前mid是:%d\n", InsertMid(mid, token), mid);
  pthread_mutex_unlock(&analyzer_midList_mutex);

  // 添加tocken
  coap_add_token(pdu, token.length, token.s);
  // 便利请求中的optlist
  coap_option_iterator_init(request, &opt_iter, COAP_OPT_ALL);

  // 为下行数据添加uri_path
  coap_optlist_t *analyzer_server_request_option;
  uint8_t uri_path[] = "read";
  analyzer_server_request_option = coap_new_optlist(COAP_OPTION_URI_PATH, sizeof(uri_path)-1, uri_path);

  if(checkObserve == 1) { 
    // 同时需要完善observelist
    printf("before insert observe list\n");
    pthread_mutex_lock(&analyzer_observeList_mutex);
    InsertObserveList(token, GlobalID, LengthOfGlobalID);
    pthread_mutex_unlock(&analyzer_observeList_mutex);
    printf("after insert observe list\n");
  }

  // 首先将InternalID添加在首段
    // 添加前将int 的InterID 转为 uint8_t
  uint8_t InternalIDTochar[10];
  sprintf(InternalIDTochar, "%d", InternalID);
   // 将InternalID优先添加
  coap_optlist_t *InternalIDopt = coap_new_optlist(COAP_OPTION_URI_QUERY, strlen(InternalIDTochar), InternalIDTochar);
  coap_insert_optlist(&analyzer_server_request_option, InternalIDopt);
  // 遍历request消息
  // 根据rfc7252 delta具有累加性，用于表示opt类型，类型值参考COAP_OPTION_URI_QUERY
  int sumDelta = 0;
  // 判断 observe 属性，0为观测，1为取消观测
  while ((option = coap_option_next(&opt_iter))) {
    uint8_t findDelta = *option >> 4;
    sumDelta += findDelta;
    int Length = coap_opt_length(option);
    uint8_t *opt = coap_opt_value(option);
    char firstChar = opt[0];
    coap_optlist_t *tem_opt;
    if(sumDelta == 11 && Length >= 6){
      // GlobalID
      continue;
    } else if (sumDelta == 11){ // uri
      tem_opt = coap_new_optlist(COAP_OPTION_URI_QUERY, Length, opt);
    } else if (sumDelta == 17){ // ACCEPT类型
      tem_opt = coap_new_optlist(COAP_OPTION_ACCEPT, Length, opt);
    } else if(sumDelta == 6) { // observe
      tem_opt = coap_new_optlist(COAP_OPTION_OBSERVE, 0, NULL);

    }
    coap_insert_optlist(&analyzer_server_request_option, tem_opt);
  }
  // 如果是observe消息需要更新observe


  // 添加optlist
  coap_add_optlist_pdu(pdu, &analyzer_server_request_option);
  // 将处理好的pdu加入到下行消息队列中
  pthread_mutex_lock(&analyzer_DL_queue_mutex);
  int numOfDLQueuue = InsertDLMsg(pdu, organizerSession, &DLQueueHead);
  pthread_mutex_unlock(&analyzer_DL_queue_mutex);


  response->type = COAP_MESSAGE_NOT_SEND;
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