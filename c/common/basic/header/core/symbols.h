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



#ifndef SYMBOLS_H_
#define SYMBOLS_H_



	//
	// constants
	//

	#define DATATYPE_INTEGER	0x01		// int identifies its value
	#define DATATYPE_STRING		0x02		// char array identifies its value
	#define DATATYPE_PROCEDURE	0x03		// unsigned int identifies its offset in source that points to line where DEFINE statement found
	#define DATATYPE_BOOLEAN	0x04		// boolean identifies "true" (1) or "false" (0) using a single byte
	#define DATATYPE_REPEAT     0x05		// used to indicate we've stacked a  REPEAT construct
	#define DATATYPE_FOR		0x06		// used to indicate we've stacked a  FOR    construct
	#define DATATYPE_IF			0x07		// used to indicate we've stacked an IF     construct
	#define DATATYPE_REAL		0x08		// float identifies its value (4 bytes in size) note we call it a "real" ("float"=technical)
	#define DATATYPE_FUNCTION	0x09		// 
	


	//
	// storage
	//

	unsigned char *heap_base;			// this is driven from "top"
	unsigned char *heap_top;			// this increases as the symbol table is populated. it's counterpart, "heap_base", is driven from "top"
	unsigned char *symbol_table_base;	// pointer to base of symbol table (can move in accordance with PROCEDUREs/FUNCTIONs)



	//
	// structures
	//

	union symbol_table_entry_value
	{
		int               ival;								// used for INTEGER
		unsigned char     sval[ MAX_STRING_LENGTH + 1 ];	// used for STRING
		unsigned long int pval;								// used for PROCEDURE (uli=32-bit; check! [value is technically the address in source where DEFINE line starts])
		unsigned char     bval;								// used for BOOLEAN
		float             rval;								// used for REAL
	};



	//
	// prototypes
	//
	
	void          initialise_symbol_table                                  ( void );
	void          add_to_symbol_table                                      ( unsigned char *name , unsigned char type , union symbol_table_entry_value value );
	void          append_to_symbol_table                                   ( unsigned char *name , unsigned char type , union symbol_table_entry_value value );
	void          _add_to_symbol_table                                     ( unsigned char *name , unsigned char type , union symbol_table_entry_value value );
	void          symbol_table_dump                                        ( void );
	int           position_in_symbol_table                                 ( unsigned char *identifier );
	int           position_in_symbol_table_procfn                          ( unsigned char *identifier );
	unsigned char symbol_table_entry_type                                  ( int position );
	unsigned char symbol_table_entry_type_procfn                           ( int position );
	union         symbol_table_entry_value symbol_table_entry_value        ( int position );
	union         symbol_table_entry_value symbol_table_entry_value_procfn ( int position );
	void          delete_from_symbol_table                                 ( unsigned char *identifier );
	void          symbol_table_entry_identifier                            ( unsigned char *identifier , int position );
	void          symbol_table_entry_identifier_procfn                     ( unsigned char *identifier , int position );
	int           number_of_parameters                                     ( int st_index );
	void          locate_nth_formal_parameter                              ( int st_index , int n , char *name );
	
	
	
#endif /*SYMBOLS_H_*/
