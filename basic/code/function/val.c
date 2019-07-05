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
#include "../library/include/stdlib.h"
*/
#include "ext/stdlib/stdlib.h"

#include "basic/header/function/val.h"
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
 * synopsis			val := val(x)
 * 					x must be a STRING. Returns the REAL equivalent to the value represented by the STRING.
 * 					if the STRING content cannot be interpreted as a numeric value, it will be considered "0" 
 * 					(due to the fact I'm using atof() which exhibits this behaviour in itself)
 * 
 */
void _function_val( void )
{

	int type;
	union symbol_table_entry_value operand;

	// VAL requires a single-element parameter list (containing a STRING).
	if ( !expect_expression()                ) { set_error_state( ERROR_SYNTAX                 ); }
	if ( !expect_token( PUNC_CLOSE_BRACKET ) ) { set_error_state( ERROR_EXPECTED_CLOSE_BRACKET ); }
	if ( error_state ) { return; }
	
 	// check for a type mismatch
 	type = *(operand_stack_ptr+operand_stack_offset-1);
 	switch ( type )
 	{
 		case DATATYPE_STRING :
 			pop_operand_string(operand.sval);
 			push_operand_real( (float) atof((char *)operand.sval) );          
 			break;
 		default               : 
 			set_error_state( ERROR_TYPE_MISMATCH );	return; 
 			break; 
 	}
	
}
