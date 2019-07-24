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

#include "basic/header/function/instr.h"
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
 * synopsis			instr := instr( expr1 , expr 2)
 * 					expr1 and expr2 are STRINGs
 * 					TO BE COMPLETED
 * 
 */
void _function_instr( void )
{

	int type , index;
	union symbol_table_entry_value operand1 , operand2;
	char *ptr;
	
	if ( !expect_expression() )
		{ set_error_state( ERROR_SYNTAX ); return; }
 	type = *(operand_stack_ptr+operand_stack_offset-1);
 	switch ( type )
 	{
 		case DATATYPE_STRING : pop_operand_string(operand1.sval);				break;
 		default              : set_error_state( ERROR_TYPE_MISMATCH ); return;	break; 
 	}

	if ( !expect_token( PUNC_COMMA ) )
		{ set_error_state( ERROR_SYNTAX ); return; }
	if ( !expect_expression() )
		{ set_error_state( ERROR_SYNTAX ); return; }
		
 	type = *(operand_stack_ptr+operand_stack_offset-1);
 	switch ( type )
 	{
 		case DATATYPE_STRING : pop_operand_string(operand2.sval);	 			break;
 		default              : set_error_state( ERROR_TYPE_MISMATCH ); return;	break; 
 	}
	
	if ( !expect_token( PUNC_CLOSE_BRACKET ) )
		{ set_error_state( ERROR_EXPECTED_CLOSE_BRACKET );    return; }

	ptr = strstr( (char *)operand1.sval , (char *)operand2.sval );
	if ( ptr == 0 )
		index = -1;
	else
		index = (char *)ptr - (char *)operand1.sval;
		
	push_operand_integer( index );
		
}
