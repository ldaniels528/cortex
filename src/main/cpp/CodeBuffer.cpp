/*
 * CodeBuffer.cpp
 *
 *  Created on: March 3, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include <stdio.h>

#include "CodeBuffer.h"

/**
 * Constructor
 */
CodeBuffer::CodeBuffer( int initialSize, int growth ): ByteBuffer( initialSize, growth )  {
	// do nothing
}

/**
 * Constructor
 */
CodeBuffer::CodeBuffer( char* buffer, int bufferSize, int growth ): ByteBuffer( buffer, bufferSize, growth ) {
	// do nothing
}

/**
 * Destruction
 */
CodeBuffer::~CodeBuffer() {
	// do nothing
}

/**
 * Allocates a block of memory
 * @param size the size of the desired memory block
 */
OFFSET_T CodeBuffer::allocate( OFFSET_T size ) {
	// make sure enough memory exists
	ensurePosition( length + size );

	// mark the start of the allocated segment
	OFFSET_T start = length;

	// allocate the block
	length += size;

	// return an offset to the block
	return start;
}

/**
 * Displays the contents of the buffer at
 * the current positions
 */
void CodeBuffer::dump( int count ) {
	const int width  = 16;
	char *buf1 = new char[width*3+1];
	char *buf2 = new char[width+1];

	// zero the buffers
	*buf1 = '\0';
	*buf2 = '\0';

	// get a pointer to the buffer
	char* p = &buffer[position];
	OFFSET_T index = position;

	int n = 0;
	for( n = 1; n <= count; n++ ) {
		const char c = *p;
		sprintf( buf1, "%s %02X", buf1, c & 0xFF );
		sprintf( buf2, "%s%c", buf2, ( c >= 33 && c <= 126 ) ? c : '.' );
		p++;
		index++;

		if( n % width == 0 ) {
			printf( "[%08lX]%s %s\n", index - width, buf1, buf2 );
			*buf1 = '\0';
			*buf2 = '\0';
		}
	}

	// display the last row
	if( n % width > 0 ) {
		printf( "[%08lX]%-48s %-16s\n", index - ( ( n - 1 ) % width ), buf1, buf2 );
	}

	// delete the buffers
	delete buf1;
	delete buf2;
}

/**
 * Retrieves the next instruction from the current position within buffer
 */
INSTRUCTION_T CodeBuffer::getInstruction( OFFSET_T offset ) {
	// cache the instruction size
	int size_t = sizeof( INSTRUCTION_T );

	// is there's sufficient capacity?
	if( offset + size_t > length ) {
		return 0;
	}

	// create an integer pointer to current position
	INSTRUCTION_T *p = (INSTRUCTION_T*)&buffer[offset];

	// return the next instruction
	return *p;
}

/**
 * Retrieves a reference to the instruction from the given offset within buffer
 */
INSTRUCTION_T* CodeBuffer::getInstructionPtr( OFFSET_T offset ) {
	// cache the instruction size
	int size_t = sizeof( INSTRUCTION_T );

	// is there's sufficient capacity?
	if( offset + size_t > length ) {
		printf( "CodeBuffer::getInstruction - attempt to access data beyond the limit (p = %05lX, limit=%05lX)\n", offset + size_t, length );
		return NULL;
	}

	// create an integer pointer to current position
	INSTRUCTION_T *p = (INSTRUCTION_T*)&buffer[offset];

	// return the value
	return p;
}

/**
 * Adds an instruction to the underlying buffer
 * @param instruction the given instruction
 */
OFFSET_T CodeBuffer::putInstruction( INSTRUCTION_T instruction ) {
	// get the current offset
	OFFSET_T curofs = position;

	// get the size of the instruction
	int size_t = sizeof(INSTRUCTION_T);

	// make sure the buffer is large enough
	ensurePosition( position + size_t );

	// create a pointer to current offset
	INSTRUCTION_T *p = (INSTRUCTION_T*)&buffer[position];

	// write the instruction to memory
	*p = instruction;

	// advance the pointer
	position += size_t;

	// protect the internal integrity
	update();

	// return the last offset
	return curofs;
}
