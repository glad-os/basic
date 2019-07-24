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



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 



#ifndef LEX_H_
#define LEX_H_



	//
	// constants
	//

	#define ID                  					0x80
	#define STR_LIT             					0x81
	#define NUM_LIT             					0x82
	#define REAL_LIT             					0x83

	#define KEYWORD_REPEAT      					0x90
	#define KEYWORD_UNTIL       					0x91
	#define KEYWORD_PRINT       					0x92
	#define KEYWORD_END         					0x93
	#define KEYWORD_IF          					0x94
	#define KEYWORD_THEN        					0x95
	#define KEYWORD_ELSE        					0x96
	#define KEYWORD_LIST        					0x97
	#define KEYWORD_RUN	        					0x98
	#define KEYWORD_NEW	        					0x99
	#define KEYWORD_RENUMBER      					0x9A
	#define KEYWORD_DEFINE      					0x9B
	#define KEYWORD_FUNCTION      					0x9C
	#define KEYWORD_PROCEDURE     					0x9D
	#define KEYWORD_REM		     					0x9E
	#define KEYWORD_COLOUR	    					0x9F
	#define KEYWORD_MODE	    					0xA0
	#define KEYWORD_CLS		    					0xA1
	#define KEYWORD_TRUE							0xA2
	#define KEYWORD_FALSE							0xA3
	#define KEYWORD_RECTANGLE						0xA4
	#define KEYWORD_LOAD							0xA5
	#define KEYWORD_SAVE							0xA6
	#define KEYWORD_FOR								0xA7
	#define KEYWORD_NEXT							0xA8
	#define KEYWORD_TO								0xA9
	#define KEYWORD_STEP							0xAA
	#define KEYWORD_RETURN							0xAB
	#define KEYWORD_POINT							0xAC
	#define KEYWORD_LINE							0xAD
	#define KEYWORD_MOD								0xAE
	#define KEYWORD_DIV								0xAF
	#define KEYWORD_AND								0xB0
	#define KEYWORD_OR								0xB1
	#define KEYWORD_NOT								0xB2
	#define KEYWORD_EOR								0xB3
	#define KEYWORD_INPUT							0xB4
	
	#define PUNC_OPEN_BRACKET						0xE0
 	#define PUNC_CLOSE_BRACKET						0xE1
	#define PUNC_PLUS								0xE2
	#define PUNC_MINUS								0xE3
	#define PUNC_MULTIPLY							0xE4
	#define PUNC_DIVIDE								0xE5
	#define PUNC_FULL_STOP							0xE6
	#define PUNC_COLON								0xE7
	#define PUNC_SEMI_COLON							0xE8
	#define PUNC_OPEN_CURLY_BRACKET					0xE9
	#define PUNC_CLOSE_CURLY_BRACKET				0xEA
	#define PUNC_OPEN_SQUARE_BRACKET				0xEB
	#define PUNC_CLOSE_SQUARE_BRACKET				0xEC
	#define PUNC_QUOTATION							0xED
	#define PUNC_APOSTROPHE							0xEE
	#define PUNC_EQUALS								0xEF
	#define PUNC_COMMA								0xF0
	#define PUNC_LESS_THAN							0xF1
	#define PUNC_GREATER_THAN						0xF2
	
	#define INPUTCLASS_ALPHA						0x01
	#define INPUTCLASS_NUMERIC						0x02
	#define INPUTCLASS_ALPHANUMERIC					0x03
	#define INPUTCLASS_PRINTABLE					0x04
	#define INPUTCLASS_PUNC_OPEN_BRACKET			0x05
	#define INPUTCLASS_PUNC_CLOSE_BRACKET			0x06
	#define INPUTCLASS_PUNC_PLUS					0x07
	#define INPUTCLASS_PUNC_MINUS					0x08
	#define INPUTCLASS_PUNC_MULTIPLY				0x09
	#define INPUTCLASS_PUNC_DIVIDE					0x0A
	#define INPUTCLASS_PUNC_FULL_STOP				0x0B
	#define INPUTCLASS_PUNC_COLON					0x0C
	#define INPUTCLASS_PUNC_SEMI_COLON				0x0D
	#define INPUTCLASS_PUNC_OPEN_CURLY_BRACKET		0x0E
	#define INPUTCLASS_PUNC_CLOSE_CURLY_BRACKET		0x0F
	#define INPUTCLASS_PUNC_OPEN_SQUARE_BRACKET		0x10
	#define INPUTCLASS_PUNC_CLOSE_SQUARE_BRACKET	0x11
	#define INPUTCLASS_PUNC_QUOTATION				0x12
	#define INPUTCLASS_PUNC_APOSTROPHE				0x13
	#define INPUTCLASS_PUNC_EQUALS					0x14
	#define INPUTCLASS_PUNC_COMMA					0x15
	#define INPUTCLASS_PUNC_LESS_THAN				0x16
	#define INPUTCLASS_PUNC_GREATER_THAN			0x17



	//
	// storage
	//
	
	unsigned char       source[ MAX_INPUT_LENGTH + 1 ];		// +1 to allow for null at end		
	int current_state;
	int current_index;



	//
	// structures
	//
	
	struct inputclass
	{
		int  id;
		char *chars;
	};

	struct transition
	{
		int current_state;
		int input_class;
		int next_state;
		int token;
	};
	
	struct lex_result
	{
		unsigned char str[ MAX_INPUT_LENGTH + 1 ];
		int   token;
	};
	
	struct lex_state
	{
		unsigned char source[ MAX_INPUT_LENGTH + 1 ];		
		int current_state;
		int current_index;
	};
		
	
	
	//
	// prototypes
	//
	
	void          initialise_lex          ( char *input );
	int           next_token              ( struct lex_result *result , int ignore_whitespace );
	int           char_matches_inputclass ( char c , int inputclass_id );
	unsigned char string_matches_keyword  ( char *str );
	char         *string_from_token       ( unsigned char c );
	void          preserve_lex_state      ( struct lex_state *state );
	void          reinstate_lex_state     ( struct lex_state *state );



#endif /*LEX_H_*/
