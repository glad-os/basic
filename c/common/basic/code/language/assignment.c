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



#include "basic/header/language/assignment.h"
#include "basic/header/core/basic.h"
#include "basic/header/core/lex.h"
#include "basic/header/core/symbols.h"
#include "basic/header/language/expression.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			id			pointer to the identifier's name
 *
 * returns			void
 *
 * synopsis			assignment := <expr>
 *					an assignment starts by being handed the identifier that was discovered which constitutes the beginning of the assignment statement.
 *					so the job here is to accept the id we're given, and exact the rest of the assignment as appropriate.
 *
 */
void _assignment( unsigned char *id )
{

	union symbol_table_entry_value st_entry;
	unsigned char                 datatype;

	if ( !expect_token(PUNC_EQUALS) ) { set_error_state( ERROR_EXPECTED_EQUALS ); return; }
	if ( !expect_expression()       ) { set_error_state( ERROR_SYNTAX );          return; }

	datatype = *(operand_stack_ptr + operand_stack_offset-1);
	switch (datatype)
	{
		case DATATYPE_REAL    : { st_entry.rval = pop_operand_real();      break; }
		case DATATYPE_INTEGER : { st_entry.ival = pop_operand_integer();   break; }
		case DATATYPE_STRING  : { pop_operand_string(st_entry.sval);       break; }
		case DATATYPE_BOOLEAN : { st_entry.bval = pop_operand_boolean();   break; }
		default               : { set_error_state( ERROR_BAD_EXPRESSION ); break; }
	}
	add_to_symbol_table( id , datatype , st_entry );

}
