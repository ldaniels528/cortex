/*
 * TypeReference.h
 *
 *  Created on: March 9, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef TYPEREFERENCE_H_
#define TYPEREFERENCE_H_

#include "../CodeBuffer.h"

/**
 * Represents a type (class reference) definition
 */
class TypeReference {
private:
	const char* className;
	bool array;
	int length;

public:

	/**
	 * Constructor
	 */
	TypeReference( const char *typeName, bool array = false, int length = 0 );

	/**
	 * Release resources
	 */
	virtual ~TypeReference();

	/**
	 * Saves the type references state to the given code buffer
	 * @param cb the given {@link CodeBuffer code buffer}
	 */
	int compile( CodeBuffer* );

	/**
	 * Compiles a type reference from the given code buffer
	 * @param cb the given {@link CodeBuffer code buffer}
	 */
	static TypeReference* decompile( CodeBuffer* );

	/**
	 * Returns the name of the referenced class
	 */
	const char* getName();

	/**
	 * Returns the length of the array
	 */
	int getLength();

	/**
	 * Indicates whether the type is an array
	 */
	bool isArray();

	/**
	 * Sets the array indicator
	 */
	void setArray( bool state );

};

#endif /* TYPEREFERENCE_H_ */
