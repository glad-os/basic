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

#include "basic/header/function/str.h"
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
 * synopsis			str := str(n)
 * 					n must be an INTEGER or a REAL
 * 					returns a STRING declaring the string representation of the value specified by n
 *
 */
void _function_str( void )
{

	int type;
	union symbol_table_entry_value operand;

	// STR requires a single-element parameter list (containing an INTEGER or a REAL).
	if ( !expect_expression() )
		{ set_error_state( ERROR_SYNTAX ); return; }
	if ( !expect_token( PUNC_CLOSE_BRACKET ) )
		{ set_error_state( ERROR_EXPECTED_CLOSE_BRACKET );    return; }

 	// check for a type mismatch
 	type = *(operand_stack_ptr+operand_stack_offset-1);
 	switch ( type )
 	{
 		case DATATYPE_INTEGER :
 			// @todo - ACU - print the integer value using stdlib facilities
 			// sprintf( (char *)operand.sval , "%d" , pop_operand_integer() );
 			push_operand_string( operand.sval );
 			break;
 		case DATATYPE_REAL :
 			// @todo - ACU - print the float value using stdlib facilities
 			// sprintf( (char *)operand.sval , "%f" , pop_operand_real() );
 			push_operand_string( operand.sval );
 			break;
 		default               :
 			set_error_state( ERROR_TYPE_MISMATCH );	return;
 			break;
 	}

}
