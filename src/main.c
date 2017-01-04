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
#include <stdio.h>

#include "tr181cli_interface.h"
/*----------------------------------------------------------------------------*/
/*                                   Macros                                   */
/*----------------------------------------------------------------------------*/


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

/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/

int main( int argc, char *argv[] )
{
	char **paramList =NULL;
	char *command;
	REQ_TYPE type;	
	int paramCount=0;
	int i=0,j=2;		

	if( 0 == argc || NULL == argv ) 
	{
		return -1;
	}

	if( argc >= 2 ) 
	{
		printf("The request queried is %s\n", argv[1]);
		command = argv[1];
		
		if (strcmp(command, "Get")==0)
		{
			printf("Received GET query\n");
			type = GET;

			paramCount = (argc-2);
			paramList = (char**) malloc(sizeof(char*)*paramCount);
			
			for(i=0; i<paramCount; i++)
			{
				paramList[i] = (char*)malloc(sizeof(char)*256);
				strcpy(paramList[i], argv[j]);
				j++;
			}
		}
		//Parse tr181 request by converting to wrp structure			
		parseTr181Request(type, paramList,  paramCount);				
	}
	else 
	{
		printf("One argument is expected..\n");
	}
	return 0;
}

