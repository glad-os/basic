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



#include "basic/header/directive/escape.h"
#include "basic/header/core/basic.h"
#include "os/header/video/video.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 * 
 * returns			void
 * 
 * synopsis			provides functionality to react to the <Esc> key having been pressed. The precise reaction depends on whether a stored
 * 					program is currently running or not.
 * 
 */
void _directive_escape( void )
{

	// it only makes sense to react to <Escape> when running a stored program
	if ( running )
	{
		set_error_state( ERROR_ESCAPE );
		escape  = 1;
		running = 0;
	}
	else
	{
		video_printline( "" );
		video_print( ERROR_ESCAPE );
		*input = 0x00;
	}
		
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void
 * 
 * synopsis			resets the escape status flag (a Boolean value indicating whether an <Esc> keypress has been flagged)
 * 
 */
void reset_escape_status( void )
{

	escape = 0;
	
}
