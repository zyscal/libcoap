# **项目概述**
## 项目名称 ： 《基于 NB-IoT 的配电物联网 QoS 管理机制与中间件研究》
## 语雀文档地址 ： https://hitcps.yuque.com/hit_cps/wdlidu/aygtr7
## 项目开源地址 ： https://github.com/zyscal/libcoap
## 项目开始日期 ： 2021.5
## 参考《RFC7252》《RFC8323》
<br/><br/>

# **核心模块**
> ## QoS组织者（QoS_organizer）
>* **描述：** 存在于边缘侧，接受、管理同一局域网下anjay客户端设备。整个系统中将会存在多个边缘设备
>* **文件目录：** my_libcoap/QoS_organizer
>* **运行指令：** ./wakaama_server -l 6100 -w 5710
>* **指令解释：** -l 参数描述了内部服务器线程监听的端口，-w 参数描述了内部客户端线程占用的端口号，用于与云端通讯。本方法便于端口管理，在同一机器上运行可以避免端口占用问题。

> ## QoS解析者（QoS_analyzer）
>* **描述：** 存在与云端，类似于反向代理服务器，用于解析网络中携带QoS的数据信息，解析成服务器可以识别的报文形式。
>* **文件目录** my_libcoap/QoS_analyzer
>* **运行指令** ./QoS_analyzer

<br/><br/>

# QoS_analyzer表结构

### anjay_node 表 
>| 类型 | 字段  | 解释 |
>| :--: | :--: | :--: |
>| int | InternalID | 内部ID |
>| int | GlobalIDSize | 全局ID长度 |
>| char* | GlobalID | 全局ID |
>| int | MID | 虚拟的mid |
>| anjay_node* | next | 属于同一个QoS_organizer的下个anjay设备 |

<br>

### organizer_node 表 
>| 类型 | 字段  | 解释 |
>| :--: | :--: | :--: |
>| coap_session_t* | session | 存储边缘测会话，通过该字段可以将数据推送到QoS_organizer |
>| anjay_node* | anjay_client_node | QoS_organizer下终端设备表头 |
>| organizer_node* | next | 指向下一个QoS_organizer |

<br>

### DLQueue表:下行数据优先队列
>| 类型 | 字段  | 解释 |
>| :--: | :--: | :--: |
>| coap_pdu_t* | data | 下行的数据包 |
>| coap_session_t* | session | 目标边缘会话 |
>| uint8_t | level | 数据包等级 |
>| DLQueue* | next | 指向下一个数据包 |

<br>

### midList表:mid与token的映射关系表
>| 类型 | 字段  | 解释 |
>| :--: | :--: | :--: |
>| coap_mid_t | mid | 一个下行请求的mid |
>| uint8_t* | token | 该请求对应的token |
>| int | tokenLength | token长度 |
>| int | count | 重传次数 |
>| midList* | next | 下一个 |

<br>

### observeList表: observe事件与token、mid映射
>| 类型 | 字段  | 解释 |
>| :--: | :--: | :--: |
>| uint8_t* | token | observe唯一token |
>| uint8_t | tokenLength | token长度 |
>| anjay_node* | anjay | obverse事件对应的anjay设备（拿mid） |
>| observeList * | next | 下一个 |

<br>

### ACKQueue表: 做出两个实例ULACKQueue,DLACKQueue
>| 类型 | 字段  | 解释 |
>| :--: | :--: | :--: |
>| coap_pdu_t* | data | ACK数据 |
>| ACKQueue* | next | 下一个 |


<br><br>

# QoS_organizer表结构

### observeThings表：记录每个设备下全部观测事件
>| 类型 | 字段  | 解释 |
>| :--: | :--: | :--: |
>| lwm2m_uri_t | URI | 观测事件 |
>| uint8_t* | token | token |
>| int | tokenLength | token长度 |
>| observeThings | next | 下一个 |

<br>

### observeList表：记录设备的观测事件
>| 类型 | 字段  | 解释 |
>| :--: | :--: | :--: |
>| uint16_t | clientID | 设备内部ID |
>| observeThings* | things | 观测事件 |
>| observeList* | next | 下一个 |

<br>

### nonQueue表：数据上报队列
>| 类型 | 字段  | 解释 |
>| :--: | :--: | :--: |
>| coap_pdu_t* | data | non消息 |
>| uint8_t* | payload | 上报的数据内容 |
>| int | Length | 上报的数据长度 |
>| unsigned int | format | 数据格式 |
>| nonQueue* | next | 下一个 |

<br>

### ACKQueue表：下行数据的ACK队列
>| 类型 | 字段  | 解释 |
>| :--: | :--: | :--: |
>| coap_pdu_t* | data | ACK消息 |
>| uint8_t* | payload | ACK数据 |
>| int | Length | 数据长度 |
>| ACKQueue* | next | 下一个 |





<br/><br/>
# 进度

## **2021.10.5进度**
>* 完成注册相关工作
**注册流程待完善**

## **2021.10.7进度**
>* QoS_analyzer 云端部署，通过WAN_PROTOCOL字段配置网通讯协议


## **2021.10.12进度**
>* 完成服务端的 **读取事件** , 具体流程如下：
>   * 扩展leshan LwM2M接口实现代码（ **CoapRequestSetter** ），支持read方法携带GlobalID，例如 : /uri_path/GlobalID/ACCEPT_TYPE
>   * QoS_analyzer_client中使用 **hnd_get_unknown()** 方法接收下行读取请求：
>       * 判断为observe：
>       * 判断为read：
>           1. 拿GlobalID，通过GlobalID找到对应边缘的会话session和InternalID，使用方法：**findSessionAndInternalIDByGlobalID()**
>           2. 包装下行数据包：
>               * 将mid-token加入到 **midlist** 表中，目的在回包时通过token找到mid（因为tcp中不携带mid）
>               * 为下行数据包添加：mid、token、uri（/read）、ACCEPT类型
>               * 使用 **非阻塞方法** 发送下行数据：使用 **InsertDLMsg()** 方法将下行数据添加到 **DLQueue** 队列中。同时，通过拓展 **COAP_MESSAGE_NOT_SEND** type类型禁止QoS_analyzer向乐山回empty包。
>   * QoS_analyzer 中发送线程调用QoS目录下符合当前QoS要求的send方法发送到QoS_organizer, 例如默认: **analyzer_DL_simple_sender()**
>   * QoS_organizer_client 中实现/read接口（方法名 **hnd_read()**），所有下行读取数据均发送到该接口：
>       1. 解析出InternalID、objectId、instanceId、resourceId、resourceInstanceId、ACCEPT类型。
>       2. **阻塞读取anjay消息** ： 调用wakaama开源接口 **lwm2m_dm_read**， 设置回调方法，在回调中组装ACK报文，并将ACK筒骨
>       3. 通过 **GetAndDelACKQueueFront()** 获取ACK报文，并配置回包
>   * QoS_analyzer_server 转发 ACK 
>       1. 通过 **analyzer_server_message_handler** 方法获取回包ACK
>       2. 根据 **token** 从 **midlist** 拿mid信息
>       3. 设置文本类型
>       4. 配置数据data
>       5. **阻塞发送**


## **2021.10.19进度**
>* 完成/observe接口开发
>   * QoS_analyzer_client 通过 **hnd_get_unknow()** 接收下行observe事件：
>       * 当判断为observe后：
>           1. 与read相仿，获取session，token，mid等操作
>           2. 需要维护更新ObserveList，目的为了让上行的non消息获取token对应的mid消息。
>   * QoS_organizer目前同样使用 **hnd_read()** 接口接收，通过判别是否是observe事件，使用不同的回调方法，具体参考wakaama的read、observe方法：
>       * **prv_observe_result_callback** 为observe事件的回调方法，该方法会在一下几种场景中被调用：
>           1. observe命令的anjay ack回包：当观测队列中尚无该观测事件，则为ACK，否则为non
>           2. 重复观测同一时间：当重复观测同一个事件时候，该方法会被唤起两次，第一次status为 **COAP_RESPONSE_CODE_DELETED**，需要从observelist中删除对应事件。第二次为1.的情况。
>           3. non消息上报：当non消息上报时需要注意status描述了observe计数器需要填充 **COAP_OPTION_OBSERVE** 选项的值。 
>* 完成non数据上报接口开发
>* 在coap over tcp/udp 场景下手动压测测试现有架构，修复多处异常：
>   * 重传问题：使用count记录con数量
>   * 多设备mid问题：在anjay表中添加mid信息，当基于tcp链路通讯时，使用模拟的自增mid