/*
 * LabelReference.cpp
 *
 *  Created on: March 16, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include <stdio.h>

#include "LabelReference.h"

/**
 * Constructor
 */
LabelReference::LabelReference( const char* name, OFFSET_T offset ) : Label( name, offset ) {
	// do nothing
}

/**
 * Destruction
 */
LabelReference::~LabelReference() {
	// do nothing
}

/**
 * Updates the instruction that references the label
 */
void LabelReference::update( CodeBuffer* cb, OFFSET_T labelOffset ) {
	// lookup the instruction
	INSTRUCTION_T *p = cb->getInstructionPtr( offset );

	// update the instruction
	INSTRUCTION_T instruction = *p;
	INSTRUCTION_T before = *p;
	const short delta = ( labelOffset - offset );
	instruction |= ( delta & 0xFFFF );

	// update the instruction
	printf( "LabelReference::update - [%05lX] '%s': %08X => %08X (%d)\n", offset, name, before, instruction, delta );
	*p = instruction;
}
