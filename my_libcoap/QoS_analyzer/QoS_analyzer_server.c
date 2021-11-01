#include "QoS_analyzer_server_header.h"
#include <netinet/in.h>
#include "coap3/coap_net_internal.h"
#include "sessions/session_list.h"
#include "queue/ACK_queue.h"
#include "queue/QoS_analyzer_DL_queue.h"

// static void hnd_unknown_put(coap_resource_t *resource, coap_session_t *session,
// const coap_pdu_t *request, const coap_string_t *query, coap_pdu_t *response) {
//   /* Remove (void) definition if variable is used */
//   (void)resource;
//   coap_pdu_code_t req_code = coap_pdu_get_code(request);
//   coap_resource_t *r;
//   coap_string_t *uri_path;
//   /* get the uri_path - which will get used by coap_resource_init() */
//   uri_path = coap_get_uri_path(request);
//   if (!uri_path) {
//     coap_pdu_set_code(response, COAP_RESPONSE_CODE_NOT_FOUND);
//     return;
//   }
//   /* Check if new URI Path is valid */
//   // if (!check_url_fn (uri_path, req_code)) {
//   //   coap_pdu_set_code(response, COAP_RESPONSE_CODE_NOT_FOUND);
//   //   coap_delete_string(uri_path);
//   //   return;
//   // }
//   /*
//    * Create a resource to handle the new URI
//    * uri_path will get deleted when the resource is removed
//    */
//   // r = coap_resource_init((coap_str_const_t*)uri_path,
//   //       COAP_RESOURCE_FLAGS_RELEASE_URI | COAP_RESOURCE_FLAGS_NOTIFY_NON);
//   // coap_register_handler(r, COAP_REQUEST_PUT, hnd_put);
//   // coap_register_handler(r, COAP_REQUEST_DELETE, hnd_delete);
//   // /* We possibly want to Observe the GETs */
//   // coap_resource_set_get_observable(r, 1);
//   // coap_register_handler(r, COAP_REQUEST_GET, hnd_get);
//   // coap_add_resource(coap_session_get_context(session), r);
//   /* Do the PUT for this first call */
//   // hnd_put(r, session, request, query, response);
//   return;
// }



void
free_xmit_data_analyzer_server(coap_session_t *session COAP_UNUSED, void *app_ptr) {
  coap_free(app_ptr);
  return;
}

coap_context_t * setup_server_context() {
  char node[NI_MAXHOST] = "::";
  char port[NI_MAXSERV] = "5800";
  coap_context_t *ctx = NULL;
  int s;
  struct addrinfo hints;
  struct addrinfo *result, *rp;

  ctx = coap_new_context(NULL);
  if (!ctx) {
    return NULL;
  }
  /* Need PKI/RPK/PSK set up before we set up (D)TLS endpoints */
  // fill_keystore(ctx);

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */

  if(WAN_PROTOCOL == COAP_PROTO_TCP) {
      hints.ai_socktype = SOCK_STREAM;
  } else if(WAN_PROTOCOL == COAP_PROTO_UDP) {
    hints.ai_socktype = SOCK_DGRAM;
  }

  hints.ai_flags = AI_PASSIVE | AI_NUMERICHOST;

  s = getaddrinfo(node, port, &hints, &result);
  if ( s != 0 ) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    coap_free_context(ctx);
    return NULL;
  }

  /* iterate through results until success */
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    coap_address_t addr, addrs;
    coap_endpoint_t *ep_udp = NULL, *ep_dtls = NULL;

    if (rp->ai_addrlen <= (socklen_t)sizeof(addr.addr)) {
      coap_address_init(&addr);
      addr.size = (socklen_t)rp->ai_addrlen;
      memcpy(&addr.addr, rp->ai_addr, rp->ai_addrlen);
      addrs = addr;
      if (addr.addr.sa.sa_family == AF_INET) {
        uint16_t temp = ntohs(addr.addr.sin.sin_port) + 1;
        addrs.addr.sin.sin_port = htons(temp);
      } else if (addr.addr.sa.sa_family == AF_INET6) {
        uint16_t temp = ntohs(addr.addr.sin6.sin6_port) + 1;
        addrs.addr.sin6.sin6_port = htons(temp);
      } else {
        goto finish;
      }

      ep_udp = coap_new_endpoint(ctx, &addr, WAN_PROTOCOL);
      // if (ep_udp) {
      //   if (coap_dtls_is_supported() && (key_defined || cert_file)) {
      //     ep_dtls = coap_new_endpoint(ctx, &addrs, COAP_PROTO_DTLS);
      //     if (!ep_dtls)
      //       coap_log(LOG_CRIT, "cannot create DTLS endpoint\n");
      //   }
      // } else {
      //   coap_log(LOG_CRIT, "cannot create UDP endpoint\n");
      //   continue;
      // }
      // if (coap_tcp_is_supported()) {
      //   coap_endpoint_t *ep_tcp;
      //   ep_tcp = coap_new_endpoint(ctx, &addr, COAP_PROTO_TCP);
      //   if (ep_tcp) {
      //     if (coap_tls_is_supported() && (key_defined || cert_file)) {
      //       coap_endpoint_t *ep_tls;
      //       ep_tls = coap_new_endpoint(ctx, &addrs, COAP_PROTO_TLS);
      //       if (!ep_tls)
      //         coap_log(LOG_CRIT, "cannot create TLS endpoint\n");
      //     }
      //   } else {
      //     coap_log(LOG_CRIT, "cannot create TCP endpoint\n");
      //   }
      // }
      if (ep_udp)
        goto finish;
    }
  }

  fprintf(stderr, "no context available for interface '%s'\n", node);
  coap_free_context(ctx);
  ctx = NULL;

finish:
  freeaddrinfo(result);
  return ctx;
}

coap_context_t * setup_server_context_UDP (void) {
  coap_endpoint_t *endpoint;
  coap_address_t listen_addr;
  coap_context_t *context = coap_new_context(NULL);
  if (!context)
    return NULL;
  coap_address_init(&listen_addr);
  listen_addr.addr.sa.sa_family = AF_INET;
  listen_addr.addr.sin.sin_port = htons (analyzer_server_port);
  // endpoint = coap_new_endpoint(context, &listen_addr, COAP_PROTO_UDP);
  endpoint = coap_new_endpoint(context, &listen_addr, WAN_PROTOCOL);
  coap_context_set_keepalive(context, 0);
  if (!endpoint) {
    printf("QoS_analyzer， failed to create server endpoint!\n");
    coap_free_context(context);
    return NULL;
  } else {
  }

  return context;
}

coap_response_t
analyzer_server_message_handler(coap_session_t *session,const coap_pdu_t *sent,
const coap_pdu_t *received,const coap_mid_t id COAP_UNUSED)
{
  // 获取token
  coap_bin_const_t token =   coap_pdu_get_token(received);
  // 来的是ACK或者NON消息 
  coap_pdu_t *ACKorNON = NULL;
  coap_opt_iterator_t opt_iter;
  coap_opt_t *option;
  coap_option_iterator_init(received, &opt_iter, COAP_OPT_ALL);
  int sumDelta = 0;
  while ((option = coap_option_next(&opt_iter))) {
    sumDelta += *option >> 4;
    if(sumDelta == COAP_OPTION_OBSERVE) { // 是Non消息或者是observe的ack
      int length = coap_opt_length(option);
      uint8_t *value = coap_opt_value(option);
      if(length == 0) { // observe 的ack ，observe选项中的值为0
        ACKorNON = coap_new_pdu(COAP_MESSAGE_ACK, coap_pdu_get_code(received), analyzer_client_session);
        // 找到对应的ACK mid
        pthread_mutex_lock(&analyzer_midList_mutex);
        coap_mid_t mid = findMidByTokenAndDel(token);
        pthread_mutex_unlock(&analyzer_midList_mutex);
        if(mid == -1) {
          return COAP_RESPONSE_NULL;
        }
        // 设置回包中的mid
        coap_pdu_set_mid(ACKorNON, mid);
        // 设置回包中的token
        coap_add_token(ACKorNON, token.length, token.s);
        // 添加选项
        coap_add_option(ACKorNON, COAP_OPTION_OBSERVE, 0, NULL);
      } else { // non消息
        printf("这是non消息\n");
        ACKorNON = coap_new_pdu(COAP_MESSAGE_NON, coap_pdu_get_code(received), analyzer_client_session);
        // 从观测列表中寻找mid
        // 由于记录mid的表是anjay表，所以需要加organizerNodeMutex锁
        pthread_mutex_lock(&organizerNodeMutex);
        int mid = findAndUpdateMidByToken(token);
        pthread_mutex_unlock(&organizerNodeMutex);
        // 设置回包中的mid
        coap_pdu_set_mid(ACKorNON, mid);
        // 设置回包中的token
        coap_add_token(ACKorNON, token.length, token.s);
        // 将observe计数器添加
        coap_insert_option(ACKorNON, COAP_OPTION_OBSERVE, length, value);
      }
    } else if(sumDelta == COAP_OPTION_CONTENT_FORMAT) { // content-format
      if(ACKorNON == NULL) { // 如果是普通的ACK应该是尚未进行初始化的，例如read方法
        ACKorNON = coap_new_pdu(COAP_MESSAGE_ACK, coap_pdu_get_code(received), analyzer_client_session);
        // 以删除mid-token项的方式寻找mid
        pthread_mutex_lock(&analyzer_midList_mutex);
        coap_mid_t mid = findMidByTokenAndDel(token);
        printf("find mid from midList : %d\n", mid);
        pthread_mutex_unlock(&analyzer_midList_mutex);
        if (mid == -1)
        {
          return COAP_RESPONSE_NULL;
        }
        // 设置回包中的mid
        coap_pdu_set_mid(ACKorNON, mid);
        // 设置回包中的token
        coap_add_token(ACKorNON, token.length, token.s);
      }
      coap_add_option(ACKorNON, COAP_OPTION_CONTENT_FORMAT, coap_opt_length(option), coap_opt_value(option));
    }
  }
  // 获取data
  int LengthOfData;
  uint8_t *Data;
  coap_get_data(received, &LengthOfData, &Data);
  // 设置回包中payload
  coap_add_data(ACKorNON, LengthOfData, Data);
  // 发送
  coap_send_large(analyzer_client_session, ACKorNON);
  return COAP_RESPONSE_NULL;
}

bool updateSession(coap_session_t *newSession, int LengthOfGlobalID, uint8_t *GlobalID, int newInternalID) {
  coap_session_t *oldSession;
  int oldInternalID = findSessionAndInternalIDByGlobalID(GlobalID, LengthOfGlobalID, &oldSession);
  if(oldSession != newSession) {
    printf("oldSession 与 newSession 不同\n");
    return true;
  }
  if(oldInternalID != newInternalID) {
    printf("oldInternalID 与 newInternalID 不同\n");
    printf("oldInternalID is : %d\n", oldInternalID);
    printf("newInternalID is : %d\n", newInternalID);
    return true;
  }
  return false;
}

void hnd_post_update(coap_resource_t *resource, coap_session_t *session, 
coap_pdu_t *request, coap_string_t *query, coap_pdu_t *response) {
  printf("---------enter into hnd post update---------\n");

  // query计数器
  int num = 0;
  // 内部id
  int internalID = 0;
  // 全局id
  int lengthOfGlobalID;
  uint8_t *GlobalID;
  // 首先拿internalID 和 GlobalID
  coap_opt_iterator_t opt_iter;
  coap_opt_t *option;
  int sumdelta = 0;
  coap_option_iterator_init(request, &opt_iter, COAP_OPT_ALL);
  while (option = coap_option_next(&opt_iter)) {
    sumdelta += (*option) >> 4;
    int length = coap_opt_length(option);
    uint8_t *value = coap_opt_value(option);
    if(sumdelta == COAP_OPTION_URI_QUERY) {
      if(num == 0) { // 更新con中规定第一个query是internalid
        num++;
        for(int i = 0; i < length; i++) {
          internalID *= 10;
          internalID += value[i] - '0';
        }
      } else if (num == 1) { // 更新中con中规定第二个query是GlobalID
        lengthOfGlobalID = length;
        GlobalID = (uint8_t *) malloc (sizeof(uint8_t) * length);
        memcpy(GlobalID, value, length);
      }
    }
  }
  // 这里尚未明确session是否会发生变化，直觉上认定应当判断
  
  if(!updateSession(session, lengthOfGlobalID, GlobalID, internalID)) {
    printf("无需进行更新\n");
  }

  // 将更新消息转发到服务器
  coap_pdu_t *pdu = coap_new_pdu(COAP_MESSAGE_CON, coap_pdu_get_code(request), analyzer_client_session);
  
  // 添加token
  coap_bin_const_t token = coap_pdu_get_token(request);
  coap_add_token(pdu, token.length, token.s);

  // uri path
  uint8_t rd[] = "rd";
  coap_insert_option(pdu, COAP_OPTION_URI_PATH, strlen(rd), rd);
  coap_insert_option(pdu, COAP_OPTION_URI_PATH, lengthOfGlobalID, GlobalID);

  // 发送
  int mid = coap_send_large(analyzer_client_session, pdu);

  // 接受更新消息并进行处理，主要是将session添加
  while (1)
  {
    pthread_mutex_lock(&analyzer_UL_ACKUNHSNDLED_queue_mutex);
    coap_pdu_t *ackToOrganizer = GetAndDelACKQueueByMid(mid, &ULACKUnhandledQueue);
    pthread_mutex_unlock(&analyzer_UL_ACKUNHSNDLED_queue_mutex);

    if(ackToOrganizer == NULL) {
      continue;
    }
    // if(WAN_PROTOCOL == COAP_PROTO_TCP) {
    //   // 如果是TCP协议则无需回包
    //   break;
    // }
    // mid
    if(WAN_PROTOCOL == COAP_PROTO_UDP) {
      coap_pdu_set_mid(ackToOrganizer, coap_pdu_get_mid(request));
    }
    // token
    int checkToken = coap_update_token(ackToOrganizer, token.length, token.s);
    printf("after add token, checkToken is : %d\n", checkToken);
    // 加入到发送队列中
    pthread_mutex_lock(&analyzer_UL_ACK_queue_mutex);
    int check = InsertACKMsg(ackToOrganizer, session, &ULACKQueue);
    pthread_mutex_unlock(&analyzer_UL_ACK_queue_mutex);
    printf("the check is : %d\n", check);
    break;
  }
  
  response->type = COAP_MESSAGE_NOT_SEND;
  return;
} 




void init_analyzer_server_resources (coap_context_t *ctx) {
  coap_resource_t *r;
  r = coap_resource_init(coap_make_str_const("rd"), 0);
  coap_register_handler(r, COAP_REQUEST_POST, hnd_post_reg);
  coap_add_resource(ctx, r);

  r = coap_resource_init(coap_make_str_const("update"), 0);
  coap_register_handler(r, COAP_REQUEST_POST, hnd_post_update);
  coap_add_resource(ctx, r);

  // 注册回调方法，用于下行数据ACK接收
  coap_register_response_handler(ctx, analyzer_server_message_handler);
}



static void
hnd_post_reg(
coap_resource_t *resource,
coap_session_t *session, 
coap_pdu_t *request, 
coap_string_t *query, 
coap_pdu_t *response) {
  printf("\n\n|---------------------------------------------|\n");
  printf("|新的注册消息到达QoS_analyzer\n");
  printf("|---------------------------------------------|\n");

  // 对请求信息进行输出
  // data长度
  int length_of_data;
  uint8_t *data;
  coap_get_data(request,&length_of_data, &data);
  printf("|len of data(payload) : %d\n", length_of_data);
  // 输出data
  printf("|data is :");
  char *check_data = data;
  for(int i = 0; i < length_of_data; i++)
  {
    printf("%c", *check_data);
    check_data++;
  }
  printf("\n");
  // 输出query
  printf("|query is :");
  char *check_query = query->s;
  for(int i = 0; i < query->length; i++){
    printf("%c",*check_query);
    check_query++;
  }
  printf("\n");

  // 转发到乐山
  // 创建 pdu
  coap_pdu_t  *pdu = coap_new_pdu(request->type, request->code, analyzer_client_session);
  if(pdu == NULL) {
    printf("create pdu failed !\n");
  }else {
  }
  // token
  coap_add_token(pdu, request->token_length, request->token);
  // CONTENT_TYPE
  unsigned char buf[4];
  coap_add_option(pdu, COAP_OPTION_CONTENT_TYPE,coap_encode_var_safe(buf, sizeof(buf),
    COAP_MEDIATYPE_APPLICATION_LINK_FORMAT),buf);
  // uri_path
  uint8_t uri_path[] = "rd";
  coap_optlist_t *analyzer_client_request_option = 
  coap_new_optlist(COAP_OPTION_URI_PATH, sizeof(uri_path)-1, uri_path);
  // uri_query, 以前的垃圾写法，不需要遍历字符串，用迭代器就可以了
  // 创建内部ID
  int InternalID = -1;
  int number_of_query = 0;
  if(query->length != 0)
  {
    number_of_query++;
    uint8_t *query_string_begin = query->s;
    uint8_t *query_string_end = query->s; 
    int option_len = 0;
    printf("|---------------------------------------------|\n");
    for(int i = 0; i < query->length; i++)
    {
      if(*query_string_end == '&')
      {
        // 从query中拿ID
        if( *query_string_begin == 'I' && *(query_string_begin + 1) == 'D'){
          // 找ID
          query_string_begin += 3; // 跳过“ID=”三个字符
          int num = 0;
          while (*query_string_begin >= '0' && *query_string_begin <= '9')
          {
            num *= 10;
            num += *query_string_begin - '0';
            query_string_begin++;
          }
          InternalID = num;
        } else { // 普通的query
          coap_optlist_t *tem_opt = coap_new_optlist(COAP_OPTION_URI_QUERY, option_len, query_string_begin);
          coap_insert_optlist(&analyzer_client_request_option, tem_opt);
          printf("|\ttem_opt len : %d\n", tem_opt->length);
          uint8_t *check_option = tem_opt -> data;
          int check_len = tem_opt -> length;
          printf("|\t");
          for(int i = 0; i < check_len; i++)
          {
            printf("%c ", *check_option);
            check_option++;
          }
          printf("\n|---------------------------------------------|\n");
        }
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
    // 末端不做ID处理，因为ID=？默认后面会接其他query
    coap_optlist_t *tem_opt = coap_new_optlist(COAP_OPTION_URI_QUERY, option_len, query_string_begin);
    coap_insert_optlist(&analyzer_client_request_option, tem_opt);
    printf("|\ttem_opt len : %d\n", tem_opt->length);
    uint8_t *check_option = tem_opt -> data;
    int check_len = tem_opt -> length;
    printf("|\t");
    for(int i = 0; i < check_len; i++)
    {
      printf("%c ", *check_option);
      check_option++;
    }
    printf("\n|---------------------------------------------|\n");
  }
  printf("|the InternalID is : %d\n", InternalID);
  printf("|---------------------------------------------|\n");

  // 讲optlist添加到pdu中
  coap_add_optlist_pdu(pdu, &analyzer_client_request_option);
  // payload
  int payload_len = request->used_size - (request->data - request->token);

  int dataLength = 0;
  uint8_t *requestData;
  coap_get_data(request, &dataLength, &requestData);
  coap_add_data(pdu, dataLength, requestData);
  coap_mid_t mid_con = coap_send_large(analyzer_client_session, pdu);

  // 拿ack中globalID消息
  uint8_t* GlobalID;
  int Length;
  // 处理回包
  coap_pdu_t *responseToOrganizer;
  while (true)
  {
    // 获取ACK
    pthread_mutex_lock(&analyzer_UL_ACKUNHSNDLED_queue_mutex);
    coap_pdu_t* ack = GetAndDelACKQueueByMid(mid_con, &ULACKUnhandledQueue);
    pthread_mutex_unlock(&analyzer_UL_ACKUNHSNDLED_queue_mutex);

    if(ack == NULL) {
      continue;
    }
    // 初始化回包
    responseToOrganizer = coap_new_pdu(coap_pdu_get_type(ack), coap_pdu_get_code(ack), session);

    //设置mid
    coap_pdu_set_mid(responseToOrganizer, coap_pdu_get_mid(request));

    // 设置回包中token
    coap_bin_const_t ackToken = coap_pdu_get_token(ack);
    coap_add_token(responseToOrganizer, ackToken.length,ackToken.s);
    // 添加option
    int i = 1;
    coap_opt_iterator_t opt_iter;
    coap_opt_t *option;
    coap_option_iterator_init(ack, &opt_iter, COAP_OPT_ALL);
    while ((option = coap_option_next(&opt_iter))) // 将/rd 替换为内部ID
    {

      if(i == 1) { //第一个为rd
        uint8_t InternalIDToString[10];
        sprintf(InternalIDToString, "%d", InternalID);
        Length = strlen(InternalIDToString);
        GlobalID = InternalIDToString;
      } else
      { // 第二个为GlobalID
        Length = coap_opt_length(option);
        GlobalID = coap_opt_value(option);
      }
      coap_add_option(responseToOrganizer, opt_iter.number, Length, GlobalID);
      i++;
    }
    break;
  }
  // 将responseToOranizer 加入到下行ack队列中
  pthread_mutex_lock(&analyzer_UL_ACK_queue_mutex);
  int check = InsertACKMsg(responseToOrganizer, session, &ULACKQueue);  
  pthread_mutex_unlock(&analyzer_UL_ACK_queue_mutex);
  printf("after insert into ULACKQueue, check is : %d\n", check);

  printf("\n\n");
  printf("|---------------------------------------------|\n");

  // 以上完成了注册信息的转发，下面讲对“session—InternalID—GlobalID”建立映射关系
  pthread_mutex_lock(&organizerNodeMutex);
  // 注册时回携带初始mid，当基于tcp传输时为0
  anjay_node* anjay_node_p = handle_anjay_node(session, InternalID, GlobalID, Length, coap_pdu_get_mid(request));
  pthread_mutex_unlock(&organizerNodeMutex);
  organizer_node *p = organizer_node_head.next;
  int numOfOrganizer = 0;
  while (p != NULL)
  {
    anjay_node* anjay = p->anjay_client_node;
    int numOfanjay = 0;
    while (anjay != NULL)
    {
      numOfanjay++;
      anjay = anjay->next;
    }
    numOfOrganizer++;
    printf("|第%d个organizer 中 anjay数量为:%d, something is session : %d\n", numOfOrganizer, numOfanjay, p->session->sock.fd);
    // 遍历输出全部anjay信息
    anjay = p->anjay_client_node;
    while (anjay != NULL)
    {
      printf("|   >\tInternalID : %d\n", anjay->InternalID);
      printf("|   >\tGlobalIDSize : %d\n", anjay->GlobalIDSize);
      printf("|   >\tGlobalID : ");
      for(int j = 0; j < anjay->GlobalIDSize; j++){
        printf("%c", anjay->GlobalID[j]);
      }
      printf("\n");
      printf("|-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.\n");
      anjay = anjay->next;    
    }
    p = p->next;
  }
  printf("|当前QoS_organizer数量为 : %d\n",numOfOrganizer);
  printf("|---------------------------------------------|\n");

  // 不在本方法中进行返回
  response->type = COAP_MESSAGE_NOT_SEND;
}


