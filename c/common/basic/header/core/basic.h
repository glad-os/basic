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



#ifndef BASIC_H
#define BASIC_H



	//
	// constants
	//
	
	#define MAX_INPUT_LENGTH    238
	#define MAX_STRING_LENGTH   254					// single byte describes length (including 0x00 terminator) hence 0xfe
	#define MAX_LINE_NUMBER     65000
	#define PROMPT              ">"
	#define INPUT_PROMPT        "?"
	#define DEFAULT_MEMORY_SIZE 64
	#define FALSE               0
	#define TRUE                1

	#define ERROR_MISSING_REPEAT					"Missing REPEAT"
	#define	ERROR_BAD_EXPRESSION					"Bad expression"
	#define ERROR_SILLY								"Silly"
	#define ERROR_MISTAKE							"Mistake"
	#define ERROR_EXPECTED_COLON					"Expected :"
	#define ERROR_EXPECTED_CLOSE_BRACKET			"Expected )"
	#define ERROR_EXPECTED_COMMA					"Expected ,"
	#define ERROR_EXPECTED_EQUALS					"Expected ="
	#define ERROR_STACK_UNDERFLOW					"Stack underflow"
	#define ERROR_ESCAPE							"Escape"
	#define ERROR_BAD_LINE_NUMBER					"Bad line number"
	#define ERROR_BAD_FILE							"Bad file"
	#define ERROR_BAD_MODE							"Bad MODE"
	#define ERROR_SYNTAX							"Syntax error"
	#define ERROR_TYPE_MISMATCH						"Type mismatch"
	#define ERROR_NOT_IMPLEMENTED					"Not yet implemented"
	#define ERROR_ILLEGAL_VALUE						"Illegal value"
	#define ERROR_UNEXPECTED_DEFINE					"Unexpected DEFINE"
	#define ERROR_ILLEGAL_COLOUR					"Illegal COLOUR"
	#define ERROR_ILLEGAL_DATATYPE					"Illegal datatype"
	#define ERROR_ILLEGAL_FUNCTION_NAME				"Illegal FUNCTION name"
	#define ERROR_MISSING_IF						"Missing IF"
	#define ERROR_MISSING_PROCEDURE					"Missing PROCEDURE"
	#define ERROR_MISSING_TO						"Missing TO"
	#define ERROR_MISSING_THEN						"Missing THEN"
	#define ERROR_MISSING_FOR						"Missing FOR"
	#define ERROR_UNKNOWN_FUNCTION					"Unknown FUNCTION"
	#define ERROR_UNKNOWN_PROCEDURE					"Unknown PROCEDURE"
	#define ERROR_BAD_PARAMETERS					"Bad parameters"
	#define ERROR_NO_SUCH_VARIABLE					"No such variable"
	#define ERROR_OUT_OF_OPERATORS					"Out of operators"
	#define ERROR_INFINITY							"Infinity"
	#define ERROR_NOT_A_NUMBER						"Not a number"
	#define ERROR_BAD_STRING						"Bad string"
	#define ERROR_STRING_TOO_LONG					"String too long"

	#define FATAL_MEMORY_ALLOCATION_FAILURE			"Fatal: could not allocate memory for BASIC\n"
	#define FATAL_UNKNOWN_DATATYPE					"Fatal: unknown datatype\n"
	
	

	//
	// storage
	//
	
	int             memory_size;						// the number of Kb RAM that the computer has allocated to it
	int             error_state;						// interpreter's error flag so we can appreciate when an error has occurred 
	int             running;							// simple boolean (int) flag declaring whether the interpreter is currently running a program or not 
	unsigned char * top;								// "top" always points to the last byte of memory that the current program source occupies
	char            input[ MAX_INPUT_LENGTH + 1 ];		// +1 to allow for null at end
	int             return_encountered;					// flags whether RETURN has been encountered



	//
	// prototypes
	//
	
	void basic             ( void );
	void adapt_environment ( int argc , char *argv[] );
	void interpret         ( void );
	int  set_error_state   ( char *err );



#endif /*BASIC_H*/
