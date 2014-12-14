/*
 * MemoryReference.cpp
 *
 *  Created on: March 11, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "MemoryReference.h"

/**
 * Constructor
 */
MemoryReference::MemoryReference( void* pointer, int quantifier ): ValueLocation( quantifier ) {
	this->pointer = pointer;
}

/**
 * Destruction
 */
MemoryReference::~MemoryReference() {
	// do nothing
}

/**
 * Returns a pointer to the value
 */
void* MemoryReference::getPointer() {
	return pointer;
}

/**
 * Returns the underlying value
 */
REG_SIZE64 MemoryReference::getValue() {
	return *((REG_SIZE64*)pointer);
}

/**
 * Sets the underlying value
 */
void MemoryReference::setValue( REG_SIZE64 value ) {
	*((REG_SIZE64*)pointer) = value;
}
