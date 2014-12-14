/*
 * Register.h
 *
 *  Created on: March 11, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef REGISTER64_H_
#define REGISTER64_H_

#include "Register.h"

/**
 * Represents a 64-bit general purpose CPU register
 */
class Register64GP : public Register {
protected:
	REG_SIZE64 value;

public:

	/**
	 * Constructor
	 */
	Register64GP( const char *name );

	/**
	 * Destruction
	 */
	virtual ~Register64GP();

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
	 * Returns the 64-bit value of the register
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

#endif /* REGISTER64_H_ */
