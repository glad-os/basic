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
#include "basic/header/core/symbols.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 



#ifndef DEFINE_H_
#define DEFINE_H_



	//
	// constants
	//

	#define FORMAL 0
	#define ACTUAL 1



	//
	// storage
	//
	
	unsigned char signature[MAX_INPUT_LENGTH];	// when define is called, id is passed. use this to construct the PROC/FN "signature"
	int parameter_count;						// when dealing with parameter lists, keep a count of the number encountered during processing
	
	
	
	//
	// prototypes
	//
	
	void _define                   ( int record_in_symbol_table , unsigned char *ptr );
	void _extract_definition       ( int type , unsigned char *ptr );
	void _expect_parameters        ( int type , int st_index );
	void _expect_var               ( int type , int st_index );
	void _expect_separator         ( int type , int st_index );
	void _expect_more_parameters   ( int type , int st_index );
	void _process_formal_parameter ( unsigned char *id );
	void _process_actual_parameter ( int datatype , union symbol_table_entry_value actual_value , int st_index );

	
	
#endif /*DEFINE_H_*/
