#include "QoS_organizer_client_header.h"
#include <coap3/coap.h> 
#include "liblwm2m.h"

void printfALLObserveThings() {
  observeList *p =  observeListHead;
  while (p != NULL)
  {
    observeThings *q = p->things;
    if(q != NULL) {
      // 说明clientID 存在列表中
      printf("++++++++   clientID is : %d   ++++++++\n", p->clientID);
    }
    int i = 1;
    while (q != NULL)
    {
      printf("第%d个观测事件是 ： /%d/%d/%d/%d\n", i, q->URI.objectId, 
      q->URI.instanceId, q->URI.resourceId, q->URI.resourceInstanceId);
      q = q->next;
      i++;
    }
    p = p->next;
  }
  printf("++++++++++++++++++++++++++++++++++++++++++\n");
}

void prv_observe_result_callback(uint16_t clientID,lwm2m_uri_t * uriP, int status,
  lwm2m_media_type_t format,  uint8_t * data,int dataLength,void * userData) {
    // 多次对同一对象、资源observe回首先返回202delete，然后才是成功observe，此处应当对边缘侧观测表进行更新。
  printf("---------------------\n");
  printf("clientID is : %d\n", clientID);
  printf("objectId : %d\n", uriP->objectId) ;
  printf("instanceId : %d\n", uriP->instanceId);
  printf("resourceId : %d\n", uriP->resourceId);
  printf("resourceInstanceId : %d\n", uriP->resourceInstanceId);
  printf("status : %d\n", status);
  printf("format : %d\n", format);
  printf("data : %d\n", dataLength);
  for(int i = 0; i < dataLength; i++) {
    printf("%c", data[i]);
  }
  printf("\n");
  printf("token : \n");
  coap_bin_const_t ACKToken = coap_pdu_get_token(userData);
  for(int i = 0 ; i < ACKToken.length; i++) {
    printf("%d", ACKToken.s[i]);
  }
  printf("\n");
  // 获取URI，这么写有点蠢但是能跑
  lwm2m_uri_t URI;
  URI.objectId = uriP->objectId;
  URI.instanceId = uriP->instanceId;
  URI.resourceId = uriP->resourceId;
  URI.resourceInstanceId = uriP->resourceInstanceId;
  // 通过InsertObserve判断是否是non消息
  bool checkNon = false;
  // 如果是non消息,需要拿token信息，token用于绑定observe事件
  uint8_t *nonToken;
  int nonTokenLength;
  // 创建回报，可能是ACK，可能是non消息,需要重新组装ACK，避免重复释放
  coap_pdu_t *ACKfromUserData = (coap_pdu_t *)userData;
  coap_pdu_t *ACKorNON = coap_pdu_duplicate(ACKfromUserData, organizer_client_ctx, ACKToken.length, ACKToken.s, NULL);
  coap_pdu_set_mid(ACKorNON, coap_pdu_get_mid(ACKfromUserData));

  if(dataLength != 0){ // 当数据段不为空时，status描述了observe计数器信息
    pthread_mutex_lock(&observeMutex);
    bool check = InsertObserve(clientID, URI, ACKToken, &nonToken, &nonTokenLength);
    pthread_mutex_unlock(&observeMutex);
    if(check) {
      printf("成功插入新的观测内容\n");
    } else {
      // non消息会触发该回调函数，需要将token赋值给ack
      printf("观测内容已经存在，本条callback是non信息上报，需要转发\n");
      ACKorNON = coap_new_pdu(COAP_MESSAGE_NON, COAP_RESPONSE_CODE_CONTENT, organizer_client_session);
      coap_add_token(ACKorNON, nonTokenLength, nonToken);
      checkNon = true;
    }
    printfALLObserveThings();
  } else if(data == NULL && status == COAP_RESPONSE_CODE_DELETED){
    pthread_mutex_lock(&observeMutex);
    bool check = DeleteObserve(clientID, URI, coap_pdu_get_token(ACKorNON));
    pthread_mutex_unlock(&observeMutex);
    printf("delete observe result : %d\n", check);
    return;
  }
  printf("------------------\n");

  // code
  if(dataLength != 0) { // 数据长度不为0时候，为content，若是non消息这个其实重复赋值了
    coap_pdu_set_code(ACKorNON, COAP_RESPONSE_CODE_CONTENT);
  } else { // 一般不会进入到这里
    coap_pdu_set_code(ACKorNON, status);
  }
  // 需要将文本类型进行添加
  if(format == 0) {
    coap_insert_option(ACKorNON, COAP_OPTION_CONTENT_FORMAT, 0, NULL);
  } else if(format <= 255) {
    uint8_t NonFormat = format;
    coap_insert_option(ACKorNON, COAP_OPTION_CONTENT_FORMAT, 1, &NonFormat);
  } else {
    uint8_t NonFormat[2];
    NonFormat[0] = (uint16_t)format >> 8;
    NonFormat[1] = (uint16_t)format & 0xff;
    coap_insert_option(ACKorNON, COAP_OPTION_CONTENT_FORMAT, 2, NonFormat);
  }
  // 插入队列中
  if(checkNon) {
    // 如果是non消息则插入到non队列中
    // 将observe计数器添加
    if(status <= 255) { // 计数器一个字节可以放下
      uint8_t count = status;
      coap_insert_option(ACKorNON, COAP_OPTION_OBSERVE, 1, &count); 
    } else if(status <= 65535) {
      uint8_t count[2];
      count[0] = status >> 8;
      count[1] = status & 0xff;
      coap_insert_option(ACKorNON, COAP_OPTION_OBSERVE, 2, count); 
    }
    pthread_mutex_lock(&nonMessageMutex);
    InsertNONMsg(ACKorNON, organizer_client_ctx, data, dataLength);
    pthread_mutex_unlock(&nonMessageMutex);
  } else {// 如果是observe的ACK则插入到ack队列中
   
    // 将observe选项添加， observe的值为0
    coap_insert_option(ACKorNON, COAP_OPTION_OBSERVE, 0, NULL); 
    pthread_mutex_lock(&organizer_DL_ACK_queue_mutex);
    int num = InsertACKMsg(ACKorNON, organizer_client_ctx, &Qorganizer_DLACKQueue, data, dataLength);
    pthread_mutex_unlock(&organizer_DL_ACK_queue_mutex);
    printf("prv_observe_result_callback, after insert ,队列中数据数量为：%d, mid is :  %d\n\n", num, ACKorNON->mid);
  }
}
void prv_result_callback(uint16_t clientID,lwm2m_uri_t * uriP, int status,
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
  printf("data : %d\n", dataLength);
  for(int i = 0; i < dataLength; i++) {
    printf("%c", data[i]);
  }
  printf("\n");
  printf("------------------\n");
  // 组装ACK，将ACK加入到队列中,这里的userdata会被释放所以需要进行一次拷贝操作
  coap_pdu_t *ACKfromUserData = (coap_pdu_t *)userData;
  coap_bin_const_t token = coap_pdu_get_token(ACKfromUserData);
  coap_pdu_t *ACK = coap_pdu_duplicate(ACKfromUserData, organizer_client_ctx, token.length, token.s, NULL);
  coap_pdu_set_mid(ACK, coap_pdu_get_mid(ACKfromUserData));
  // code
  coap_pdu_set_code(ACK, status);
    // 需要将文本类型进行添加
  if(format == 0) {
    coap_insert_option(ACK, COAP_OPTION_CONTENT_FORMAT, 0, NULL);
  } else if(format <= 255) {
    uint8_t NonFormat = format;
    coap_insert_option(ACK, COAP_OPTION_CONTENT_FORMAT, 1, &NonFormat);
  } else {
    uint8_t NonFormat[2];
    NonFormat[0] = (uint16_t)format >> 8;
    NonFormat[1] = (uint16_t)format & 0xff;
    coap_insert_option(ACK, COAP_OPTION_CONTENT_FORMAT, 2, NonFormat);
  }
  pthread_mutex_lock(&organizer_DL_ACK_queue_mutex);
  int num = InsertACKMsg(ACK, organizer_client_ctx, &Qorganizer_DLACKQueue, data, dataLength);
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

void findURI(lwm2m_uri_t *uri, uint8_t *clientID, coap_string_t *query) {
  int queryID = 1;
  int objectId = 65535;
  int instanceId = 65535;
  int resourceId = 65535;
  int resourceInstanceId = 65535;
  for(int i = 0; i < query->length; i++) {
    char a = query->s[i];
    if(queryID == 1 && a >= '0' && a <= '9') { // 首先开始寻找客户端ID
      (*clientID) *= 10;
      (*clientID) += a - '0';
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
  uri->objectId = objectId;
  uri->instanceId = instanceId;
  uri->resourceId = resourceId;
  uri->resourceInstanceId = resourceInstanceId;
}

void hnd_write(coap_resource_t *resource,coap_session_t *session, 
coap_pdu_t *request, coap_string_t *query, coap_pdu_t *response) {
  printf("***********enter into hnd write*****************\n\n");
  lwm2m_uri_t uri;
  uint8_t clientID = 0;
  // 从query中拿出uri和clientID
  findURI(&uri, &clientID, query);
  // 输出一些信息看看
  printf("client id is : %d\n", clientID);
  printf("uri.objectId is : %d\n", uri.objectId);
  printf("uri.instanceId : %d\n",uri.instanceId);
  printf("uri.resourceId : %d\n", uri.resourceId);
  printf("uri.resourceInstanceId : %d\n", uri.resourceInstanceId);
  // 找到write数据中的文本类型
  coap_opt_iterator_t opt_iter;
  coap_opt_t *option;
  int sumdelta = 0;
  uint16_t type = 0;
  coap_option_iterator_init(request, &opt_iter, COAP_OPT_ALL);
  while (option = coap_option_next(&opt_iter))
  {
    sumdelta += (*option) >> 4;
    if(sumdelta == COAP_OPTION_CONTENT_FORMAT) { // ACCEPT
    printf("enter into COAP_OPTION_CONTENT_FORMAT\n");
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
  printf("type is : %d\n", type);
  // 获取数据
  uint8_t *data;
  int dataLength;
  coap_get_data(request, &dataLength, &data);


  // 创建回调的ACK
  coap_pdu_t *ACK = coap_new_pdu(COAP_TYPE_CON, 0, organizer_client_session);
  // 回包的mid
  if(WAN_PROTOCOL == COAP_PROTO_TCP) {
    coap_pdu_set_mid(ACK, 0);
  } else if(WAN_PROTOCOL == COAP_PROTO_UDP) {
    coap_pdu_set_mid(ACK, coap_pdu_get_mid(request));
  }  
  // 回包的token
  coap_bin_const_t token = coap_pdu_get_token(request);
  coap_add_token(ACK, token.length, token.s);

  // lwm2m 接口
  lwm2m_dm_write(lwm2mH, clientID, &uri, type, data, dataLength, prv_result_callback, ACK);
  response->type = COAP_MESSAGE_NOT_SEND;
}


void hnd_read(coap_resource_t *resource,coap_session_t *session, 
coap_pdu_t *request, coap_string_t *query, coap_pdu_t *response) {

  printf("***********enter into hnd read*****************\n\n");
  lwm2m_uri_t uri;
  uint8_t clientID;
  findURI(&uri, &clientID, query);
  // observe
  bool checkObserve = false;
  // ACCEPT 类型，默认为text为0
  coap_opt_iterator_t opt_iter;
  coap_opt_t *option;
  int sumdelta = 0;
  coap_option_iterator_init(request, &opt_iter, COAP_OPT_ALL);
  uint16_t type = 0;
  while (option = coap_option_next(&opt_iter))
  {
    sumdelta += *option >> 4;
    if(sumdelta == 17) { // ACCEPT
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
    } else if(sumdelta == 6) { // observe
      checkObserve = true;
    }
  }
  // 配置accept参数
  lwm2m_client_t * targetP = (lwm2m_client_t *)lwm2m_list_find((lwm2m_list_t *)lwm2mH->clientList, clientID);
  targetP->format = type;
  // 创建ACK
  coap_pdu_t *ACK = coap_new_pdu(COAP_MESSAGE_ACK, 0, organizer_client_session);
  // 组装token
  coap_bin_const_t token = coap_pdu_get_token(request);
  coap_add_token(ACK, token.length, token.s);
  // mid
  if(WAN_PROTOCOL == COAP_PROTO_TCP) {
    coap_pdu_set_mid(ACK, 0);
    printf("after set mid is : %d\n", coap_pdu_get_mid(ACK));
  } else if(WAN_PROTOCOL == COAP_PROTO_UDP) {
    coap_pdu_set_mid(ACK, coap_pdu_get_mid(request));
  }
  printf("request mid is : %d\n", request->mid);

  if(checkObserve) {
    // wakaama observe
    printf("before wakaama observe, ACK mid is : %d\n", coap_pdu_get_mid(ACK));
    lwm2m_observe(lwm2mH, clientID, &uri,prv_observe_result_callback, ACK);
  } else {
    printf("before wakaama read\n");
    // wakaama的read方法， 将ACK信息进行传递
    lwm2m_dm_read(lwm2mH, clientID, &uri, prv_result_callback, ACK);
  }
  response->type = COAP_MESSAGE_NOT_SEND;
}

void init_organizer_client_resources (coap_context_t *ctx) {
  coap_resource_t *r;
  r = coap_resource_init(coap_make_str_const("read"), 0);
  coap_register_handler(r, COAP_REQUEST_GET, hnd_read);
  coap_add_resource(ctx, r);
  r = coap_resource_init(coap_make_str_const("write"), 0);
  coap_register_handler(r, COAP_REQUEST_PUT, hnd_write);
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

