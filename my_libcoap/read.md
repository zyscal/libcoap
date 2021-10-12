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


### **2021.10.5进度**
>* 完成注册相关工作
**注册流程待完善**

### **2021.10.7进度**
>* QoS_analyzer 云端部署，通过WAN_PROTOCOL字段配置网通讯协议


### **2021.10.12进度**
>* 完成服务端的读取事件, 具体流程如下：
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
