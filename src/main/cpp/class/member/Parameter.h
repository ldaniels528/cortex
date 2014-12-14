/*
 * MethodParam.h
 *
 *  Created on: Mar 9, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef METHODPARAM_H_
#define METHODPARAM_H_

#include <stdio.h>

#include "../../CodeBuffer.h"
#include "../TypeReference.h"

/**
 * Represents a method argument (parameter)
 */
class Parameter {
private:
	TypeReference* type;
	char* data;

public:

	/**
	 * Constructor
	 */
	Parameter( TypeReference* type, char* data = NULL );

	/**
	 * Releases all resources
	 */
	virtual ~Parameter();

	/**
	 * Compiles the data into byte code
	 */
	int compile( CodeBuffer* );

	/**
	 * Decompiles the byte code into a parameter
	 */
	static Parameter* decompile( CodeBuffer* );

	/**
	 * Returns the parameter's type
	 */
	TypeReference* getType();

	/**
	 * Returns the parameter's value
	 */
	char* getData();

};

#endif /* METHODPARAM_H_ */
