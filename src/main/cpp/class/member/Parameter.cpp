/*
 * MethodParam.cpp
 *
 *  Created on: March 9, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "Parameter.h"

/**
 * Constructor
 */
Parameter::Parameter( TypeReference* type, char* data ) {
	this->type	= type;
	this->data	= data;
}

/**
 * Releases all resources
 */
Parameter::~Parameter() {
	// do nothing
}

/**
 * Compiles the data into byte code
 */
int Parameter::compile( CodeBuffer* cb ) {
	type->compile( cb );
	return 0;
}

/**
 * Decompiles the byte code into a parameter
 */
Parameter* Parameter::decompile( CodeBuffer* cb ) {
	TypeReference* type = TypeReference::decompile( cb );
	return new Parameter( type, 0 );
}


/**
 * Returns the parameter's type
 */
TypeReference* Parameter::getType() {
	return type;
}

/**
 * Returns the parameter's value
 */
char* Parameter::getData() {
	return data;
}
