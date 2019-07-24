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
#include "string.h"
#include "stdlib.h"
#include "maths.h"

#include "basic/header/language/expression.h"
#include "basic/header/core/basic.h"
#include "basic/header/core/lex.h"
#include "basic/header/core/stack.h"
#include "basic/header/core/symbols.h"
#include "basic/header/directive/run.h"
#include "basic/header/function/abs.h"
#include "basic/header/function/asc.h"
#include "basic/header/function/chr.h"
#include "basic/header/function/concat.h"
#include "basic/header/function/cos.h"
#include "basic/header/function/get.h"
#include "basic/header/function/inkey.h"
#include "basic/header/function/instr.h"
#include "basic/header/function/int.h"
#include "basic/header/function/left.h"
#include "basic/header/function/len.h"
#include "basic/header/function/mid.h"
#include "basic/header/function/right.h"
#include "basic/header/function/rnd.h"
#include "basic/header/function/sgn.h"
#include "basic/header/function/sin.h"
#include "basic/header/function/sqr.h"
#include "basic/header/function/str.h"
#include "basic/header/function/tan.h"
#include "basic/header/function/time.h"
#include "basic/header/function/val.h"
#include "basic/header/language/define.h"	// for the optional parameters handling code



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			void
 *
 * returns			int			boolean value determining whether the result of looking for an expression resulted in an error state being set
 *
 * synopsis			when expecting an expression, we should stack the current positions of the stack pointers. then when we've finished, we pop the stacks until
 * 					we get back to where the stacks originally were. this allows for recursive use of the stacks, important for expressions with functions in 'em.
 * 					notice how an Id can be a reference to a FUNCTION (the parentheses after help us differentiate)
 *
 * 					Expr   := Term { + Term | - Term | mod Term | div Term } [ [=|AND|OR|EOR] Expr]
 * 					Term   := Factor { * Factor | / Factor }
 * 					Factor := NOT Factor | +Factor | -Factor | Id {(params)} | Num | Str | Boolean | ( Expr )    (NOT/+/- are recursive?)
 *
 * 					unary operators
 * 					---------------
 * 					"+" and "-" are also indicators of the unary operator
 * 					-> they are optional
 * 					-> they (i think) should go at the start of Factor
 * 					-> they have a higher precedence than their binary counterparts "+" (plus) and "-" (minus)
 *
 * 					NOTE
 * 					----
 * 					this whole expression engine could do with a decent rewrite. it works, but it could be a lot better second time round - this is really the heart of
 * 					the language; initial build has been good but slow addition of functions, recursive aspects and extra operators (bitwise) has shown a rewrite could improve
 * 					not only its efficiency, but the author's depth of understanding of this area and its maintainability for the future.
 *
 */
int expect_expression(void)
{

	struct stacked_operator value;
	struct lex_state        state;
	unsigned int            temp_operator_stack_offset;
	int                     temp_precedence_offset;

	 // first things first - if expression expected and there's nothing in front of us, automatically fail
	preserve_lex_state(&state);
	if ( !next_token( &expression_result , TRUE ) )
	{
		set_error_state(ERROR_BAD_EXPRESSION);
		return !error_state;
	}
	reinstate_lex_state(&state);

	 // remember operator stack offset and precedence offset values - you only "pop" to this position
	 // when complete and reinstate the current precedence value at the end of this expression (allows recursive expressions wrt FUNCTIONs)
	 temp_operator_stack_offset = operator_stack_offset;
	 temp_precedence_offset     = precedence_offset;
	 precedence_offset = 0;
	 current_operator_stack_offset_base = operator_stack_offset; // make a mental note of this so process_stacked_operator appreciates the "base"

	if ( !expect_term() )
		{ set_error_state( ERROR_SYNTAX ); return FALSE; }
	while ( expect_token(PUNC_PLUS) || expect_token(PUNC_MINUS) || expect_token(KEYWORD_MOD) || expect_token(KEYWORD_DIV) )
	{
		value.token      = expression_result.token;
		value.precedence = precedence_offset + 1;
		push_operator( value );
	 	if ( !expect_term() ) break;
	}
	// new bit - toying with "=Expr" to start adding the whole basic idea of "if expr" where "expr" can be "expr = expr"...
	// also add in AND and OR
	if ( expect_token(PUNC_EQUALS) || expect_token(PUNC_LESS_THAN) || expect_token(PUNC_GREATER_THAN) || expect_token(KEYWORD_AND) || expect_token(KEYWORD_OR) || expect_token(KEYWORD_EOR) )
	{
		value.token      = expression_result.token;
		if ( value.token == PUNC_EQUALS || value.token == PUNC_LESS_THAN || value.token == PUNC_GREATER_THAN )
			value.precedence = precedence_offset+1;	// not thought about precedence yet (should = be lowest?)
		else
			value.precedence = precedence_offset;	// not thought about precedence yet (should = be lowest?)
		push_operator( value );
		expect_expression();
	}

	// ensure stacks are emptied of postfix notation - unless stacks are badly balanced, the final operand will remain on-stack
	// (take into account original positions of stack pointers, see comments at top of function definition)
	// also reinstate the original precedence offset value
	while ( (operator_stack_offset > temp_operator_stack_offset) && !error_state )
		process_stacked_operator();
	precedence_offset = temp_precedence_offset;

	// if error state remains clear, this method succeeded (expression value remains on stack)
	return (error_state == 0) ? TRUE : FALSE;

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			void
 *
 * returns			int			boolean declaring whether or not a Term was found
 *
 * synopsis			this method is part of the main expression engine, implemented using recursive descent methods.
 *
 */
int expect_term( void )
{

	struct stacked_operator value;

	if ( !expect_factor() )
		{ set_error_state( ERROR_SYNTAX ); return FALSE; }

	if ( !error_state )
	{
		if ( expect_token(PUNC_MULTIPLY) || expect_token(PUNC_DIVIDE) )
		{
			value.token      = expression_result.token;
			value.precedence = precedence_offset + 2;
			push_operator( value );
			expect_term();
		}

		return TRUE;

	}
	else
		return FALSE;
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			void
 *
 * returns			int			boolean declaring whether or not a Factor was found
 *
 * synopsis			this method is part of the main expression engine, implemented using recursive descent methods.
 * 					the size of this method indicates the importance of the elementary definition of a "Factor" - this method has grown over time to help cater with
 * 					aspects such as recursion ("local" symbol tables) and functions (both internal and user-defined).
 *
 */
int expect_factor( void )
{

	int symbol_table_index , st_position , temp_running;
	unsigned char symbol_table_type , st_type , temp_runtime_linelength;
	unsigned char *local_symbol_table , *temp_runtime_ptr , *temp_symbol_table_base , *temp_heap_top;
	union symbol_table_entry_value symbol_table_entry , st_value;
	struct stacked_operator value;
	struct lex_state state , state2;
	struct lex_result temp_expression_result;

	// deal with a potential unary prefix of "+" or "-" or "NOT"
	while ( expect_token(PUNC_PLUS) || expect_token(PUNC_MINUS) || expect_token(KEYWORD_NOT) )
	{
		value.token      = expression_result.token;
		value.precedence = precedence_offset + 3;
		push_operator( value );
		expression_result.token = 0x00; // reset required
	}

	preserve_lex_state(&state);
	if ( next_token( &expression_result , TRUE ) )
	{

		switch ( expression_result.token )
		{
			case ID :
			{

				// this is where we'll need to differentiate between an identifier of a variable and one of a FUNCTION...
				temp_expression_result = expression_result;
				if ( expect_token(PUNC_OPEN_BRACKET) )
				{
					expression_result = temp_expression_result;

					// 8 < - - - - - - 8 < - - - - - - 8 < - - - - - - 8 < - - - - - - 8 < - - - - - - 8 < - - - - - - 8 < - - - - - -

					// first step is to stack the current "extent" of the symbol table (too early?!!)
					temp_symbol_table_base = symbol_table_base;
					temp_heap_top          = heap_top;
					local_symbol_table  = heap_top; // remember where we're going to start from
					*local_symbol_table = 0x00;		// and initialise it before we do anything with it

					// new! see if it's an in-built (library) function such as abs...
					if      ( strcmp((char *)expression_result.str,"abs"    ) == 0 ) { _function_abs();    }
					else if ( strcmp((char *)expression_result.str,"asc"    ) == 0 ) { _function_asc();    }
					else if ( strcmp((char *)expression_result.str,"chr"    ) == 0 ) { _function_chr();    }
					else if ( strcmp((char *)expression_result.str,"concat" ) == 0 ) { _function_concat(); }
					else if ( strcmp((char *)expression_result.str,"cos"    ) == 0 ) { _function_cos();    }
					else if ( strcmp((char *)expression_result.str,"get"    ) == 0 ) { _function_get();    }
					else if ( strcmp((char *)expression_result.str,"inkey"  ) == 0 ) { _function_inkey();  }
					else if ( strcmp((char *)expression_result.str,"instr"  ) == 0 ) { _function_instr();  }
					else if ( strcmp((char *)expression_result.str,"int"    ) == 0 ) { _function_int();    }
					else if ( strcmp((char *)expression_result.str,"left"   ) == 0 ) { _function_left();   }
					else if ( strcmp((char *)expression_result.str,"len"    ) == 0 ) { _function_len();    }
					else if ( strcmp((char *)expression_result.str,"mid"    ) == 0 ) { _function_mid();    }
					else if ( strcmp((char *)expression_result.str,"right"  ) == 0 ) { _function_right();  }
					else if ( strcmp((char *)expression_result.str,"rnd"    ) == 0 ) { _function_rnd();    }
					else if ( strcmp((char *)expression_result.str,"sgn"    ) == 0 ) { _function_sgn();    }
					else if ( strcmp((char *)expression_result.str,"sin"    ) == 0 ) { _function_sin();    }
					else if ( strcmp((char *)expression_result.str,"sqr"    ) == 0 ) { _function_sqr();    }
					else if ( strcmp((char *)expression_result.str,"str"    ) == 0 ) { _function_str();    }
					else if ( strcmp((char *)expression_result.str,"tan"    ) == 0 ) { _function_tan();    }
					else if ( strcmp((char *)expression_result.str,"time"   ) == 0 ) { _function_time();   }
					else if ( strcmp((char *)expression_result.str,"val"    ) == 0 ) { _function_val();    }
					else
					{

						// ok - locate the FUNCTION's identifier in the symbol table
						st_position = position_in_symbol_table_procfn( expression_result.str );
						st_type     = symbol_table_entry_type_procfn( st_position );
						if ( st_type != DATATYPE_FUNCTION )
							{ set_error_state( ERROR_UNKNOWN_FUNCTION ); return FALSE; }

						// if you are in interactive mode and use an expression with a FN name in it, we need to "pretend"
						// we're "running" for the moment (as _interpret() operates in a "running" context)
						temp_running = running;
						running      = 1;

						st_value = symbol_table_entry_value_procfn( st_position );

						// now we have to (optionally) expect a parameter list. set parameter_count automatically as if no "(" is found, we don't need
						// to call _expect_parameters (which in itself resets the known parameter count)
						parameter_count = 0;
						if ( !expect_token( PUNC_CLOSE_BRACKET ) )
						{
							_expect_parameters( ACTUAL , st_position );
							if ( !expect_token( PUNC_CLOSE_BRACKET ) )
								{ set_error_state( ERROR_EXPECTED_CLOSE_BRACKET ); return FALSE; }
						}

						// if we didn't match the number of parameters, complain
						if ( number_of_parameters( st_position ) != parameter_count )
							{ set_error_state( ERROR_BAD_PARAMETERS ); return FALSE; }

						// ok - now let's invoke our new local symbol table (is this too late in recursion? (FN calling FN)??
						symbol_table_base  = local_symbol_table;

						temp_runtime_ptr        = runtime_ptr;
						temp_runtime_linelength = runtime_linelength;
						preserve_lex_state(&state2);

						runtime_ptr        = (unsigned char *)st_value.pval;	// this is a pointer to the declared block of code
						nested_function_context++;								// recognise we are now 1 level deeper into a FN call (otherwise RETURN cannot recognise if it's being called correctly or not)
						_interpret();
						nested_function_context--;
						runtime_ptr        = temp_runtime_ptr;
						runtime_linelength = temp_runtime_linelength;
						reinstate_lex_state(&state2);

						// remember to restore the previous state of the "running" flag (see earlier)
						running = temp_running;

					}

					// pop the (previous) symbol table "extent" that was stacked
					heap_top          = temp_heap_top;
					symbol_table_base = temp_symbol_table_base;
					*heap_top = 0;

					// 8 < - - - - - - 8 < - - - - - - 8 < - - - - - - 8 < - - - - - - 8 < - - - - - - 8 < - - - - - - 8 < - - - - - -
				}
				else
				{

					// this is where we have to take the identifier supplied and substitute in the actual value from the symbol table
					// if the identifier cannot be located, then we fail with a "No such variable" error state
					expression_result = temp_expression_result;
					symbol_table_index = position_in_symbol_table( expression_result.str );
					if ( !symbol_table_index )
					{
						set_error_state( ERROR_NO_SUCH_VARIABLE );
						return FALSE;
					}
					symbol_table_type  = symbol_table_entry_type ( symbol_table_index );
					symbol_table_entry = symbol_table_entry_value( symbol_table_index );

					switch ( symbol_table_type )
					{
						case DATATYPE_REAL    :	{ push_operand_real( symbol_table_entry.rval );						break; }
						case DATATYPE_INTEGER :	{ push_operand_integer( symbol_table_entry.ival );					break; }
						case DATATYPE_STRING  : { push_operand_string( (unsigned char *)symbol_table_entry.sval );	break; }
						case DATATYPE_BOOLEAN :	{ push_operand_boolean( symbol_table_entry.bval );					break; }
						default               : { set_error_state( ERROR_NO_SUCH_VARIABLE ); return FALSE;			break; }
					}

				}
				break;
			}
			case REAL_LIT :
			{
				push_operand_real( atof((char *)expression_result.str) );
				break;
			}
			case NUM_LIT :
			{
				push_operand_integer( atoi((char *)expression_result.str) );
				break;
			}
			case STR_LIT :
			{

				// remove quotation marks from the string literal (start at index 1, and nullify the final char)
				unsigned char *string = expression_result.str;
				*( string + strlen((char *)string)-1 ) = 0x00;
				push_operand_string( string+1 );
				break;
			}
			case PUNC_OPEN_BRACKET :
			{

				precedence_offset += 10;
				expect_expression();
				if ( !expect_token(PUNC_CLOSE_BRACKET) )
				{
					set_error_state( ERROR_EXPECTED_CLOSE_BRACKET );
					break;
				}
				precedence_offset -= 10;
				break;
			}
			case KEYWORD_TRUE :
			{
				push_operand_boolean( 1 );
				break;
			}
			case KEYWORD_FALSE :
			{
				push_operand_boolean( 0 );
				break;
			}
			default :
			{
				set_error_state( ERROR_SYNTAX );
				return FALSE;
				break;
			}
		}
	}
	else
		reinstate_lex_state(&state);

	return TRUE;

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			token_value		declaration from the client as to what token is being (ideally) expected at this point in time
 *
 * returns			int				boolean value indicating whether the token was found
 *
 * synopsis			client calls this to see if the next token is what they expect it to be. returns an int (bool) indicating whether this was in fact
 *					encountered or not, and if it wasn't encountered then lex is reset to its pre-entry state, so the client can potentially perform other
 *					tests if so desired.
 *
 */
int expect_token( unsigned char token_value )
{

	// Question: Do I *need* to preserve/reinstate lex state? Surely I work optimistically - grab next token and see if it matches
	// one of a set of possibilities; if it doesn't, as an interpreter you simply fail and stop there...
	struct lex_state state;

	// ensure that this is set to zero before you check the token, as otherwise the value in "token" can remain across calls
	// (this caused havoc with END PROCEDURE coming back to END; flow of code meant expect_token was called on the END line and this had
	// PROCEDURE token still sat in its value, so stack was popped once again for return address and fatal underflows ensued!)
	expression_result.token = 0;

	// preserve lex state; consume if successful match, otherwise reinstate pre-entry lex state
	preserve_lex_state(&state);
	next_token( &expression_result , TRUE );
	if ( expression_result.token != token_value )
		{ reinstate_lex_state(&state); }

	return ( expression_result.token == token_value );

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			value			the integer to be pushed onto the operand stack
 *
 * returns			void
 *
 * synopsis			pushes an integer value onto the operand stack
 *
 */
void push_operand_integer( int value )
{

	// stack the integer in a 4-byte (MSB..LSB) format and append the "datatype indicator"
	*(operand_stack_ptr+operand_stack_offset) = ((value >> 24) & 0xff);	operand_stack_offset++;
	*(operand_stack_ptr+operand_stack_offset) = ((value >> 16) & 0xff);	operand_stack_offset++;
	*(operand_stack_ptr+operand_stack_offset) = ((value >>  8) & 0xff);	operand_stack_offset++;
	*(operand_stack_ptr+operand_stack_offset) = ((value      ) & 0xff);	operand_stack_offset++;
	*(operand_stack_ptr+operand_stack_offset) = DATATYPE_INTEGER;		operand_stack_offset++;

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			value			the real to be pushed onto the operand stack
 *
 * returns			void
 *
 * synopsis			pushes a real value onto the operand stack
 *
 */
void push_operand_real( float value )
{

	// stack the float in a 4-byte format and append the "datatype indicator"
	*((float *)(operand_stack_ptr+operand_stack_offset)) = value; operand_stack_offset += 4;
	*(operand_stack_ptr+operand_stack_offset) = DATATYPE_REAL;    operand_stack_offset++;

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			value			the string to be pushed onto the operand stack
 *
 * returns			void
 *
 * synopsis			pushes a string onto the operand stack
 *
 */
void push_operand_string( unsigned char *value )
{

	// stack the string in a back-to-front ordering format and append the "datatype indicator"
	unsigned char *ptr;

	// find end of string (the null terminator)
	ptr = value;
	while ( *ptr )
		ptr++;

	// stack string backwards
	while ( ptr >= value )
	{
		*(operand_stack_ptr+operand_stack_offset) = *ptr;
		operand_stack_offset++;
		ptr--;
	}

	// stack the datatype indicator
	*(operand_stack_ptr+operand_stack_offset) = DATATYPE_STRING;
	operand_stack_offset++;

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			value			the boolean to be pushed onto the operand stack
 *
 * returns			void
 *
 * synopsis			pushes a boolean value onto the operand stack
 *
 */
void push_operand_boolean( unsigned char value )
{

	// stack the boolean as a single byte
	*(operand_stack_ptr+operand_stack_offset) = value;	operand_stack_offset++;

	// stack the datatype indicator
	*(operand_stack_ptr+operand_stack_offset) = DATATYPE_BOOLEAN;
	operand_stack_offset++;

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			void
 *
 * returns			int			the integer popped from the operand stack
 *
 * synopsis			pops an integer value from the operand stack
 *
 */
int pop_operand_integer( void )
{

	// integers are stacked in a 4-byte (MSB..LSB) format followed by the 1-byte datatype indicator.
	// so pop it off the stack in reverse order
	int value;

	operand_stack_offset -= 5;
	value  = *(operand_stack_ptr+operand_stack_offset+0) << 24;
	value += *(operand_stack_ptr+operand_stack_offset+1) << 16;
	value += *(operand_stack_ptr+operand_stack_offset+2) <<  8;
	value += *(operand_stack_ptr+operand_stack_offset+3);

	return value;

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			void
 *
 * returns			float		the float popped from the operand stack
 *
 * synopsis			pops a float value from the operand stack
 *
 */
float pop_operand_real( void )
{

	// float stacked in 4-bytes followed by the 1-byte datatype indicator.
	// so pop it off the stack in reverse order
	float value;

	operand_stack_offset -= 5;
	value = *((float *)(operand_stack_ptr+operand_stack_offset+0));

	return value;

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			unsigned char *		pointer to where the popped string will be copied to
 *
 * returns			void
 *
 * synopsis			pops a string from the operand stack
 *
 */
void pop_operand_string( unsigned char *value )
{

	// string indicator pops first, then string can be popped in correct order (stacked backwards)
	unsigned char *start,*end,*p1,*p2,e1,e2;

	// pop (ignoring) the datatype indicator and point at the first element of the string
	operand_stack_offset -= 2;

	// determine the start and end locations of the string
	// ensure that at the end the stack offset remains pointing at the (new) next free location
	end = operand_stack_ptr + operand_stack_offset;
	while ( *(operand_stack_ptr + operand_stack_offset) )
		operand_stack_offset--;
	start = operand_stack_ptr + operand_stack_offset;

	// get the string into the parameter that the client passed
	int strlength;
	strlength = end - start + 1;
	memcpy( (void *)value , (void *)start , strlength );

	// reverse the string
	p1 = value;
	p2 = value + strlength - 1;
	while ( p1 < p2 )
	{
		e1    = *p1;
		e2    = *p2;
		*p1++ = e2;
		*p2-- = e1;
	}

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			void
 *
 * returns			unsigned char	the boolean popped from the operand stack
 *
 * synopsis			pops a boolean value from the operand stack
 *
 */
unsigned char pop_operand_boolean( void )
{

	// integers are stacked in a 4-byte (MSB..LSB) format followed by the 1-byte datatype indicator.
	// so pop it off the stack in reverse order
	unsigned char value;

	operand_stack_offset -= 2;
	value  = *(operand_stack_ptr+operand_stack_offset+0);

	return value;

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			value		a stacked_operator struct containing the operator details to be pushed onto the operator stack
 *
 * returns			void
 *
 * synopsis			an operator on the operator stack consumes 2 bytes:
 * 					1 byte for the token representing the operator
 * 					1 byte for the precedence value
 *
 */
void push_operator( struct stacked_operator value )
{

	struct stacked_operator op;
	int process;

	// don't just do it once - do it as many times as there are operators of a higher precedence than you
	while ( operator_stack_offset && (operator_stack_offset > current_operator_stack_offset_base) )
	{

		op = pop_operator();
		push_operator(op);

		// determine LHS or RHS associativity - binary are LHS, unary are RHS
		if ( value.precedence % 10 == 3 )
			process = ( op.precedence >  value.precedence );
		else
			process = ( op.precedence >= value.precedence );

		if ( process )
	 		process_stacked_operator();
	 	else
	 		break;

	}

	*(operator_stack_ptr+operator_stack_offset+0) = value.token;
	*(operator_stack_ptr+operator_stack_offset+1) = value.precedence;
	operator_stack_offset += 2;

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			void
 *
 * returns			stacked_operator	provides details of the operator popped from the operator stack
 *
 * synopsis			an operator on the operator stack consumes 2 bytes:
 * 					1 byte for the token representing the operator
 * 					1 byte for the precedence value
 *
 */
struct stacked_operator pop_operator( void )
{

	struct stacked_operator value;

	if ( operator_stack_offset <= 0 )
	{
		set_error_state( ERROR_OUT_OF_OPERATORS );
		value.precedence = 0x00;
		value.token      = 0x00;
		return value;
	}

	// don't forget you pop them in reverse order than what you stacked them...
	value.precedence = *(operator_stack_ptr+operator_stack_offset-1);
	value.token      = *(operator_stack_ptr+operator_stack_offset-2);
	operator_stack_offset -= 2;

	return value;

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			void
 *
 * returns			void
 *
 * synopsis			processed the next stacked operator - utilises the top contents of both operand and operator stacks to perform the necessary
 * 					operations that are required to (typically) process 1 operator and 2 operands. result of the process is that the processed value
 * 					is then pushed on the top of the operand stack, ready for further (later) processing, or in the event that an expression is completed,
 * 					so that the final expresson value is then available simply by popping the top of the operand stack afterwards.
 *
 */
void process_stacked_operator( void )
{

	union symbol_table_entry_value operand1 , operand2;
	struct stacked_operator op;
 	int type2 , type1 , basic_precedence;

 	op = pop_operator();

 	// check for a type mismatch - for now, just ensure int<->int or real<->real
 	type2 = *(operand_stack_ptr+operand_stack_offset-1);
 	switch ( type2 )
 	{
 		case DATATYPE_REAL    : operand2.rval = pop_operand_real();         break;
 		case DATATYPE_INTEGER : operand2.ival = pop_operand_integer();      break;
 		case DATATYPE_BOOLEAN : operand2.ival = pop_operand_boolean();      break;
 		case DATATYPE_STRING  : pop_operand_string( operand2.sval );        break;
 		default               : set_error_state( ERROR_TYPE_MISMATCH ); return; break;
 	}

 	// distinction now made between unary and binary operators...
 	basic_precedence = op.precedence % 10;
 	if ( basic_precedence == 3 )
 	{

 		// unary operation
 		switch ( op.token )
 		{

 			case PUNC_MINUS :
 			{
 				switch ( type2 )
				{
					case DATATYPE_INTEGER : operand2.ival = operand2.ival * -1; push_operand_integer(operand2.ival); break;
					case DATATYPE_REAL    : operand2.rval = operand2.rval * -1; push_operand_real(operand2.rval);    break;
					default               : set_error_state( ERROR_SYNTAX ); return;								 break;
				}
 				break;
 			}

 			case PUNC_PLUS :
 			{
 				switch ( type2 )
				{
					case DATATYPE_INTEGER : operand2.ival = operand2.ival * +1; push_operand_integer(operand2.ival); break;
					case DATATYPE_REAL    : operand2.rval = operand2.rval * +1; push_operand_real(operand2.rval);    break;
					default               : set_error_state( ERROR_SYNTAX ); return;								 break;
				}
 				break;
 			}

 			case KEYWORD_NOT :
 			{
 				switch ( type2 )
 				{
 					case DATATYPE_BOOLEAN : operand2.bval = !operand2.bval; push_operand_boolean(operand2.bval); break;
 					default               : set_error_state( ERROR_TYPE_MISMATCH ); return; break;
 				}
 			}

 		}

 	}
 	else
 	{

		// binary operation
	 	type1 = *(operand_stack_ptr+operand_stack_offset-1);
		switch ( type1 )
 		{
	 		case DATATYPE_REAL    : operand1.rval = pop_operand_real();         break;
 			case DATATYPE_INTEGER : operand1.ival = pop_operand_integer();      break;
 			case DATATYPE_BOOLEAN : operand1.ival = pop_operand_boolean();      break;
 			case DATATYPE_STRING  : pop_operand_string( operand1.sval );        break;
 			default               : set_error_state( ERROR_TYPE_MISMATCH ); return; break;
 		}

	 	// if we have a REAL/INTEGER pairing, perform type promotion...
	 	if ( (type2 == DATATYPE_REAL) & (type1 == DATATYPE_INTEGER) )
	 		{ type1 = DATATYPE_REAL; operand1.rval = (float) operand1.ival; }
	 	if ( (type1 == DATATYPE_REAL) & (type2 == DATATYPE_INTEGER) )
	 		{ type2 = DATATYPE_REAL; operand2.rval = (float) operand2.ival; }

	 	// division operator forces integers to reals (PRINT 3/9!)
	 	if ( op.token == PUNC_DIVIDE )
	 	{
		 	if ( type1 == DATATYPE_INTEGER )
		 		{ type1 = DATATYPE_REAL; operand1.rval = (float) operand1.ival; }
		 	if ( type2 == DATATYPE_INTEGER )
		 		{ type2 = DATATYPE_REAL; operand2.rval = (float) operand2.ival; }
	 	}

	 	// ensure that BOTH operands are of the same type at this point
	 	if ( type1 != type2 )
	 		{ set_error_state( ERROR_TYPE_MISMATCH ); return; }

	 	switch ( type1 )
	 	{

	 		case DATATYPE_INTEGER :
				switch ( op.token )
				{
					case PUNC_PLUS         : push_operand_integer(operand1.ival + operand2.ival); 								break;
					case PUNC_MINUS        : push_operand_integer(operand1.ival - operand2.ival); 								break;
					case PUNC_MULTIPLY     : push_operand_integer(operand1.ival * operand2.ival); 								break;
					case PUNC_DIVIDE       : push_operand_integer(operand1.ival / operand2.ival); 								break;
					case KEYWORD_MOD       : push_operand_integer(operand1.ival % operand2.ival); 								break;
					case KEYWORD_DIV       : push_operand_integer(operand1.ival / operand2.ival); 								break;
					case PUNC_EQUALS       : push_operand_boolean( operand1.ival == operand2.ival );							break;
					case PUNC_LESS_THAN    : push_operand_boolean( operand1.ival <  operand2.ival );							break;
					case PUNC_GREATER_THAN : push_operand_boolean( operand1.ival >  operand2.ival );							break;
					case KEYWORD_AND       : push_operand_integer( operand1.ival & operand2.ival );								break;
					case KEYWORD_OR        : push_operand_integer( operand1.ival | operand2.ival );								break;
					case KEYWORD_EOR       : push_operand_integer( operand1.ival ^ operand2.ival );								break;
					default                : set_error_state( ERROR_TYPE_MISMATCH ); return;
				}
	 			break;

	 		case DATATYPE_REAL :
				switch ( op.token )
				{
					case PUNC_PLUS         : push_operand_real(operand1.rval + operand2.rval); 									break;
					case PUNC_MINUS        : push_operand_real(operand1.rval - operand2.rval); 									break;
					case PUNC_MULTIPLY     : push_operand_real(operand1.rval * operand2.rval); 									break;
					case PUNC_DIVIDE       : push_operand_real(operand1.rval / operand2.rval); 									break;
					case KEYWORD_MOD       : push_operand_integer((int)operand1.rval % (int)operand2.rval); 					break;
					case KEYWORD_DIV       : push_operand_integer(floor((int)operand1.rval / (int)operand2.rval));				break;
					case PUNC_EQUALS       : push_operand_boolean( operand1.rval == operand2.rval );							break;
					case PUNC_LESS_THAN    : push_operand_boolean( operand1.rval <  operand2.rval );							break;
					case PUNC_GREATER_THAN : push_operand_boolean( operand1.rval >  operand2.rval );							break;
					default                : set_error_state( ERROR_SYNTAX ); return;
				}
	 			break;

	 		case DATATYPE_BOOLEAN :
	 			switch ( op.token )
	 			{
		 			case KEYWORD_AND       : push_operand_boolean( operand1.ival && operand2.ival );							break;
		 			case KEYWORD_OR        : push_operand_boolean( operand1.ival || operand2.ival );							break;
		 			case PUNC_EQUALS       : push_operand_boolean( operand1.ival == operand2.ival );							break;
					default                : set_error_state( ERROR_SYNTAX ); return;
	 			}
	 			break;

	 		case DATATYPE_STRING :
	 			switch ( op.token )
	 			{
	 				case PUNC_EQUALS       : push_operand_boolean( strcmp((char *)operand1.sval,(char *)operand2.sval) == 0 );	break;
	 				case PUNC_LESS_THAN    : push_operand_boolean( strcmp((char *)operand1.sval,(char *)operand2.sval) <  0 );	break;
	 				case PUNC_GREATER_THAN : push_operand_boolean( strcmp((char *)operand1.sval,(char *)operand2.sval) >  0 );	break;
	 				default                : set_error_state( ERROR_SYNTAX ); return;
	 			}
	 			break;

	 	}

 	}

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			void
 *
 * returns			void
 *
 * synopsis			clients should call this method before beginning any work with expressions - it resets the state of both operand and operator stacks
 *
 */
void _initialise_expression_stacks( void )
{

	// prep the expression stacks (operand and operator - this should only need be done once)
	// begin by identifying the two stacks we need to operate with (operand and operator)
	operand_stack_ptr     = heap_top          + 1;			// operand stack sits above "top" (NB: if FN/PROC in expr, this could cause corruption!)
	operator_stack_ptr    = operand_stack_ptr + 1*1024;		// both stacks are given 1Kb to play with in theory.
	operand_stack_offset  = 0;								// the stack offsets identify the NEXT FREE LOCATION
	operator_stack_offset = 0;

	// agghh! see above comments! push the operand/operator stacks up a bit for the mo so PROC/FN doesn't instantly corrupt them!
	operand_stack_ptr  += 1024;
	operator_stack_ptr += 1024;

}
