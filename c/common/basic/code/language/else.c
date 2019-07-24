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



#include "basic/header/language/else.h"
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
 * synopsis			ELSE := ELSE
 * 
 * 					it is a requirement that the stack be checked to ensure an IF... construct has previously been encountered at this stage.
 * 
 *					the ELSE keyword indicates a "toggling" of the execution_gate (which controls whether statements are actually processed with or not)
 *					however with the possibility of nested conditions, the PREVIOUS (stacked!) execution_gate value must play the part of a MASK to
 *					the toggled version; as if you have an outer IF block that's currently false, any inner IF block must always be "masked" by this
 *					value as nothing inside the outer block is allowed to be visible until such time as the outer block has toggled its own state. 
 * 
 */
void _else( void )
{

	int previous_gate,indicator;

	// check there's a corresponding IF construct
	if ( (is_empty()) || (peekByte() != DATATYPE_IF) )
		{ set_error_state( ERROR_MISSING_IF ); return; }
		
	indicator     = popByte();
	previous_gate = popByte();
	pushByte( previous_gate );
	pushByte( indicator     );
	execution_gate = (!execution_gate) & previous_gate;
	
}
