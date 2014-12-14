/*
 * Common.cpp
 *
 *  Created on: March 13, 2010
 *      Author: lawrence.daniels@gmail.cm
 */

#include <iostream.h>

#include "Common.h"

/**
 * Returns the binary representation of the given value
 * @param value the given value
 */
const char* toBinary( const long value ) {
	// if the value is zero ...
	if( value == 0 ) {
		return "0";
	}

	// build the string
	std::string s;
	long factor = 1;
	while( value >= factor ) {
		s.insert( 0, value & factor ? "1" : "0" );
		factor <<= 1;

	}
	return s.data();
}
