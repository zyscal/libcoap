#include "QoS_analyzer_server_header.h"
#include <netinet/in.h>
#include "coap3/coap_net_internal.h"
#include "sessions/session_list.h"
#include "queue/ACK_queue.h"
#include "queue/QoS_analyzer_DL_queue.h"
static void hnd_post_unknown(coap_resource_t *resource, coap_session_t *session,
const coap_pdu_t *request, const coap_string_t *query, coap_pdu_t *response){
  printf("enter into hnd_post_unknow\n");


}

static void hnd_unknown_put(coap_resource_t *resource, coap_session_t *session,
const coap_pdu_t *request, const coap_string_t *query, coap_pdu_t *response) {
  /* Remove (void) definition if variable is used */
  (void)resource;
  coap_pdu_code_t req_code = coap_pdu_get_code(request);
  coap_resource_t *r;
  coap_string_t *uri_path;
  /* get the uri_path - which will get used by coap_resource_init() */
  uri_path = coap_get_uri_path(request);
  if (!uri_path) {
    coap_pdu_set_code(response, COAP_RESPONSE_CODE_NOT_FOUND);
    return;
  }
  /* Check if new URI Path is valid */
  // if (!check_url_fn (uri_path, req_code)) {
  //   coap_pdu_set_code(response, COAP_RESPONSE_CODE_NOT_FOUND);
  //   coap_delete_string(uri_path);
  //   return;
  // }
  /*
   * Create a resource to handle the new URI
   * uri_path will get deleted when the resource is removed
   */
  // r = coap_resource_init((coap_str_const_t*)uri_path,
  //       COAP_RESOURCE_FLAGS_RELEASE_URI | COAP_RESOURCE_FLAGS_NOTIFY_NON);
  // coap_register_handler(r, COAP_REQUEST_PUT, hnd_put);
  // coap_register_handler(r, COAP_REQUEST_DELETE, hnd_delete);
  // /* We possibly want to Observe the GETs */
  // coap_resource_set_get_observable(r, 1);
  // coap_register_handler(r, COAP_REQUEST_GET, hnd_get);
  // coap_add_resource(coap_session_get_context(session), r);
  /* Do the PUT for this first call */
  // hnd_put(r, session, request, query, response);
  return;
}



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

  printf("enter into analyzer_server_message_handler\n\n");
  // 来的都是ACK，并将结果转发到leshan
  coap_pdu_t *ACK = coap_new_pdu(COAP_MESSAGE_ACK, coap_pdu_get_code(received), analyzer_client_session);
  
  // 获取token，通过token找到mid，因为基于tcp传输不携带mid
  coap_bin_const_t token =   coap_pdu_get_token(received);
  pthread_mutex_lock(&analyzer_midList_mutex);
  coap_mid_t mid = findMidByToken(token);
  pthread_mutex_unlock(&analyzer_midList_mutex);

  // 设置回包中的mid
  coap_pdu_set_mid(ACK, mid);

  // 设置回包中的token
  coap_add_token(ACK, token.length, token.s);

  // 设置文本类型
  coap_opt_iterator_t opt_iter;
  coap_opt_t *option;
  coap_option_iterator_init(received, &opt_iter, COAP_OPT_ALL);
  int sumDelta = 0;
  while ((option = coap_option_next(&opt_iter))) {
    sumDelta += *option >> 4;
    if(sumDelta == 12) { // content-format
      coap_add_option(ACK, COAP_OPTION_CONTENT_FORMAT, coap_opt_length(option), coap_opt_value(option));
    }
  }

  

  // 获取data
  int LengthOfData;
  uint8_t *Data;
  coap_get_data(received, &LengthOfData, &Data);
  // 设置回包中payload
  coap_add_data(ACK, LengthOfData, Data);
  // 发送
  coap_send_large(analyzer_client_session, ACK);
  coap_response_t ans;
  return ans;
}


void init_analyzer_server_resources (coap_context_t *ctx) {
  coap_resource_t *r;
  r = coap_resource_init(coap_make_str_const("rd"), 0);
  coap_register_handler(r, COAP_REQUEST_POST, hnd_post_reg);
  coap_add_resource(ctx, r);

  // 注册回调方法，用于下行数据ACK接收
  coap_register_response_handler(ctx, analyzer_server_message_handler);
}



int check_segment_Q_A_S(const uint8_t *s, size_t length) {
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


void decode_segment(const uint8_t *seg, size_t length, unsigned char *buf) {
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

int cmdline_input_Q_A_S(char *text, coap_string_t *buf) {
  int len;
  len = check_segment_Q_A_S((unsigned char *)text, strlen(text));
  if (len < 0)
    return 0;
  buf->s = (unsigned char *)coap_malloc(len);
  if (!buf->s)
  {
    return 0;
  }
  buf->length = len;
  decode_segment((unsigned char *)text, strlen(text), buf->s);
  return 1;
}

// // 用于判断option中是否存在rd，存在rd则将其替换为InternalID
// bool match(uint8_t* a, int lengtha, uint8_t* b, lengthb) {
//   if(lengtha != lengthb) {
//     return false;
//   }
//   for(int i = 0; i < lengtha; i++){
//     if(a[i] != b[i]) {
//       return false;
//     }
//   }
//   return true;
// }

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
  // printf("payload size : %d\n", payload_len);

  coap_string_t payload;
  cmdline_input_Q_A_S(request->data, &payload);
  coap_add_data_large_request(analyzer_client_session, pdu, payload.length, payload.s,
  free_xmit_data_analyzer_server,  payload.s);
  coap_mid_t mid_con = coap_send_large(analyzer_client_session, pdu);

  // 拿ack中globalID消息
  uint8_t* GlobalID;
  int Length;
  while (true)
  {
    // 获取ACK
    coap_pdu_t* ack = GetAndDelACKQueueFront(mid_con, &ULACKQueue ,&analyzer_UL_ACK_queue_mutex);    /* code */
    if(ack == NULL) {
      continue;
    }
    coap_pdu_set_code(response, coap_pdu_get_code(ack));
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
      coap_add_option(response, opt_iter.number,Length,GlobalID);
      i++;
    }
    break;
  }

  printf("\n\n");
  printf("|---------------------------------------------|\n");

  // 以上完成了注册信息的转发，下面讲对“session—InternalID—GlobalID”建立映射关系
  anjay_node* anjay_node_p = handle_anjay_node(session, InternalID, GlobalID, Length);
  printf("after anjay_node_p insert\n");
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

  // coap_endpoint_t *tem_endpoint = session->endpoint;
  // int i = 0;
  // while (tem_endpoint != NULL)
  // {
  //   printf("第%d个endpoint地址为：%d, endpoint中ctx地址：%d，endpoint中session ： %d\n", i,
  //   tem_endpoint, tem_endpoint->context, tem_endpoint->sessions);
  //   i++;
  //   tem_endpoint  = tem_endpoint->next;
  // }
  // printf("session addr is : %d\n", session);
  // printf("session 中 endpoint : %d\n", session->endpoint);
  // printf("session 中 endpoint 中 session ： %d\n", session->endpoint->sessions);
  // printf("session 中 endpoint 中 ctx ： %d\n", session->endpoint->context);
  // printf("session ctx : %d\n", session->context);
  // printf("session ctx 中 endpoint: %d\n", session->context->endpoint);
}


