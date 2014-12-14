/*
 * Register64FP.h
 *
 *  Created on: Mar 26, 2010
 *      Author: ldaniels
 */

#ifndef REGISTER64FP_H_
#define REGISTER64FP_H_

#include "Register.h"

/**
 * Represents a 64-bit floating point register
 */
class Register64FP : public Register {
private:
	double value;

public:

	/**
	 * Constructor
	 */
	Register64FP( const char* name );

	/**
	 * Destruction
	 */
	virtual ~Register64FP();

	/**
	 * Cast to double
	 */
	operator double();

	/**
	 * Enables assignment
	 */
	Register& operator = ( REG_SIZE64 );

	/**
	 * Enables equality
	 */
	bool operator == ( REG_SIZE64 value );

	/**
	 * Enables addition + assignment
	 */
	Register& operator += ( REG_SIZE64 value );

	/**
	 * Enables subtraction + assignment
	 */
	Register& operator -= ( REG_SIZE64 value );

	/**
	 * Enables increment
	 */
	Register& operator ++ ( int );

	/**
	 * Enables decrement
	 */
	Register& operator -- ( int );

	/**
	 * Returns the string representation of the register
	 */
	const char* toString();

	/**
	 * Returns a pointer to the value
	 */
	void* getPointer();

	/**
	 * Returns the 64-bit value of the register
	 */
	REG_SIZE64 getValue();

	/**
	 * Sets the value of the register
	 */
	void setValue( REG_SIZE64 value );

};

#endif /* REGISTER64FP_H_ */
