/*
 * Debugger.cpp
 *
 *  Created on: March 11, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>

#include "Debugger.h"

/**
 * Constructor
 */
Debugger::Debugger() {
	this->runtime	= new RuntimeState();
	this->cpu 		= new CPUCore( runtime );
	this->state	 	= new CompilerState();
	this->parser 	= new AssemblyParser( state );
	this->alive	 	= true;
}

/**
 * Destruction
 */
Debugger::~Debugger() {
	delete runtime;
	delete cpu;
	delete parser;
	delete state;
}

/**
 * Starts the "main()" method of the given class file
 */
int Debugger::debug( const char* classname, int argc, char* args[] ) {
	static const int MAX_SIZE  = 256;
	char* command = new char[MAX_SIZE];

	// print the version #
	printf( "Cortex Compiler/Runtime (%s) v%d.%02d\n"
			"Lawrence L. Daniels (lawrence.daniels@gmail.com)\n"
			"Copyright (C) Feb 28, 2010\n"
			"All rights reserved\n\n",
			RELEASE, MAJOR_VERSION, MINOR_VERSION );

	// display the "debug mode" indicator
	printf( "[DEBUG MODE]\n" );
	printf( "Enter \"?\" for help\n" );

	// initialize the debugger
	if( init( classname ) ) {
		return -2;
	}

	// begin handling user input
	while( alive ) {
		// get a command from the user input
		printf( "%s# ", cpu->getScope()->getName() );
		fgets( command, MAX_SIZE, stdin );
		fflush( stdin );

		// evaluate the command
		evaluateUserInput( command );
	}

	// free allocated objects
	delete command;

	// return the exit code
	return 0;
}

/**
 * Evaluates the given user input
 */
void Debugger::evaluateUserInput( const char* input ) {
	Token* t;

	// set the text to parse
	tok.setText( input );

	// was a token available?
	if( tok.hasNext() && ( t = tok.next() ) ) {
		// get the command
		const char *command = t->getText();

		// evaluate the command
		evaluateCommand( command, tok );
	}
}

/**
 * Evaluates the given command
 */
void Debugger::evaluateCommand( const char* command, Tokenizer tok ) {
	// is it a shortcut?
	const char shortcut = ( strlen(command) == 1 ) ? *command : 0;
	switch( shortcut ) {
		// breakpoint
		case 'b': breakpoint( tok ); break;

		// classes
		case 'c': listClasses( tok ); break;

		// class information
		case 'C': classInfo( tok ); break;

		// dump memory
		case 'd': dump( tok ); break;

		// go
		case 'g': executeCode(); break;

		// help
		case '?': help(); break;

		// list
		case 'l': disassemble( tok ); break;

		// load class
		case 'L': loadClass( tok ); break;

		// methods
		case 'm': listMethods( tok ); break;

		// next instruction
		case 'n': executeCodeStep(); break;

		// quit
		case 'Q':;
		case 'q': quit(); break;

		// registers
		case 'r': registers( tok ); break;

		// unrecognized
		default:
			printf( "Command \"%s\" was not recognized\n", command );
			printf( "Enter \"?\" for help\n" );
	}
}

/**
 * Toggles a breakpoint
 */
void Debugger::breakpoint( Tokenizer tok ) {
	printf( "Function is not yet implemented\n" );
}

/**
 * Displays detail information about a loaded class
 */
void Debugger::classInfo( Tokenizer tok ) {
	const char* format = "%-10s: %s\n";
	int n;

	// display the basic information
	printf( format, "Class name", classinst->getName() );
	printf( format, "Namespace", classinst->getPackageName() );
	printf( format, "Modifiers", getModifiers( classinst ) );

	// display the field information
	vector<Field*> fields = classinst->getFields();
	if( !fields.empty() ) {
		printf( "\nFields\n" );
		n = 0;
		for( vector<Field*>::iterator i = fields.begin(); i != fields.end(); ++i ) {
			Field* field = *i;

			// display it all
			printf( "[%03d] %s %s <%s>\n", ++n, field->getName(), field->getType()->getName(), getModifiers( field ) );
		}
	}

	// display the method information
	printf( "\nMethods\n" );
	vector<Method*> methods = classinst->getMethods();
	n = 0;
	for( vector<Method*>::iterator i = methods.begin(); i != methods.end(); ++i ) {
		Method* method = *i;

		// is this the active method?
		const char* asterisk = ( method == cpu->getScope() ? "*" : "" );

		// display it all
		printf( "[%03d] %s%s <%s>\n", ++n, asterisk, method->getSignature(), getModifiers( method ) );
		showReferenceData( method->getReferenceDataList(), 1 );
	}

	// display the reference data
	showReferenceData( classinst->getReferenceDataList(), 0 );
}

/**
 * Displays a list of instructions for the current method
 */
void Debugger::disassemble( Tokenizer tok ) {
	// cache the virtual memory instance
	VirtualMemory* mem = runtime->getMemory();

	// cache the scope
	Scope* scope = cpu->getScope();

	// get a pointer to the current memory location
	OFFSET_T offset = cpu->r.ip;
	INSTRUCTION_T *p = mem->getInstructionPtr( offset );

	// get the desired instruction count
	int count = getIntegerValue( tok );
	if( count < 1 ) {
		count = 10;
	}

	// display some instructions
	for( int n = 0; n < count; n++ ) {
		INSTRUCTION_T instruction = *p++;
		const char *code = parser->disassemble( scope, offset, instruction );
		printf( "[%08lX] %08X %s\n", offset, instruction, code );
		offset += 4;
		delete code;
	}
}

/**
 * Dumps the contents of memory
 */
void Debugger::dump( Tokenizer tok ) {
	// get the virtual memory instance
	VirtualMemory* mem = runtime->getMemory();

	// get the desired data count
	int count = getIntegerValue( tok );

	// bound the range
	const int maxCount = mem->getLength() - mem->getPosition();
	if( count < 1 ) {
		count = 160;
	}
	else if( count > maxCount ) {
		count = maxCount;
	}

	// dump the memory contents
	printf( "Displaying %d of %ld bytes\n", count, mem->getLength() );
	mem->dump( count );
}

/**
 * Evaluates the given command
 */
int Debugger::executeCode() {
	int errorCode;

	// cache the virtual memory instance
	VirtualMemory* mem = runtime->getMemory();

	// cache the scope
	Scope* scope = cpu->getScope();

	// reset the IP
	cpu->r.ip = scope->getCodeOffset();

	// continue to cycle until the CPU has
	// completed its work.
	while( cpu->isAlive() ) {
		// capture the offset
		const OFFSET_T offset = cpu->r.ip;

		// get the next instruction
		const INSTRUCTION_T instruction = mem->getInstruction( offset );

		// evaluate the next instruction
		printf( "[%08lX] %08X %s\n", offset, instruction, parser->disassemble( scope, offset, instruction ) );
		if( ( errorCode = cpu->execute( instruction ) ) ) {
			return errorCode;
		}

		// advance the IP
		cpu->r.ip += sizeof(INSTRUCTION_T);
	}
	return 0;
}

/**
 * Executes the next instruction
 */
int Debugger::executeCodeStep() {
	// cache the virtual memory instance
	VirtualMemory* mem = runtime->getMemory();

	// cache the scope
	Scope* scope = cpu->getScope();

	// capture the offset
	const OFFSET_T offset = cpu->r.ip;

	// get the next instruction
	const INSTRUCTION_T instruction = mem->getInstruction( offset );

	// display the next instruction
	const char *code = parser->disassemble( scope, offset, instruction );
	printf( "[%08lX] %08X %s\n", offset, instruction, code );
	delete code;

	// evaluate the next instruction
	if( cpu->execute( instruction ) ) {
		return -1;
	}

	// advance the instruction pointer
	cpu->r.ip += sizeof(INSTRUCTION_T);

	return 0;
}

/**
 * Displays all available input options
 */
void Debugger::help() {
	const int count = 12;
	const char* format = "%-14s %s\n";
	const char* instructions[count][2] = {
			{ "[?] help", "Displays the list of acceptable commands" },
			{ "[b] breakpoint", "Sets/removes a breakpoint" },
			{ "[c] class", "Lists classes or sets active class" },
			{ "[C] class info", "Displays detailed information about the active class" },
			{ "[d] dump", "Displays the contents of memory" },
			{ "[g] go", "Executes the instruction at the current IP position" },
			{ "[l] list", "List the instructions" },
			{ "[L] load class", "Loads a class into memory" },
			{ "[m] method", "Lists methods or sets active method" },
			{ "[n] next", "Executes the next instruction" },
			{ "[q] quit", "Exits the program" },
			{ "[r] register", "Displays the registers" }
	};
	printf( format, "Instruction", "Description" );
	for( int n = 0; n < count; n++ ) {
		printf( format, instructions[n][0], instructions[n][1] );
	}
}

/**
 * Lists the currently loaded classes
 */
int Debugger::listClasses( Tokenizer tok ) {
	// get the list of ,methods
	vector<Class*> classList = runtime->getClassLoader()->getClasses();

	// is there another token?
	if( tok.hasNext() ) {
		// get the index from the stream
		int index = getIntegerValue( tok );
		if( index >= 0 ) {
			// validate the index range
			if( index < 1 || index > (int)classList.size() ) {
				printf( "Method #%d is out of bounds\n", index );
				return -1;
			}

			// switch the focus
			Class* theClass = classList[index-1];
			Method* method = getDefaultMethod( theClass );
			if( method != NULL ) {
				classinst = theClass;
				cpu->focus( method );
				return 0;
			}
			return -2;
		}
	}

	// display the table
	printf( "Resident classes:\n" );
	printf( "----------------------------------\n"
			"### Class name\n"
			"----------------------------------\n" );

	int n = 0;
	for( vector<Class*>::iterator i = classList.begin(); i != classList.end(); ++i ) {
		Class* theClass = *i;
		const char* asterisk = ( theClass == classinst ? "*" : "" );
		printf( "%03d %s%s\n", ++n, asterisk, theClass->getFullyQualifiedName() );
	}

	printf( "----------------------------------\n"
			"%d classes found\n",
			(int)classList.size() );
	return 0;
}

/**
 * Lists the methods available in the loaded class
 */
int Debugger::listMethods( Tokenizer tok ) {
	// get the list of methods
	vector<Method*> methods = classinst->getMethods();

	// is there another token?
	if( tok.hasNext() ) {
		// get the index from the stream
		int index = getIntegerValue( tok );
		if( index >= 0 ) {
			// validate the index range
			if( index < 1 || index > (int)methods.size() ) {
				printf( "Method #%d is out of bounds\n", index );
				return -2;
			}

			// switch the focus
			cpu->focus( methods[index-1] );
			return 0;
		}
	}

	// display the table
	printf( "Class '%s'\n", classinst->getName() );
	printf( "----------------------------------\n"
			"### %-8s %s\n"
			"----------------------------------\n",
			"Offset", "Method name" );

	int n = 0;
	for( vector<Method*>::iterator i = methods.begin(); i != methods.end(); ++i ) {
		Method* method = *i;
		const char* asterisk = ( method == cpu->getScope() ? "*" : "" );
		printf( "%03d %08lX %s%s\n", ++n, method->getCodeOffset(), asterisk, method->getSignature() );
	}

	printf( "----------------------------------\n"
			"%d method(s) found\n",
			(int)methods.size() );
	return 0;
}

/**
 * Loads a class into memory
 */
int Debugger::loadClass( Tokenizer tok ) {
	// get the class name
	const char* classname = getStringValue( tok );
	if( classname == NULL ) {
		return -1;
	}

	// load the class
	runtime->getClassLoader()->resolveClass( classname );
	return 0;
}

/**
 * Exits the debugger
 */
void Debugger::quit() {
	alive = false;
}

/**
 * Show register(s)
 */
void Debugger::registers( Tokenizer tok ) {
	cpu->dump();
}

/**
 * Returns the integer value of the given token
 */
int Debugger::getIntegerValue( Tokenizer tok ) {
	Token *t;

	if( ( t = tok.next() ) ) {
		if( t->getType() != tok::INTEGER ) {
			printf( "Numeric (integer) value expected\n" );
			return -1;
		}
		else {
			// get the desired value
			return atoi( t->getText() );
		}
	}
	return 0;
}

/**
 * Returns the string value of the given token
 */
const char* Debugger::getStringValue( Tokenizer tok ) {
	Token *t;

	if( ( t = tok.next() ) ) {
		if( t->getType() != tok::ALPHA_NUMERIC ) {
			printf( "Text value expected\n" );
			return NULL;
		}
		else {
			// get the desired value
			return t->getText();
		}
	}
	return NULL;
}

/**
 * Sets the default method
 */
int Debugger::init( const char* classname ) {
	const int MAX_COUNT = 160;

	// load the class
	classinst = runtime->getClassLoader()->resolveClass( classname );
	if( classinst == NULL ) {
		return -1;
	}

	// retrieve the default method
	Method* method = getDefaultMethod( classinst );
	if( method == NULL ) {
		return -2;
	}

	// point the CPU at the main method
	cpu->focus( method );

	// dump the contents of memory
	VirtualMemory* mem = runtime->getMemory();
	const int count = ( mem->getLength() > MAX_COUNT ) ? MAX_COUNT : mem->getLength();
	mem->top();
	mem->dump( count );
	return 0;
}

/**
 * Returns the default method (usually "main")
 */
Method* Debugger::getDefaultMethod( Class* theClass ) {
	Method* method = NULL;

	// is there a main method?
	method = theClass->lookupMethod( MAIN_METHOD );
	if( method == NULL ) {
		// get the list of methods for the class
		vector<Method*> methods = theClass->getMethods();

		// were there any?
		if( !methods.empty() ) {
			method = methods.front();
		}
		else {
			printf( "No methods found in class '%s'\n", theClass->getFullyQualifiedName() );
		}
	}

	return method;
}

/**
 * Returns the modifiers for the given field
 */
const char* Debugger::getModifiers( Field* field ) {
	std::string buf;

	// append the privacy information
	buf.append(
		field->isPublic() ? "public" :
		field->isPrivate() 	? "private" :
		field->isProtected() ? "protected" : "package"
	);

	// is the scope static?
	if( field->isStatic() ) {
		buf.append( ", static" );
	}

	// is the scope final?
	if( field->isFinal() ) {
		buf.append( ", final" );
	}

	// return the data
	return buf.data();
}

/**
 * Returns the modifiers for the given scope
 */
const char* Debugger::getModifiers( Scope* scope ) {
	std::string buf;

	// append the privacy information
	buf.append(
		scope->isPublic() ? "public" :
		scope->isPrivate() 	? "private" :
		scope->isProtected() ? "protected" : "package"
	);

	// is the scope static?
	if( scope->isStatic() ) {
		buf.append( ", static" );
	}

	// is the scope abstract?
	if( scope->isAbstract() ) {
		buf.append( ", abstract" );
	}

	// is the scope final?
	if( scope->isFinal() ) {
		buf.append( ", final" );
	}

	// return the data
	return buf.data();
}

/**
 * Displays detail information about a loaded class
 */
void Debugger::showReferenceData( vector<ReferenceData*> refdatalist, int level ) {
	if( !refdatalist.empty() ) {
		TAB(level); printf( "Reference Data\n" );
		for( vector<ReferenceData*>::iterator i = refdatalist.begin(); i != refdatalist.end(); ++i ) {
			ReferenceData* refdata = *i;

			// display it all
			TAB(level); printf( "%03d: \"%s\"\n", refdata->getReferenceId(), refdata->getContent() );
		}
		printf( "\n" );
	}
}
