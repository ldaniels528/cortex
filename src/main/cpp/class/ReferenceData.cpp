/*
 * ReferenceData.cpp
 *
 *  Created on: March 10, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include <stdio.h>

#include "ReferenceData.h"

/**
 * Constructor
 */
ReferenceData::ReferenceData( int referenceId, const char* content, OFFSET_T codeOffset ) {
	this->referenceId	= referenceId;
	this->content 		= content;
	this->codeOffset	= codeOffset;
	this->dataOffset	= dataOffset;
}

/**
 * Destruction
 */
ReferenceData::~ReferenceData() {
	// do nothing
}

/**
 * Compiles the class file into byte code
 */
int ReferenceData::compile( CodeBuffer* cb, OFFSET_T codeEntryPoint ) {
	// compute the instruction offset
	OFFSET_T instructionOffset = codeEntryPoint + codeOffset;

	// write the reference data
	cb->putInt( referenceId );
	this->dataOffset = cb->getPosition();
	cb->putString( content );
	//printf( "ClassData::compile - Writing [%s] to data offset %04lX; code offset %04lX\n", content, dataOffset, instructionOffset );

	// rewrite the instruction
	INSTRUCTION_T *p = cb->getInstructionPtr( instructionOffset );
	//printf( "ClassData::compile - (before) %08X - (after) %08X\n", *p, *p | referenceId );
	*p |= referenceId;
	return 0;
}

/**
 * Decompiles the class from byte code
 */
ReferenceData* ReferenceData::decompile( CodeBuffer* cb ) {
	// retrieve the reference data information
	int referenceId		= cb->getInt();
	OFFSET_T offset		= cb->getPosition() + sizeof(short);
	const char* content = cb->getString();
	//printf( "ClassData::decompile - retrieved [%04lX] '%s'\n", offset, content );

	// create the reference data
	ReferenceData* data = new ReferenceData( referenceId, content );
	data->setDataOffset( offset );
	return data;
}

/**
 * Returns the contents of the data
 */
const char* ReferenceData::getContent() {
	return content;
}

/**
 * Returns the memory offset of the code reference
 */
OFFSET_T ReferenceData::getCodeOffset() {
	return codeOffset;
}

/**
 * Sets the memory offset of the data
 */
void ReferenceData::setCodeOffset( OFFSET_T offset ) {
	this->codeOffset = offset;
}

/**
 * Returns the memory offset
 */
OFFSET_T ReferenceData::getDataOffset() {
	return dataOffset;
}

/**
 * Sets the memory offset of the data
 */
void ReferenceData::setDataOffset( OFFSET_T offset ) {
	this->dataOffset = offset;
}

/**
 * Returns the reference identifier
 */
int ReferenceData::getReferenceId() {
	return referenceId;
}

/**
 * Sets the reference identifier
 */
void ReferenceData::setReferenceId( int referenceId ) {
	this->referenceId = referenceId;
}

