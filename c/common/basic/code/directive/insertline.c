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
#include "../library/include/stdlib.h"
#include "../library/include/string.h
*/
#include "stdlib.h"
#include "string.h"

#include "basic/header/directive/insertline.h"
#include "basic/header/core/basic.h"
#include "basic/header/core/lex.h"
#include "basic/header/core/memory.h"
#include "basic/header/core/stack.h"
#include "basic/header/core/symbols.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			line_number	integer value indicating the line number that is to be stored
 *
 * returns			void
 * 
 * synopsis			if the interpreter receives an instruction that begins with a number, this indicates that the user wishes to store 
 * 					the line provided in the stored program memory. this function provides the functionality to intelligently tokenise and 
 * 					store the line of source.
 * 
 */
void _directive_insertline( unsigned char *line_number )
{
	
	struct        lex_result result;
	int  		  line_num;
	int           ignore_whitespace;
	int           index;
	int           line_length;
	unsigned char token_value;
	extern unsigned char *top;
	unsigned char * ptr;
	unsigned int    next_line_num;
	int             next_line_len;
	int             found;
	int             loop;
	long int        bytes_to_shift;
	int             line_size;

	// get line number; check both ranges to ensure it's a sensible line number
	// get line number; check both ranges to ensure it's a sensible line number
	line_num = atoi((char *)line_number);
	if ( ( line_num < 0 ) || ( line_num > MAX_LINE_NUMBER ) )
	{
		set_error_state( ERROR_BAD_LINE_NUMBER );
		return;
	}
	
	/**
	 * now we need to call upon lex to provide us the tokens for the line being created - ALLOW whitespace under this circumstance
	 */
	ignore_whitespace = FALSE;
	line_length       = 0;
	while ( next_token(&result,ignore_whitespace) )
	{
		
		// we store the token that lex returns us each time. however - if its
		// an ID/NUM_LIT/STR_LIT - we store the untokenised version of it
		switch ( result.token )
		{
			case ID       :
			case NUM_LIT  :
			case REAL_LIT :
			case STR_LIT  :
			case 0x00     :
				// 0x00 also has to be considered, as if next_token() returns a token value of 0x00 this indicates that it failed
				// to find a token, and so returns instead the token 0x00 and the associated string is simply the next char from the feed
				for ( index = 0 ; index < strlen((char *)result.str) ; index++ )
				{
					token_value = result.str[index];
					pushByte( token_value );
					line_length++;
				}
				break;
			default :
				token_value = result.token;
				pushByte( token_value );
				line_length++;
				break;
		}
		
	}
	
	// to ensure smooth creation of requested line, enforce deletion of any existing line with the same number
	delete_line( line_num );
	
	// at this point, the stack has had the tokenised line pushed to it and we know how long it is. so now we are required
	// to identify at which point in program storage this requested line should go. notice that we only bother going through the
	// remainder of this process if the line_length is more than 0 (ie. the user specified something other than a blank line!)
	if ( line_length )
	{
		
		ptr   = get_mem_base();
		found = 0;
		while ( !found )
		{
			// get current line number and compare to requested line number
			next_line_num = (*(ptr+0)<<0) + (*(ptr+1)<<8) + (*(ptr+2)<<16) + (*(ptr+3)<<24);
			next_line_len = *(ptr+4);
			// either we find the position to put this line, or we have to continue looking...
			if ( ((next_line_num == 0xFFFFFFFF) && (next_line_len == 5)) || (next_line_num > line_num) )
				found = 1;
			else
				ptr += next_line_len;
		}
		
		// calculate the memory movement requirement at this stage (use "top" not "mem_top"!)
		bytes_to_shift = (long int) top - (long int) ptr + 1;
		line_size      = line_length + 5; // remember to add on the line storage overhead (5 bytes)
		memmove( ptr + line_size , ptr , bytes_to_shift );
		
		// now insert the line (backwards due to stack method)
		*(ptr+0) = ((line_num >>  0) & 0xFF); // header - line number and (total) line length 
		*(ptr+1) = ((line_num >>  8) & 0xFF);
		*(ptr+2) = ((line_num >> 16) & 0xFF);
		*(ptr+3) = ((line_num >> 24) & 0xFF);
		*(ptr+4) = line_size;
		ptr += line_size-1;
		for ( loop = line_length ; loop > 0 ; loop-- )
			*ptr-- = popByte();

		// finally, now the line has been inserted, increase the value of "top" by the (total) line length
		top += line_size;

	}

	// regardless of the above, whenever a new line is inserted we empty the symbol table to remain wholesome, happy people
	initialise_symbol_table();
	
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			line_no		integer indicating the line number in stored program memory that requires deletion (if it exists)
 *
 * returns			void
 * 
 * synopsis			this function can be called to intelligently delete the given line of source from stored program memory.
 * 
 */
void delete_line( unsigned int line_no )
{

	unsigned char * ptr;
	unsigned int    next_line_num;
	int             next_line_len;
	int             keep_searching;
	long int        bytes_to_shift;
		
	ptr            = get_mem_base();
	keep_searching = 1;
	while ( keep_searching )
	{
		// get current line number and compare to requested line number
		next_line_num = (*(ptr+0)<<0) + (*(ptr+1)<<8) + (*(ptr+2)<<16) + (*(ptr+3)<<24);
		next_line_len = *(ptr+4);
		if ( (next_line_num == line_no) || ((next_line_num == 0xFFFFFFFF) && (next_line_len == 5)) )
			keep_searching = 0;
		else
			ptr += next_line_len;
	}

	// having searched for the line, only shuffle program source around if we came across the line	
	if ( next_line_num == line_no )
	{
		// calculate memory movement required, move the memory "down", and adjust "top" suitably
		bytes_to_shift = (long int) top - (long int) ptr + next_line_len + 1;
		memmove( ptr , ptr + next_line_len , bytes_to_shift ); // dest , src , size
		top -= next_line_len;
		
	}
		
}
