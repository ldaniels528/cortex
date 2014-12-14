/*
 * StringClass.h
 *
 *  Created on: Mar 10, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef STRINGCLASS_H_
#define STRINGCLASS_H_

#include "../../class/Class.h"

/**
 * Represents a string data type
 */
class StringClass : public Class {
public:

	/**
	 * Constructor
	 */
	StringClass();

	/**
	 * Destruction
	 */
	virtual ~StringClass();

	/**
	 * Builds the code for the "append" method
	 */
	Method* appendMethod();

	/**
	 * Builds the code for the "flip" method
	 */
	Method* flipMethod();

	/**
	 * Builds the code for the "length" method
	 */
	Method* lengthMethod();

	/**
	 * Builds the code for the "ltrim" method
	 */
	Method* ltrimMethod();

	/**
	 * Builds the code for the "rtrim" method
	 */
	Method* rtrimMethod();

	/**
	 * Builds the code for the "toString" method
	 */
	Method* toStringMethod();

};

#endif /* STRINGCLASS_H_ */
