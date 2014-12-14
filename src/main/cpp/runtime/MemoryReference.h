/*
 * MemoryReference.h
 *
 *  Created on: March 11, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef MEMORYLOCATION_H_
#define MEMORYLOCATION_H_

#include "ValueLocation.h"

/**
 * Represents a memory reference; a pointer to
 * a specific location in virtual memory.
 */
class MemoryReference : public ValueLocation {
private:
	void* pointer;

public:

	/**
	 * Constructor
	 */
	MemoryReference( void* pointer, int quantifier );

	/**
	 * Destructor
	 */
	virtual ~MemoryReference();

	/**
	 * Returns a pointer to the value
	 */
	void* getPointer();

	/**
	 * Returns the underlying value
	 */
	REG_SIZE64 getValue();

	/**
	 * Sets the underlying value
	 */
	void setValue( REG_SIZE64 value );
};

#endif /* MEMORYLOCATION_H_ */
