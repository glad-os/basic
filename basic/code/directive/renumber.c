/**
 * Copyright 2019 AbbeyCatUK
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



#include "basic/header/directive/renumber.h"
#include "basic/header/core/basic.h"
#include "basic/header/core/memory.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void
 * 
 * synopsis			RENUMBER := RENUMBER
 * 					renumbers each and every line in the current stored program memory, starting from 10 and increasing by 10 for each line.
 * 
 */
void _directive_renumber( void )
{

	unsigned char *ptr;
	unsigned int line_no , next_line_no;
	unsigned char line_length;
	int keep_going;
	
	ptr          = (unsigned char *)get_mem_base();
	keep_going   = 1;
	next_line_no = 10;
	while ( keep_going )
	{
		line_no     = (*(ptr+0)<<0) + (*(ptr+1)<<8) + (*(ptr+2)<<16) + (*(ptr+3)<<24);
		line_length = *(ptr+4);
		if ( (line_no == 0xFFFFFFFF) && (line_length == 5) )
			keep_going = 0;
		else
		{
			*(ptr+0) = ( (next_line_no >>  0) & 0xFF );
			*(ptr+1) = ( (next_line_no >>  8) & 0xFF );
			*(ptr+2) = ( (next_line_no >> 16) & 0xFF );
			*(ptr+3) = ( (next_line_no >> 24) & 0xFF );
			ptr += line_length;
			next_line_no += 10;
		}
	}
	
}
