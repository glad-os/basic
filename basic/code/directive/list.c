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


// include the library
/*
#include "../library/include/stdio.h"
#include "../library/include/string.h"
*/
#include "ext/stdlib/stdio.h"
#include "ext/stdlib/string.h"

#include "basic/header/directive/list.h"
#include "basic/header/core/lex.h"
#include "basic/header/core/memory.h"
#include "os/header/video/video.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void
 * 
 * synopsis			LIST := LIST
 * 					this function will list the program in stored memory in untokenised (human readable) form.
 * 
 */
void _directive_list( void )
{
	
	unsigned int line_no;
	unsigned char *ptr;
	unsigned char line_length;
	unsigned char n;
	int keep_going;
	char *detokenised;
	char shortstring[8];
	char detokenised_line[MAX_INPUT_LENGTH];
	int i , dst;
	
	ptr        = (unsigned char *)get_mem_base();
	keep_going = 1;

	while ( keep_going )
	{
		line_no     = (*(ptr+0)<<0) + (*(ptr+1)<<8) + (*(ptr+2)<<16) + (*(ptr+3)<<24);
		line_length = *(ptr+4);
		if ( (line_no == 0xFFFFFFFF) && (line_length == 5) )
			keep_going = 0;
		else
		{
			// display untokenised line

			// sprintf( shortstring , "%5d" , line_no );
			// strcpy( detokenised_line , shortstring );
			int i_length = sprintf_i( shortstring, line_no );
			strcpy( detokenised_line , shortstring );
			dst = i_length-1; // and this is where the line *content* can now be detokenised to

			i = 5; // dst = 5;
			while ( i < line_length )
			{
				n = *(ptr+i);
				if ( (detokenised = string_from_token(n)) )
				{
					strcpy( detokenised_line+dst , detokenised );
					dst += strlen(detokenised);
				}
				else
				{
					*(detokenised_line+dst) = n;
					dst++;
				}
				*(detokenised_line+dst) = 0;
				i++;
			}
			video_printline( detokenised_line );
			ptr += line_length;
		}
		// keep_going=0;

	}
	
}
