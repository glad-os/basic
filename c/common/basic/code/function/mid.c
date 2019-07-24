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

#include "basic/header/function/mid.h"
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
 * synopsis			mid := mid( expr1 , expr2 , expr3 )
 * 					expr1 is a string
 * 					expr2 is an integer (declares the start offset, from 0 .. length of string-1)
 * 					expr3 is an integer ( > 0 )
 * 					returns the mid-section of the specified string, as defined by the values of expr2 and expr3
 * 					if expr3 (length) exceeds the length of the source string, truncation occurs
 * 
 */
void _function_mid( void )
{

	int type;
	union symbol_table_entry_value operand;
	int offset , length;

	// MID requires a 3-element parameter list (STRING,INTEGER,INTEGER).
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

	if ( !expect_token(PUNC_COMMA) )
		{ set_error_state( ERROR_EXPECTED_COMMA ); return; }
	if ( !expect_expression() )
		{ set_error_state( ERROR_SYNTAX ); return; }
 	type = *(operand_stack_ptr+operand_stack_offset-1);
 	switch ( type )
 	{
 		case DATATYPE_INTEGER : 
 			length = pop_operand_integer();
 			break;
 		default               : 
 			set_error_state( ERROR_TYPE_MISMATCH );	return; 
 			break; 
 	}
		
	if ( !expect_token( PUNC_CLOSE_BRACKET ) )
		{ set_error_state( ERROR_EXPECTED_CLOSE_BRACKET );    return; }
		
	// validate the index and offset (complain if user attempts a silly thing)
	if ( (offset < 0) || (offset > strlen((char *)operand.sval)) )
		{ set_error_state( ERROR_ILLEGAL_VALUE ); return; }
	if ( (length < 1) || (offset + length > strlen((char *)operand.sval)) )
		{ set_error_state( ERROR_ILLEGAL_VALUE ); return; }
		
	
	// ok - now do the work
	*(operand.sval+offset+length) = 0x00;
	push_operand_string( &operand.sval[offset] );
	
}
