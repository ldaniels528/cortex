/*
 * Compiler.cpp
 *
 *  Created on: February 28, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include <string.h>
#include <iostream.h>
#include <fstream.h>
#include <hash_map.h>

#include "Compiler.h"

using namespace std;

#define BUFFER_SIZE 16384

/**
 * Default Constructor
 */
Compiler::Compiler() {
	// initialize the compiler state
	this->state = new CompilerState();

	// initialize the source code parser
	this->parser = new CodeParser( state );
}

/**
 * Releases all resources
 */
Compiler::~Compiler() {
	delete parser;
	delete state;
}

/**
 * Compiles the given source file
 */
int Compiler::compile( const char* sourcePath ) {
	// perform compile pass #1
	int errorCode = compile_pass1( sourcePath );
	if( !errorCode ) {
		// perform compile pass #2
		errorCode = compile_pass2();
	}

	// was there an error code?
	if( errorCode ) {
		printf( "Compile failed with error code %d\n", errorCode );
	}

	return errorCode;
}

/**
 * The initial pass of the compiler focuses on the
 * source files. This is where all sources are compiled
 * in memory as class file objects.
 */
int Compiler::compile_pass1( const char* sourcePath ) {
	int errorCode = 0;

	// open the input file
	ifstream in;
	in.open( sourcePath, ios::in );
	if( !in.is_open() ) {
		printf( "File '%s' not found\n", sourcePath );
		return 1;
	}

	// create the read buffer
	char* buffer = new char[BUFFER_SIZE];

	// instantiate the parser
	Tokenizer* tok = new Tokenizer();
	tok->setIgnoreDelimiters( false );
	tok->setNewLineSignificance( false );

	// instantiate the stack
	TokenStack* stack = new TokenStack( tok );

	// create the root name space
	NameSpace* rootns = new NameSpace( "" );
	state->addNameSpace( rootns );

	// interpret the contents
	while( !errorCode && !in.eof() ) {
		// read a block of text
		in.read( buffer, BUFFER_SIZE );

		// set the text
		tok->setText( buffer );
		stack->reset();

		// parse the text
		errorCode = parser->parse( rootns, stack );
	}

	// close the file handles
	in.close();

	// was this pass successful
	printf( "Pass 1: %s\n", ( errorCode ? "failed" : "success" ) );

	// release the resources
	delete buffer;
	delete tok;
	delete stack;

	// return the error code
	return errorCode;
}

/**
 * This is the resolution part of the compile process
 * where all class files must be resolved, and all
 * declared classes are written to disk.
 */
int Compiler::compile_pass2() {
	int errorCode = 0;

	// determine the output path
	string outputPath;

	// iterate the name spaces
	list<NameSpace*> nameSpaces = state->getNameSpaces();
	for( list<NameSpace*>::iterator n = nameSpaces.begin(); n != nameSpaces.end(); n++ ) {
		// get the classes
		list<Class*> classes = (*n)->getClasses();

		// write the class files to disk
		for( list<Class*>::iterator i = classes.begin(); i != classes.end(); i++ ) {
			Class* classdef = *i;
			//printf( "Generating class '%s'...\n", classdef->getFullyQualifiedName() );

			// create the output file name
			outputPath.clear();
			outputPath.append( classdef->getName() );
			outputPath.append( ".scc" );

			// compile the class file
			state->codeBuffer->reset();
			errorCode = classdef->compile( state->codeBuffer );

			// write the class to disk
			writeFile( state->codeBuffer, outputPath.data() );
		}
	}

	// was this pass successful
	printf( "Pass 2: %s\n", ( errorCode ? "failed" : "success" ) );

	// return the error code
	return errorCode;
}

/**
 * Writes the class file to disk as the given output path
 */
bool Compiler::writeFile( CodeBuffer *cb, const char *outputPath ) {
	// open the output file
	FILE *out = fopen( outputPath, "wb" );
	if( out == NULL ) {
		printf( "Could not open '%s' for writing\n", outputPath );
		return false;
	}

	// write the context to disk
	printf( "Writing %s: %ld bytes...\n", outputPath, cb->getLength() );
	fwrite( cb->getBuffer(), 1, cb->getLength(), out );
	fflush( out );
	fclose( out );
	return true;
}
