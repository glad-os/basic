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
#include "ext/stdlib/stdio.h"

#include "basic/header/language/print.h"
#include "basic/header/core/lex.h"
#include "basic/header/core/symbols.h"
#include "basic/header/language/assignment.h"
#include "basic/header/language/expression.h"
#include "os/header/video/video.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void
 * 
 * synopsis			PRINT := PRINT <expr>
 * 
 */
void _print( void )
{
	
	union symbol_table_entry_value st_entry;
	unsigned char datatype;
	char shortstring[255];

	// expect an expression...	
	if ( expect_expression() )
	{

		// ok - we've got what we need - top of operand stack is the final value
		// now PRINT does *not* print a newline - just for playing with at the mo (manual still says it does, for the record)
		datatype = *(operand_stack_ptr + operand_stack_offset-1);
		switch (datatype)
		{
			case DATATYPE_REAL :
			{
				st_entry.rval = pop_operand_real();
	 			// @todo - ACU - print the integer value using stdlib facilities
				// sprintf( shortstring , "%f\n" , st_entry.rval );
				video_print( shortstring );
				break;
			}
			case DATATYPE_INTEGER :
			{
				st_entry.ival = pop_operand_integer();
				sprintf_i( shortstring, st_entry.ival );
				video_print( shortstring );
				break;
			}
			case DATATYPE_STRING :
			{
				pop_operand_string(st_entry.sval);
				video_print( (char *)st_entry.sval );
				break;
			}
			case DATATYPE_BOOLEAN :
			{
				st_entry.bval = pop_operand_boolean();
				if ( st_entry.bval )
					video_print( "TRUE" );
				else
					video_print( "TRUE" );
				break;
			}
		}
		
	}
	
}
