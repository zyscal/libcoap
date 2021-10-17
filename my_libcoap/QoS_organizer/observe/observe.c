#include "observe.h"

bool URImatch(lwm2m_uri_t URI1, lwm2m_uri_t URI2) {
    if(URI1.objectId == URI2.objectId && URI1.instanceId == URI2.instanceId &&
    URI1.resourceId == URI2.resourceId && URI1.resourceInstanceId == URI2.resourceInstanceId) {
        return true; 
    }
    return false;
}
bool InsertObserveThing (observeList *node, lwm2m_uri_t URI, coap_bin_const_t token, uint8_t **nonToken, int *nonTokenLength) {
    
    if(node->things == NULL) { // 当前clientid节点下没有观测的内容
        printf("当前clientid节点下没有观测的内容\n");
        node->things = (observeThings *) malloc (sizeof(observeThings));
        node->things->URI = URI;
        node->things->tokenLength = token.length;
        node->things->token = (uint8_t *) malloc (sizeof(uint8_t) * token.length);
        for(int i = 0; i < token.length; i++) {
            (node->things->token)[i] = token.s[i];
        }
        node->things->next = NULL;
        return true;
    } else { // 当前节点下有观测的内容
        printf("当前clientID下已经存在观测内容\n");
        observeThings *p = node->things;
        while (p->next != NULL)
        {
            // 查看当前uri是否已经存在，如果存在说明当前的消息为信息上报
            if(URImatch(p->URI, URI)) {
                *nonTokenLength = p->tokenLength;
                *nonToken = (uint8_t *) malloc(sizeof(uint8_t) * p->tokenLength);
                for(int i = 0; i < p->tokenLength; i++) {
                    (*nonToken)[i] = (p->token)[i];
                }
                return false;
            }
            p = p->next;
        }
        if(URImatch(p->URI, URI)) {
            *nonTokenLength = p->tokenLength;
            *nonToken = (uint8_t *) malloc(sizeof(uint8_t) * p->tokenLength);
            for(int i = 0; i < p->tokenLength; i++) {
                (*nonToken)[i] = (p->token)[i];
            }
            return false;
        }
        p->next = (observeThings *) malloc (sizeof(observeThings));
        p->next->URI = URI;
        p->next->tokenLength = token.length;
        p->next->token = (uint8_t *)malloc (sizeof(uint8_t) * token.length);
        for(int i = 0; i < token.length; i++) {
            (p->next->token)[i] = token.s[i];
        }
        p->next->next = NULL;
        return true;
    }
    return false;
}

bool InsertObserve (uint16_t clientID, lwm2m_uri_t URI, coap_bin_const_t token, uint8_t **nonToken, int *nonTokenLength) {
    if(observeListHead == NULL) { // 观测表中为空，需要添加clientID为key 的表头
        printf("当前观测表为空\n");
        observeListHead = (observeList *) malloc (sizeof (observeList));
        observeListHead->clientID = clientID;
        observeListHead->next = NULL;
        observeListHead->things = NULL;
        return InsertObserveThing(observeListHead, URI, token, nonToken, nonTokenLength);
    } else { // 观测表中存在内容，使用双指针将新的clientID添加
        printf("当前观测表不为空\n");
        observeList *pFront = observeListHead;
        observeList *pBack = observeListHead->next;
        while (pBack != NULL)
        {
            if(pFront->clientID == clientID) {
                return InsertObserveThing(pFront, URI, token, nonToken, nonTokenLength);
            } else {
                pBack = pBack->next;
                pFront = pFront->next;
            }
        }
        if(pFront->clientID == clientID) {
            return InsertObserveThing(pFront, URI, token, nonToken, nonTokenLength);
        }
        pFront->next = (observeList *) malloc (sizeof(observeList));
        pFront->next->things = NULL;
        pFront->next->next = NULL;
        return InsertObserveThing(pFront->next, URI, token, nonToken, nonTokenLength);
    }
}


bool DeleteObserve (uint16_t clientID, lwm2m_uri_t URI, coap_bin_const_t token){
    observeList *p = observeListHead;
    while (p != NULL)
    {
        if(p->clientID == clientID) { // 找到对应的设备，遍历该设备下已经存在的observe事件，将匹配的事件删除，表中节点不删除
            if(p->things == NULL) {
                return false; // 没有可以删除的节点
            }
            if(URImatch(p->things->URI, URI)) { // 第一个节点匹配
                // 释放节点内容
                free(p->things->token);
                observeThings *needToFree = p->things; // 暂时保存头结点
                p->things = p->things->next;
                free(needToFree);
                return true;
            }
            // 寻找后面的节点
            observeThings *qFront = p->things;
            observeThings *qBack = qFront->next;
            while (qBack != NULL)
            {
                if(URImatch(qBack->URI, URI)) {
                    free(qBack->token);
                    qFront->next = qBack->next;
                    free(qBack);
                    return true;
                }
                qBack = qBack->next;
                qFront = qFront->next;
            }
            return false;
        }
        p = p->next;
    }
    return false;
}