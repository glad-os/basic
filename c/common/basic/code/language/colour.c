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



#include "basic/header/language/colour.h"
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
 * synopsis			COLOUR := COLOUR <expr1> [ <expr2> , <expr3> , <expr4> ]
 *
 * 					In the first instance, expr1 must evaluate to an integer, and must fall in the range 0-7 to request
 * 					the foreground colour be set (0-7) a-la BBC Micro; if the range has bit 7 set (128) change the background colour instead
 *
 * 					In the second instance, expr1 must evaluate to an integer, where 0 indicates a foreground colour change and 1 indicates a
 * 					background colour change. After this, expr2-4 must all evaluate to integers and fall in the range 0-255, indicating the
 * 					individual red, green and blue components of the colour required.
 *
 */
void _colour( void )
{

	int component[4];
	int i;
	int style;
	unsigned char datatype;

	i     = 0;
	style = 4;
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

				datatype = *(operand_stack_ptr + operand_stack_offset-1);
				switch (datatype)
				{
					// this shouldn't be here - but for now, let REALs work (user should force a CAST operation, knowingly!)
					case DATATYPE_REAL :
					{
						component[i] = (int) pop_operand_real();
						if ( component[i] < 0x00 ) { component[i] = 0x00; }
						if ( component[i] > 0xff ) { component[i] = 0xff; }
						break;
					}
					case DATATYPE_INTEGER :
					{
						component[i] = pop_operand_integer();
						if ( component[i] < 0x00 ) { component[i] = 0x00; }
						if ( component[i] > 0xff ) { component[i] = 0xff; }
						break;
					}
					default :
					{
						set_error_state( ERROR_TYPE_MISMATCH );
						break;
					}
				}

			}

			// expect "," between integers
			if ( i < 3 )
			{
				if ( !expect_token(PUNC_COMMA) )
				{
					if ( i == 0 )
					{
						// accept - a single colour value's been given (e.g. COLOUR 0-7)
						i     = 4;
						style = 1;
					}
					else
 						set_error_state( ERROR_EXPECTED_COMMA );
				}
			}

			// 8 < - - - - - - - 8 < - - - - - - - 8 < - - - - - - - 8 < - - - - - - - 8 < - - - - - - - 8 < - - - - - - -

		}

		i++;

	}

	if ( !error_state )
	{
		if ( style == 4 )
		{
			switch ( component[0] )
			{
				case 0  : video_setcolour( 0, component[1], component[2], component[3] ); break;
				case 1  : video_setcolour( 1, component[1], component[2], component[3] ); break;
				default : set_error_state( ERROR_ILLEGAL_COLOUR ); break;
			}
		}
		else
		{
			switch ( component[0] )
			{
				case     0 : { video_setcolour( 1, 0x00, 0x00, 0x00 ); break; }
				case     1 : { video_setcolour( 1, 0xff, 0x00, 0x00 ); break; }
				case     2 : { video_setcolour( 1, 0x00, 0xff, 0x00 ); break; }
				case     3 : { video_setcolour( 1, 0xff, 0xff, 0x00 ); break; }
				case     4 : { video_setcolour( 1, 0x00, 0x00, 0xff ); break; }
				case     5 : { video_setcolour( 1, 0xff, 0x00, 0xff ); break; }
				case     6 : { video_setcolour( 1, 0x00, 0xff, 0xff ); break; }
				case     7 : { video_setcolour( 1, 0xff, 0xff, 0xff ); break; }
				case 128+0 : { video_setcolour( 0, 0x00, 0x00, 0x00 ); break; }
				case 128+1 : { video_setcolour( 0, 0xff, 0x00, 0x00 ); break; }
				case 128+2 : { video_setcolour( 0, 0x00, 0xff, 0x00 ); break; }
				case 128+3 : { video_setcolour( 0, 0xff, 0xff, 0x00 ); break; }
				case 128+4 : { video_setcolour( 0, 0x00, 0x00, 0xff ); break; }
				case 128+5 : { video_setcolour( 0, 0xff, 0x00, 0xff ); break; }
				case 128+6 : { video_setcolour( 0, 0x00, 0xff, 0xff ); break; }
				case 128+7 : { video_setcolour( 0, 0xff, 0xff, 0xff ); break; }
			}
		}

	}

	return;

}
