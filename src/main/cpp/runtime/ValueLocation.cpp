/*
 * ValueLocation.cpp
 *
 *  Created on: March 11, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include <stdio.h>

#include "ValueLocation.h"

/**
 * Constructor
 */
ValueLocation::ValueLocation( int quantifier ) {
	this->quantifier = quantifier;
}

/**
 * Destructor
 */
ValueLocation::~ValueLocation() {
	// do nothing
}

/**
 * Returns the size of the value (quantifier)
 */
int ValueLocation::getQuantifier() {
	return quantifier;
}

/**
 * Returns the character/byte value
 */
char ValueLocation::charValue() {
	CAST(char);
}

/**
 * Sets the character value
 */
void ValueLocation::setCharValue( char value ) {
	switch( quantifier ) {
		case DQ_BYTE:	SETVAL(char,value); break;
		case DQ_SHORT:	SETVAL(short,value); break;
		case DQ_INT:	SETVAL(int,value); break;
		case DQ_LONG:	SETVAL(long,value); break;
		case DQ_FLOAT:	SETVAL(float,value); break;
		case DQ_DOUBLE: SETVAL(double,value); break;
		default:
			printf( "Quantifier type (%d) was not handled\n", quantifier );
	}
}

/**
 * Returns the short value
 */
short ValueLocation::shortValue() {
	CAST(short);
}

/**
 * Sets the short value
 */
void ValueLocation::setShortValue( short  value) {
	switch( quantifier ) {
		case DQ_BYTE:	SETVAL(char,value); break;
		case DQ_SHORT:	SETVAL(short,value); break;
		case DQ_INT:	SETVAL(int,value); break;
		case DQ_LONG:	SETVAL(long,value); break;
		case DQ_FLOAT:	SETVAL(float,value); break;
		case DQ_DOUBLE: SETVAL(double,value); break;
		default:
			printf( "Quantifier type (%d) was not handled\n", quantifier );
	}
}

/**
 * Returns the integer value
 */
int ValueLocation::intValue() {
	CAST(int);
}

/**
 * Sets the integer value
 */
void ValueLocation::setIntValue( int value ) {
	switch( quantifier ) {
		case DQ_BYTE:	SETVAL(char,value); break;
		case DQ_SHORT:	SETVAL(short,value); break;
		case DQ_INT:	SETVAL(int,value); break;
		case DQ_LONG:	SETVAL(long,value); break;
		case DQ_FLOAT:	SETVAL(float,value); break;
		case DQ_DOUBLE: SETVAL(double,value); break;
		default:
			printf( "Quantifier type (%d) was not handled\n", quantifier );
	}
}

/**
 * Returns the long value
 */
long ValueLocation::longValue() {
	CAST(long);
}

/**
 * Sets the long integer value
 */
void ValueLocation::setLongValue( long value ) {
	switch( quantifier ) {
		case DQ_BYTE:	SETVAL(char,value); break;
		case DQ_SHORT:	SETVAL(short,value); break;
		case DQ_INT:	SETVAL(int,value); break;
		case DQ_LONG:	SETVAL(long,value); break;
		case DQ_FLOAT:	SETVAL(float,value); break;
		case DQ_DOUBLE: SETVAL(double,value); break;
		default:
			printf( "Quantifier type (%d) was not handled\n", quantifier );
	}
}

/**
 * Returns the float value
 */
float ValueLocation::floatValue() {
	CAST(float);
}

/**
 * Sets the float value
 */
void ValueLocation::setFloatValue( float value ) {
	switch( quantifier ) {
		case DQ_BYTE:	SETVAL(char,value); break;
		case DQ_SHORT:	SETVAL(short,value); break;
		case DQ_INT:	SETVAL(int,value); break;
		case DQ_LONG:	SETVAL(long,value); break;
		case DQ_FLOAT:	SETVAL(float,value); break;
		case DQ_DOUBLE: SETVAL(double,value); break;
		default:
			printf( "Quantifier type (%d) was not handled\n", quantifier );
	}
}

/**
 * Returns the double value
 */
double ValueLocation::doubleValue() {
	CAST(double);
}

/**
 * Sets the double value
 */
void ValueLocation::setDoubleValue( double value ) {
	switch( quantifier ) {
		case DQ_BYTE:	SETVAL(char,value); break;
		case DQ_SHORT:	SETVAL(short,value); break;
		case DQ_INT:	SETVAL(int,value); break;
		case DQ_LONG:	SETVAL(long,value); break;
		case DQ_FLOAT:	SETVAL(float,value); break;
		case DQ_DOUBLE: SETVAL(double,value); break;
		default:
			printf( "Quantifier type (%d) was not handled\n", quantifier );
	}
}
