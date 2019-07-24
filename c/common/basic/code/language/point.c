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



#include "basic/header/language/point.h"
#include "basic/header/core/basic.h"
#include "basic/header/core/stack.h"
#include "basic/header/core/symbols.h"
#include "basic/header/language/expression.h"
#include "os/header/video/video.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void
 * 
 * synopsis			POINT := POINT <expr1> , <expr2>
 * 
 * 					expr1 must be an integer or real, and determines the X co-ordinate of the point to be plotted
 * 					expr2 must be an integer or real, and determines the Y co-ordinate of the point to be plotted
 * 
 * 					the range of X should cover the logical screen resolution (0-1279)
 * 					the range of Y should cover the logical screen resolution (0-1023)
 * 
 * 					calls upon the video environment to plot a single pixel at the appropriate physical position on-screen
 * 
 */
void _point( void )
{
	
	unsigned char datatype;
	int x , y;
	
	if ( !expect_expression() )
		{ set_error_state( ERROR_SYNTAX ); return; }
	datatype = *(operand_stack_ptr + operand_stack_offset-1);
	switch (datatype)
	{
		case DATATYPE_INTEGER :
			x = pop_operand_integer();		break;
		case DATATYPE_REAL    :
			x = (int) pop_operand_real();	break;
		default :
			set_error_state( ERROR_TYPE_MISMATCH );	break;
	}

	if ( !expect_token(PUNC_COMMA) )
		{ set_error_state( ERROR_EXPECTED_COMMA ); return; }

	if ( !expect_expression() )
		{ set_error_state( ERROR_SYNTAX ); return; }
	datatype = *(operand_stack_ptr + operand_stack_offset-1);
	switch (datatype)
	{
		case DATATYPE_INTEGER :
			y = pop_operand_integer();		break;
		case DATATYPE_REAL    :
			y = (int) pop_operand_real();	break;
		default :
			set_error_state( ERROR_TYPE_MISMATCH );	break;
	}
	
	video_point(x,y);
		
}
