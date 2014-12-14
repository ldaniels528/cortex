/*
 * TypeReference.cpp
 *
 *  Created on: March 9, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include <stdio.h>

#include "TypeReference.h"

/**
 * Constructor
 */
TypeReference::TypeReference( const char *typeName, bool array, int length ) {
	this->className = typeName;
	this->array		= array;
	this->length	= length;
}

/**
 * Release resources
 */
TypeReference::~TypeReference() {
	// do nothing
}

/**
 * Saves the type references state to the given code buffer
 * @param cb the given {@link CodeBuffer code buffer}
 */
int TypeReference::compile( CodeBuffer *cb ) {
	cb->putString( className );
	cb->putChar( array );
	cb->putInt( length );
	return 0;
}

/**
 * Compiles a type reference from the given code buffer
 * @param cb the given {@link CodeBuffer code buffer}
 */
TypeReference* TypeReference::decompile( CodeBuffer *cb ) {
	const char* name 	= cb->getString();
	const bool array	= cb->getChar();
	const int length	= cb->getInt();
	return new TypeReference( name, array, length );
}

/**
 * Returns the class of the referenced class
 */
const char* TypeReference::getName() {
	return className;
}

/**
 * Returns the length of the array
 */
int TypeReference::getLength() {
	return length;
}

/**
 * Indicates whether the type is an array
 */
bool TypeReference::isArray() {
	return array;
}

/**
 * Sets the array indicator
 */
void TypeReference::setArray( bool state ) {
	this->array = state;
}
