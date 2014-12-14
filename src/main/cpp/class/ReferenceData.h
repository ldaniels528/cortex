/*
 * ReferenceData.h
 *
 *  Created on: March 10, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef CLASSDATA_H_
#define CLASSDATA_H_

#include "../CodeBuffer.h"

/**
 * Represents a unit of reference data, and is used to
 * refer to fields, methods, and classes by name.
 */
class ReferenceData {
private:
	OFFSET_T codeOffset;
	OFFSET_T dataOffset;
	const char* content;
	int referenceId;

public:

	/**
	 * Constructor
	 */
	ReferenceData( int referenceId, const char* content, OFFSET_T codeOffset = 0 );

	/**
	 * Destruction
	 */
	virtual ~ReferenceData();

	/**
	 * Adds the given data to the class
	 * @param data the class data
	 */
	void addData( ReferenceData* );

	/**
	 * Compiles the class into byte code
	 */
	int compile( CodeBuffer*, OFFSET_T );

	/**
	 * Decompiles the class from byte code
	 */
	static ReferenceData* decompile( CodeBuffer* );

	/**
	 * Returns the contents of the data
	 */
	const char* getContent();

	/**
	 * Returns the memory offset of the code reference
	 */
	OFFSET_T getCodeOffset();

	/**
	 * Sets the memory offset of the data
	 */
	void setCodeOffset( OFFSET_T );

	/**
	 * Returns the memory offset of the data
	 */
	OFFSET_T getDataOffset();

	/**
	 * Sets the memory offset of the data
	 */
	void setDataOffset( OFFSET_T );

	/**
	 * Returns the reference identifier
	 */
	int getReferenceId();

	/**
	 * Sets the reference identifier
	 */
	void setReferenceId( int );

};

#endif /* CLASSDATA_H_ */
