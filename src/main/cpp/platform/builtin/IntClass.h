/**
 * IntClass.h
 *
 *  Created on: March 7, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef INTCLASS_H_
#define INTCLASS_H_

#include "../../class/Class.h"
#include "../../class/member/Method.h"

#define MIN_VALUE -2147483648;
#define MAX_VALUE  2147483647;

class IntClass : public Class {
private:
	int datarefSeq;

public:

	/**
	 * Constructor
	 */
	IntClass();

	/**
	 * Deconstructor
	 */
	virtual ~IntClass();

private:

	/**
	 * Builds the code for the absolute value ("abs") method
	 */
	Method* absMethod();

	/**
	 * Builds the code for the maximum value ("max") method
	 */
	Method* maxMethod();

	/**
	 * Builds the code for the minimum value ("min") method
	 */
	Method* minMethod();

	/**
	 * Builds the code for the "toString" method
	 */
	Method* toStringMethod();

};

#endif /* INTCLASS_H_ */
