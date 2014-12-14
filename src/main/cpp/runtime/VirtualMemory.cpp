/*
 * VirtualMemory.cpp
 *
 *  Created on: March 11, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include <stdio.h>
#include <string.h>

#include "../OpCodes.h"
#include "VirtualMemory.h"

/**
 * Constructor
 */
VirtualMemory::VirtualMemory() : CodeBuffer( 65536, 1024 ) {
	// allocate the stacks
	this->datastack	= new char[16384];
	this->callstack	= new char[16384];

	// define the quantifiers
	QUANTIFIERS = new int[8];
	QUANTIFIERS[0] = BYTE_SIZE;
	QUANTIFIERS[1] = SHORT_SIZE;
	QUANTIFIERS[2] = INT_SIZE;
	QUANTIFIERS[3] = LONG_SIZE;
	QUANTIFIERS[4] = INT_SIZE;
	QUANTIFIERS[6] = LONG_SIZE;
}

/**
 * Destructor
 */
VirtualMemory::~VirtualMemory() {
	// do nothing
}

/**
 * Copies @count characters from the source position
 * to the destination position
 * @param src the source position
 * @param dest the destination position
 * @param count the number of units to copy
 **/
void VirtualMemory::copy( OFFSET_T src, OFFSET_T dest, OFFSET_T count, int quantifier ) {
	// get the type size
	int size_t = QUANTIFIERS[quantifier];

	// get a pointer to the source position
	void *p1 = &buffer[src];

	// get a pointer to the destination position
	void *p2 = &buffer[dest];

	// copy the data
	memccpy( p2, p1, count, size_t );
}

/**
 * Copies @count characters from the source position
 * to the destination position
 * @param src the source position
 * @param dest the destination position
 * @param count the number of units to copy
 */
void VirtualMemory::flip( OFFSET_T src, OFFSET_T count, int quantifier ) {
	// get the type size
	int size_t = QUANTIFIERS[quantifier];

	// compute the span
	// TODO add logic to verify that source + count is not outside
	// of the data length get a point to the data
	OFFSET_T span = src + count * size_t;

	// determine the length and midpoint of the data
	OFFSET_T mid = span / 2;

	// flip the bytes
	void *p1, *p2;
	for( OFFSET_T n = 0; n < mid; n++ ) {
		// compute the index
		int index = n * size_t;

		// set the pointer
		p1 = &buffer[src + index];
		p2 = &buffer[span - index];

		// swap the values
		switch( size_t ) {
			case BYTE_SIZE:	SWAP( char, ((char*)p1),  ((char*)p2) ); break;
			case SHORT_SIZE:SWAP( short, (short*)p1, (short*)p2 ); break;
			case INT_SIZE: 	SWAP( int, (int*)p1,   (int*)p2 ); break;
			case LONG_SIZE: SWAP( long, (long*)p1,  (long*)p2 ); break;
		}
	}
}

/**
 * Returns a pointer to the given memory offset
 */
MemoryReference* VirtualMemory::getPointer( OFFSET_T offset ) {
	// is the pointer beyond the extends of memory?
	if( offset + LONG_SIZE > length ) {
		return NULL;
	}

	// create the pointer
	void *p = &buffer[offset];

	// get the quantifier
	int quantifier = DQ_LONG; // TODO fix this!

	// return the pointer
	return new MemoryReference( p, quantifier );
}

/**
 * Returns the call stack
 */
char* VirtualMemory::getCallStack() {
	return callstack;
}

/**
 * Returns the data stack
 */
char* VirtualMemory::getDataStack() {
	return datastack;
}

/**
 * Scans the contents of memory between the
 * given start and end (start + count) positions
 * @param start the given start position
 * @param count the given number of elements to scan
 * @return if the value was found, a pointer to the value is returned
 */
SearchResult* VirtualMemory::scan( OFFSET_T start, OFFSET_T count, ValueLocation* value, int quantifier ) {
	// get the type size
	int size_t = QUANTIFIERS[quantifier];

	// search for the value
	bool found = false;
	void* p;
	OFFSET_T n = 0;
	for( n = 0; !found && ( n < count ); n++ ) {
		p = &buffer[start + n * size_t];
		switch( quantifier ) {
			case DQ_BYTE:	found = ( *((char*)p) == value->charValue() ); break;
			case DQ_SHORT:	found = ( *((short*)p) == value->shortValue() ); break;
			case DQ_INT:	found = ( *((int*)p) == value->intValue() ); break;
			case DQ_LONG: 	found = ( *((long*)p) == value->longValue() ); break;
			case DQ_FLOAT:	found = ( *((float*)p) == value->floatValue() ); break;
			case DQ_DOUBLE: found = ( *((double*)p) == value->doubleValue() ); break;
		}
	}

	// create the search result
	SearchResult* result = new SearchResult();
	result->found = found;
	result->position = n;
	return result;
}

/**
 * Sorts the contents of memory between the
 * given start and end positions
 * @param start the given start position
 * @param end the given end position
 * @param ascending indicates ascending (or conversely descending) order
 */
void VirtualMemory::sort( OFFSET_T start, OFFSET_T count, bool ascending, int quantifier ) {
	OFFSET_T end = start + count;
	switch( quantifier ) {
		case DQ_BYTE:	QSORT(char); break;
		case DQ_SHORT:	QSORT(short); break;
		case DQ_INT:	QSORT(int); break;
		case DQ_LONG:	QSORT(long); break;
		case DQ_FLOAT:	QSORT(float); break;
		case DQ_DOUBLE:	QSORT(double); break;
	}
}
