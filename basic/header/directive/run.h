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



#ifndef RUN_H_
#define RUN_H_



	//
	// storage
	//

	unsigned char *runtime_ptr;
	unsigned char  runtime_linelength;
	unsigned char  execution_gate;			// simple BOOLEAN (manipulated by IF...ELSE...END IF) to determine whether to EXECUTE current statement
	unsigned int   line_no;
	int            procedures_scanned;		// a Boolean flag determining whether we've recently scanned for PROC/FN elements
	int			   nested_function_context;	// describes the level of nesting of FN calls (else RETURN cannot know if it's been called appropriately!)



	//
	// prototypes
	//

	void _interpret          ( void );
	void _directive_run      ( void );
	void _declare_procedures ( void );



#endif /*RUN_H_*/
