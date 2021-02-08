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
#include "stdlib.h"
#include "stdio.h"

#include "basic/header/language/next.h"
#include "basic/header/core/basic.h"
#include "basic/header/core/lex.h"
#include "basic/header/core/stack.h"
#include "basic/header/core/symbols.h"
#include "basic/header/directive/run.h"
#include "basic/header/language/expression.h" // for expect_token



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void
 * 
 * synopsis			NEXT := NEXT
 * 
 * 					Firstly, the stack is checked to ensure a previous "FOR" construct has been stacked, if not a "missing IF" error will 
 * 					be reported. Otherwise, the "FOR" construct is popped (see for.c for definition details), the controlling parameter is 
 * 					adjusted by the required STEP rate and, if necessary, control is returned to the start of the FOR...NEXT code block.
 * 
 */
void _next( void )
{

	unsigned char *source_ptr;
	union symbol_table_entry_value st_entry;
	union symbol_table_entry_value st_entry_finish;
	union symbol_table_entry_value st_entry_step;
	struct lex_result result;
	int st_index;
	int loop;
	unsigned char basetype;
	char flt[9];

	// do we actually have a FOR stacked?
	if ( (is_empty()) || (peekByte() != DATATYPE_FOR) )
		{ set_error_state( ERROR_MISSING_FOR ); return; }
	
	popByte();
	basetype             = popByte();
	source_ptr           = (unsigned char *)(uintptr_t) popInteger();
	switch ( basetype )
	{
		case DATATYPE_INTEGER :
			st_entry_finish.ival = popInteger();
			st_entry_step.ival   = popInteger();
			break;
		case DATATYPE_REAL :
			st_entry_finish.rval = popReal();
			st_entry_step.rval   = popReal();
			break;
	}
	popString( result.str );
		
	// add STEP
	st_index = position_in_symbol_table( result.str );
	st_entry = symbol_table_entry_value( st_index );
	switch ( basetype )
	{
		case DATATYPE_INTEGER :
			st_entry.ival += st_entry_step.ival;
			add_to_symbol_table( result.str , DATATYPE_INTEGER , st_entry );
			break;
		case DATATYPE_REAL :
			// if we do manual float additions, we'll inadvertently suffer rounding issues. however, if we ask C to
			// cast the current float as a string (e.g. "2.2000000") and put this "back into" the current control value,
			// then adding (for example) 0.1 to it won't end up (in a loop from 1.0 to 3.0 step 0.1 for example) with rounding errors.
 			// @todo - ACU - print the float value using stdlib facilities
			// sprintf( flt , "%f" , st_entry.rval );
			st_entry.rval = atof(flt) + st_entry_step.rval;
			add_to_symbol_table( result.str , DATATYPE_REAL    , st_entry );
			break;
	}
	
	// boundary check
	switch ( basetype )
	{
		case DATATYPE_INTEGER :
			if ( st_entry_step.ival > 0 )
				loop = ( st_entry.ival > st_entry_finish.ival ) ? 0 : 1;
			else
				loop = ( st_entry.ival < st_entry_finish.ival ) ? 0 : 1;
			break;
		case DATATYPE_REAL    :
			if ( st_entry_step.rval > 0 )
				loop = ( st_entry.rval > st_entry_finish.rval ) ? 0 : 1; 
			else
				loop = ( st_entry.rval < st_entry_finish.rval ) ? 0 : 1;
			break;
	}
	
	if ( loop )
	{
		// we could speed up here simply by altering the stack pointer rather than restacking the same data...
		pushString ( result.str           );
		switch ( basetype )
		{
			case DATATYPE_INTEGER :
				pushInteger( st_entry_step.ival   );
				pushInteger( st_entry_finish.ival );
				break;
			case DATATYPE_REAL :
				pushReal( st_entry_step.rval   );
				pushReal( st_entry_finish.rval );
				break;
		}
		pushInteger( (unsigned int) (uintptr_t) source_ptr );
		pushByte   ( basetype             );
		pushByte   ( DATATYPE_FOR         );
		runtime_ptr        = (unsigned char *) ( source_ptr );
		runtime_linelength = 0; // so that run directive when "finishing" current line doesn't unknowingly move us past the intended line
	}
		
}
