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



#include "basic/header/core/basic.h"
#include "basic/header/core/stack.h"
#include "basic/header/core/symbols.h"
#include "basic/header/language/expression.h"
#include "basic/header/language/point.h"
#include "os/header/video/video.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void
 * 
 * synopsis			LINE := LINE <expr1> , <expr2> , <expr3> , <expr4>
 * 					expr1-4 must be integer or real

 * 					expr1 and expr2 declare the starting X,Y co-ordinate pair of the line to be drawn
 * 					expr3 and expr4 declare the ending   X,Y co-ordinate pair of the line to be drawn
 * 
 * 					both expr1 and expr3 should offer a range of the logical resolution of the screen (0-1279)
 * 					both expr2 and expr4 should offer a range of the logical resolution of the screen (0-1023)
 * 
 * 					uses the video environment to draw a line of single-pixel width from the starting X,Y position to the ending X,Y position
 * 
 */
void _line( void )
{
	
	unsigned char datatype;
	int x1 , y1 , x2 , y2;
	
	if ( !expect_expression() )
		{ set_error_state( ERROR_SYNTAX ); return; }
	datatype = *(operand_stack_ptr + operand_stack_offset-1);
	switch (datatype)
	{
		case DATATYPE_INTEGER :
			x1 = pop_operand_integer();		break;
		case DATATYPE_REAL    :
			x1 = (int) pop_operand_real();	break;
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
			y1 = pop_operand_integer();		break;
		case DATATYPE_REAL    :
			y1 = (int) pop_operand_real();	break;
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
			x2 = pop_operand_integer();		break;
		case DATATYPE_REAL    :
			x2 = (int) pop_operand_real();	break;
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
			y2 = pop_operand_integer();		break;
		case DATATYPE_REAL    :
			y2 = (int) pop_operand_real();	break;
		default :
			set_error_state( ERROR_TYPE_MISMATCH );	break;
	}
	
	video_line(x1,y1 , x2,y2);
		
}
