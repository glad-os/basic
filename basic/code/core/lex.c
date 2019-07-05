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


// include the library
/*
#include "../library/include/string.h"
*/
#include "ext/stdlib/string.h"

#include "basic/header/core/lex.h"
#include "basic/header/core/basic.h"
#include "basic/header/core/symbols.h"


struct transition stt[] =
{

	{  0 , INPUTCLASS_PUNC_OPEN_BRACKET         ,  1 , PUNC_OPEN_BRACKET         } ,
	{  0 , INPUTCLASS_PUNC_CLOSE_BRACKET        ,  1 , PUNC_CLOSE_BRACKET        } ,
	{  0 , INPUTCLASS_PUNC_PLUS                 ,  1 , PUNC_PLUS                 } ,
	{  0 , INPUTCLASS_PUNC_MINUS                ,  1 , PUNC_MINUS                } ,
	{  0 , INPUTCLASS_PUNC_MULTIPLY             ,  1 , PUNC_MULTIPLY             } ,
	{  0 , INPUTCLASS_PUNC_DIVIDE               ,  1 , PUNC_DIVIDE               } ,
	{  0 , INPUTCLASS_PUNC_FULL_STOP            ,  1 , PUNC_FULL_STOP            } ,
	{  0 , INPUTCLASS_PUNC_COLON                ,  1 , PUNC_COLON                } ,
	{  0 , INPUTCLASS_PUNC_SEMI_COLON           ,  1 , PUNC_SEMI_COLON           } ,
	{  0 , INPUTCLASS_PUNC_OPEN_CURLY_BRACKET   ,  1 , PUNC_OPEN_CURLY_BRACKET   } ,
	{  0 , INPUTCLASS_PUNC_CLOSE_CURLY_BRACKET  ,  1 , PUNC_CLOSE_CURLY_BRACKET  } ,
	{  0 , INPUTCLASS_PUNC_OPEN_SQUARE_BRACKET  ,  1 , PUNC_OPEN_SQUARE_BRACKET  } ,
	{  0 , INPUTCLASS_PUNC_CLOSE_SQUARE_BRACKET ,  1 , PUNC_CLOSE_SQUARE_BRACKET } ,
	{  0 , INPUTCLASS_PUNC_QUOTATION            ,  2 , PUNC_QUOTATION            } ,
	{  0 , INPUTCLASS_PUNC_APOSTROPHE           ,  1 , PUNC_APOSTROPHE           } ,
	{  0 , INPUTCLASS_PUNC_EQUALS               ,  1 , PUNC_EQUALS               } ,
	{  0 , INPUTCLASS_PUNC_COMMA                ,  1 , PUNC_COMMA                } ,
	{  0 , INPUTCLASS_PUNC_LESS_THAN            ,  1 , PUNC_LESS_THAN            } ,
	{  0 , INPUTCLASS_PUNC_GREATER_THAN         ,  1 , PUNC_GREATER_THAN         } ,
	{  0 , INPUTCLASS_ALPHA                     ,  3 , ID                        } ,
	{  0 , INPUTCLASS_NUMERIC                   ,  4 , NUM_LIT                   } ,
		
	{  2 , INPUTCLASS_PRINTABLE                 ,  2 , 0                         } ,
	{  2 , INPUTCLASS_PUNC_QUOTATION            ,  1 , STR_LIT                   } ,
	
	{  3 , INPUTCLASS_ALPHANUMERIC              ,  3 , ID                        } ,
	
	{  4 , INPUTCLASS_NUMERIC                   ,  4 , NUM_LIT                   } ,
	{  4 , INPUTCLASS_PUNC_FULL_STOP            ,  5 , REAL_LIT                  } ,
	
	{  5 , INPUTCLASS_NUMERIC                   ,  5 , REAL_LIT                  } ,
	
};



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 



/**
 * 
 * requires			input			pointer to the string of input that is to be lexically dealt with
 *
 * returns			void
 * 
 * synopsis			this function should be called prior to the lex facilities being invoked. it essentially copies the supplied
 * 					string into the "source" variable, and resets the index and state values associated with lex (which are used 
 * 					to indicate where in the source we currently are, and what 'state' lex has determined). 
 * 
 */
void initialise_lex( char *input )
{

	strcpy( (char *)source , (char *)input );

	current_index = 0;
	current_state = 0;
	
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			state			pointer to a lex_state structure, used for preserving the current state of lex
 *
 * returns			void
 * 
 * synopsis			should the current 'position' (state) of the lexical analyser need to be preserved or put on hold, this 
 * 					function can be called (along with a lex_state structure) to recognise and preserve the current state of lex. 
 * 
 */
void preserve_lex_state( struct lex_state *state )
{
	
	strcpy( (char *)state->source , (char *)source );
	state->current_index = current_index;
	state->current_state = current_state;
	
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			state			pointer to a lex_state structure, used for reinstating the current state of lex
 *
 * returns			void
 * 
 * synopsis			provides complementary functionality to the preserve_lex_state function above. 
 * 
 */
void reinstate_lex_state( struct lex_state *state )
{
	
	strcpy( (char *)source , (char *)state->source );
	current_index = state->current_index;
	current_state = state->current_state;
	
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			result				pointer to a lex_result structure, used for recording the detail of what lex encountered
 * 					ignore_whitespace	Boolean (0/1) value indicating whether whitespace should be ignored or not
 *
 * returns			int					a Boolean (0/1) value indicating a degree of success, as follows:
 * 											0 - there was no useful input left to tokenise
 * 											1 - tokenisation attempted (ie. there was at least 1 char left in the input stream)
 * 
 * synopsis			the "ignore whitespace" parameter is used by the interpreter when wanting to interpret and store lines of 
 * 					source: ordinarily, the interpreter is free to skip any whitespace, but when a user requests a line to be 
 * 					stored in memory, it is important to lexically process a given line precisely as the user intended it. 
 * 
 */
int next_token( struct lex_result *result , int ignore_whitespace )
{

	int        located_transition;
	int        remembered_index;
	int        keyword_matched;
	int        current_token;
	int        stt_size;
	int        i;
	char       c;
	char       extracted[MAX_INPUT_LENGTH + 1];
	
	// initialisation
	stt_size      = sizeof(stt) / sizeof(struct transition);
	current_state = current_token = extracted[0] = 0;
		
	// start lexical anaylsis by skipping unnecessary white space
	if ( ignore_whitespace )
	{
		while ( (current_index < strlen((char *)source)) && (c=source[current_index]) && ((c == 0x20)||(c == 0x09)) )
			current_index++;
	}
	
	// having processed any white space, if the string is exhausted return a "failure" code to the client
	if ( current_index >= strlen((char *)source) )
		return 0;
	
	// main control code for lex - the code that uses the STT to identify (if possible) the next token
	remembered_index = current_index;
	while ( (current_index < strlen((char *)source)) && (c=source[current_index]) )
	{

		// examine the STT
		located_transition = -1;
		c = source[current_index];
		for ( i = 0 ; ( (i < stt_size) && (located_transition == -1) ) ; i++ )
		{
			if ( (stt[i].current_state == current_state) && (char_matches_inputclass(c,stt[i].input_class)) )
				located_transition = i;
		}
		
		if ( located_transition == -1 )
		{
			// no match against the input
			if ( !current_token )
			{
				current_index = remembered_index;
				current_token = source[current_index++];
				extracted[0]  = current_token;
				extracted[1]  = '\0';
			}
			break;
		}
		else
		{
			// stt matched - "consume" the char and progress
			current_index++;
			extracted[strlen(extracted)+1] = '\0';
			extracted[strlen(extracted)  ] = c;
			current_token = stt[located_transition].token;
			current_state = stt[located_transition].next_state;
			// keywords identification - if we're in an [ID] state, check if a keyword token should overrule
			if ( (current_token == ID) && (keyword_matched = string_matches_keyword(extracted)) )
				current_token = keyword_matched;
		}
		
	}

	// return a "successful" result to client
	strcpy( (char *)(*result).str , extracted );
	(*result).token = current_token;
	return 1;
	
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			c					the character to be checked
 * 					inputclass_id		identifier of the "input class" the character is to be checked against
 *
 * returns			int					a Boolean (0/1) value indicating whether the character is contained within the input class 
 * 
 * synopsis			an "input class" is a list of acceptable characters for a certain context. this function allows a comparison
 * 					to be made of whether the character supplied exists in the given input class.
 * 
 */
int char_matches_inputclass( char c , int inputclass_id )
{
	
	int match;
	
	switch ( inputclass_id )
	{
		case INPUTCLASS_ALPHA                     : match = !( strchr( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" , (char) c ) == 0 );            	 break;
		case INPUTCLASS_NUMERIC                   :	match = !( strchr( "0123456789" , (char) c ) == 0 );                                                      	 break;
		case INPUTCLASS_ALPHANUMERIC              :	match = !( strchr( "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_0123456789" , (char) c ) == 0 ); 	 break;
		case INPUTCLASS_PRINTABLE                 :	match = ( ((unsigned char) c >= 32) && ((unsigned char) c <= 127) && ((unsigned char) c != 34) );            break; // excludes " symbol
		case INPUTCLASS_PUNC_OPEN_BRACKET         :	match = ( '('  == (char) c );                                                                                break;
		case INPUTCLASS_PUNC_CLOSE_BRACKET        :	match = ( ')'  == (char) c );                                                                                break;
		case INPUTCLASS_PUNC_PLUS                 :	match = ( '+'  == (char) c );                                                                                break;
		case INPUTCLASS_PUNC_MINUS                :	match = ( '-'  == (char) c );                                                                                break;
		case INPUTCLASS_PUNC_MULTIPLY             :	match = ( '*'  == (char) c );                                                                                break;
		case INPUTCLASS_PUNC_DIVIDE               :	match = ( '/'  == (char) c );                                                                                break;
		case INPUTCLASS_PUNC_FULL_STOP            :	match = ( '.'  == (char) c );                                                                                break;
		case INPUTCLASS_PUNC_COLON                :	match = ( ':'  == (char) c );                                                                                break;
		case INPUTCLASS_PUNC_SEMI_COLON           :	match = ( ';'  == (char) c );                                                                                break;
		case INPUTCLASS_PUNC_OPEN_CURLY_BRACKET   : match = ( '{'  == (char) c );                                                                                break;
		case INPUTCLASS_PUNC_CLOSE_CURLY_BRACKET  :	match = ( '}'  == (char) c );                                                                                break;
		case INPUTCLASS_PUNC_OPEN_SQUARE_BRACKET  :	match = ( '['  == (char) c );                                                                                break;
		case INPUTCLASS_PUNC_CLOSE_SQUARE_BRACKET :	match = ( ']'  == (char) c );                                                                                break;
		case INPUTCLASS_PUNC_QUOTATION            :	match = ( '"'  == (char) c );                                                                                break;
		case INPUTCLASS_PUNC_APOSTROPHE           :	match = ( '\'' == (char) c );                                                                                break;
		case INPUTCLASS_PUNC_EQUALS               :	match = ( '='  == (char) c );                                                                                break;
		case INPUTCLASS_PUNC_COMMA                :	match = ( ','  == (char) c );                                                                                break;
		case INPUTCLASS_PUNC_LESS_THAN            :	match = ( '<'  == (char) c );                                                                                break;
		case INPUTCLASS_PUNC_GREATER_THAN         :	match = ( '>'  == (char) c );                                                                                break;
		default                                   :	match = 0;                                                                                                   break;
	}
	
	return match;
	
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			str				pointer to a string containing the (untokenised) string to be checked
 *
 * returns			unsigned char	the tokenised value of the string supplied, or 0x00 if the process failed.
 * 
 * synopsis			given a string, this function will provide by return the tokenised value of the string.
 * 					if the string is not recognised, the value 0x00 is returned (indicating failure).
 * 
 */
unsigned char string_matches_keyword( char *str )
{

	if ( strcmp( str , "REPEAT"    ) ==0 ) { return KEYWORD_REPEAT   ; }
	if ( strcmp( str , "UNTIL"     ) ==0 ) { return KEYWORD_UNTIL    ; }
	if ( strcmp( str , "PRINT"     ) ==0 ) { return KEYWORD_PRINT    ; }
	if ( strcmp( str , "END"       ) ==0 ) { return KEYWORD_END      ; }
	if ( strcmp( str , "IF"        ) ==0 ) { return KEYWORD_IF       ; }
	if ( strcmp( str , "THEN"      ) ==0 ) { return KEYWORD_THEN     ; }
	if ( strcmp( str , "ELSE"      ) ==0 ) { return KEYWORD_ELSE     ; }
	if ( strcmp( str , "RUN"       ) ==0 ) { return KEYWORD_RUN      ; }
	if ( strcmp( str , "LIST"      ) ==0 ) { return KEYWORD_LIST     ; }
	if ( strcmp( str , "NEW"       ) ==0 ) { return KEYWORD_NEW      ; }
	if ( strcmp( str , "RENUMBER"  ) ==0 ) { return KEYWORD_RENUMBER ; }
	if ( strcmp( str , "DEFINE"    ) ==0 ) { return KEYWORD_DEFINE   ; }
	if ( strcmp( str , "FUNCTION"  ) ==0 ) { return KEYWORD_FUNCTION ; }
	if ( strcmp( str , "PROCEDURE" ) ==0 ) { return KEYWORD_PROCEDURE; }
	if ( strcmp( str , "REM"       ) ==0 ) { return KEYWORD_REM      ; }
	if ( strcmp( str , "COLOUR"    ) ==0 ) { return KEYWORD_COLOUR   ; }
	if ( strcmp( str , "MODE"      ) ==0 ) { return KEYWORD_MODE     ; }
	if ( strcmp( str , "CLS"       ) ==0 ) { return KEYWORD_CLS      ; }
	if ( strcmp( str , "TRUE"      ) ==0 ) { return KEYWORD_TRUE     ; }
	if ( strcmp( str , "FALSE"     ) ==0 ) { return KEYWORD_FALSE    ; }
	if ( strcmp( str , "RECTANGLE" ) ==0 ) { return KEYWORD_RECTANGLE; }
	if ( strcmp( str , "LOAD"      ) ==0 ) { return KEYWORD_LOAD     ; }
	if ( strcmp( str , "SAVE"      ) ==0 ) { return KEYWORD_SAVE     ; }
	if ( strcmp( str , "FOR"       ) ==0 ) { return KEYWORD_FOR      ; }
	if ( strcmp( str , "NEXT"      ) ==0 ) { return KEYWORD_NEXT     ; }
	if ( strcmp( str , "TO"        ) ==0 ) { return KEYWORD_TO       ; }
	if ( strcmp( str , "STEP"      ) ==0 ) { return KEYWORD_STEP     ; }
	if ( strcmp( str , "RETURN"    ) ==0 ) { return KEYWORD_RETURN   ; }
	if ( strcmp( str , "POINT"     ) ==0 ) { return KEYWORD_POINT    ; }
	if ( strcmp( str , "LINE"      ) ==0 ) { return KEYWORD_LINE     ; }
	if ( strcmp( str , "MOD"       ) ==0 ) { return KEYWORD_MOD      ; }
	if ( strcmp( str , "DIV"       ) ==0 ) { return KEYWORD_DIV      ; }
	if ( strcmp( str , "AND"       ) ==0 ) { return KEYWORD_AND      ; }
	if ( strcmp( str , "OR"        ) ==0 ) { return KEYWORD_OR       ; }
	if ( strcmp( str , "NOT"       ) ==0 ) { return KEYWORD_NOT      ; }
	if ( strcmp( str , "EOR"       ) ==0 ) { return KEYWORD_EOR      ; }
	if ( strcmp( str , "INPUT"     ) ==0 ) { return KEYWORD_INPUT    ; }

	// failed to match any known keyword - return 0x00 (false)	
	return 0x00;
	
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			c			an unsigned char indicating the token that requires detokenising
 *
 * returns			char *		pointer to the detokenised string (or 0x00 if not a recognised token)
 * 
 * synopsis			takes the supplied token value (0x00-0xff) and supplies a pointer to the detokenised string by return.
 * 					if the token is not recognised, the pointer is assigned the value 0x00 (indicating failure).
 * 
 */
char *string_from_token( unsigned char c )
{
	
	char *res;
	
	switch ( c )
	{
		case KEYWORD_REPEAT   			: res = "REPEAT";		break; 
		case KEYWORD_UNTIL    			: res = "UNTIL"; 	 	break;
		case KEYWORD_PRINT    			: res = "PRINT"; 	 	break;
		case KEYWORD_END      			: res = "END"; 	 		break;
		case KEYWORD_IF    				: res = "IF"; 	 		break;
		case KEYWORD_THEN    			: res = "THEN"; 	 	break;
		case KEYWORD_ELSE    			: res = "ELSE"; 	 	break;
		case KEYWORD_LIST    			: res = "LIST"; 	 	break;
		case KEYWORD_RUN   				: res = "RUN"; 	 		break;
		case KEYWORD_NEW    			: res = "NEW"; 	 		break;
		case KEYWORD_RENUMBER			: res = "RENUMBER";  	break;
		case KEYWORD_DEFINE				: res = "DEFINE";	  	break;
		case KEYWORD_FUNCTION			: res = "FUNCTION";  	break;
		case KEYWORD_PROCEDURE			: res = "PROCEDURE";  	break;
		case KEYWORD_REM				: res = "REM"; 		 	break;
		case KEYWORD_COLOUR				: res = "COLOUR";	 	break;
		case KEYWORD_MODE				: res = "MODE";	 		break;
		case KEYWORD_CLS				: res = "CLS";	 		break;
		case KEYWORD_TRUE				: res = "TRUE";	 		break;
		case KEYWORD_FALSE				: res = "FALSE";		break;
		case KEYWORD_RECTANGLE			: res = "RECTANGLE";	break;
		case KEYWORD_LOAD				: res = "LOAD";			break;
		case KEYWORD_SAVE				: res = "SAVE";			break;
		case KEYWORD_FOR				: res = "FOR";			break;
		case KEYWORD_NEXT				: res = "NEXT";			break;
		case KEYWORD_TO					: res = "TO";			break;
		case KEYWORD_STEP				: res = "STEP";			break;
		case KEYWORD_RETURN				: res = "RETURN";		break;
		case KEYWORD_POINT				: res = "POINT";		break;
		case KEYWORD_LINE				: res = "LINE";			break;
		case KEYWORD_MOD				: res = "MOD";			break;
		case KEYWORD_DIV				: res = "DIV";			break;
		case KEYWORD_AND				: res = "AND";			break;
		case KEYWORD_OR 				: res = "OR";			break;
		case KEYWORD_NOT				: res = "NOT";			break;
		case KEYWORD_EOR				: res = "EOR";			break;
		case KEYWORD_INPUT				: res = "INPUT";		break;
		case PUNC_OPEN_BRACKET			: res = "(";    	 	break;
		case PUNC_CLOSE_BRACKET			: res = ")";    	 	break;
		case PUNC_PLUS					: res = "+";    	 	break;
		case PUNC_MINUS					: res = "-";    	 	break;
		case PUNC_MULTIPLY				: res = "*";    	 	break;
		case PUNC_DIVIDE				: res = "/";    	 	break;
		case PUNC_FULL_STOP				: res = ".";    	 	break;
		case PUNC_COLON					: res = ":";    	 	break;
		case PUNC_SEMI_COLON			: res = ";";    	 	break;
		case PUNC_OPEN_CURLY_BRACKET	: res = "{";    	 	break;
		case PUNC_CLOSE_CURLY_BRACKET	: res = "}";    	 	break;
		case PUNC_OPEN_SQUARE_BRACKET	: res = "[";    	 	break;
		case PUNC_CLOSE_SQUARE_BRACKET	: res = "]";    	 	break;
		case PUNC_QUOTATION   			: res = "\"";    	 	break;
		case PUNC_APOSTROPHE 			: res = "'";    	 	break;
		case PUNC_EQUALS 				: res = "=";    	 	break;
		case PUNC_COMMA 				: res = ",";    	 	break;
		case PUNC_LESS_THAN				: res = "<";    	 	break;
		case PUNC_GREATER_THAN			: res = ">";    	 	break;
		default							: res = 0; 				break;
	}
	
	return res;
	
}
