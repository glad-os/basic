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
#include "../library/include/maths.h"
*/
#include "maths.h"

#include "basic/header/function/abs.h"
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
 * synopsis			abs := abs(x)
 * 					x can be a REAL or an INTEGER. Returns the +ive value of x, regardless of current value polarity.
 * 
 */
void _function_abs( void )
{

	int type;
	union symbol_table_entry_value operand;

	// ABS requires a single-element parameter list (containing a REAL or an INTEGER).
	if ( !expect_expression() )
		{ set_error_state( ERROR_SYNTAX ); return; }
	if ( !expect_token( PUNC_CLOSE_BRACKET ) )
		{ set_error_state( ERROR_EXPECTED_CLOSE_BRACKET );    return; }
	
 	// check for a type mismatch - for now, just ensure int or real
 	type = *(operand_stack_ptr+operand_stack_offset-1);
 	switch ( type )
 	{
 		case DATATYPE_REAL    : 
 			operand.rval = fabs(pop_operand_real());
 			push_operand_real( operand.rval );          
 			break;
 		case DATATYPE_INTEGER : 
 			operand.ival = abs(pop_operand_integer());
 			if ( operand.ival < 0 ) { operand.ival = -operand.ival; }
 			push_operand_integer( operand.ival );          
 			break;
 		default               : 
 			set_error_state( ERROR_TYPE_MISMATCH );	return; 
 			break; 
 	}
	
}
