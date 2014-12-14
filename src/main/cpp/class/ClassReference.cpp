/*
 * ClassReference.cpp
 *
 *  Created on: Mar 17, 2010
 *      Author: ldaniels
 */

#include "ClassReference.h"

/**
 * Constructor
 */
ClassReference::ClassReference( const char* classname ) {
	this->classname	= classname;
}

/**
 * Destruction
 */
ClassReference::~ClassReference() {
	// do nothing
}

/**
 * Returns the referenced class name
 */
const char* ClassReference::getClassName() {
	return classname;
}

/**
 * Compiles the reference into byte code
 */
int ClassReference::compile( CodeBuffer* cb ) {
	cb->putString( classname );
	return 0;
}

/**
 * Decompiles the byte code into a class reference
 */
ClassReference* ClassReference::decompile( CodeBuffer* cb ) {
	const char *classname = cb->getString();
	return new ClassReference( classname );
}
