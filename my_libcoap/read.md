# **项目概述**
## 项目名称 ： 《基于 NB-IoT 的配电物联网 QoS 管理机制与中间件研究》
## 语雀文档地址 ： https://hitcps.yuque.com/hit_cps/wdlidu/aygtr7
## 项目开源地址 ： https://github.com/zyscal/libcoap
## 项目开始日期 ： 2021.6
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
