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



#include "basic/header/function/get.h"
#include "basic/header/core/basic.h"
#include "basic/header/language/define.h"	// for the optional parameters handling code
#include "basic/header/language/expression.h"
#include "os/header/keyboard/keyboard.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void
 * 
 * synopsis			get := get()
 * 					Awaits a keypress from the user and returns a single-character String with content representing the keyboard character pressed
 * 
 */
void _function_get( void )
{

	unsigned char result[2];

	// get is a parameter-free function
	if ( !expect_token( PUNC_CLOSE_BRACKET ) )
		{ set_error_state( ERROR_EXPECTED_CLOSE_BRACKET );	return; }

	result[0] = keyboard_readchar();
	result[1] = 0x00;

	push_operand_string(result);
	
}
