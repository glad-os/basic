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



#include "basic/header/language/for.h"
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
 * synopsis			FOR := FOR <assignment> TO <expr1> [ STEP <expr2> ]
 * 
 * 					If the above syntax is successfully found, this function will compile a "FOR" construct to go on the stack. It requires 
 * 					consistency in the underlying datatypes to be successful (it will complain if there's a datatype mismatch, deliberately).
 * 
 * 					All going well, the construct will be stacked in anticipation that the subsequent NEXT keyword can successfully unstack and 
 * 					process it, returning to repeat the encompassed code block if required to do so.
 * 
 * 					If no STEP keyword was encountered, a default STEP value of +1 (or +1.0 if real datatypes are in play) is used.
 * 
 */
void _for( void )
{


	struct lex_result result;
	union symbol_table_entry_value st_entry_finish;
	union symbol_table_entry_value st_entry_step;
	unsigned char datatype;
	unsigned int source_ptr;
	unsigned char basetype;

	// assignment	
	next_token(&result,TRUE);
	if ( result.token != ID )
		{ set_error_state( ERROR_SYNTAX ); return; }
	_assignment(result.str);
	
	// TO
	if ( !expect_token(KEYWORD_TO) )
		{ set_error_state( ERROR_MISSING_TO ); return; }

	// expression
	if ( !expect_expression() )
		{ return; }
	basetype = *(operand_stack_ptr + operand_stack_offset-1);
	switch (basetype)
	{
		case DATATYPE_REAL    : st_entry_finish.rval = pop_operand_real();    break;
		case DATATYPE_INTEGER :	st_entry_finish.ival = pop_operand_integer(); break;
		default               : set_error_state( ERROR_SYNTAX ); return;  break;
	}
		
	// [ STEP <expr> ]
	if ( expect_token(KEYWORD_STEP) )
	{
		if ( !expect_expression() )
			{ set_error_state( ERROR_SYNTAX ); return; }
		else
		{
			datatype = *(operand_stack_ptr + operand_stack_offset-1);
			if ( basetype != datatype) { set_error_state( ERROR_TYPE_MISMATCH ); return; }
			switch (datatype)
			{
				case DATATYPE_REAL    : st_entry_step.rval = pop_operand_real();    break;
				case DATATYPE_INTEGER :	st_entry_step.ival = pop_operand_integer();	break;
				default               : set_error_state( ERROR_TYPE_MISMATCH );     break;
			}
		}
	}
	else
	{
		switch ( basetype )
		{
			case DATATYPE_INTEGER : st_entry_step.ival = +1  ; break;
			case DATATYPE_REAL    : st_entry_step.rval = +1.0; break;
		}
	}
		
	// sensibility check
	if ( ((basetype == DATATYPE_INTEGER) & (st_entry_step.ival == 0)) || ((basetype == DATATYPE_REAL) & (st_entry_step.rval == 0.0)) )
		{ set_error_state( ERROR_INFINITY ); return; }
	
	/*
	 * 
	 * ok - stack the FOR construct => (id) (step) (finish) (position) (indicator)
	 * id        => the identifier name of the symbol table entry (e.g. "xyz")
	 * step      => step rate of per-loop increments (e.g. -1)
	 * finish    => boundary value of identifier before loop failure is incurred (e.g. 100)
	 * position  => position in source of 1st in-block statement
	 * basetype  => indicates whether the values should be treated as INTEGER or REAL (e.g. "1 TO 2 STEP 1", or "1.0 TO 2.0 STEP 0.1")
	 * indicator => datatype indicator (DATATYPE_FOR)
	 * 
	 * this stacked construct will be used by the NEXT keyword to complete the construct execution process
	 * when reals come into the picture, we'll also end up stacking the datatype of the construct as well
	 * (ie. whether we manipulating an integer or a real in the loop, e.g ."FOR i = 1 TO 10" or "FOR i = 1.0 TO 2.0")
	 * 
	 */
	 
	// let's get ourselves some nicer functionality into the stack...
	source_ptr  = (unsigned long int) runtime_ptr;
	source_ptr += (unsigned long int) runtime_linelength;
	pushString ( result.str   );
	switch ( basetype )
	{
		case DATATYPE_INTEGER : pushInteger( st_entry_step.ival ); pushInteger( st_entry_finish.ival ); break;
		case DATATYPE_REAL    : pushReal   ( st_entry_step.rval ); pushReal   ( st_entry_finish.rval ); break;
	}
	pushInteger( source_ptr   );
	pushByte   ( basetype     );
	pushByte   ( DATATYPE_FOR );
	
}
