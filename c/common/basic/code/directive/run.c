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
#include "../library/include/string.h"
*/
#include "string.h"
#include "stdio.h"

#include "basic/header/directive/run.h"
#include "basic/header/core/basic.h"
#include "basic/header/core/lex.h"
#include "basic/header/core/memory.h"
#include "basic/header/core/symbols.h"
#include "basic/header/directive/escape.h"
#include "basic/header/language/define.h"			// before running, DEFINEs are symbol table'd so we need this to do it
#include "basic/header/language/expression.h"
#include "os/header/video/video.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			void
 *
 * returns			void
 *
 * synopsis			this is the function that actually interprets the current line of source, and continues to execute until such time as
 * 					the <Esc> key has been recognised, or a RETURN keyword has been encountered, at which point control returns to the
 * 					calling function.
 *
 * 					This code was originally part of the RUN directive function itself, although this core has had to be factored out to
 * 					better cope with the controlling code for FUNCTIONs (which typically breaks off and calls this function
 * 					which continues interpretation until the associated RETURN is encountered).
 *
 */
void _interpret( void )
{

	return_encountered = 0;
	reset_escape_status();

	while ( running && !return_encountered && !escape )
	{

		line_no            = (*(runtime_ptr+0)<<0) + (*(runtime_ptr+1)<<8) + (*(runtime_ptr+2)<<16) + (*(runtime_ptr+3)<<24);
		runtime_linelength = *(runtime_ptr+4);

		if ( (line_no == 0xFFFFFFFF) && (runtime_linelength == 5) ) {
			running = 0;
		}
		else
		{
			memcpy(input,runtime_ptr+5,runtime_linelength-5);
			*(input+runtime_linelength-5) = 0x00; // stick NUL at end of char array
			*(input+runtime_linelength-5+1) = 0x00; // stick NUL at end of char array
			*(input+runtime_linelength-5+2) = 0x00; // stick NUL at end of char array
			*(input+runtime_linelength-5+3) = 0x00; // stick NUL at end of char array
			*(input+runtime_linelength-5+4) = 0x00; // stick NUL at end of char array
			*(input+runtime_linelength-5+5) = 0x00; // stick NUL at end of char array

			error_state = 0;
			initialise_lex(input);
			interpret();
			// appreciate that "END" may have just been encountered...
			if ( running )
			{
				// if no error state, we can interpret the next statement
				if ( !error_state && !escape ) {
					runtime_ptr += runtime_linelength;
				} else {
					running = 0;
				}
			}
		}

	}

	// when interpretation is complete, ensure that the "RETURN encountered" flag and "ESCAPE status" flags are reset
	running = 0;
	return_encountered = 0;
	reset_escape_status();

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			void
 *
 * returns			void
 *
 * synopsis			RUN := RUN
 * 					executes the program currently held in stored program memory. at the end, control is returned to the interpreter
 * 					to continue receiving further instructions from the user.
 *
 */
void _directive_run( void )
{

	struct lex_result result;
	struct lex_state state;

	if ( next_token(&result,TRUE) )
	{

		set_error_state( ERROR_SYNTAX );

	}
	else
	{

		preserve_lex_state(&state);

		initialise_symbol_table();
		running            = 1;
		return_encountered = 0;
		precedence_offset  = 0;	// do this at the start
		nested_function_context = 0;
		procedures_scanned = 0;

		_declare_procedures();
		_initialise_expression_stacks();

		if ( !error_state )
		{
			runtime_ptr    = get_mem_base();
			execution_gate = 1;	// used by IF... statements to determine whether to execute current lines or not
			_interpret();
		}
		else
		{
			running = 0;
		}

		reinstate_lex_state(&state);

	}

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			void
 *
 * returns			void
 *
 * synopsis			before program execution begins (in the RUN directive), the source requires scanning for definitions of both
 * 					PROCEDUREs and FUNCTIONs.
 *
 * 					It is important that the DEFINE keyword is the first keyword on a line (this implies a limitation on the language).
 *
 * 					If a DEFINE is encountered, the _define function is invoked to parse the definition and store the relevant information
 * 					in the (global) symbol table, so that - during execution - invocations of the PROCEDUREs and FUNCTIONs can happen.
 *
 * 					If there is a semantic error in the definition, this will not be determined until runtime, when invocation of the
 * 					PROCEDURE or FUNCTION will fail (typically due to mismatched parameters).
 *
 */
void _declare_procedures( void )
{

	struct lex_state state;
	int end_of_program;
	char input[ MAX_INPUT_LENGTH + 1 ];	// +1 to allow for null at end
	unsigned char *ptr;
	unsigned char line_length;
	struct lex_result result;

	if ( !procedures_scanned )
	{

		preserve_lex_state(&state);
		ptr            = get_mem_base();
		end_of_program = 0;
		while ( !end_of_program )
		{
			line_no     = (*(ptr+0)<<0) + (*(ptr+1)<<8) + (*(ptr+2)<<16) + (*(ptr+3)<<24);
			line_length = *(ptr+4);
			if ( (line_no == 0xFFFFFFFF) && (line_length == 5) )
				end_of_program = 1;
			else
			{
				memcpy(input,ptr+5,line_length-5);
				*(input+line_length-5) = 0x00; // errr... why? (ensure end of line!)
				initialise_lex(input);
				if ( (next_token(&result,TRUE)) && (result.token == KEYWORD_DEFINE) )
				{
					_define( TRUE , ptr + line_length ); // declare DEFINE as starting at NEXT line (1st line of code within DEFINEd area!)
				}
				ptr += line_length;
			}
		}
		reinstate_lex_state(&state);

		procedures_scanned = 1;

	}

}
