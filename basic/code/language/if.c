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



#include "basic/header/language/if.h"
#include "basic/header/core/basic.h"
#include "basic/header/core/lex.h"
#include "basic/header/core/stack.h"
#include "basic/header/core/symbols.h"
#include "basic/header/directive/run.h"
#include "basic/header/language/assignment.h" // for assignment
#include "basic/header/language/expression.h" // for expect_token



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void
 * 
 * synopsis			IF := IF <expr> THEN <statements> ELSE <statements> END IF
 * 
 * 					The resultant datatype of expr must be a Boolean (true/false) or a syntax error will be reported.
 * 
 * 					all going well, an "IF" construct will be stacked and the execution gate in its current state will be stacked before 
 * 					being updated to the new value (dependent on the value of the boolean expression evaluated). The stacking of execution gate 
 * 					ensures that, once the IF...END IF block has completed, the previous value of the execution gate can be reinstated, this is 
 * 					critical to ensure that nested IF... blocks can operate successfully and independently of each other.
 * 
 */
void _if( void )
{

	unsigned char datatype;
	
	if ( !expect_expression()        )
		{ set_error_state( ERROR_SYNTAX       ); return; }
	if ( !expect_token(KEYWORD_THEN) )
		{ set_error_state( ERROR_MISSING_THEN ); return; }

	// the expression for an IF must yield a boolean - dictating (partially) the upcoming state of the execution_gate
	datatype = *(operand_stack_ptr + operand_stack_offset-1);
	if ( datatype == DATATYPE_BOOLEAN )
	{
		pushByte( execution_gate );									// stack an IF construct 
		pushByte( DATATYPE_IF    );
		execution_gate = pop_operand_boolean() & execution_gate;	// set execution_gate [NB: in conjunction with its current state!]
	}
	else
		set_error_state( ERROR_SYNTAX );
	
}
