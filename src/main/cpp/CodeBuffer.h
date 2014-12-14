/*
 * CodeBuffer.h
 *
 *  Created on: March 3, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef CODEBUFFER_H_
#define CODEBUFFER_H_

#include "ByteBuffer.h"

/**
 * Code Buffer
 */
class CodeBuffer : public ByteBuffer {
public:

	/**
	 * Constructor
	 */
	CodeBuffer( int initialSize, int growth );

	/**
	 * Constructor
	 */
	CodeBuffer( char* buffer, int bufferSize, int growth );

	/**
	 * Destruction
	 */
	virtual ~CodeBuffer();

	/**
	 * Allocates a block of memory
	 * @param size the size of the desired memory block
	 */
	OFFSET_T allocate( OFFSET_T size );

	/**
	 * Displays the contents of the buffer at
	 * the current positions
	 */
	void dump( int count );

	/**
	 * Retrieves the next instruction from the current position within buffer
	 */
	INSTRUCTION_T getInstruction( OFFSET_T );

	/**
	 * Retrieves a reference to the instruction from the given offset within buffer
	 */
	INSTRUCTION_T* getInstructionPtr( OFFSET_T );

	/**
	 * Adds an instruction to the underlying buffer
	 * @param instruction the given instruction
	 */
	 OFFSET_T putInstruction( INSTRUCTION_T instruction );

};

#endif /* CODEBUFFER_H_ */
