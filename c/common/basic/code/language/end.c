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



#include <stdint.h>

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
 * synopsis			END := END [ PROCEDURE | IF ]
 * 
 * 					The END keyword on its own indicates that the stored program being run is now required to stop.
 * 
 * 					Coupled with PROCEDURE, it indicates that the current PROCEDURE being executed is now required to return control back
 * 					to where the PROCEDURE was invoked. The stack needs checking to ensure a PROCEDURE return point has actually been stacked 
 * 					at that point and, if so, the return point is popped and control is returned. At the same time the previous symbol table 
 * 					that was stacked must also be reinstated, effectively destroying the current (local) symbol table associated with the 
 * 					PROCEDURE that is now terminating.
 * 
 * 					Coupled with the "IF" keyword, "END IF" indicates that the current IF... block is now completed. It is a requirement to 
 * 					check that there is a corresponding "IF" construct on the stack and, if so, the previous value of the "execution gate" 
 * 					must be reinstated before the "encompassing" code block can continue.
 * 
 */
void _finished( void )
{
	
	unsigned char *pval;
	
	// 8 < - - - - - - - - - 8 < - - - - - - - - - 8 < - - - - - - - - - 8 < - - - - - - - - - 8 < - - - - - - - - - 8 < - - - 
	// TODO - allow for "END PROCEDURE" to pop return position from stack and continue running...
	// 8 < - - - - - - - - - 8 < - - - - - - - - - 8 < - - - - - - - - - 8 < - - - - - - - - - 8 < - - - - - - - - - 8 < - - - 

	if ( expect_token(KEYWORD_PROCEDURE) )
	{

		// check there's a PROCEDURE construct stacked
		if ( (is_empty()) || (peekByte() != DATATYPE_PROCEDURE) )
			{ set_error_state( ERROR_MISSING_PROCEDURE ); return; }
		
		// pop the construct from the stack and process it (construct identifies the address of PROCEDURE to execute)
		popByte();
		pval               = (unsigned char *) (uintptr_t) ( (popByte()<<24) + (popByte()<<16) + (popByte()<<8) + popByte() );
		runtime_ptr        = pval;	// this is a pointer to the line to return control to
		runtime_linelength = 0;		// on return, _directive_run will use this to "push" us onto the next line, so cancel that effect
		
		// pop the (previous) symbol table "extent" that was stacked
		heap_top          = (unsigned char *) (uintptr_t) popInteger();
		symbol_table_base = (unsigned char *) (uintptr_t) popInteger();
		
	}
	else if ( expect_token(KEYWORD_IF) )
	{
		
		// check there's an IF construct stacked
		if ( (is_empty()) || (peekByte() != DATATYPE_IF) )
			{ set_error_state( ERROR_MISSING_IF ); return; }
		
		// pop the IF construct from the stack and restore the previous value of execution_gate	
		popByte();
		execution_gate = popByte();
		
	}
	else
	{
		
		// nothing recognised (that we might expect) *after* "END"? Consider it "END" at that point
		running = 0;
		
	}
	
}
