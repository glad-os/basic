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



#ifndef STACK_H_
#define STACK_H_



	//
	// storage
	//
	
	int offset;
	unsigned char *ptr;



	//
	// prototypes
	//
	
	void initialise_stack    ( void );
	void empty               ( void );
	int  is_empty            ( void );
	void pushByte            ( unsigned char value );
	void pushInteger         ( unsigned int value );
	void pushString          ( unsigned char *str );
	void pushReal            ( float value );
	unsigned char popByte    ( void );
	unsigned int  popInteger ( void );
	void          popString  ( unsigned char *ptr );
	float         popReal    ( void );
	unsigned char peekByte   ( void );



#endif /*STACK_H_*/
