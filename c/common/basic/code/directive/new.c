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



#include "basic/header/directive/new.h"
#include "basic/header/core/basic.h"
#include "basic/header/core/memory.h"
#include "basic/header/core/symbols.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void
 * 
 * synopsis			new := NEW
 * 					destroys any current stored program in memory by forcing the end-of-program line to be the first. it also forcibly
 * 					initialises the symbol table (ensuring any existing symbols no longer exist)
 *
 * 					the end-of-program line is deemed to have:
 * 					* a line number of  -1 (0xffffffff)
 * 					* a line length of 255 (0xff)
 * 
 */
void _directive_new( void )
{
	
	top = (unsigned char *)get_mem_base();
	
	// store the default "end of source" line marker
	*top = 0xFF; top += 1;
	*top = 0xFF; top += 1;
	*top = 0xFF; top += 1;
	*top = 0xFF; top += 1;
	*top = 0x05;
	
	initialise_symbol_table();
		
}
