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
#include "ext/stdlib/string.h"

#include "basic/header/function/right.h"
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
 * 					expr2 is an integer (declares the start offset, from 0 .. length of string-1)
 * 					returns the right-section of the specified string, the first character defined by the index value of expr2
 * 					if expr2 is less than zero, a "Bad String" error will be returned (this indicates a -ive index into the string)
 * 					if expr2 is larger than the length of the supplied string in expr1, a "Bad String" error will be returned (this indicates an index into the string that exceeds the string length)
 * 
 */
void _function_right( void )
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
	if ( (offset < 0) || (offset > strlen((char *)operand.sval)) )
		{ set_error_state( ERROR_BAD_STRING ); return; }
			
	// ok - now do the work
	push_operand_string( &operand.sval[strlen((char *)operand.sval) - offset] );
		
}
