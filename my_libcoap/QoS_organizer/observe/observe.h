#ifndef ORGANIZER_OBSERVE
#define ORGANIZER_OBSERVE
#include <stdio.h>
#include <coap3/coap.h>
#include <pthread.h>
#include <stdbool.h>
#include "internals.h"
#include <pthread.h>

struct observeThings
{
    lwm2m_uri_t URI;
    uint8_t *token;
    int tokenLength;
    struct observeThings *next;
};

struct observeList
{
    struct observeList* next;
    uint16_t clientID;
    struct observeThings* things;
};
typedef struct observeThings observeThings;
typedef struct observeList observeList;

observeList *observeListHead;
pthread_mutex_t observeMutex;
bool InsertObserve (uint16_t clientID, lwm2m_uri_t URI, coap_bin_const_t token, uint8_t **nonToken, int *nonTokenLength);
bool DeleteObserve (uint16_t clientID, lwm2m_uri_t URI, coap_bin_const_t token);

#endif
