/*
 * Builder.cpp
 *
 *  Created on: March 21, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "Builder.h"

#include "../compiler/Compiler.h"
#include "../compiler/CompilerState.h"

#include "builtin/BooleanClass.h"
#include "builtin/CharClass.h"
#include "builtin/DoubleClass.h"
#include "builtin/FloatClass.h"
#include "builtin/IntClass.h"
#include "builtin/LongClass.h"
#include "builtin/ShortClass.h"
#include "builtin/StringClass.h"
#include "builtin/VoidClass.h"

/**
 * Constructor
 */
Builder::Builder() {
	// define all built-in class types
	classes.push_back( new BooleanClass() );
	classes.push_back( new CharClass() );
	classes.push_back( new DoubleClass() );
	classes.push_back( new FloatClass() );
	classes.push_back( new IntClass() );
	classes.push_back( new LongClass() );
	classes.push_back( new ShortClass() );
	classes.push_back( new StringClass() );
	classes.push_back( new VoidClass() );
}

/**
 * Destruction
 */
Builder::~Builder() {
	classes.clear();
}

/**
 * Builds the class library
 */
int Builder::build() {
	int errorCode = 0;
	std::string outputPath;

	// create an instance of the compiler
	Compiler compiler = Compiler();
	CompilerState* state = new CompilerState();

	// write the class files to disk
	for( vector<Class*>::iterator i = classes.begin(); i != classes.end(); i++ ) {
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
		compiler.writeFile( state->codeBuffer, outputPath.data() );
	}

	return errorCode;
}
