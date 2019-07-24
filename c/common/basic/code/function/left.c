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

#include "basic/header/function/left.h"
#include "basic/header/core/basic.h"
#include "basic/header/language/define.h"	// for the optional parameters handling code
#include "basic/header/language/expression.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void
 * 
 * synopsis			left := left( expr1 , expr2 )
 * 					expr1 is a string
 * 					expr2 is an integer (declares the end offset, from 0 .. length of string-1)
 * 					returns the left-section of the specified string, with the end character position defined by the value of expr2
 * 					if expr2 equates to a value less than or equal to 0 then "0" is substituted by default, and an empty string will be returned
 * 					if expr2 equates to a value greater than the length of the string passed in expr1, then expr1 is returned "unharmed" as the result
 * 
 */
void _function_left( void )
{

	int type;
	union symbol_table_entry_value operand;
	int offset;

	// LEFT requires a 2-element parameter list (STRING,INTEGER).
	if ( !expect_expression() )
		{ set_error_state( ERROR_SYNTAX ); return; }
 	type = *(operand_stack_ptr+operand_stack_offset-1);
 	switch ( type )
 	{
 		case DATATYPE_STRING : 
 			pop_operand_string( (unsigned char *)operand.sval );
 			break;
 		default : 
 			set_error_state( ERROR_TYPE_MISMATCH );	return; 
 			break; 
 	}

	if ( !expect_token(PUNC_COMMA) )
		{ set_error_state( ERROR_EXPECTED_COMMA ); return; }
	if ( !expect_expression() )
		{ set_error_state( ERROR_SYNTAX ); return; }
 	type = *(operand_stack_ptr+operand_stack_offset-1);
 	switch ( type )
 	{
 		case DATATYPE_INTEGER : 
 			offset = pop_operand_integer();
 			break;
 		default               : 
 			set_error_state( ERROR_TYPE_MISMATCH );	return; 
 			break; 
 	}

	if ( !expect_token( PUNC_CLOSE_BRACKET ) )
		{ set_error_state( ERROR_EXPECTED_CLOSE_BRACKET );    return; }
		
	// validate the index and offset (complain if user attempts a silly thing)
	if ( offset < 0                             ) { offset = 0;                            }
	if ( offset >= strlen((char *)operand.sval) ) { offset = strlen((char *)operand.sval); } 
			
	// ok - now do the work
	operand.sval[offset] = 0x00;
	push_operand_string( (unsigned char *)operand.sval );
	
}
