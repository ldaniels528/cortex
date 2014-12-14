/*
 * Flags.cpp
 *
 *  Created on: March 11, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include <stdio.h>

#include "RegisterFlags.h"

/**
 * Constructor
 */
RegisterFlags::RegisterFlags() : ValueLocation( DQ_INT ) {
	this->buf = new char[60];
	setDirection( true );
	setZero( true );
}

/**
 * Destruction
 */
RegisterFlags::~RegisterFlags() {
	// do nothing
}

/**
 * Returns a pointer to the value
 */
void* RegisterFlags::getPointer() {
	return &value;
}

/**
 * Indicates whether the carry flag is set
 */
bool RegisterFlags::isCarry() {
	return isSet( FLAG_CF );
}

/**
 * Sets the carry flag
 */
void RegisterFlags::setCarry( bool state ) {
	setBit( FLAG_CF, state );
}

/**
 * Indicates whether the direction flag is set to increasing
 */
bool RegisterFlags::isIncreasing() {
	return isSet( FLAG_DF );
}

/**
 * Sets the direction flag
 */
void RegisterFlags::setDirection( bool state ) {
	setBit( FLAG_DF, state );
}

/**
 * Indicates whether the divide by zero flag is set
 */
bool RegisterFlags::isDivideByZero() {
	return isSet( FLAG_XDF );
}

/**
 * Sets the divide by zero flag
 */
void RegisterFlags::setDivideByZero( bool state ) {
	setBit( FLAG_XDF, state );
}

/**
 * Indicates whether the exception flag is set
 */
bool RegisterFlags::isException() {
	return isSet( FLAG_XF );
}

/**
 * Sets the exception flag
 */
void RegisterFlags::setException( bool state ) {
	setBit( FLAG_XF, state );
}

/**
 * Indicates whether the found flag is set
 */
bool RegisterFlags::isFound() {
	return isSet( FLAG_FF );
}

/**
 * Sets the found flag
 */
void RegisterFlags::setFound( bool state ) {
	setBit( FLAG_FF, state );
}

/**
 * Indicates whether the greater flag is set
 */
bool RegisterFlags::isGreater() {
	return isSet( FLAG_GF );
}

/**
 * Sets the greater flag
 */
void RegisterFlags::setGreater( bool state ) {
	setBit( FLAG_GF, state );
}

/**
 * Indicates whether the less flag is set
 */
bool RegisterFlags::isLess() {
	return isSet( FLAG_LF );
}

/**
 * Sets the less flag
 */
void RegisterFlags::setLess( bool state ) {
	setBit( FLAG_LF, state );
}

/**
 * Indicates whether the overflow flag is set
 */
bool RegisterFlags::isOverflow() {
	return isSet( FLAG_OF );
}

/**
 * Sets the overflow flag
 */
void RegisterFlags::setOverflow( bool state ) {
	setBit( FLAG_OF, state );
}

/**
 * Indicates whether the parity flag is set
 */
bool RegisterFlags::isParity() {
	return isSet( FLAG_PF );
}

/**
 * Sets the parity flag
 */
void RegisterFlags::setParity( bool state ) {
	setBit( FLAG_PF, state );
}

/**
 * Indicates whether the signed flag is set
 */
bool RegisterFlags::isSigned() {
	return isSet( FLAG_SF );
}

/**
 * Sets the signed flag
 */
void RegisterFlags::setSigned( bool state ) {
	setBit( FLAG_SF, state );
}

/**
 * Returns the value of the flags
 */
REG_SIZE64 RegisterFlags::getValue() {
	return value;
}

/**
 * Sets the value of the flags
 */
void RegisterFlags::setValue( REG_SIZE64 value ) {
	this->value = value;
}

/**
 * Indicates whether the parity flag is set
 */
bool RegisterFlags::isZero() {
	return isSet( FLAG_ZF );
}

/**
 * Sets the parity flag
 */
void RegisterFlags::setZero( bool state ) {
	setBit( FLAG_ZF, state );
}

/**
 * Returns the string representation of the current
 * status of the flags
 */
const char* RegisterFlags::toString() {
	sprintf( buf, "[ %s %s %s %s %s %s %s %s | %s %s %s ]",
			isCarry() 			? "C "  : "NC",
			isFound() 			? "F "  : "NF",
			isGreater() 		? "G "  : "NG",
			isLess()			? "L "  : "NL",
			isOverflow() 		? "O "  : "NO",
			isParity() 			? "PE"  : "PO",
			isSigned() 			? "S "  : "NS",
			isZero() 			? "Z "  : "NZ",
			isIncreasing()		? "INC" : "DEC",
			isException()	 	? "X "  : "  ",
			isDivideByZero()	? "XDZ" : "   " );
	return buf;
}

/**
 * Indicates whether the Nth bit is set
 */
bool RegisterFlags::RegisterFlags::isSet( int nth ) {
	return value & ( 1 << nth );
}

/**
 * Sets the Nth bit
 */
void RegisterFlags::setBit( int nth, bool state ) {
	if( state ) {
		value |= ( 1 << nth );
	}
	else {
		value &= ( 0xFFFFFFFFFFFFFFFF - ( 1 << nth ) );
	}
}
