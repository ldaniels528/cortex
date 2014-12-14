/*
 * VirtualMemory.h
 *
 *  Created on: Mar 11, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef VIRTUALMEMORY_H_
#define VIRTUALMEMORY_H_

#include "../CodeBuffer.h"

#include "MemoryReference.h"

#define QSORT(T) \
		{	T* values = (T*)&buffer[start]; \
			for( int i = start; ( i < end ) && ( i < length ); i++ ) { \
				int j = i - 1; \
				while( ( j >= 0 ) && ( values[j] > values[i] ) ) { \
					SWAP(T, &values[j], &values[j+1] ); \
					j--; \
				} \
			} \
		}

/**
 * Represents the results of a search, generally performed
 * from the "scan" method
 */
struct SearchResult {
	OFFSET_T position;
	bool found;
};

/**
 * Represents virtual memory
 */
class VirtualMemory : public CodeBuffer {
private:
	int* QUANTIFIERS;
	char* callstack;
	char* datastack;

public:

	/**
	 * Constructor
	 */
	VirtualMemory();

	/**
	 * Destructor
	 */
	virtual ~VirtualMemory();

	/**
	 * Copies @count characters from the source position
	 * to the destination position
	 * @param src the source position
	 * @param dest the destination position
	 * @param count the number of units to copy
	 * @param quantifier the unit type
	 */
	void copy( OFFSET_T src, OFFSET_T dest, OFFSET_T count, int quantifier );

	/**
	 * Copies @count characters from the source position
	 * to the destination position
	 * @param src the source position
	 * @param count the number of units to copy
	 * @param quantifier the unit type
	 */
	void flip( OFFSET_T src, OFFSET_T count, int quantifier );

	/**
	 * Retrieves a pointer to the memory address at the given offset
	 * @param the given memory offset
	 */
	MemoryReference* getPointer( OFFSET_T offset );

	/**
	 * Returns the call stack
	 */
	char* getCallStack();

	/**
	 * Returns the data stack
	 */
	char* getDataStack();

	/**
	 * Scans the contents of memory between the
	 * given start and end (start + count) positions
	 * @param start the given start position
	 * @param count the given number of elements to scan
	 * @return if the value was found, a pointer to the value is returned
	 */
	SearchResult* scan( OFFSET_T start, OFFSET_T count, ValueLocation* value, int quantifier );

	/**
	 * Sorts the contents of memory between the
	 * given start and end positions
	 * @param start the given start position
	 * @param count the given number of elements to sort
	 */
	void sort( OFFSET_T start, OFFSET_T count, bool ascending, int quantifier );

};

#endif /* VIRTUALMEMORY_H_ */
