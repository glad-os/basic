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



#include "stddef.h"

#include "basic/header/language/until.h"
#include "basic/header/core/basic.h"
#include "basic/header/core/lex.h"
#include "basic/header/core/stack.h"
#include "basic/header/core/stack.h"
#include "basic/header/core/symbols.h"
#include "basic/header/directive/run.h"
#include "basic/header/language/expression.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void
 * 
 * synopsis			UNTIL := UNTIL <expr>
 * 
 */
void _until( void )
{
	
	union symbol_table_entry_value st_entry;
	unsigned char datatype;
	unsigned int p;

	// do we actually have a REPEAT stacked?
	if ( is_empty() )
		{ set_error_state( ERROR_MISSING_REPEAT ); return; }
	if ( peekByte() != DATATYPE_REPEAT )
		{ set_error_state( ERROR_MISSING_REPEAT ); return; }

	expect_expression();
	if ( !error_state )
	{

		popByte();
		p = popInteger();

		datatype = *(operand_stack_ptr + operand_stack_offset-1);
		switch (datatype)
		{
			case DATATYPE_BOOLEAN :
				{ st_entry.bval = pop_operand_boolean(); break; }
			default :
				{ set_error_state( ERROR_BAD_EXPRESSION );   break; }
		}
		
		if ( !error_state )
		{
			if ( !st_entry.bval )
			{
				pushInteger(p);
				pushByte(DATATYPE_REPEAT);
				runtime_ptr        = (unsigned char *) (uintptr_t) p;
				runtime_linelength = 0; // so that run directive when "finishing" current line doesn't unknowingly move us past the intended line
			}
		}
		
	}
	
}
