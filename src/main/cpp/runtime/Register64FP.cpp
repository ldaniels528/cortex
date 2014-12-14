/*
 * Register64FP.cpp
 *
 *  Created on: Mar 26, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "Register64FP.h"

/**
 * Constructor
 */
Register64FP::Register64FP( const char* name ): Register( name, DQ_DOUBLE ) {
	this->value = 0;
}

/**
 * Destruction
 */
Register64FP::~Register64FP() {
	// do nothing
}

/**
 * Cast to double
 */
Register64FP::operator double() {
	return value;
}

/**
 * Enables value assignment
 */
Register& Register64FP::operator = ( REG_SIZE64 value ) {
	this->value = value;
	return *this;
}

/**
 * Enables equality
 */
bool Register64FP::operator == ( REG_SIZE64 value ) {
	return this->value == value;
}

/**
 * Enables addition + assignment
 */
Register& Register64FP::operator += ( REG_SIZE64 value ) {
	this->value += value;
	return *this;
}

/**
 * Enables subtraction + assignment
 */
Register& Register64FP::operator -= ( REG_SIZE64 value ) {
	this->value -= value;
	return *this;
}

/**
 * Enables increment
 */
Register& Register64FP::operator ++ ( int value ) {
	value++;
	return *this;
}

/**
 * Enables decrement
 */
Register& Register64FP::operator -- ( int value ) {
	value--;
	return *this;
}

/**
 * Returns a pointer to the data
 */
void* Register64FP::getPointer() {
	return (void*)&value;
}

/**
 * Returns the 64-bit value of the register
 */
REG_SIZE64 Register64FP::getValue() {
	return value;
}

/**
 * Sets the value of the register
 */
void Register64FP::setValue( REG_SIZE64 value ) {
	this->value = value;
}

/**
 * Returns the string representation of the register
 */
const char* Register64FP::toString() {
	sprintf( buf, "%s %016f", name, value );
	return buf;
}
