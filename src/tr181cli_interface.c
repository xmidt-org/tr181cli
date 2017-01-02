#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <nanomsg/nn.h>
#include <nanomsg/pipeline.h>
#include "tr181cli_interface.h"

void parseTr181Request(REQ_TYPE type, char** paramList, int paramCount)
{
	char* payload = NULL;
	wrp_msg_t *wrp_msg,*message;
	wrp_msg_t  *responseMsg;
	void* packedMsg, *buf;
	int packedSize, rval;
	int sock, sock1 , rv;
	int byte, bytes=0;
	
	
	printf("Parsing the request to encoded wrp structure\n");	
	wrp_msg = (wrp_msg_t *)malloc(sizeof(wrp_msg_t));
        memset(wrp_msg, 0, sizeof(wrp_msg_t));
	
        wrp_msg->msg_type = WRP_MSG_TYPE__REQ;
        wrp_msg->u.req.source = TR181_URL;
        wrp_msg->u.req.dest = ENDPOINT;
        wrp_msg->u.req.transaction_uuid = NULL;
        
        if(type == GET)
        {  
        	prepareJsonPayload((const char**)paramList, paramCount, &payload);
        }
              
        wrp_msg->u.req.payload = (void *)payload;
        wrp_msg->u.req.payload_size = strlen(wrp_msg->u.req.payload);
        
       	printf("Msgpack encoding for tr181 request\n");
	packedSize = wrp_struct_to( wrp_msg, WRP_BYTES, &packedMsg ); 
	printf("packedMsg is %s with size %d\n", (char*)packedMsg, packedSize);
	
	/*** This is just to decode and verify the packed msg **/		
	rv = wrp_to_struct(packedMsg, packedSize, WRP_BYTES, &message);
	printf("rv is %d\n", rv);
	printf("decoded msgType:%d\n", message->msg_type);
	printf("decoded source:%s\n", message->u.req.source);
	printf("decoded dest:%s\n", message->u.req.dest);
	printf("decoded payload:%s\n", (char *)message->u.req.payload);
	printf("decoded payload_size:%zu\n", message->u.req.payload_size);
	wrp_free_struct(message);
	/****/
		
	//Sending encoded data to Webpa adpater through nanomsg for processing
	sock = nn_socket (AF_SP, NN_PUSH);
	int connect = nn_connect (sock, ENDPOINT);
	printf("connect is %d\n", connect);
	sleep(1);

	printf("Nanomsg client is sending encoded request\n");
	byte = nn_send (sock, packedMsg, packedSize, 0);		
	if(byte == packedSize )
	{
		printf("Nanomsg sent the message successfully\n");
	}
	else
	{	printf("Nanomsg failed to send message\n");
		printf("Expected byte to send: %d, actual byte sent:%d \n", packedSize, byte);
	}
	
	//free(packedMsg);
	//nn_shutdown(sock, 0);	

	//Waiting for response from webpa adapter
	sock1 = nn_socket( AF_SP, NN_PULL );
	printf("trcli is listening to url %s\n", TR181_URL);
	nn_bind(sock1, TR181_URL );
	bytes = nn_recv (sock1, &buf, NN_MSG, 0);			
	printf ("Response received through nanomsg: \"%s\"\n", (char*)buf);

	printf("Decoding the response received \n");
	rval = wrp_to_struct((void *)buf, bytes, WRP_BYTES, &responseMsg);
	printf("rval is %d\n", rval);
	printf("*********** Tr181 query output ********** %s\n",(char *)(responseMsg->u.req.payload));

	nn_freemsg(buf);		
	wrp_free_struct(responseMsg);	
	nn_shutdown(sock1, 0);	
}

void prepareJsonPayload (const char** paramList, int paramCount, char** payloadStr)
{
	cJSON *payload = NULL, *paramArray =NULL;
	
	payload = cJSON_CreateObject();
	printf("Preparing payload json\n");
	cJSON_AddStringToObject(payload, "command", "GET" );
	paramArray= cJSON_CreateStringArray(paramList, paramCount);
	cJSON_AddItemToObject(payload, "names", paramArray );
	
	*payloadStr = cJSON_PrintUnformatted(payload);
	printf("Payload formed: %s\n", *payloadStr);		

}
