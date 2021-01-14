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

// include the standard library
/*
#include "../library/include/string.h"
#include "../library/include/stdio.h"
#include "../kernel/include/video.h"
#include "../kernel/include/define.h"
#include "../kernel/include/fat32.h"
*/
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "swi.h"

#include "basic/header/core/basic.h"
#include "basic/header/core/lex.h"
#include "basic/header/core/memory.h"
#include "basic/header/core/stack.h"
#include "basic/header/core/symbols.h"
#include "basic/header/directive/break.h"
#include "basic/header/directive/insertline.h"
#include "basic/header/directive/list.h"
#include "basic/header/directive/load.h"
#include "basic/header/directive/new.h"
#include "basic/header/directive/renumber.h"
#include "basic/header/directive/run.h"
#include "basic/header/directive/run.h"
#include "basic/header/directive/save.h"
#include "basic/header/language/assignment.h"
#include "basic/header/language/cls.h"
#include "basic/header/language/colour.h"
#include "basic/header/language/define.h"
#include "basic/header/language/else.h"
#include "basic/header/language/end.h"
#include "basic/header/language/expression.h"
#include "basic/header/language/for.h"
#include "basic/header/language/if.h"
#include "basic/header/language/input.h"
#include "basic/header/language/line.h"
#include "basic/header/language/mode.h"
#include "basic/header/language/next.h"
#include "basic/header/language/point.h"
#include "basic/header/language/print.h"
#include "basic/header/language/procedure.h"
#include "basic/header/language/rectangle.h"
#include "basic/header/language/rem.h"
#include "basic/header/language/repeat.h"
#include "basic/header/language/return.h"
#include "basic/header/language/until.h"
#include "os/header/keyboard/keyboard.h"
#include "os/header/video/video.h"



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			argc			number of command-line arguments available
 * 					argv			array of strings describing the command-line arguments
 *
 * returns			void
 *
 * synopsis			entrypoint for BASIC. initialises the environment, issues a BREAK directive, and repeatedly accepts
 * 					input (commands) for BASIC to execute until such time as "QUIT" is entered.
 *
 */
int main( int argc, char **argv )
{

	int wantToQuit = 0;
	memory_size =  256;

	// configure memory available to BASIC
	// adapt_environment(argc,argv);
	initialise_memory(memory_size);	// Kb required
	initialise_stack();

	_directive_break();

	// stuff in sample BASIC
	error_state = 0; _initialise_expression_stacks(); initialise_lex("10 FOR x = 1 TO 7"); interpret();
	error_state = 0; _initialise_expression_stacks(); initialise_lex("20 COLOUR x"); interpret();
	error_state = 0; _initialise_expression_stacks(); initialise_lex("30 PRINT \" ARM COMPUTER \""); interpret();
	error_state = 0; _initialise_expression_stacks(); initialise_lex("40 PRINT chr(10)"); interpret();
	error_state = 0; _initialise_expression_stacks(); initialise_lex("50 NEXT"); interpret();
	error_state = 0; _initialise_expression_stacks(); initialise_lex("60 END"); interpret();


	while ( !wantToQuit )
	{
		keyboard_readline( input );

		// let's see about this cacheing issue - every BASIC line entered will print this out again
		// _video_display_memory( ARM_MAILBOX_TAG_ARM_MEMORY ); _kernel_video_print_char( (char) 10 );

		wantToQuit = ( !strcmp(input,"QUIT")) || (!strcmp(input,"quit") );
		if ( !wantToQuit && strlen(input) )
		{
			error_state = 0;					// reset the error state
			_initialise_expression_stacks();	// initialise the expression stacks in-between each interpreted line
			initialise_lex(input);				// initialise and interpret the supplied (tokenised) line
			interpret();
		}
		video_print( PROMPT );
	}

	video_print( "BASIC EXITED\n");

    // use the OS to exit this process formally
    process_exit();
	return 0;

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			argc			number of command-line arguments available
 * 					argv			array of strings describing the command-line arguments
 *
 * returns			void
 *
 * synopsis			examines the supplied command-line arguments to determine if a "-RAM" directive is specified. If so, the numeric
 * 					value attached to this identifies the amount of RAM (in Kb) to make available for the BASIC user. If such a directive
 * 					is not found, the default memory size is defaulted to.
 *
 */
void adapt_environment( int argc , char *argv[] )
{

	memory_size = DEFAULT_MEMORY_SIZE;

	// ACU - ignore adapting environment for now - stick with what memory has been defined statically
	// (this interpreter no longer runs from a Linux command line!)
	/*
	int i = 1;
	while ( i < argc )
	{
		if ( !strcmp( "-RAM" , argv[i] ) )
			memory_size = atoi(argv[i+1]);
		i++;
	}
	*/

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			void
 *
 * returns			void
 *
 * synopsis			this function is used by the following interpret() function; it is used to skip past the current statement in the stored
 * 					program; used in circumstances where the "execution gate" determines that the statement should NOT be interpreted.
 *
 */
void skip( void )
{

	struct lex_result result;
	struct lex_state  state;
	int res;

	while ( !error_state )
	{
		preserve_lex_state(&state);
		res = next_token(&result,TRUE);
		if ( (!res) || (result.token == PUNC_COLON) || (result.token == KEYWORD_ELSE) )
		{
			reinstate_lex_state(&state);
			break;
		}
	}

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			void
 *
 * returns			void
 *
 * synopsis			the main function used during interpretation; takes the next statement and, where appropriate, invokes the relevant
 * 					function(s) to handle its interpretation. Utilises two key variables to assist; "running" determines whether or not the
 * 					current statement is in relation to a running program (directives cannot be executed during a running program), and
 * 					"execution gate" is used to determine whether or not the current statement is allowed to be executed (in such instances
 * 					as the IF...ELSE...END IF construct, where only one of two code blocks is allowed to be executed).
 *
 */
void interpret( void )
{

	struct lex_result result;
	//extern int procedures_scanned;

	while ( next_token(&result,TRUE) && (!error_state) )
	{

		switch ( result.token )
		{

			// directives that affect the source result in proc/fn table being discarded
			case NUM_LIT			: if ( !running ) { _directive_insertline(result.str);	initialise_symbol_table(); procedures_scanned = 0;	} else { set_error_state( ERROR_SILLY ); } break;
			case KEYWORD_LIST		: if ( !running ) { _directive_list(); 		   		   														} else { set_error_state( ERROR_SILLY ); } break;
			case KEYWORD_RUN		: if ( !running ) { _directive_run(); 				   														} else { set_error_state( ERROR_SILLY ); } break;
			case KEYWORD_NEW		: if ( !running ) { _directive_new();					initialise_symbol_table(); procedures_scanned = 0;	} else { set_error_state( ERROR_SILLY ); } break;
			case KEYWORD_RENUMBER	: if ( !running ) { _directive_renumber(); 			   														} else { set_error_state( ERROR_SILLY ); } break;
			case KEYWORD_LOAD		: if ( !running ) { _directive_load(); 					initialise_symbol_table(); procedures_scanned = 0;	} else { set_error_state( ERROR_SILLY ); } break;
			case KEYWORD_SAVE		: if ( !running ) { _directive_save(); 				   														} else { set_error_state( ERROR_SILLY ); } break;

			// (certain) keywords need to ensure the proc/fn table is available (when using expressions)
			// TODO - argh - can't blanket initialise the symbol table or "a=1" followed by "PRINT a" will say "No such variable"!
			// ahhh. break (and therefore new()) initialise the symbol table. so we ought to leave it be unless we corrupt it via source alterations?
			case KEYWORD_PRINT		: if ( execution_gate ) { if ( !running ) { _declare_procedures(); } _print();					} else { skip(); }	break;
			case KEYWORD_DEFINE     : if ( execution_gate ) { if ( !running ) { _declare_procedures(); } _define(FALSE,0);			} else { skip(); }	break;
			case KEYWORD_PROCEDURE  : if ( execution_gate ) { if ( !running ) { _declare_procedures(); } _procedure();				} else { skip(); }	break;
			case KEYWORD_REM		: if ( execution_gate ) { if ( !running ) { _declare_procedures(); } _rem();					} else { skip(); }	break;
			case KEYWORD_COLOUR		: if ( execution_gate ) { if ( !running ) { _declare_procedures(); } _colour();					} else { skip(); }	break;
			case KEYWORD_REPEAT		: if ( execution_gate ) { if ( !running ) { _declare_procedures(); } _repeat();					} else { skip(); }	break;
			case KEYWORD_UNTIL		: if ( execution_gate ) { if ( !running ) { _declare_procedures(); } _until();					} else { skip(); }	break;
			case KEYWORD_MODE		: if ( execution_gate ) { if ( !running ) { _declare_procedures(); } _mode();					} else { skip(); }	break;
			case KEYWORD_CLS		: if ( execution_gate ) { if ( !running ) { _declare_procedures(); } _cls();					} else { skip(); }	break;
			case KEYWORD_RECTANGLE	: if ( execution_gate ) { if ( !running ) { _declare_procedures(); } _rectangle();				} else { skip(); }	break;
			case KEYWORD_FOR		: if ( execution_gate ) { if ( !running ) { _declare_procedures(); } _for();					} else { skip(); }	break;
			case KEYWORD_NEXT		: if ( execution_gate ) { if ( !running ) { _declare_procedures(); } _next();					} else { skip(); }	break;
			case KEYWORD_RETURN		: if ( execution_gate ) { if ( !running ) { _declare_procedures(); } _return(); 				} else { skip(); }	break;
			case KEYWORD_POINT		: if ( execution_gate ) { if ( !running ) { _declare_procedures(); } _point(); 					} else { skip(); }	break;
			case KEYWORD_LINE		: if ( execution_gate ) { if ( !running ) { _declare_procedures(); } _line(); 					} else { skip(); }	break;
			case KEYWORD_INPUT		: if ( execution_gate ) { if ( !running ) { _declare_procedures(); } _input(); 					} else { skip(); }	break;
			case ID					: if ( execution_gate ) { if ( !running ) { _declare_procedures(); } _assignment(result.str);	} else { skip(); }	break;

			case KEYWORD_IF			: _if();								break;
			case KEYWORD_ELSE		: _else();								break;
			case KEYWORD_END		: _finished(); return;					break;

			default					: set_error_state(ERROR_MISTAKE);		break;
		}

		// if there's been no error, and there's another token available, then if it's NOT a separator then you should complain
		if ( (!error_state) && (next_token(&result,TRUE)) && (result.token != PUNC_COLON) )
			set_error_state( ERROR_EXPECTED_COLON );

	}

}



// 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - - 8 < - - - - - - - - - -



/**
 *
 * requires			err			string indicating the nature of the error being reported
 *
 * returns			void
 *
 * synopsis			a function that can be called by almost any other element in the system. if an error state has not already been set,
 * 					this will set the error state, and report the error on-screen to the user. If the error is being set when a program is
 * 					running, will append to the string an indication of the current line at which the error has occurred.
 *
 */
int set_error_state( char *err )
{

	extern int error_state;
	char stuff[100];

	if ( !error_state )
	{
		if ( !running ) {
			video_print( err );
		} else {
			video_print( err );
			video_print( " at line " );
			sprintf_i( stuff, line_no );
			video_print( stuff );
		}
		video_print( "\n" );
	}

	error_state = 1;

	return error_state;

}
