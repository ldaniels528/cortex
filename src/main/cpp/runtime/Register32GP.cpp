/*
 * Register32.cpp
 *
 *  Created on: Mar 24, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "Register32GP.h"

/**
 * Constructor
 */
Register32GP::Register32GP( const char* name ): Register( name, DQ_INT ) {
	this->value = 0;
}

/**
 * Destruction
 */
Register32GP::~Register32GP() {
	// do nothing
}

/**
 * Enables cast to a value
 */
Register32GP::operator REG_SIZE64() {
	return value;
}

/**
 * Enables value assignment
 */
Register& Register32GP::operator = ( REG_SIZE64 value ) {
	this->value = value;
	return *this;
}

/**
 * Enables equality
 */
bool Register32GP::operator == ( REG_SIZE64 value ) {
	return this->value == value;
}

/**
 * Enables addition + assignment
 */
Register& Register32GP::operator += ( REG_SIZE64 value ) {
	this->value += value;
	return *this;
}

/**
 * Enables subtraction + assignment
 */
Register& Register32GP::operator -= ( REG_SIZE64 value ) {
	this->value -= value;
	return *this;
}

/**
 * Enables increment
 */
Register& Register32GP::operator ++ ( int value ) {
	value++;
	return *this;
}

/**
 * Enables decrement
 */
Register& Register32GP::operator -- ( int value ) {
	value--;
	return *this;
}

/**
 * Returns the 32-bit value of the register
 */
void* Register32GP::getPointer() {
	return (void*)&value;
}

/**
 * Returns the 32-bit value of the register
 */
REG_SIZE64 Register32GP::getValue() {
	return value;
}

/**
 * Sets the value of the register
 */
void Register32GP::setValue( REG_SIZE64 val ) {
	this->value = val;
}

/**
 * Returns the string representation of the register
 */
const char* Register32GP::toString() {
	sprintf( buf, "%s %08X", name, value );
	return buf;
}
