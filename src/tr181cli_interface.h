/**
 * Copyright 2015 Comcast Cable Communications Management, LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
/**
 * @file tr181cli_interface.h
 *
 * @description This file is used to manage internal functions of tr181cli interface
 *
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
