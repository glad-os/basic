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



#include "basic/header/core/memory.h"
#include "basic/header/core/basic.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			kb_required		integer describing the number of Kb required for the total memory available to BASIC
 *
 * returns			void
 * 
 * synopsis			utilises malloc to initialise the required amount of RAM to be utilised by BASIC for program storage and the heap 
 * 					on success, mem_base and mem_top indicate the first and last bytes of (contiguous) memory available to BASIC
 * 
 */
void initialise_memory( int kb_required )
{
	
	// memory allocation not possible - will be provided directly by kernel in due course
	/*
	if ( (mem_base = malloc(kb_required*1024)) == 0)
	{
		printf( FATAL_MEMORY_ALLOCATION_FAILURE );
		// exit(1);
	}

	mem_top = mem_base + (kb_required * 1024 ) - 1;
	*/

	mem_base = (void *) (6*1024*1024); // mem_base set to 6Mb (6Mb * 1024Kb * 1024bytes)
	mem_top = mem_base + (kb_required * 1024 ) - 1;

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void *			pointer to the last memory location available to BASIC
 * 
 * synopsis			this function can be called to return a pointer to the last memory location available to BASIC 
 * 
 */
void *get_mem_top( void )
{
	
	return mem_top;
	
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void *			pointer to the first memory location available to BASIC
 * 
 * synopsis			this function can be called to return a pointer to the first memory location available to BASIC 
 * 
 */
void *get_mem_base( void )
{
	
	return mem_base;
	
}
