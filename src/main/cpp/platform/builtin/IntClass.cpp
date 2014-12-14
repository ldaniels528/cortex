/*
 * IntClass.cpp
 *
 *  Created on: March 7, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "../../OpCodes.h"

#include "IntClass.h"

/**
 * Constructor
 */
IntClass::IntClass() : Class( "int", "sera.lang" ) {
	this->datarefSeq = 0;
	addMember( absMethod() );
	addMember( maxMethod() );
	addMember( minMethod() );
	addMember( toStringMethod() );
}

/**
 * Destruction
 */
IntClass::~IntClass() {
	// do nothing
}

/**
 * Builds the code for the absolute value ("abs") method
 */
Method* IntClass::absMethod() {
	// create the method
	Method* method = new Method( this, "abs", "abs", typeref, PUBLIC | STATIC | FINAL );

	// define the byte code
	CodeBuffer* cb = method->getCodeBuffer();
	cb->putInstruction( MOV << A_OP | REF_REG_REG << A_REF | GPF << A_R1 );	// MOV RP,&value
	cb->putInstruction( RET << A_OP | REF_REG_REG << A_REF | AX << A_R1 );	// RET
	return method;
}

/**
 * Builds the code for the maximum value ("max") method
 */
Method* IntClass::maxMethod() {
	OFFSET_T offset;

	// create the method
	Method* method = new Method( this, "max", "max", typeref, PUBLIC | STATIC | FINAL );

	// define the maximum value
	int* max = new int();
	*max = MAX_VALUE;

	// define the byte code
	CodeBuffer* cb = method->getCodeBuffer();
	offset = cb->putInstruction( STAGE << A_OP | ARG_DIRECT << A_ARG );					// STAGE,&value
	cb->putInstruction( POP << A_OP | REF_REG_REG << A_REF | AX << A_R1 );				// POP AX
	cb->putInstruction( MOV << A_OP | REF_REG_MEM << A_REF | GPF << A_R1 | AX << A_R2 );// MOV RP,[AX]
	cb->putInstruction( RET << A_OP );													// RET

	// add the data for it
	method->addReferenceData( new ReferenceData( datarefSeq++, (char*)max, offset ) );
	return method;
}

/**
 * Builds the code for the minimum value ("min") method
 */
Method* IntClass::minMethod() {
	OFFSET_T offset;

	// create the method
	Method* method = new Method( this, "min", "min", typeref, PUBLIC | STATIC | FINAL );

	// define the maximum value
	int* min = new int();
	*min = MIN_VALUE;

	// define the byte code
	CodeBuffer* cb = method->getCodeBuffer();
	offset = cb->putInstruction( STAGE << A_OP | ARG_DIRECT << A_ARG );					// STAGE,&value
	cb->putInstruction( POP << A_OP | REF_REG_REG << A_REF | AX << A_R1 );				// POP AX
	cb->putInstruction( MOV << A_OP | REF_REG_MEM << A_REF | GPF << A_R1 | AX << A_R2 );// MOV RP,[AX]
	cb->putInstruction( RET << A_OP );													// RET

	// add the data for it
	method->addReferenceData( new ReferenceData( datarefSeq++, (char*)min, offset ) );
	return method;
}

/**
 * Builds the code for the "toString" method
 */
Method* IntClass::toStringMethod() {
	OFFSET_T offset;

	// create the method
	Method* method = new Method( this, "toString", "toString", new TypeReference( "string" ), PUBLIC );

	// define the byte code
	CodeBuffer* cb = method->getCodeBuffer();
	offset = cb->putInstruction( STAGE << A_OP | ARG_DIRECT << A_ARG );					// STAGE,&value
	cb->putInstruction( POP << A_OP | REF_REG_REG << A_REF | AX << A_R1 );				// POP AX
	cb->putInstruction( MOV << A_OP | REF_REG_MEM << A_REF | GPF << A_R1 | AX << A_R2 );// MOV RP,[AX]
	cb->putInstruction( RET << A_OP );													// RET
	return method;
}
