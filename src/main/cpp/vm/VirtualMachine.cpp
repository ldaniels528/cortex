/*
 * VirtualMachine.cpp
 *
 *  Created on: February 28, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "VirtualMachine.h"

/**
 * Constructor
 */
VirtualMachine::VirtualMachine() {
	this->runtime	= new RuntimeState();
	this->cpu 		= new CPUCore( runtime );
}

/**
 * Release all resources
 */
 VirtualMachine::~VirtualMachine() {
	 delete runtime;
	 delete cpu;
}

/**
 * Starts the "main()" method of the given class file
 */
int VirtualMachine::run( const char* classname, int argc, char* args[] ) {
	// load the class
	Class* classdef = runtime->getClassLoader()->resolveClass( classname );

	// was the class retrieved?
	if( classdef != NULL ) {
		// retrieve the "main" method
		Method* method = classdef->lookupMethod( MAIN_METHOD );

		// if the method was found ...
		if( ( method != NULL ) && method->isStatic() && method->isPublic() ) {
			// build the parameters
			vector<Parameter*> parameters = getMethodArguments( argc, args );

			// execute the method
			return execute( method, parameters );
		}
		else {
			printf( "No suitable \"%s\" method was found to invoke\n", MAIN_METHOD );
			return -2;
		}
	}
	else {
		printf( "'%s' could not be loaded\n", classname );
		return -1;
	}
}

/**
 * Builds a list of method arguments
 */
vector<Parameter*> VirtualMachine::getMethodArguments( int argc, char* args[] ) {
	TypeReference* type = new TypeReference( "string" );
	vector<Parameter*> parameters;
	for( int n = 1; n < argc; n++ ) {
		parameters.push_back( new Parameter( type, args[n] ) );
	}
	return parameters;
}

/**
 * Executes the method
 * @param method the given method
 * @param parameters the method argument parameters
 */
int VirtualMachine::execute( Method* method, vector<Parameter*> parameters ) {
	int errorCode;

	// execute the method's byte code
	cpu->focus( method );

	// continue to cycle until the CPU has completed
	// its work.
	while( cpu->isAlive() ) {
		// evaluate the next instruction
		if( ( errorCode = cpu->execute() ) ) {
			return errorCode;
		}
	}

	return 0;
}
