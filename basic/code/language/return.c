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



#include "basic/header/language/return.h"
#include "basic/header/core/basic.h"
#include "basic/header/core/symbols.h"
#include "basic/header/directive/run.h"			// for nested_function_context
#include "basic/header/language/expression.h"	// for expect_token()



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			void
 *
 * returns			void
 *
 * synopsis			RETURN := RETURN <expr>
 * 					expr can be of any known datatype
 *
 * 					the "return_encountered" flag needs to be set at this stage, to signal to the (hosting) interpret function that no more
 * 					lines need interpreting at this point; RETURN is the result of invoking a FUNCTION, therefore it's important to appreciate
 * 					that once the interpret function returns control to the expression handling code, the expression handling code is then in a
 * 					position to simply pop the expression value (expected here) from the top of the stack to "replace" the FUNCTION invocation
 * 					before continuing the evaluation of the overall expression that included the FUNCTION invocation in the first place.
 *
 */
void _return( void )
{

	// a) if this is being invoked outside the context of a FN execution, we cannot continue
	// b) at this point we should also expect an expression - one that resolves, obviously or an error should be thrown
	if ( nested_function_context < 1 )	{ set_error_state( ERROR_MISTAKE ); }
	if ( !expect_expression()        )	{ set_error_state( ERROR_SYNTAX  ); }

	// providing no errors encountered, we can set the "return encountered" flag
	// (see the function commentary above to appreciate what this flag ultimately makes happen)
	return_encountered = !error_state;

}
