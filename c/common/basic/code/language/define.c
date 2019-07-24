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

#include "basic/header/language/define.h"
#include "basic/header/core/basic.h"
#include "basic/header/core/lex.h"
#include "basic/header/core/symbols.h"
#include "basic/header/language/expression.h"	// for expect_token()



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			record_in_symbol_table			flag determining whether or not definition should result in a symbol table entry being recorded
 * 					ptr								pointer to the character array declaring the name of the PROCEDURE or FUNCTION encountered
 *
 * returns			void
 *
 * synopsis			define ::= <DEFINE> [<PROCEDURE>|<FUNCTION>] <id>
 *
 *					the _define implementation can be called in one of two contexts. if the stored program is running and a DEFINE is encountered, this
 *					indicates an unacceptable execution state (DEFINEs should be "heard but never seen"!)
 *
 * 					alternatively, _define can be called BEFORE the stored program is actually run, to request that the DEFINE statement be interpreted
 * 					with the aim of the definition being stored in the symbol table, so that - on subsequent execution - the stored program can successfully
 * 					invoke procedures and functions.
 *
 * 					the record_in_symbol_table flag passed to this method declares which of the two contexts is in use at the time.
 *
 * 					if a stored program is being executed and a DEFINE has been encountered - unacceptable runtime behaviour
 * 					otherwise it entails the definition of a PROCEDURE of FUNCTION follows (used on pre-run code to gather definitions)
 *
 */
void _define( int record_in_symbol_table , unsigned char *ptr )
{

	if ( !record_in_symbol_table )
		set_error_state( ERROR_UNEXPECTED_DEFINE );
	else
	{
		if      ( expect_token( KEYWORD_PROCEDURE ) )
			_extract_definition( KEYWORD_PROCEDURE , ptr );
		else if ( expect_token( KEYWORD_FUNCTION  ) )
		{
			_extract_definition( KEYWORD_FUNCTION  , ptr );
		}
		else
			set_error_state( ERROR_SYNTAX );
	}

	return;

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			type			?
 * 					st_index		?
 *
 * returns			void
 *
 * synopsis			there is a slightly better way of expressing this...
 *
 * 					parameters      := <var> <separator>
 *					var             := <id> | <e>
 * 					separator       := <,> <more_parameters> | <e>
 * 					more_parameters := <id> <more>
 *
 * 					if ACTUAL parameters, an <id> is altered for a value type (e.g. int/string)
 *
 */
void _expect_parameters( int type , int st_index )
{

	parameter_count = 0;
	_expect_var( type , st_index );
	// don't forget - if you expect a variable, it had better exist (ie. no error state exists before we try to continue!)
	if ( !error_state)
		_expect_separator( type , st_index );

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			type			?
 * 					st_index		?
 *
 * returns			void
 *
 * synopsis			?
 *
 */
void _expect_var( int type , int st_index )
{

	struct lex_result result;
	struct lex_state  state;
	union symbol_table_entry_value st_entry;
	unsigned char datatype;

	switch ( type )
	{

		case FORMAL :

			preserve_lex_state(&state);
			if ( next_token( &result , TRUE ) )
			{
				if ( result.token == ID )
					_process_formal_parameter( result.str );
				else
				{
					reinstate_lex_state(&state);
					set_error_state( ERROR_SYNTAX );
					return;
				}
			}
			else
				reinstate_lex_state(&state);
			break;

		case ACTUAL :

			// expecting an expression may result in an error (e.g. if the identifier doesn't exist, for example)
			expect_expression();
			if ( !error_state )
			{
				datatype = *(operand_stack_ptr + operand_stack_offset-1);
				switch (datatype)
				{
					case DATATYPE_REAL :
						{ st_entry.rval = pop_operand_real();    break; }
					case DATATYPE_INTEGER :
						{
						st_entry.ival = pop_operand_integer();
						break;
						}
					case DATATYPE_STRING :
						{ pop_operand_string(st_entry.sval);     break; }
					case DATATYPE_BOOLEAN :
						{ st_entry.bval = pop_operand_boolean(); break; }
					default :
					{ set_error_state( ERROR_ILLEGAL_DATATYPE ); break; }
				}

				_process_actual_parameter( datatype , st_entry , st_index );
			}
			break;

	}

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			type			?
 * 					st_index		?
 *
 * returns			void
 *
 * synopsis			?
 *
 */
void _expect_separator( int type , int st_index )
{

	if ( expect_token(PUNC_COMMA) )
		_expect_more_parameters(type,st_index);

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			type			?
 * 					st_index		?
 *
 * returns			void
 *
 * synopsis			?
 *
 */
void _expect_more_parameters( int type , int st_index )
{

	struct lex_result result;
	struct lex_state  state;
	union symbol_table_entry_value st_entry;
	unsigned char datatype;

	switch ( type )
	{

		case FORMAL :

			preserve_lex_state(&state);
			if ( next_token( &result , TRUE ) )
			{
				if ( result.token == ID )
				{
					_process_formal_parameter( result.str );
					_expect_separator(type,st_index);
				}
				else
				{
					reinstate_lex_state(&state);
					set_error_state( ERROR_SYNTAX );
					return;
				}
			}
			else
				reinstate_lex_state(&state);
			break;

		case ACTUAL :

			expect_expression();
			// ok - we've got what we need - top of operand stack is the final value
			// but again, don't forget that an expression may cause an error state (e.g. missing identifier!)
			if ( !error_state)
			{
				datatype = *(operand_stack_ptr + operand_stack_offset-1);
				switch (datatype)
				{
					case DATATYPE_REAL :
						{ st_entry.rval = pop_operand_real();    break; }
					case DATATYPE_INTEGER :
						{ st_entry.ival = pop_operand_integer(); break; }
					case DATATYPE_STRING :
						{ pop_operand_string(st_entry.sval);     break; }
					case DATATYPE_BOOLEAN :
						{ st_entry.bval = pop_operand_boolean(); break; }
				}

				_process_actual_parameter( datatype , st_entry , st_index );
				_expect_separator(type,st_index);
			}
			break;

	}

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			id			?
 *
 * returns			void
 *
 * synopsis			?
 *
 */
void _process_formal_parameter( unsigned char *id )
{

	int i;

	// append the formal parameter to the current signature
	i  = strlen( (char *)signature );
	strncpy( (char *)signature+i , (char *)id , strlen((char *)id) );
	i += strlen( (char *)id        );
	signature[i++] = ':';
	signature[i]   = 0x00;

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			datatype			?
 * 					actual_value		?
 * 					st_index			?
 *
 * returns			void
 *
 * synopsis			?
 *
 */
void _process_actual_parameter( int datatype , union symbol_table_entry_value actual_value , int st_index )
{

	char formal_parameter_name[MAX_INPUT_LENGTH];

	parameter_count++;

	// if the nth formal parameter name exists, create the (one day!) "local" identifier with the associated value
	// if not, ignore the assignment as further up the chain a final check is made against actual-to-formal parameter counts
	locate_nth_formal_parameter( st_index , parameter_count , formal_parameter_name ); // index into stable, n = 0... , ptr to resultant name

 	// with an actual parameter we need to force the base to be the top of the stack, otherwise the auto-delete
 	// of existing entries will delete any existing identifier of the same name

	if ( strlen(formal_parameter_name) > 0 )
	{
		append_to_symbol_table( (unsigned char *)formal_parameter_name , datatype , actual_value );
	}

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			definition_type			?
 * 					ptr						?
 *
 * returns			void
 *
 * synopsis			?
 *
 */
/*
 *
 * definition_type is either KEYWORD_PROCEDURE or KEYWORD_FUNCTION
 *
 */
void _extract_definition( int definition_type , unsigned char *ptr )
{

	struct lex_result result;
	int index;
	union symbol_table_entry_value st_entry;

	next_token( &result , TRUE );
	if ( result.token != ID )
		{ set_error_state( ERROR_SYNTAX ); return; }

	// safety check - if it's a FUNCTION being defined and it (fully) clashes with an internal function name, then complain
	if ( definition_type == KEYWORD_FUNCTION )
	{
		// copy the string into signature, surrounded by []\0
		index = strlen( (char *)result.str );
		strcpy( (char *)signature+1 , (char *)result.str );
		*signature = '[';
		signature[index+1] = ']';
		signature[index+2] = 0x00;
		if ( strstr("[abs][asc][chr][concat][cos][get][inkey][instr][int][left][len][mid][right][rnd][sgn][sin][sqr][str][tan][time][val]",(char *)signature) )
			{ set_error_state( ERROR_ILLEGAL_FUNCTION_NAME ); return; }
	}

	// copy the PROC/FN across to the "signature" that we'll construct. append ":" at end of it (separates elements in signature)
	strcpy( (char *)signature , (char *)result.str );
	index = strlen( (char *)signature );
	signature[index++] = ':';
	signature[index  ] = 0x00;

	// and now the NEW part I'm working on - processing the formal parameter list (not sure how to actually do this YET...)
	if ( expect_token( PUNC_OPEN_BRACKET ) )
	{
		if ( !expect_token( PUNC_CLOSE_BRACKET ) )
		{
			_expect_parameters( FORMAL , 0 );
			if ( !expect_token( PUNC_CLOSE_BRACKET ) )
				{ set_error_state( ERROR_EXPECTED_CLOSE_BRACKET ); return; }
		}
	}
	else
	{
		if ( definition_type == KEYWORD_FUNCTION )
			{ set_error_state( ERROR_SYNTAX ); return ; }
	}

	// all being well, create an entry in the symbol table for the PROCEDURE/FUNCTION name (and where it resides in memory)
	st_entry.pval = (unsigned long int) ptr;
	switch ( definition_type )
	{
		case KEYWORD_PROCEDURE :
			add_to_symbol_table( signature , DATATYPE_PROCEDURE , st_entry );
			break;
		case KEYWORD_FUNCTION  :
			add_to_symbol_table( signature , DATATYPE_FUNCTION  , st_entry );
			break;
		default :
			set_error_state( ERROR_ILLEGAL_DATATYPE );
			break;
	}


}
