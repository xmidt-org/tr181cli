/**
 * @file tr181cli_interface.h
 *
 * @description This file is used to manage internal functions of tr181cli interface
 *
 * Copyright (c) 2015  Comcast
 */

#include <stdlib.h>
#include <stdio.h>
#include "wrp-c.h"
#include <cJSON.h>

#define ENDPOINT "tcp://192.168.101.3:7777"
#define TR181_URL "tcp://192.168.101.3:8888"

typedef enum
{
    GET = 0,
    SET
} REQ_TYPE;


void parseTr181Request(REQ_TYPE type, char** paramList, int paramCount);
void prepareJsonPayload (const char** paramList, int paramCount, char** payloadStr);
