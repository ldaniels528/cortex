/*
 * ValueLocation.h
 *
 *  Created on: March 11, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef VALUELOCATION_H_
#define VALUELOCATION_H_

#include "../Common.h"
#include "../OpCodes.h"

#define CAST(type) \
	void *pointer = getPointer(); \
	switch( quantifier ) { \
		case DQ_BYTE:	return (type)*((char*)pointer); \
		case DQ_SHORT:	return (type)*((short*)pointer); \
		case DQ_INT:	return (type)*((int*)pointer); \
		case DQ_LONG:	return (type)*((long*)pointer); \
		case DQ_FLOAT:	return (type)*((float*)pointer); \
		case DQ_DOUBLE:	return (type)*((double*)pointer); \
		default: \
			printf( "Quantifier type (%d) was not handled\n", quantifier ); \
			return -1; \
	}

#define SETVAL(type,value) \
{ \
	type *p = (type*)getPointer(); \
	*p = (type)value; \
}

/**
 * Represents a value location; a register, memory location, etc.
 */
class ValueLocation {
protected:
	int quantifier;

public:

	/**
	 * Constructor
	 */
	ValueLocation( int quantifier );

	/**
	 * Destructor
	 */
	~ValueLocation();

	/**
	 * Returns the size of the value (quantifier)
	 */
	int getQuantifier();

	/**
	 * Returns a pointer to the value
	 */
	virtual void* getPointer() = 0;

	/**
	 * Returns the character/byte value
	 */
	virtual char charValue();

	/**
	 * Sets the character value
	 */
	void setCharValue( char );

	/**
	 * Returns the short value
	 */
	virtual short shortValue();

	/**
	 * Sets the short value
	 */
	void setShortValue( short );

	/**
	 * Returns the integer value
	 */
	virtual int intValue();

	/**
	 * Sets the integer value
	 */
	void setIntValue( int );

	/**
	 * Returns the long value
	 */
	virtual long longValue();

	/**
	 * Sets the long integer value
	 */
	void setLongValue( long );

	/**
	 * Returns the float value
	 */
	virtual float floatValue();

	/**
	 * Sets the float value
	 */
	void setFloatValue( float );

	/**
	 * Returns the double value
	 */
	virtual double doubleValue();

	/**
	 * Sets the double value
	 */
	void setDoubleValue( double );

};

#endif /* VALUELOCATION_H_ */
