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




#include "basic/header/core/stack.h"
#include "basic/header/core/basic.h"
#include "basic/header/core/memory.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void
 * 
 * synopsis			this function should be called prior to the interpreter starting up (and after memory has been allocated).
 * 					the stack starts from the top of memory, and works on a descending basic (the idea being that the heap starts from 
 * 					the bottom up, and the stack starts from the top down; if they ever meet/cross, this indicates an "Out of memory" error). 
 * 
 */
void initialise_stack( void )
{
	
	ptr = get_mem_top();
	empty();
	
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void
 * 
 * synopsis			this function should be called to declare that the stack is "empty" (by resetting the stack's offset value)
 * 
 */
void empty( void )
{

	offset = 0;
	
}



// 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - -



/**
 * 
 * requires			void
 *
 * returns			int			a boolean (0/1) value indicating whether or not the stack is currently empty
 * 
 * synopsis			this function offers a simple test to determine whether or not the stack is considered empty
 * 
 */
int is_empty( void )
{

	return ( offset == 0 );
	
}



// 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - -



/**
 * 
 * requires			value			an unsigned char value (equivalent to a single byte) that is to be pushed on the stack
 *
 * returns			void
 * 
 * synopsis			takes the supplied value and pushes it to the stack
 * 
 */
void pushByte( unsigned char value )
{
	
	*(ptr-offset) = value;
	offset += 1;

}



// 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - -



/**
 * 
 * requires			value			an unsigned int value (equivalent to a 4-byte Integer) that is to be pushed on the stack
 *
 * returns			void
 * 
 * synopsis			takes the supplied value and pushes it to the stack
 * 
 */
void pushInteger( unsigned int value )
{
	
	pushByte( (value >> 24) & 0xff );
	pushByte( (value >> 16) & 0xff );
	pushByte( (value >>  8) & 0xff );
	pushByte( (value      ) & 0xff );
	
}



// 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - -



/**
 * 
 * requires			str			pointer to the string that is to be pushed on the stack
 *
 * returns			void
 * 
 * synopsis			takes the supplied value and pushes it to the stack. note that strings are pushed to the stack in a 
 * 					back-to-front fashion, so that popping them from the stack becomes the simple process of popping until the
 * 					string terminator (0x00) value is encountered (this saves having to also push the LENGTH of the string on the stack)
 * 
 */
void pushString( unsigned char *str )
{

	int index;
	
	index = 0;	
	while ( *(str+index++) ) {}
	index--;
	
	while ( index >= 0 )
	{
		*(ptr-offset) = *(str+index--);
		offset += 1;
	}
	
}



// 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - -



/**
 * 
 * requires			value			a float value (used to store "real" values in BASIC) that is to be pushed on the stack
 *
 * returns			void
 * 
 * synopsis			takes the supplied value and pushes it to the stack
 * 
 */
void pushReal( float value )
{
	
	float *x;
	
	offset += 3;
	x = (float *)ptr-offset;
	*x = value;
	
	offset += 1;
	
}



// 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - -



/**
 * 
 * requires			void
 *
 * returns			int			the value popped from the stack
 * 
 * synopsis			pops the required value from the stack
 * 
 */
unsigned char popByte( void )
{

	unsigned char result;
	
	if ( is_empty() )
		{ set_error_state(ERROR_STACK_UNDERFLOW); return 0; }
	
	offset -= 1;
	result  = *(ptr-offset);
	
	return result;
	
}



// 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - -



/**
 * 
 * requires			void
 *
 * returns			unsigned int			the unsigned int value (equivalent to a 4-byte Integer) popped from the stack
 * 
 * synopsis			pops the required value from the stack
 * 
 */
unsigned int popInteger( void )
{
	
	unsigned char p1,p2,p3,p4;
	unsigned int  value;
	
	p1    = popByte();
	p2    = popByte();
	p3    = popByte();
	p4    = popByte();
	value = ( (p1<<0) + (p2<<8) + (p3<<16) + (p4<<24) );
	
	return value;
	
}



// 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - -



/**
 * 
 * requires			ptr			pointer to the string value (populated by the function as the value is popped from the stack)
 *
 * returns			void			
 * 
 * synopsis			pops the required value from the stack
 * 
 */
void popString( unsigned char *ptr )
{
	
	int index;
	unsigned char ch;

	index = 0;
	while ( (ch = popByte()) )
	{
		*(ptr+index++) = ch;
	}
	*(ptr+index) = 0x00;
	
}
	


// 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - -



/**
 * 
 * requires			void
 *
 * returns			float			the float value (used to store "real" values in BASIC) popped from the stack
 * 
 * synopsis			pops the required value from the stack
 * 
 */
float popReal( void )
{
	
	float *x;
	float value;
	
	offset -= 1;
	x = (float *)ptr-offset;
	value = *x;
	
	offset -= 3;
	
	return value;
	
}



// 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - - 8 < - - - - -



/**
 * 
 * requires			void
 *
 * returns			unsigned char		the value "peeked" at from the stack
 * 
 * synopsis			returns the next value that would be popped from the stack
 * 
 */
unsigned char peekByte( void )
{

	unsigned char result;
	
	if ( is_empty() )
		{ set_error_state(ERROR_STACK_UNDERFLOW); return 0; }
	
	offset -= 1;
	result  = *(ptr-offset);
	offset += 1;
	
	return result;
	
}
