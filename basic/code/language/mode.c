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



#include "basic/header/language/mode.h"
#include "basic/header/core/basic.h"
#include "basic/header/core/stack.h"
#include "basic/header/core/symbols.h"
#include "basic/header/language/expression.h"
#include "os/header/video/video.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void
 * 
 * synopsis			MODE := MODE <expr>
 * 					expr must be an integer
 * 
 * 					the MODE statement follows the original BBC Micro format. Allowable MODEs are 0,1,2,4,5 and 7. Any other number
 * 					will result in a "Bad Mode" error being reported.
 * 
 * 					uses the video environment to subsequently configure the video display layout according to the MODE requested.
 * 
 */
void _mode( void )
{
	
	unsigned char datatype;
	int requested_mode;
	
	if ( expect_expression() )
	{
		
		// ok - we've got what we need - populate the symbol table (needs extending at this point, obviously!)
		datatype = *(operand_stack_ptr + operand_stack_offset-1);
		switch (datatype)
		{
			case DATATYPE_INTEGER :
			{
				requested_mode = pop_operand_integer();
				if ( (requested_mode != 0) & (requested_mode != 1) & (requested_mode != 2) & (requested_mode != 4) & (requested_mode != 5) & (requested_mode != 7) )
					set_error_state( ERROR_BAD_MODE );
				else
					video_setmode( requested_mode );
				break;
			}
			default :
			{
				set_error_state( ERROR_BAD_MODE );
				break;
			}
		}
		
	}
	else
		set_error_state( ERROR_SYNTAX );
		
}
