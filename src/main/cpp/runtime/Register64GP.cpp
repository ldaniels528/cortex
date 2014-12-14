/*
 * Register.cpp
 *
 *  Created on: March 11, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "Register64GP.h"

/**
 * Constructor
 */
Register64GP::Register64GP( const char* name ): Register( name, DQ_LONG ) {
	this->value	= 0;
}

/**
 * Destruction
 */
Register64GP::~Register64GP() {
	// do nothing
}

/**
 * Enables assignment
 */
Register& Register64GP::operator = ( REG_SIZE64 value ) {
	this->value = value;
	return *this;
}

/**
 * Enables equality
 */
bool Register64GP::operator == ( REG_SIZE64 value ) {
	return this->value == value;
}

/**
 * Enables addition + assignment
 */
Register& Register64GP::operator += ( REG_SIZE64 value ) {
	this->value += value;
	return *this;
}

/**
 * Enables subtraction + assignment
 */
Register& Register64GP::operator -= ( REG_SIZE64 value ) {
	this->value -= value;
	return *this;
}

/**
 * Enables increment
 */
Register& Register64GP::operator ++ ( int value ) {
	value++;
	return *this;
}

/**
 * Enables decrement
 */
Register& Register64GP::operator -- ( int value ) {
	value--;
	return *this;
}

/**
 * Returns a pointer to the data
 */
void* Register64GP::getPointer() {
	return (void*)&value;
}

/**
 * Returns the 64-bit value of the register
 */
REG_SIZE64 Register64GP::getValue() {
	return value;
}

/**
 * Sets the value of the register
 */
void Register64GP::setValue( REG_SIZE64 value ) {
	this->value = value;
}

/**
 * Returns the string representation of the register
 */
const char* Register64GP::toString() {
	sprintf( buf, "%s %016lX", name, value );
	return buf;
}
