/*
 * ByteBuffer.cpp
 *
 *  Created on: March 17, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include <stdio.h>
#include <string.h>

#include "ByteBuffer.h"

/**
 * Constructor
 */
ByteBuffer::ByteBuffer( int initialSize, int growth ) {
	this->buffer 	= new char[ initialSize ];
	this->capacity	= initialSize;
	this->growth	= growth;
	this->position	= 0;
	this->length	= 0;
}

/**
 * Constructor
 */
ByteBuffer::ByteBuffer( char* buffer, int bufferSize, int growth ) {
	this->buffer 	= buffer;
	this->capacity	= bufferSize;
	this->growth	= growth;
	this->position	= 0;
	this->length	= 0;
}

/**
 * Destruction
 */
ByteBuffer::~ByteBuffer() {
	delete buffer;
}

/**
 * Returns the offset pointer for a given memory address
 */
void* ByteBuffer::operator [] ( OFFSET_T offset ) {
	return (void*)&buffer[ offset ];
}


/**
 * Indicates whether @a count bytes can be read
 * from the buffer without exceeding the length of bytes
 * @param count the number of bytes to read
 */
bool ByteBuffer::canRead( int count ) {
	return position + count < length;
}

/**
 * Returns the underlying memory buffer
 */
char* ByteBuffer::getBuffer() {
	return buffer;
}

/**
 * Returns the current capacity of the buffer
 */
OFFSET_T ByteBuffer::getCapacity() {
	return capacity;
}

/**
 * Retrieves a character from the current position within buffer
 */
char ByteBuffer::getChar() {
	if( position + 1 > length ) {
		return 0;
	}

	// return the character
	return buffer[position++];
}

/**
 * Retrieves a string from the current position within buffer
 */
char* ByteBuffer::getChars( int size ) {
	if( position + size > length ) {
		printf( "Not enough bytes left in stream (position %ld, %d required, %ld left)\n", position, size, length );
		return NULL;
	}

	// create a string large enough to hold the data
	char *s = new char[size];

	// point to the current position of the buffer
	const char *p = &buffer[position];

	// copy the contents
	memcpy( s, p, size );

	// adjust the position
	position += size;

	// return the character
	return s;
}

/**
 * Retrieves a double from the current position within buffer
 */
double ByteBuffer::getDouble() {
	// is there's sufficient capacity?
	if( position + LONG_SIZE > length ) {
		return 0;
	}

	// create a double pointer to current position
	double *p = (double*)&buffer[position];

	// get four bytes from the stream
	position += LONG_SIZE;

	// return the integer
	return *p;
}

/**
 * Retrieves a double from the given offset within buffer
 */
double ByteBuffer::getDouble( OFFSET_T offset ) {
	// is there's sufficient capacity?
	if( offset + LONG_SIZE > length ) {
		return NULL;
	}

	// create an integer pointer to current position
	double *p = (double*)&buffer[offset];

	// return the value
	return *p;
}

/**
 * Retrieves an integer from the current position within buffer
 */
int ByteBuffer::getInt() {
	// is there's sufficient capacity?
	if( position + INT_SIZE > length ) {
		return 0;
	}

	// create an integer pointer to current position
	int *p = (int*)&buffer[position];

	// get four bytes from the stream
	position += INT_SIZE;

	// return the integer
	return *p;
}

/**
 * Retrieves a reference to an integer from the given offset within buffer
 */
int ByteBuffer::getInt( OFFSET_T offset ) {
	// is there's sufficient capacity?
	if( offset + INT_SIZE > length ) {
		return NULL;
	}

	// create an integer pointer to current position
	int *p = (int*)&buffer[offset];

	// return the value
	return *p;
}

/**
 * Retrieves a reference to an integer from the given offset within buffer
 */
int* ByteBuffer::getIntPtr( OFFSET_T offset ) {
	// is there's sufficient capacity?
	if( offset + INT_SIZE > length ) {
		printf( "ByteBuffer::getIntPtr - attempt to access data beyond the limit (p = %05lX, limit=%05lX)\n", offset + INT_SIZE, length );
		return NULL;
	}

	// create an integer pointer to current position
	int *p = (int*)&buffer[offset];

	// return the value
	return p;
}

/**
 * Returns the current length of the buffer
 */
long ByteBuffer::getLength() {
	return length;
}

/**
 * Retrieves a long integer from the current position within buffer
 */
long ByteBuffer::getLong() {
	if( position + LONG_SIZE > length ) {
		return 0;
	}

	// create the pointer
	long *p = (long*)&buffer[position];

	// advance the cursor
	position += LONG_SIZE;

	// return the long
	return *p;
}

/**
 * Retrieves a long integer from the given offset within buffer
 */
long ByteBuffer::getLong( OFFSET_T offset ) {
	if( offset + LONG_SIZE > length ) {
		return NULL;
	}

	// create the pointer
	long *p = (long*)&buffer[offset];

	// return the value
	return *p;
}

/**
 * Retrieves a pointer to the long integer from the given offset within buffer
 */
long* ByteBuffer::getLongPtr( OFFSET_T offset ) {
	if( offset + LONG_SIZE > length ) {
		return NULL;
	}

	// create the pointer
	long *p = (long*)&buffer[offset];

	// return the value
	return p;
}

/**
 * Retrieves an offset from the current position within buffer
 */
OFFSET_T ByteBuffer::getOffset() {
	// get the size of the offset type
	int size_t = sizeof(OFFSET_T);

	// are we beyond the limit?
	if( position + size_t > length ) {
		printf( "ByteBuffer::getOffset - attempt to access data beyond the limit (p = %05lX, limit=%05lX)\n", position + size_t, length );
		return 0;
	}

	// create the pointer
	OFFSET_T *p = (OFFSET_T*)&buffer[position];

	// advance the cursor
	position += size_t;

	// return the offset
	return *p;
}

/**
 * Retrieves a short integer from the current position within buffer
 */
short ByteBuffer::getShort() {
	if( position + SHORT_SIZE > length ) {
		return 0;
	}

	// create the pointer
	short *p = (short*)&buffer[position];

	// advance the cursor
	position += SHORT_SIZE;

	// return the integer
	return *p;
}

/**
 * Retrieves a reference to a short integer from the given offset within buffer
 */
short ByteBuffer::getShort( OFFSET_T offset ) {
	if( offset + SHORT_SIZE > length ) {
		return NULL;
	}

	// create the pointer
	short *p = (short*)&buffer[offset];

	// return the value
	return *p;
}

/**
 * Retrieves a string from the current position within buffer
 */
char* ByteBuffer::getString() {
	int size = getShort();
	if( size < 0 ) {
		return NULL;
	}
	return getChars( size );
}

/**
 * Retrieves a string from the current position within buffer
 */
char* ByteBuffer::getString( OFFSET_T offset ) {
	// get a pointer to the string
	return &buffer[offset];
}

/**
 * Moves the cursor forward (+) or backward (-) by the given delta
 */
OFFSET_T ByteBuffer::move( OFFSET_T delta ) {
	// adjust the position
	position += delta;

	// can't be before the start
	if( position < 0 ) position = 0;

	// protect the internal integrity
	update();

	// return the new position
	return position;
}

/**
 * Moves the point to the beginning of the stream
 */
void ByteBuffer::top() {
	position = 0;
}

/**
 * Moves the point to the end of the stream
 */
void ByteBuffer::bottom() {
	position = length;
}

/**
 * Imports the given code buffer
 * codeBuffer - the given code buffer
 */
void ByteBuffer::putBuffer( ByteBuffer buf ) {
	// make sure the buffer is large enough
	ensureCapacity( buf.length );

	// point to the end of the buffer
	char *p = &buffer[length];

	// copy the contents
	memcpy( p, buf.buffer, buf.length );

	// adjust the position
	position += buf.length;

	// protect the internal integrity
	update();
}

/**
 * Adds a character (1 byte) value
 * value - a character (1 byte) value
 */
void ByteBuffer::putChar( char value ) {
	// make sure the buffer is large enough
	ensureCapacity( 1 );

	// add the 4 bytes
	buffer[position++] = value;

	// protect the internal integrity
	update();
}

/**
 * Adds a character (1 byte) value
 * value - a character (1 byte) value
 */
void ByteBuffer::putChar( OFFSET_T position, char value ) {
	// make sure the buffer is large enough
	ensurePosition( position );

	// add the 4 bytes
	buffer[position] = value;
}

/**
 * Adds a string of characters
 * s - a string of characters
 */
void ByteBuffer::putChars( const char *s, int size ) {
	// make sure the buffer is large enough
	ensureCapacity( size );

	// point to the end of the buffer
	char *p = &buffer[length];

	// copy the contents
	memcpy( p, s, size );

	// adjust the position
	position += size;

	// protect the internal integrity
	update();
}

/**
 * Adds a double value
 * @param value a double value
 */
void ByteBuffer::putDouble( double value ) {
	// make sure the buffer is large enough
	ensurePosition( position + LONG_SIZE );

	// create an integer pointer to current position
	int *p = (int*)&buffer[position];

	// set the value
	*p = value;

	// advance the pointer
	position += LONG_SIZE;

	// protect the internal integrity
	update();
}

/**
 * Adds an double (8 byte) value
 * value - an double (8 byte) value
 */
void ByteBuffer::putDouble( OFFSET_T offset, double value ) {
	// make sure the buffer is large enough
	ensurePosition( offset + LONG_SIZE );

	// create an integer pointer to current position
	double *p = (double*)&buffer[offset];

	// set the value
	*p = value;
}

/**
 * Adds an integer (4 byte) value
 * value - an integer (4 byte) value
 */
void ByteBuffer::putInt( int value ) {
	// make sure the buffer is large enough
	ensurePosition( position + INT_SIZE );

	// create an integer pointer to current position
	int *p = (int*)&buffer[position];

	// set the value
	*p = value;

	// advance the pointer
	position += INT_SIZE;

	// protect the internal integrity
	update();
}

/**
 * Adds an integer (4 byte) value
 * value - an integer (4 byte) value
 */
void ByteBuffer::putInt( OFFSET_T offset, int value ) {
	// make sure the buffer is large enough
	ensurePosition( offset + INT_SIZE );

	// create an integer pointer to current position
	int *p = (int*)&buffer[offset];

	// set the value
	*p = value;
}

/**
 * Adds a short integer (4 byte) value
 * value - a short integer (4 byte) value
 */
void ByteBuffer::putShort( short value ) {
	// make sure the buffer is large enough
	ensurePosition( position + SHORT_SIZE );

	// create the pointer
	short *p = (short*)&buffer[position];

	// set the value
	*p = value;

	// advance the cursor
	position += SHORT_SIZE;

	// protect the internal integrity
	update();
}

/**
 * Adds a short integer value
 * value - a short integer value
 */
void ByteBuffer::putShort( OFFSET_T offset, short value ) {
	// make sure the buffer is large enough
	ensurePosition( offset + SHORT_SIZE );

	// create the pointer
	short *p = (short*)&buffer[offset];

	// set the value
	*p = value;
}

/**
 * Adds a string of characters
 * s - a string of characters
 */
void ByteBuffer::putString( const char *s ) {
	putShort( strlen(s ) + 1 );

	// get the string length
	int size = strlen( s ) + 1;

	// make sure the buffer is large enough
	ensureCapacity( size );

	// point to the end of the buffer
	char *p = &buffer[length];

	// copy the contents
	memcpy( p, s, size );

	// adjust the position
	position += size;

	// protect the internal integrity
	update();
}

/**
 * Adds a long integer (8 byte) value
 * value - a long integer (8 byte) value
 */
void ByteBuffer::putLong( long value ) {
	// make sure the buffer is large enough
	ensurePosition( position + LONG_SIZE );

	// add the 8 bytes
	long *p = (long*)&buffer[position];

	// set the value
	*p = value;

	// advance the cursor
	position += LONG_SIZE;

	// protect the internal integrity
	update();
}

/**
 * Adds a long integer (8 byte) value
 * value - a long integer (8 byte) value
 */
void ByteBuffer::putLong( OFFSET_T position, long value ) {
	// make sure the buffer is large enough
	ensurePosition( position + LONG_SIZE );

	// add the 8 bytes
	long *p = (long*)&buffer[position];

	// set the value
	*p = value;
}

/**
 * Resets the buffer for re-use
 */
void ByteBuffer::reset() {
	this->position = 0;
	this->length = 0;
}

/**
 * Returns the current cursor position
 */
OFFSET_T ByteBuffer::getPosition() {
	return position;
}

/**
 * Sets the new position within the buffer
 * @param position the new position
 */
void ByteBuffer::setPosition( OFFSET_T position ) {
	this->position = position;
}

/**
 * Ensures the the buffer is large enough to
 * accept the required number of bytes with
 * overflow.
 * required - the required number of bytes
 */
void ByteBuffer::ensureCapacity( int required ) {
	// is there enough capacity to accept the required size?
	if( length + required >= capacity ) {
		// expand the buffer
		expand( required + growth );
	}
}

/**
 * Ensures the the buffer is large enough to
 * accept the required number of bytes with
 * overflow.
 * required - the required number of bytes
 */
void ByteBuffer::ensurePosition( OFFSET_T position ) {
	// is there enough capacity to accept the required size?
	if( position >= capacity ) {
		// expand the buffer
		expand( capacity + growth );
	}
}

/**
 * Expands the the buffer by the given delta
 */
void ByteBuffer::expand( int delta ) {
	// define the new capacity
	int newCapacity = capacity + delta;

	// create the new memory buffer
	char *newBuffer = new char[ newCapacity ];

	// copy the data from the old buffer to the new
	memcpy( newBuffer, buffer, length );

	// free the old buffer, and point to the new one
	//delete buffer;
	this->buffer = newBuffer;
	this->capacity = newCapacity;
}


/**
 * Updates the length and capacity
 * based on the current position
 */
void ByteBuffer::update() {
	// if the position is beyond the length
	if( position >= length ) {
		length = position;
	}

	// if the length is beyond the capacity
	if( length > capacity ) {
		ensureCapacity( growth );
	}
}
