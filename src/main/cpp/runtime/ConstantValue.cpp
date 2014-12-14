/*
 * ConstantValue.cpp
 *
 *  Created on: Mar 16, 2010
 *      Author: ldaniels
 */

#include "ConstantValue.h"

ConstantValue::ConstantValue( REG_SIZE64 value, int quantifier ) : ValueLocation( quantifier ) {
	this->value = value;
}

ConstantValue::~ConstantValue() {
	// do nothing
}

/**
 * Returns the underlying value
 */
void* ConstantValue::getPointer() {
	return (void*)&value;
}
