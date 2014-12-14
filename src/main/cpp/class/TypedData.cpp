/*
 * TypedData.cpp
 *
 *  Created on: March 20, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include <stdio.h>

#include "TypedData.h"

/**
 * Constructor
 */
TypedData::TypedData( int referenceId, TypeReference* type, char* data, OFFSET_T length ) {
	this->referenceId	= referenceId;
	this->type			= type;
	this->data 			= data;
	this->length		= length;
}

/**
 * Destruction
 */
TypedData::~TypedData() {
	delete data;
	delete type;
}

/**
 * Compiles the typed data into byte code
 */
int TypedData::compile( CodeBuffer* cb ) {
	cb->putInt( referenceId );
	if( type->compile( cb ) ) return -1;
	cb->putInt( length );
	cb->putChars( data, length );
	return 0;
}

/**
 * Decompiles the typed data from the byte code
 */
TypedData* TypedData::decompile( CodeBuffer* cb ) {
	TypeReference* type;
	int referenceId = cb->getInt();
	if( !( type = TypeReference::decompile( cb ) ) ) return NULL;
	int length	= cb->getInt();
	char* data	= cb->getChars( length );
	return new TypedData( referenceId, type, data, length );
}

/**
 * Returns the binary representation of the data
 */
char* TypedData::getData() {
	return data;
}

/**
 * Returns the length of the data
 */
OFFSET_T TypedData::getLength() {
	return length;
}

/**
 * Returns the type of the typed data
 */
TypeReference* TypedData::getType() {
	return type;
}
