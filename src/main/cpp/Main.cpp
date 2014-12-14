/*
 * Serpent Virtual Machine (SVM)
 * JUX - Juxtaposition of programming languages
 *
 *  Created on: February 28, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include <stdio.h>
#include <string.h>

#include "Main.h"
#include "Common.h"
#include "compiler/Compiler.h"
#include "debugger/Debugger.h"
#include "platform/Builder.h"
#include "runtime/StringFormatter.h"
#include "vm/VirtualMachine.h"

/**
 * Constructor
 */
Main::Main() {
	// do nothing
}

/**
 * Destruction
 */
Main::~Main() {
	// do nothing
}

/**
 * Application startup function
 * @param argc the number of arguments
 * @param argv the argument array
 */
int main( int argc, char* argv[] ) {
	// were input files passed in?
	if( argc < 2 ) {
		printf( "No input files\n" );
		return -1;
	}

	// create an instance of main
	Main app;

	// get the command directive
	const char *cmd = argv[1];

	// compile the sources
	if( !strcmp( cmd, "-c" ) ) {
		return app.compileSources( 2, argc, argv );
	}

	// dumps the contents of a class file
	else if( !strcmp( cmd, "-d" ) ) {
		return app.debug( argc, argv );
	}

	// build the "sera.lang" assembly
	if( !strcmp( cmd, "-i" ) ) {
		return app.build();
	}

	// test something
	else if( !strcmp( cmd, "-t" ) ) {
		return app.test();
	}

	// display version #
	else if( !strcmp( cmd, "-v" ) ) {
		printf( "%s version %d.%02d\n", APP_NAME, MAJOR_VERSION, MINOR_VERSION );
		return 0;
	}

	// start the SVM
	else {
		return app.executeBinary( argc, argv );
	}
}

/**
 * Compiles the given source files
 * @param start the starting index
 * @param count the ending index
 * @param argv the given file arguments
 */
int Main::compileSources( int start, int end, char* argv[] ) {
	// are there enough arguments
	if( end < 3 ) {
		printf( "No source files\n" );
		return -1;
	}

	// instantiate the compiler instance
	Compiler *compiler = new Compiler();

	// print out the input
	int index = 1;
	for( int n = start; n < end; n++ ) {
		printf( "[%03d] Compiling '%s'\n", index++, argv[n] );
		int result = compiler->compile( argv[n] );
		if( !result ) {
			return result;
		}
	}
	return 0;
}

/**
 * Builds the "sera.lang" assembly
 */
int Main::build() {
	Builder builder = Builder();
	return builder.build();
}

/**
 * Debugs the given SVM class
 * @param argc the given number of arguments
 * @param argv the given arguments
 */
int Main::debug( int argc, char* argv[] ) {
	// cache the class name
	const char* classname = argv[2];

	// startup the virtual machine
	Debugger *debugger = new Debugger();

	// execute/debug the class
	return debugger->debug( classname, argc, argv );
}

/**
 * Executes the given SVM class
 * @param argc the number of arguments passed
 * @param argv the given file arguments
 */
int Main::executeBinary( int argc, char* argv[] ) {
	// cache the class name
	const char* classname = argv[1];

	// startup the virtual machine
	VirtualMachine *vm = new VirtualMachine();

	// execute the class
	return vm->run( classname, argc, argv );
}

/**
 * Perform testing here
 */
int Main::test() {
	StringFormatter formatter;

	puts( formatter.format( "Hello %b %o %x World\n", 16, 256, 65535 ) );
	return 0;
}
