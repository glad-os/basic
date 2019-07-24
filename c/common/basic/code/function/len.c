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

#include "basic/header/function/len.h"
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
 * synopsis			len := len( expr1 )
 * 					expr1 is a string
 * 					returns an INTEGER describing the length of the string supplied by expr1 (excludes counting the terminating 0x00 character)
 * 
 */
void _function_len( void )
{

	int type;
	union symbol_table_entry_value operand;

	// LEN requires a 1-element parameter list (STRING).
	if ( !expect_expression() )
		{ set_error_state( ERROR_SYNTAX ); 					return; }
	if ( !expect_token( PUNC_CLOSE_BRACKET ) )
		{ set_error_state( ERROR_EXPECTED_CLOSE_BRACKET );	return; }

 	type = *(operand_stack_ptr+operand_stack_offset-1);
 	switch ( type )
 	{
 		case DATATYPE_STRING : 
 			pop_operand_string( (unsigned char *)operand.sval );
 			push_operand_integer( strlen((char *)operand.sval) );
 			break;
 		default : 
 			set_error_state( ERROR_TYPE_MISMATCH );	return; 
 			break; 
 	}

}
