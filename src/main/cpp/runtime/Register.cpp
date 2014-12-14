/*
 * Register.cpp
 *
 *  Created on: Mar 24, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "Register.h"

/**
 * Constructor
 */
Register::Register( const char* name, int quantifier ) : ValueLocation( quantifier ) {
	this->name	= name;
	this->buf	= new char[60];
}

/**
 * Destruction
 */
Register::~Register() {
	delete buf;
}

/**
 * Returns the name of the register
 */
const char* Register::getName() {
	return name;
}
