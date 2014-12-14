/*
 * ConstantValue.h
 *
 *  Created on: Mar 16, 2010
 *      Author: ldaniels
 */

#ifndef CONSTANTVALUE_H_
#define CONSTANTVALUE_H_

#include "ValueLocation.h"

/**
 * Represents a constant value
 */
class ConstantValue : public ValueLocation {
private:
	REG_SIZE64 value;

public:

	/**
	 * Constructor
	 */
	ConstantValue( REG_SIZE64 value, int quantifier );

	/**
	 * Destructor
	 */
	virtual ~ConstantValue();

	/**
	 * Returns the underlying value
	 */
	void* getPointer();

	/**
	 * Sets the underlying value
	 */
	void setValue( void* value );

};

#endif /* CONSTANTVALUE_H_ */
