/*
 * ByteBuffer.h
 *
 *  Created on: Mar 17, 2010
 *      Author: ldaniels
 */

#ifndef BYTEBUFFER_H_
#define BYTEBUFFER_H_

#define BYTE_SIZE	1
#define SHORT_SIZE	2
#define INT_SIZE	4
#define LONG_SIZE	8

#include "Common.h"

/**
 * Represents a byte buffer
 */
class ByteBuffer {
protected:
	char *buffer;
	OFFSET_T growth;
	OFFSET_T capacity;
	OFFSET_T position;
	OFFSET_T length;

public:

	/**
	 * Constructor
	 */
	ByteBuffer( int initialSize, int growth );

	/**
	 * Constructor
	 */
	ByteBuffer( char* buffer, int bufferSize, int growth );

	/**
	 * Destructor
	 */
	virtual ~ByteBuffer();

	/**
	 * Returns the offset pointer for a given memory address
	 */
	void* operator [] ( OFFSET_T );

	/**
	 * Indicates whether @a count bytes can be read
	 * from the buffer without exceeding the length of bytes
	 * @param count the number of bytes to read
	 */
	bool canRead( int count );

	/**
	 * Returns the underlying memory buffer
	 */
	char* getBuffer();

	/**
	 * Returns the current capacity of the buffer
	 */
	OFFSET_T getCapacity();

	/**
	 * Retrieves a character from the current position within buffer
	 */
	char getChar();

	/**
	 * Retrieves a character array from the current position within buffer
	 */
	char* getChars( int size );

	/**
	 * Retrieves a double from the current position within buffer
	 */
	double getDouble();

	/**
	 * Retrieves a double from the given offset within buffer
	 */
	double getDouble( OFFSET_T offset );

	/**
	 * Retrieves an integer from the current position within buffer
	 */
	int getInt();

	/**
	 * Retrieves a reference to an integer from the given position within buffer
	 */
	int getInt( OFFSET_T offset );

	/**
	 * Retrieves a reference to an integer from the given offset within buffer
	 */
	int* getIntPtr( OFFSET_T offset );

	/**
	 * Retrieves a long integer from the current position within buffer
	 */
	long getLong();

	/**
	 * Retrieves a long integer from the current position within buffer
	 */
	long getLong( OFFSET_T offset );

	/**
	 * Retrieves a pointer to the long integer from the given offset within buffer
	 */
	long* getLongPtr( OFFSET_T offset );

	/**
	 * Retrieves an offset from the current position within buffer
	 */
	OFFSET_T getOffset();

	/**
	 * Retrieves a short integer from the current position within buffer
	 */
	short getShort();

	/**
	 * Retrieves a reference to a short integer from the given offset within buffer
	 */
	short getShort( OFFSET_T offset );

	/**
	 * Retrieves a string from the current position within buffer
	 */
	char* getString();

	/**
	 * Retrieves a string from the given offset within buffer
	 */
	char* getString( OFFSET_T position );

	/**
	 * Returns the current length of the buffer
	 */
	long getLength();

	/**
	 * Moves the cursor forward (+) or backward (-) by the given delta
	 */
	OFFSET_T move( OFFSET_T delta );

	/**
	 * Moves the point to the beginning of the stream
	 */
	void top();

	/**
	 * Moves the point to the end of the stream
	 */
	void bottom();

	/**
	 * Imports the given code buffer
	 * codeBuffer - the given code buffer
	 */
	void putBuffer( ByteBuffer );

	/**
	 * Adds a character (1 byte) value
	 * value - a character (1 byte) value
	 */
	void putChar( char value );

	/**
	 * Adds a character (1 byte) value
	 * value - a character (1 byte) value
	 */
	void putChar( OFFSET_T offset, char value );

	/**
	 * Adds a string of characters
	 * s - a string of characters
	 */
	void putChars( const char *s, int size );

	/**
	 * Adds a double value
	 * @param value a double value
	 */
	void putDouble( double value );

	/**
	 * Adds an double (8 byte) value
	 * value - an double (8 byte) value
	 */
	void putDouble( OFFSET_T offset, double value );

	/**
	 * Adds an integer (4 byte) value
	 * value - an integer (4 byte) value
	 */
	void putInt( int value );

	/**
	 * Adds an integer (4 byte) value
	 * value - an integer (4 byte) value
	 */
	void putInt( OFFSET_T offset, int value );

	/**
	 * Adds a long integer (8 byte) value
	 * value - a long integer (8 byte) value
	 */
	void putLong( long value );

	/**
	 * Adds a long integer (8 byte) value
	 * value - a long integer (8 byte) value
	 */
	void putLong( OFFSET_T offset, long value );

	/**
	 * Adds a short integer (4 byte) value
	 * value - a short integer (4 byte) value
	 */
	void putShort( short value );

	/**
	 * Adds a short integer (4 byte) value
	 * value - a short integer (4 byte) value
	 */
	void putShort( OFFSET_T offset, short value );

	/**
	 * Adds a string of characters
	 * s - a string of characters
	 */
	void putString( const char *s );

	/**
	 * Resets the buffer for re-use
	 */
	void reset();

	/**
	 * Returns the current cursor position
	 */
	OFFSET_T getPosition();

	/**
	 * Sets the new position within the buffer
	 * @param position the new position
	 */
	void setPosition( OFFSET_T position );

protected:

	/**
	 * Ensures the the buffer is large enough to
	 * accept the required number of bytes with
	 * overflow.
	 * required - the required number of bytes
	 */
	void ensureCapacity( int required );

	/**
	 * Ensures the the buffer is large enough to
	 * accept the required number of bytes with
	 * overflow.
	 * required - the required number of bytes
	 */
	void ensurePosition( OFFSET_T position );

	/**
	 * Expands the the buffer by the given delta
	 */
	void expand( int delta );

	/**
	 * Updates the length and capacity
	 * based on the current position
	 */
	void update();

};

#endif /* BYTEBUFFER_H_ */
