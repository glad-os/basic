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



#include "basic/header/core/lex.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 



#ifndef EXPRESSION_H_
#define EXPRESSION_H_



	//
	// structures
	//
	
	struct stacked_operator
	{
		unsigned char token;
		unsigned char precedence;
	};



	//
	// storage
	//

	unsigned char   precedence_offset;		// consumes 1 byte (0-255)
	unsigned char * operand_stack_ptr;
	unsigned char * operator_stack_ptr;
	unsigned int    operand_stack_offset;
	unsigned int    operator_stack_offset;
	struct lex_result expression_result;
	unsigned int current_operator_stack_offset_base;



	//
	// prototypes
	//
	
	int expect_expression                ( void );
	int expect_term                      ( void );
	int expect_factor                    ( void );
	int expect_token                     ( unsigned char token_value );
	void push_operator                   ( struct stacked_operator );
	struct stacked_operator pop_operator ( void );
	void process_stacked_operator        ( void );
	void push_operand_integer            ( int            value );
	void push_operand_real               ( float          value );
	void push_operand_string             ( unsigned char *value );
	void push_operand_boolean            ( unsigned char  value );
	int pop_operand_integer              ( void );
	float pop_operand_real               ( void );
	void pop_operand_string              ( unsigned char *value );
	unsigned char pop_operand_boolean    ( void );
	void _initialise_expression_stacks   ( void );



#endif /*EXPRESSION_H_*/
