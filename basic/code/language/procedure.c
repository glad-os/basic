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
#include "basic/header/core/lex.h"
#include "basic/header/core/stack.h"	// runtime_ptr required
#include "basic/header/core/symbols.h"
#include "basic/header/directive/run.h"
#include "basic/header/language/define.h"	// for the optional parameters handling code
#include "basic/header/language/expression.h"
#include "basic/header/language/print.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void
 * 
 * synopsis			PROCEDURE := PROCEDURE <id> [ ( <parameters> ) ]
 * 
 */
void _procedure( void )
{

	int st_position;
	unsigned char st_type;
	union symbol_table_entry_value st_value;
	unsigned char *nextline_ptr;
	unsigned char *local_symbol_table;
	
	if ( !expect_token( ID ) )
		{ set_error_state( ERROR_SYNTAX ); return; }
	
	// first step is to stack the current "extent" of the symbol table
	pushInteger( (unsigned long int) symbol_table_base );
	pushInteger( (unsigned long int) heap_top          );
	local_symbol_table  = heap_top; // remember where we're going to start from
	*local_symbol_table = 0x00;		// and initialise it before we do anything with it
	
	// ok - locate the PROCEDURE's identifier in the symbol table
	st_position = position_in_symbol_table_procfn( expression_result.str );
	st_type     = symbol_table_entry_type( st_position );
	if ( st_type != DATATYPE_PROCEDURE )
		{ set_error_state( ERROR_UNKNOWN_PROCEDURE ); return; }
	
	st_value = symbol_table_entry_value( st_position );

	// now we have to (optionally) expect a parameter list. set parameter_count automatically as if no "(" is found, we don't need
	// to call _expect_parameters (which in itself resets the known parameter count)
	parameter_count = 0;
	if ( expect_token( PUNC_OPEN_BRACKET) )
	{
		_expect_parameters( ACTUAL , st_position );
		if ( !expect_token( PUNC_CLOSE_BRACKET ) )
			{ set_error_state( ERROR_EXPECTED_CLOSE_BRACKET ); return; }
	}
    
	// if we didn't match the number of parameters, complain
	if ( number_of_parameters( st_position ) != parameter_count )
	{ set_error_state( ERROR_BAD_PARAMETERS ); return; }

	// 8 < - - - - - - - - - 8 < - - - - - - - - - 8 < - - - - - - - - - 8 < - - - - - - - - - 8 < - - - - - - - - - 8 < - - - 
	// TODO - this is starting to get messy - we really need to re-factor this stuff
	// 8 < - - - - - - - - - 8 < - - - - - - - - - 8 < - - - - - - - - - 8 < - - - - - - - - - 8 < - - - - - - - - - 8 < - - - 

	// ok - now let's invoke our new local symbol table
	symbol_table_base  = local_symbol_table;
	
	// figure out the NEXT line (as this is where control RETURNS to) and stack it for END PROCEDURE to reference later
	nextline_ptr = runtime_ptr + *(runtime_ptr+4);
	pushByte( ((unsigned long int) nextline_ptr) >>  0 );
	pushByte( ((unsigned long int) nextline_ptr) >>  8 );
	pushByte( ((unsigned long int) nextline_ptr) >> 16 );
	pushByte( ((unsigned long int) nextline_ptr) >> 24 );
	pushByte( DATATYPE_PROCEDURE );

	// alter control flow
	runtime_ptr        = (unsigned char *)st_value.pval;	// this is a pointer to the declared block of code
	runtime_linelength = 0;									// on return, _directive_run will use this to "push" us onto the next line, so cancel that effect 
			
	// 8 < - - - - - - - - - 8 < - - - - - - - - - 8 < - - - - - - - - - 8 < - - - - - - - - - 8 < - - - - - - - - - 8 < - - - 
	
}
