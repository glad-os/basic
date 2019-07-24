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
#include <stdint.h>
#include "string.h"
#include "stdio.h"

#include "basic/header/core/symbols.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			void
 *
 * returns			void
 * 
 * synopsis			this function should be used before running a program, or during a general "reset" process, to ensure that
 * 					the symbol table is initialised and marked as "empty"; the symbol table sits at the start of the "heap" which
 * 					itself always begins immediately after the stored program. hence the heap and symbol table variables are
 * 					set in tandem.
 * 
 */
void initialise_symbol_table( void )
{

	unsigned int alter;
	alter = 16 - ( ((unsigned int) (uintptr_t) top ) % 16);

	// initialise the heap (base & top) pointers to be just above any stored program (align to 16 byte boundary)
	heap_base         = top + alter;
	heap_top          = top + alter;
	
	// now initialise the symbol table (base) pointer to be at the start of the heap (align to 16 byte boundary)
	// and then mark the symbol table as empty
	symbol_table_base  = top + alter;
	*symbol_table_base = 0x00;
	
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			st_index			the index in the (global) symbol table area where the PROCEDURE or FUNCTION is to be found
 *
 * returns			int					the number of formal parameters associated with the PROCEDURE or FUNCTION in question
 * 
 * synopsis			examines the definition of the PROCEDURE or FUNCTION as held in the (global) symbol table area, and
 * 					returns the number of formal parameters associated with it. From the definition process, a PROCEDURE or FUNCTION
 * 					has the formal parameter list attached to its identifier separated by ":" symbols, so DEFINE FUNCTION a(b,c) is
 * 					preserved in the symbol table with an identifier of "a:b:c:"
 * 
 */
int number_of_parameters( int st_index )
{
	
	int index , param_count;
	char identifier[MAX_INPUT_LENGTH];
	
	// pull out the identifier from the symbol table as specified by the client
	symbol_table_entry_identifier_procfn( (unsigned char *)identifier , st_index );

	// to count how many formal parameters, count the number of separator symbols ":" in the string (less 1)
	index       = 0;
	param_count = 0;
	while ( *(identifier+index) )
	{
		if ( *(identifier+index) == ':' )
			param_count++;
		index++;
	}
	
	return param_count - 1;
	
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			st_index			index into the (global) symbol table where the PROCEDURE or FUNCTION is to be found
 * 					n					identifiers the nth formal parameter
 * 					name				pointer to the string where the formal name will be stored
 *
 * returns			void
 * 
 * synopsis			given the index into the (global) symbol table area, and "n" identifying which formal parameter to locate,
 * 					this function will extract the name of the nth formal parameter for the PROCEDURE or FUNCTION in question
 * 					and copy it into the string pointed to by "name".
 * 
 */
void locate_nth_formal_parameter( int st_index , int n , char *name )
{

	int index , element_num , keep_looking;
	char identifier[MAX_INPUT_LENGTH];

	// pull out the identifier from the symbol table as specified by the client
	symbol_table_entry_identifier_procfn( (unsigned char *)identifier , st_index );

	// now we have to determine what the nth parameter is (if it exists!) against the name of this identifier
	// bear in mind that for PROC/FN the identifier follows the form <id>:<param-1>:<param-2>:...:<param-n>:
	index = 0;
	
	// find the next ":" separator, then move on to the 1st char of the next element
	while ( *(identifier+index) != ':' )
		index++;
	index++;
	
	element_num  = 1;
	keep_looking = 1;
	while ( (element_num<n) && keep_looking )
	{
		
		if ( *(identifier+index) != 0x00 )
		{
			// move to next
			while ( *(identifier+index) != ':' )
				index++;
			index++;
			element_num++;
		}
		else
			keep_looking = 0;
		
	}
	
	// copy (move) the element that we're at (which could be just a null) to return to the client
	strcpy( name , identifier+index );

	// ensure that a terminator is put in as we have to chop at the first available nul or :
	index = 0;
	while ( (*(name+index) != 0x00) && (*(name+index) != ':') )
		index++;
	*(name+index) = 0x00;

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			name			pointer to a string declaring the identifier to be used for the symbol table entry
 * 					type			declares the data type of the identifier (integer/real/string...)
 * 					value			symbol_table_entry_value struct containing the relevant data value associated with the identifier
 *
 * returns			void
 * 
 * synopsis			this function will add the supplied symbol table entry into the symbol table, explicitly destroying any
 * 					EXISTING entry by the same name beforehand. the operation occurs in the context of the current symbol table
 * 					"frame".
 * 
 */
void add_to_symbol_table( unsigned char *name , unsigned char type , union symbol_table_entry_value value )
{

	delete_from_symbol_table( name );
	_add_to_symbol_table( name , type , value );

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			name			pointer to a string declaring the identifier to be used for the symbol table entry
 * 					type			declares the data type of the identifier (integer/real/string...)
 * 					value			symbol_table_entry_value struct containing the relevant data value associated with the identifier
 *
 * returns			void
 * 
 * synopsis			this function will append the supplied symbol table entry into the current symbol table frame. it is useful 
 *					when processing PROCEDURE or FUNCTION actual parameters as - when instantiating an actual parameter "x" - we
 *					don't want to remove any existing identifier "x" as (at that point in time) we remain in the *current* symbol 
 * 					table "frame" [we need to, or we won't be able to locate the existing identifiers to construct the formal parameters with!]
 * 
 */
void append_to_symbol_table( unsigned char *name , unsigned char type , union symbol_table_entry_value value )
{

	_add_to_symbol_table( name , type , value );

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			name			pointer to a string declaring the identifier to be used for the symbol table entry
 * 					type			declares the data type of the identifier (integer/real/string...)
 * 					value			symbol_table_entry_value struct containing the relevant data value associated with the identifier
 *
 * returns			void
 * 
 * synopsis			this is the underlying (common) functionality utilised by both functions above (add/append to symbol table).
 * 					it performs the actual task of adding the supplied identifier/type/value information onto the end of the 
 * 					heap (thus extending the size of the symbol table overall).
 * 
 */
void _add_to_symbol_table( unsigned char *name , unsigned char type , union symbol_table_entry_value value )
{
	
	unsigned char *ptr;
	unsigned int record_length;

	// ACU
	// AArch64 will want these symbol entries much more aligned
	// changes being made here:
	// 1) the "header" will be allocated 4 bytes (padded) rather than 2
	// 2) calculate a PADDED length of the "name" (identifier) so that all names are padded to a 4-byte boundary
	//    and store this at offset 0x2 in the header (for convenience)
	unsigned int padded_name_length, padded_value_length;
	padded_name_length = strlen((char *)name)+1;
	padded_name_length = padded_name_length + (4 - (padded_name_length % 4) );

	ptr = heap_top;
	//sprintf( "_add_to_symbol_table : ptr = 0x" ); sprintf_i( ptr, 10 ); sprintf( "\n" );
	switch ( type )
	{
		
		case DATATYPE_REAL    :

			record_length = 4 + padded_name_length + 4;
			*(ptr+0) = record_length;
			*(ptr+1) = DATATYPE_REAL;
			*(ptr+2) = padded_name_length;
			strcpy( (char *)ptr+4 , (char *)name );
			* (float *)(ptr+4+padded_name_length+0) = value.rval;
			break;
		
		case DATATYPE_INTEGER :
			
			record_length = 4 + padded_name_length + 4;
			*(ptr+0) = record_length;
			*(ptr+1) = DATATYPE_INTEGER;
			*(ptr+2) = padded_name_length;
			strcpy( (char *)ptr+4 , (char *)name );
			* (int *)(ptr+4+padded_name_length+0) = value.ival;

			break;
			
		case DATATYPE_STRING :
		
			// also pad the length of the String's value
			padded_value_length = strlen((char *)value.sval)+1;
			padded_value_length = padded_value_length + 4 - (padded_value_length % 4);

			record_length = 4 + padded_name_length + padded_value_length;
			*(ptr+0) = record_length;
			*(ptr+1) = DATATYPE_STRING;
			*(ptr+2) = padded_name_length;
			strcpy( (char *)ptr+4 , (char *)name );
			strcpy( (char *)ptr+4+padded_name_length , (char *)value.sval );
			break;

		case DATATYPE_PROCEDURE :
		
			record_length = 4 + padded_name_length + 4;
			*(ptr+0) = record_length;
			*(ptr+1) = DATATYPE_PROCEDURE;
			*(ptr+2) = padded_name_length;
			strcpy( (char *)ptr+4 , (char *)name );
			*(ptr+4+padded_name_length+0) = (unsigned char) (value.pval >>  0);		
			*(ptr+4+padded_name_length+1) = (unsigned char) (value.pval >>  8);
			*(ptr+4+padded_name_length+2) = (unsigned char) (value.pval >> 16);
			*(ptr+4+padded_name_length+3) = (unsigned char) (value.pval >> 24);
			break;

		case DATATYPE_FUNCTION :
		
			record_length = 4 + padded_name_length + 4;
			*(ptr+0) = record_length;
			*(ptr+1) = DATATYPE_FUNCTION;
			*(ptr+2) = padded_name_length;
			strcpy( (char *)ptr+4 , (char *)name );
			*(ptr+4+padded_name_length+0) = (unsigned char) (value.pval >>  0);		
			*(ptr+4+padded_name_length+1) = (unsigned char) (value.pval >>  8);
			*(ptr+4+padded_name_length+2) = (unsigned char) (value.pval >> 16);
			*(ptr+4+padded_name_length+3) = (unsigned char) (value.pval >> 24);
			break;
			
		case DATATYPE_BOOLEAN :
			
			record_length = 4 + padded_name_length + 4;
			*(ptr+0) = record_length;
			*(ptr+1) = DATATYPE_BOOLEAN;
			*(ptr+2) = padded_name_length;
			strcpy( (char *)ptr+4 , (char *)name );
			*(ptr+4+padded_name_length+0) = value.bval;		
			break;

		default :
		
			set_error_state( FATAL_UNKNOWN_DATATYPE );
			record_length = 0;
			break;
			
	}

	// adjust heap_top
	heap_top += record_length;
	*heap_top = 0x00;

	//sprintf( "_add_to_symbol_table : record_length = 0x" ); sprintf_i( record_length, 10 ); sprintf( "\n" );
	//sprintf( "_add_to_symbol_table : heap_top = 0x" );      sprintf_i( heap_top, 10 );      sprintf( "\n\n" );
	
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			identifier			pointer to the string declaring the identifier to be searched for
 * 					base				pointer to the symbol table base that searching must start from
 *
 * returns			int					index into the given symbol table area where the identifier was found
 * 										(if no identifier was found, 0x00 is returned to indicate a failure)
 * 
 * synopsis			given an identifier and a pointer to the base of where to start looking from (in terms of symbol table frames)
 * 					this function will search for the given identifier (intelligently taking into account PROCEDURE and FUNCTION
 * 					signatures - see inline commentary). note that due to the function returning "0x00" to indicate failure, the
 * 					index value of any successfully located identifier will always start from "1".
 * 
 */
int _position_in_symbol_table( unsigned char *identifier , unsigned char *base )

{

	unsigned char *ptr;
	unsigned char record_length;	// total bytes consumed by this record (1+1+n+v)
	int           index;
	unsigned char c;
	int position;
	int keep_looking;
	
	/*
	 * note that with PROC/FN having a "signature" (and not just the id) - e.g. "factor(num1,num2) -> factor:num1:num2:"
	 * we have to anticipate a ":" being present in the name of the identifier (which should equate to a nul at that point)
	 * 
	 * to do this, use strncmp rather than strcmp so the source identifier can stop as soon as it matches, but then ensure you 
	 * check the FOLLOWING character - it's got to be a "terminator" of either NUL or ':'. If you don't perform this fundamental check
	 * you run the risk of incorrectly matching stuff. For example, you have "ab" and "abc" symbols present and someone looks for "a".
	 * If you just check to see if "a" is present, either one of the symbols present would incorrectly match against the "a"!
	 * 
	 */
	
	ptr          = base;
	index        = 1;
	keep_looking = 1;
	position     = 0; // default to say we didn't find it and try to prove otherwise
	while ( keep_looking )
	{
		record_length = *ptr;
		if ( record_length == 0x00 )
			keep_looking = 0;
		else
		{
			if ( strncmp( (const char *)identifier , (const char *)ptr+4 , strlen((char *)identifier) ) == 0x00 )
			{
				// ok - identifier found - but is its definition followed by a terminator? (":" or a NUL)
				c = *( ptr + 4 + strlen((char *)identifier) );
				if ( c == ':' || !c )
				{
					position     = index;
					keep_looking = 0;
				}
			}
		}
		if ( keep_looking )
		{
			ptr  += record_length;
			index++;
		}
	}		
	
	return position;
		
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			identifier			pointer to the identifier to be searched for
 *
 * returns			int					index in symbol table where identifier is to be found (0x00 indicates failure)
 * 
 * synopsis			this function allows for the searching of the (global) symbol table specifically for a PROCEDURE or FUNCTION
 * 
 */
int position_in_symbol_table_procfn( unsigned char *identifier )
{
 	
	return _position_in_symbol_table( identifier , heap_base );
 	
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			identifier			pointer to the identifier to be searched for
 *
 * returns			int					index in symbol table where identifier is to be found (0x00 indicates failure)
 * 
 * synopsis			this function allows for the searching of the current symbol table for a given identifier
 * 
 */
int position_in_symbol_table( unsigned char *identifier )
{
 	
	return _position_in_symbol_table( identifier , symbol_table_base );
 	
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			position			index into the given symbol table where the entry is to be found
 * 					base				pointer to the base of the symbol table where searching is to take place
 *
 * returns			unsigned char		indicator as to the datatype of the entry in question
 * 
 * synopsis			given a pointer to the base of the required symbol table, and the index (1..n) of the symbol table entry
 * 					required, this function will return the datatype of the element held in that position of the symbol table. 
 * 
 */
unsigned char _symbol_table_entry_type( int position , unsigned char *base )
{
	
	unsigned char *ptr;
	unsigned char record_length;	// total bytes consumed by this record (1+1+n+v)
	int           index;
	
	ptr   = base;
	index = 1;
	while ( TRUE )
	{
		
		record_length = *ptr;
		
		// if we've reached the end (ie. client gave an incorrect 'position' value) - return default value of 0
		if ( !record_length )
			return 0;
		if ( index == position )
			return *(ptr+1);
			
		ptr  += record_length;
		index++;
		
	}			
	
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			position			index into the (current) symbol table where the entry is to be found
 *
 * returns			unsigned char		indicator as to the datatype of the entry in question
 * 
 * synopsis			given the index (1..n) of the (current) symbol table entry, this function will return the 
 * 					datatype of the element held in that position of the symbol table. utilises the underlying (shared)
 * 					function _symbol_table_entry_type. 
 * 
 */
unsigned char symbol_table_entry_type( int position )
{
	
	return _symbol_table_entry_type( position , symbol_table_base );
	
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			position			index into the (global) symbol table where the PROCEDURE or FUNCTION entry is to be found
 *
 * returns			unsigned char		indicator as to the datatype of the entry in question (will give PROCEDURE or FUNCTION)
 * 
 * synopsis			given the index (1..n) of the (global) symbol table entry, this function will return the 
 * 					datatype of the element held in that position of the symbol table. utilises the underlying (shared)
 * 					function _symbol_table_entry_type. 
 * 
 */
unsigned char symbol_table_entry_type_procfn( int position )
{
	
	return _symbol_table_entry_type( position , heap_base );
	
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			position					index into the given symbol table where the entry is to be found
 * 					base						pointer to the base of the symbol table where searching is to take place
 *
 * returns			symbol_table_entry_value	struct containing the associated entry's current value
 * 
 * synopsis			given an entry value (1..n), return the value of the element retained in the given position in the symbol table 
 * 
 */
union symbol_table_entry_value _symbol_table_entry_value( int position , unsigned char *base )
{
	
	unsigned char *ptr;
	unsigned char record_length;	// total bytes consumed by this record (1+1+n+v)
	unsigned char data_type;		// e.g. 0x01 for INTEGER
	int            index;
	union symbol_table_entry_value value;
	unsigned int padded_name_length;

	ptr   = base;
	index = 1;
	while ( index < position )
	{
		record_length = *ptr;
		ptr  += record_length;
		index++;
	}			

	// ok - we've now located the base pointer to the element in the symbol table - now reconstitute its value/structure
	record_length      = *ptr++;
	data_type          = *ptr++;
	padded_name_length = *ptr++;
	ptr++;

	// skip the identifier (including the terminating element); ensure padded (aligned) to 4 byte boundary
	ptr += padded_name_length;

	switch ( data_type )

	{
		case DATATYPE_REAL      : value.rval = *((float *)ptr);													break;
		case DATATYPE_INTEGER   : value.ival = *((int *)ptr);													break;
		case DATATYPE_STRING    : strcpy( (char *)value.sval , (char *)ptr );									break;
		case DATATYPE_PROCEDURE : value.pval = (*(ptr+0)<<0) + (*(ptr+1)<<8) + (*(ptr+2)<<16) + (*(ptr+3)<<24);	break;
		case DATATYPE_FUNCTION  : value.pval = (*(ptr+0)<<0) + (*(ptr+1)<<8) + (*(ptr+2)<<16) + (*(ptr+3)<<24);	break;
		case DATATYPE_BOOLEAN   : value.bval = *ptr;															break;
	}
	
	return value;
	
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			position					index into the (current) symbol table where the entry is to be found
 *
 * returns			symbol_table_entry_value	struct containing the associated entry's current value
 * 
 * synopsis			given an entry value (1..n), return the value of the element retained in the given position in the symbol table
 * 					based on the current symbol table. utilises the underlying _symbol_table_entry_value function. 
 * 
 */
union symbol_table_entry_value symbol_table_entry_value( int position )
{

	return _symbol_table_entry_value( position , symbol_table_base );

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			position					index into the (global) symbol table where the entry is to be found
 *
 * returns			symbol_table_entry_value	struct containing the associated entry's current value
 * 
 * synopsis			given an entry value (1..n), return the value of the element retained in the given position in the symbol table
 * 					based on the global symbol table, which is PROCEDURE and FUNCTION identifiers are to be found. 
 * 					utilises the underlying _symbol_table_entry_value function. 
 * 
 */
union symbol_table_entry_value symbol_table_entry_value_procfn( int position )
{

	return _symbol_table_entry_value( position , heap_base );

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			identifier			pointer to the string describing the identifier that is to be removed
 *
 * returns			void
 * 
 * synopsis			this function, provided with an identifier, will remove any existing identifier of the given name
 * 					from the current symbol table. utilises position_in_symbol_table to help it locate the identifier.
 * 
 */
void delete_from_symbol_table( unsigned char *identifier )
{

	unsigned char *ptr_src;
	unsigned char *ptr_dest;
	unsigned char record_length;	// total bytes consumed by this record (1+1+n+v)
	int            index;
	int            block_size;
		
	int position;
	if ( (position = position_in_symbol_table(identifier)) )
	{

		ptr_dest = symbol_table_base;
		index    = 1;
		while ( index < position )
		{
			ptr_dest += *ptr_dest;
			index++;
		}
		
		// ptr_dest points to base of memory to move block down TO
		// ptr_src will be adjusted to point to the NEXT element (where memory will be moved FROM)
		record_length = *ptr_dest;
		ptr_src = ptr_dest + record_length;
		
		// determine block size
		block_size = (heap_top - ptr_src) + 1;

		memmove(ptr_dest,ptr_src,block_size);
					
		// adjust heap_top appropriately
		heap_top -= (ptr_src - ptr_dest);
		
	}
	
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			identifier			pointer to the string that will - on exit - contain the identifier
 * 					position			the index into the (current) symbol table
 *
 * returns			void
 * 
 * synopsis			probe the (current) symbol table for the nth entry, returning (by means of "identifier") the
 * 					associated identifier.
 * 
 */
void symbol_table_entry_identifier( unsigned char *identifier , int position )
{
	
	unsigned char *ptr;
	unsigned char record_length;	// total bytes consumed by this record (1+1+n+v)
	int           index;
	
	ptr   = symbol_table_base;
	index = 1;
	while ( index < position )
	{
		record_length = *ptr;
		ptr  += record_length;
		index++;
	}
	
	strcpy( (char *)identifier , (char *)ptr+4 );
	
}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 * 
 * requires			identifier			pointer to the string that will - on exit - contain the identifier
 * 					position			the index into the (global) symbol table
 *
 * returns			void
 * 
 * synopsis			probe the (global) symbol table for the nth entry, returning (by means of "identifier") the
 * 					associated identifier. used specifically for identifying PROCEDURE or FUNCTION identifiers at the global level.
 * 
 */
void symbol_table_entry_identifier_procfn( unsigned char *identifier , int position )
{
	
	unsigned char *ptr;
	unsigned char record_length;	// total bytes consumed by this record (1+1+n+v)
	int           index;
	
	ptr   = heap_base;
	index = 1;
	while ( index < position )
	{
		record_length = *ptr;
		ptr  += record_length;
		index++;
	}
	
	strcpy( (char *)identifier , (char *)ptr+4 );
	
}
