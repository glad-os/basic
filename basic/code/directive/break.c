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



#include "basic/header/directive/break.h"
#include "basic/header/language/colour.h"
#include "basic/header/core/basic.h"
#include "basic/header/directive/new.h"
#include "basic/header/directive/run.h"
#include "basic/header/language/colour.h"
#include "os/header/video/video.h"

// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			void
 *
 * returns			void
 *
 * synopsis			performs a hard "break" on the interpreter and OS environment. resets the screen mode, prints the default start-up messages,
 * 					declares no program to be running, switches the execution gate back on, issues a "NEW" directive and finishes by printing
 * 					the input prompt, as the system is declared to be in a state ready to start receiving instructions at that point.
 *
 */
void _directive_break( void )
{

	video_print( "BASIC\n\n" );
	video_print( PROMPT );

	running        = 0;
	execution_gate = 1;
	_directive_new();

}
