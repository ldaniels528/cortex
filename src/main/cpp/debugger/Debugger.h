/*
 * Debugger.h
 *
 *  Created on: March 11, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef DEBUGGER_H_
#define DEBUGGER_H_

#include <vector.h>

#include "../compiler/AssemblyParser.h"
#include "../compiler/CompilerState.h"
#include "../vm/VirtualMachine.h"

#define TAB(n) \
	for( int i = 0; i < n; i++ ) \
		printf( "\t" )

/**
 * SVM Assembly Language Debugger
 */
class Debugger {
private:
	AssemblyParser* parser;
	CompilerState* state;
	RuntimeState* runtime;
	Class* classinst;
	CPUCore *cpu;
	Tokenizer tok;
	bool alive;

public:

	/**
	 * Constructor
	 */
	Debugger();

	/**
	 * Destruction
	 */
	virtual ~Debugger();

	/**
	 * Starts the "main()" method of the given class file
	 */
	int debug( const char* classFilePath, int argc, char* args[] );

	/**
	 * Evaluates the given user input
	 */
	void evaluateUserInput( const char * );

protected:

	/**
	 * Evaluates the given command
	 */
	void evaluateCommand( const char *, Tokenizer );

	/**
	 * Toggles a breakpoint
	 */
	void breakpoint( Tokenizer tok );

	/**
	 * Displays detail information about a loaded class
	 */
	void classInfo( Tokenizer tok );

	/**
	 * Displays a list of instructions
	 */
	void disassemble( Tokenizer );

	/**
	 * Dumps the contents of memory
	 */
	void dump( Tokenizer );

	/**
	 * Evaluates the given command
	 */
	int executeCode();

	/**
	 * Executes the next instruction
	 */
	int executeCodeStep();

	/**
	 * Lists the currently loaded classes
	 */
	int listClasses( Tokenizer );

	/**
	 * Lists the methods available in the loaded class
	 */
	int listMethods( Tokenizer );

	/**
	 * Loads a class into memory
	 */
	int loadClass( Tokenizer );

	/**
	 * Exits the debugger
	 */
	void quit();

	/**
	 * Show register(s)
	 */
	void registers( Tokenizer tok );

	/**
	 * Displays all available input options
	 */
	void help();

private:

	/**
	 * Sets the default method
	 */
	int init( const char* );

	/**
	 * Displays detail information about a loaded class
	 */
	void showReferenceData( vector<ReferenceData*> refdatalist, int level );

	/**
	 * Returns the integer value of the given token
	 */
	int getIntegerValue( Tokenizer );

	/**
	 * Returns the string value of the given token
	 */
	const char* getStringValue( Tokenizer );

	/**
	 * Returns the default method (usually "main")
	 */
	Method* getDefaultMethod( Class* );

	/**
	 * Returns the modifiers for the given field
	 */
	const char* getModifiers( Field* );

	/**
	 * Returns the modifiers for the given scope
	 */
	const char* getModifiers( Scope* );

};

#endif /* DEBUGGER_H_ */
