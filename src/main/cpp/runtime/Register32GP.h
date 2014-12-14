/*
 * Register32.h
 *
 *  Created on: Mar 24, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef REGISTER32_H_
#define REGISTER32_H_

#include "Register.h"

/**
 * Represents a 32-bit general purpose CPU register
 */
class Register32GP : public Register {
protected:
	REG_SIZE32 value;

public:

	/**
	 * Constructor
	 */
	Register32GP( const char *name );

	/**
	 * Destruction
	 */
	virtual ~Register32GP();

	/**
	 * Enables cast to a value
	 */
	operator REG_SIZE64();

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
	 * Returns a pointer to the value
	 */
	void* getPointer();

	/**
	 * Returns the 32-bit value of the register
	 */
	REG_SIZE64 getValue();

	/**
	 * Sets the value of the register
	 */
	void setValue( REG_SIZE64 value );

	/**
	 * Returns the string representation of the register
	 */
	const char* toString();

};

#endif /* REGISTER32_H_ */
