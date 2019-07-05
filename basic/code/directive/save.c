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



#include "basic/header/directive/save.h"
#include "basic/header/core/basic.h"
#include "basic/header/core/lex.h"
#include "basic/header/core/memory.h"
#include "basic/header/language/expression.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void
 * 
 * synopsis			SAVE := SAVE <filename>
 * 					filename is a string
 * 					saves the program currently in stored program memory to the file specified. insofar as the user is aware, filenames do not have a
 * 					.bas file extension, the load and save directives automatically append these (so as its easier in the host environment
 * 					to differentiate BASIC programs from any others).
 * 				
 */
void _directive_save( void )
{
	
	/*
	unsigned char *ptr , *string;
	int index;
	FILE *fp;

	if ( running )
		{ set_error_state( ERROR_SILLY ); return; }

	next_token( &expression_result , TRUE );
	switch ( expression_result.token )
	{
		case STR_LIT : 
		{

			// remove quotation marks from the string literal (start at index 1, and nullify the final char)
			string = expression_result.str;
			index  = strlen((char *)string)-1;
			*( string + index++ ) = '.';
			*( string + index++ ) = 'b';
			*( string + index++ ) = 'a';
			*( string + index++ ) = 's';
			*( string + index++ ) = 0x00;	

			// save...
			fp = fopen( (char *)string+1, "w");
			if (fp == NULL)
				{ set_error_state( ERROR_BAD_FILE ); return; }
			ptr = mem_base;
			while ( ptr <= top )
				fprintf( fp , "%c" , *ptr++ );
			fclose(fp);
			break;
		}
		default :
		{
			set_error_state( ERROR_SYNTAX );
			break;
		}
	}
	*/
	
}
