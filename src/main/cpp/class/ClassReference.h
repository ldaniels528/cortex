/*
 * ClassReference.h
 *
 *  Created on: March 17, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef CLASSREFERENCE_H_
#define CLASSREFERENCE_H_

#include "../CodeBuffer.h"

/**
 * Represents a reference to a class; these are typically
 * used for importing classes.
 */
class ClassReference {
private:
	const char* classname;

public:

	/**
	 * Constructor
	 */
	ClassReference( const char* classname );

	/**
	 * Destructor
	 */
	virtual ~ClassReference();

	/**
	 * Returns the referenced class name
	 */
	const char* getClassName();

	/**
	 * Compiles the reference into byte code
	 */
	int compile( CodeBuffer* cb );

	/**
	 * Decompiles the byte code into a class reference
	 */
	static ClassReference* decompile( CodeBuffer* cb );

};

#endif /* CLASSREFERENCE_H_ */
