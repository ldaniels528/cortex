/*
 * TypedData.h
 *
 *  Created on: March 20, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef TYPEDDATA_H_
#define TYPEDDATA_H_

#include "../CodeBuffer.h"
#include "../Common.h"

#include "TypeReference.h"

/**
 * Represents a unit of typed data
 */
class TypedData {
private:
	TypeReference* type;
	int referenceId;
	OFFSET_T length;
	char* data;

public:

	/**
	 * Constructor
	 */
	TypedData( int referenceId, TypeReference* type, char* data, OFFSET_T length );

	/**
	 * Destruction
	 */
	virtual ~TypedData();

	/**
	 * Compiles the typed data into byte code
	 */
	int compile( CodeBuffer* );

	/**
	 * Decompiles the typed data from the byte code
	 */
	static TypedData* decompile( CodeBuffer* );

	/**
	 * Returns the binary representation of the data
	 */
	char* getData();

	/**
	 * Returns the length of the data
	 */
	OFFSET_T getLength();

	/**
	 * Returns the type of the typed data
	 */
	TypeReference* getType();

};

#endif /* TYPEDDATA_H_ */
