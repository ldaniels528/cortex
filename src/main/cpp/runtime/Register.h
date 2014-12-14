/*
 * Register.h
 *
 *  Created on: March 24, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef REGISTER_H_
#define REGISTER_H_

#include <stdio.h>

#include "../OpCodes.h"
#include "ValueLocation.h"

/**
 * Represents a CPU register
 */
class Register : public ValueLocation {
protected:
	const char* name;
	char* buf;

public:

	/**
	 * Constructor
	 */
	Register( const char* name, int quantifier );

	/**
	 * Destruction
	 */
	virtual ~Register();

	/**
	 * Enables value assignment
	 */
	virtual Register& operator = ( REG_SIZE64 ) = 0;

	/**
	 * Enables value assessment
	 */
	virtual bool operator == ( REG_SIZE64 value ) = 0;

	/**
	 * Enables value addition + assignment
	 */
	virtual Register& operator += ( REG_SIZE64 value ) = 0;

	/**
	 * Enables value subtraction + assignment
	 */
	virtual Register& operator -= ( REG_SIZE64 value ) = 0;

	/**
	 * Enables increment
	 */
	virtual Register& operator ++ ( int ) = 0;

	/**
	 * Enables decrement
	 */
	virtual Register& operator -- ( int ) = 0;

	/**
	 * Returns the name of the register
	 */
	const char* getName();

	/**
	 * Returns the value of the register
	 */
	virtual REG_SIZE64 getValue() = 0;

	/**
	 * Sets the value of the register
	 */
	virtual void setValue( REG_SIZE64 value ) = 0;

	/**
	 * Returns the string representation of the register
	 */
	virtual const char* toString() = 0;

};

#endif /* REGISTER_H_ */
