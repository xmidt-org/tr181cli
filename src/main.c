/*
 * Copyright (c) 2016 Comcast
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */


#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "wrp-c.h"
#include <cJSON.h>
/*----------------------------------------------------------------------------*/
/*                                   Macros                                   */
/*----------------------------------------------------------------------------*/

typedef enum
{
    GET = 0,
    SET
} REQ_TYPE;

/*----------------------------------------------------------------------------*/
/*                               Data Structures                              */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                            File Scoped Variables                           */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                             Function Prototypes                            */
/*----------------------------------------------------------------------------*/
void formWrpMsg(REQ_TYPE type, char** paramList, int paramCount, void** packedMsg);
void formCJSONpayload (char** paramList, int paramCount, char** payloadStr);
/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/
int main( int argc, char *argv[] )
{
    char **paramList =NULL;
    char *command;
    REQ_TYPE type;
    void* packedMsg;
    int paramCount =0;
    int i=0,j;
        
    
    if( 0 == argc || NULL == argv ) {
        return -1;
    }
 
   if( argc >= 2 ) {
      printf("The argument supplied is %s\n", argv[1]);
      command = argv[1];
      if (strcmp(command, "Get")==0)
      {
      	type = GET;
      	
      	paramCount = (argc-2);
      	paramList = (char**)malloc(sizeof(char*)*paramCount);
      	
      	j=2;
      	for(i=0; i<paramCount; i++)
      	{
      		paramList[i] = (char*)malloc(sizeof(char)*256);
      		strcpy(paramList[i], argv[j]);
      		j++;
      	}
      }
      
      //frame wrp-c request structure and send it to webpa adapter through nanomsg
      
      formWrpMsg(type, paramList,  paramCount, &packedMsg);
      printf("packedMsg is %s\n", (char*)packedMsg);
   }
   else {
      printf("One argument expected.\n");
   }
    return 0;
}


/*----------------------------------------------------------------------------*/
/*                             Internal functions                             */
/*----------------------------------------------------------------------------*/

void formWrpMsg(REQ_TYPE type, char** paramList, int paramCount, void** packedMsg)
{
	char* payload = NULL;
	wrp_msg_t *wrp_msg ;
	void *bytes;
	int size;
	
	
	wrp_msg = (wrp_msg_t *)malloc(sizeof(wrp_msg_t));
        memset(wrp_msg, 0, sizeof(wrp_msg_t));
	
        wrp_msg->msg_type = WRP_MSG_TYPE__REQ;
        wrp_msg->u.req.source = "tcp://192.168.101.1:8888";
        wrp_msg->u.req.dest = "tcp://192.168.101.3:6667";
        wrp_msg->u.req.transaction_uuid = NULL;
        
        if(type == GET)
        {  
        	formCJSONpayload(paramList, paramCount, &payload);
        }
              
        wrp_msg->u.req.payload = (void *)payload;
        wrp_msg->u.req.payload_size = strlen(wrp_msg->u.req.payload);
        
       	printf("packing tr181cli request\n");
	size = wrp_struct_to( wrp_msg, WRP_BYTES, &bytes ); 
	printf("packed msg size:%d\n", size);
	*packedMsg = bytes;
	
	/*** Enable this to decode and verify packed msg **/
	wrp_msg_t *message;
	int rv;	
	rv = wrp_to_struct(bytes, size, WRP_BYTES, &message);
	printf("decoded msgType:%d\n", message->msg_type);
	printf("decoded source:%s\n", message->u.req.source);
	printf("decoded dest:%s\n", message->u.req.source);
	printf("decoded payload:%s\n", (char *)message->u.req.payload);
	printf("decoded payload_size:%d\n", message->u.req.payload_size);
	wrp_free_struct(message);	
	
	

}

void formCJSONpayload (char** paramList, int paramCount, char** payloadStr)
{
	cJSON *payload = NULL, *paramArray =NULL;
	const char ** names = paramList;
	payload = cJSON_CreateObject();
	paramArray= cJSON_CreateStringArray(names, paramCount);
	cJSON_AddItemToObject(payload, "names", paramArray );
	
	*payloadStr = cJSON_PrintUnformatted(payload);
	printf("Payload formed: %s\n", *payloadStr);		

}
