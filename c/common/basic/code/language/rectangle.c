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



#include "basic/header/language/rectangle.h"
#include "basic/header/core/basic.h"
#include "basic/header/core/lex.h"
#include "basic/header/core/symbols.h"
#include "basic/header/language/expression.h"	// for expect_token()
#include "os/header/video/video.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void
 * 
 * synopsis			RECTANGLE := RECTANGLE <expr1> , <expr2> , <expr3> , <expr4>
 * 
 * 					expr1 and expr2 equate to the x1,y1 (upper-left)  co-ordinates of the rectange to be drawn
 * 					expr3 and expr4 equate to the x2,y2 (lower-right) co-ordinates of the rectange to be drawn
 * 
 * 					both expr1 and expr3 must be an integer or real and the valid range falls in the logical resolution range (0-1279)
 * 					both expr2 and expr4 must be an integer or real and the valid range falls in the logical resolution range (0-1023)
 * 
 * 					if the rectangle to be rendered specifies co-ordinates where x2<x1 or y2<y1 an "illegal rectangle" result will occur.
 * 
 */
void _rectangle( void )
{

	int i , component[4];

	i = 0;
	while ( (i < 4) && (!error_state) )
	{
	
		expect_expression();
		
		if ( !error_state )
		{
		
			// ensure the stacks are emptied of the postfix notation, and then the final operand will remain on the operand stack
			while ( operator_stack_offset && !error_state )
				process_stacked_operator();
			
			// if no errors (type mismatch?) during postfix processing, then we can complete the assignment
			if ( !error_state )
			{
				switch ( *(operand_stack_ptr + operand_stack_offset-1) )
				{
					case DATATYPE_INTEGER :
						component[i] = pop_operand_integer();
						break;
					case DATATYPE_REAL :
						component[i] = (int) pop_operand_real();
						break;
					default :
						set_error_state( ERROR_TYPE_MISMATCH );
						break;
				}
			}
			
			// expect "," between integers
			if ( (i < 3) && (!expect_token(PUNC_COMMA)) )
				set_error_state( ERROR_EXPECTED_COMMA );
			
			// 8 < - - - - - - - 8 < - - - - - - - 8 < - - - - - - - 8 < - - - - - - - 8 < - - - - - - - 8 < - - - - - - -
			
			
		}
		
		i++;
			
	}
	
	if ( !error_state )
		video_rectangle( component[0],component[1] , component[2],component[3] );
	
	return;
	
}
