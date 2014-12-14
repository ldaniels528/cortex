/*
 * Flags.h
 *
 *  Created on: March 11, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef FLAGS_H_
#define FLAGS_H_

#include "../OpCodes.h"

#include "ValueLocation.h"

/**
 * Represents the flags register
 */
class RegisterFlags : public ValueLocation {
private:
	REG_SIZE32 value;
	char* buf;

public:

	/**
	 * Constructor
	 */
	RegisterFlags();

	/**
	 * Destruction
	 */
	virtual ~RegisterFlags();

	/**
	 * Returns a pointer to the value
	 */
	void* getPointer();

	/**
	 * Indicates whether the carry flag is set
	 */
	bool isCarry();

	/**
	 * Sets the carry flag
	 */
	void setCarry( bool state );

	/**
	 * Indicates whether the direction flag is set
	 * (moves forward if set)
	 */
	bool isIncreasing();

	/**
	 * Sets the direction flag
	 */
	void setDirection( bool state );

	/**
	 * Indicates whether the divide by zero flag is set
	 */
	bool isDivideByZero();

	/**
	 * Sets the divide by zero flag
	 */
	void setDivideByZero( bool state );

	/**
	 * Indicates whether the exception flag is set
	 */
	bool isException();

	/**
	 * Sets the exception flag
	 */
	void setException( bool state );

	/**
	 * Indicates whether the found flag is set
	 */
	bool isFound();

	/**
	 * Sets the found flag
	 */
	void setFound( bool state );

	/**
	 * Indicates whether the greater flag is set
	 */
	bool isGreater();

	/**
	 * Sets the greater flag
	 */
	void setGreater( bool state );

	/**
	 * Indicates whether the less flag is set
	 */
	bool isLess();

	/**
	 * Sets the less flag
	 */
	void setLess( bool state );

	/**
	 * Indicates whether the overflow flag is set
	 */
	bool isOverflow();

	/**
	 * Sets the overflow flag
	 */
	void setOverflow( bool state );

	/**
	 * Indicates whether the parity flag is set
	 */
	bool isParity();

	/**
	 * Sets the parity flag
	 */
	void setParity( bool state );

	/**
	 * Indicates whether the signed flag is set
	 */
	bool isSigned();

	/**
	 * Sets the signed flag
	 */
	void setSigned( bool state );

	/**
	 * Returns the value of the flags
	 */
	REG_SIZE64 getValue();

	/**
	 * Sets the value of the flags
	 */
	void setValue( REG_SIZE64 value );

	/**
	 * Indicates whether the parity flag is set
	 */
	bool isZero();

	/**
	 * Sets the parity flag
	 */
	void setZero( bool state );

	/**
	 * Returns the string representation of the current
	 * status of the flags
	 */
	const char* toString();

protected:

	/**
	 * Indicates whether the Nth bit is set
	 */
	bool isSet( int nth );

	/**
	 * Sets the Nth bit
	 */
	void setBit( int nth, bool state );

};


#endif /* FLAGS_H_ */
