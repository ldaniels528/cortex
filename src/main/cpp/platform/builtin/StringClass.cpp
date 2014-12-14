/*
 * StringClass.cpp
 *
 *  Created on: Mar 10, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "../../OpCodes.h"

#include "StringClass.h"

/**
 * Constructor
 */
StringClass::StringClass() : Class( "string", "sera.lang" ) {
	// fields
	addField( new Field( "len", new TypeReference("int"), PUBLIC ) );

	// methods
	addMember( appendMethod() );
	addMember( flipMethod() );
	addMember( lengthMethod() );
	addMember( ltrimMethod() );
	addMember( rtrimMethod() );
	addMember( toStringMethod() );
}

/**
 * Destruction
 */
StringClass::~StringClass() {
	// do nothing
}

/**
 * Builds the code for the "append" method
 */
Method* StringClass::appendMethod() {
	OFFSET_T offset;

	// create the method
	Method* method = new Method( this, "append", "append", new TypeReference( "void" ), PUBLIC );

	// define the byte code
	CodeBuffer* cb = method->getCodeBuffer();
	offset = cb->putInstruction( STAGE << A_OP | ARG_DIRECT << A_ARG );					// STAGE,&value
	cb->putInstruction( POP << A_OP | REF_REG_REG << A_REF | AX << A_R1 );				// POP AX
	cb->putInstruction( MOV << A_OP | REF_REG_MEM << A_REF | GPF << A_R1 | AX << A_R2 );// MOV RP,[AX]
	cb->putInstruction( RET << A_OP );													// RET
	return method;
}

/**
 * Builds the code for the "flip" method
 */
Method* StringClass::flipMethod() {
	OFFSET_T offset;

	// create the method
	Method* method = new Method( this, "flip", "flip", new TypeReference( "void" ), PUBLIC );

	// define the byte code
	CodeBuffer* cb = method->getCodeBuffer();
	offset = cb->putInstruction( STAGE << A_OP | ARG_DIRECT << A_ARG );					// STAGE,&value
	cb->putInstruction( POP << A_OP | REF_REG_REG << A_REF | AX << A_R1 );				// POP AX
	cb->putInstruction( MOV << A_OP | REF_REG_MEM << A_REF | GPF << A_R1 | AX << A_R2 );// MOV RP,[AX]
	cb->putInstruction( RET << A_OP );													// RET
	return method;
}

/**
 * Builds the code for the "length" method
 */
Method* StringClass::lengthMethod() {
	OFFSET_T offset;

	// create the method
	Method* method = new Method( this, "length", "length", new TypeReference( "int" ), PUBLIC );

	// define the byte code
	CodeBuffer* cb = method->getCodeBuffer();
	offset = cb->putInstruction( STAGE << A_OP | ARG_DIRECT << A_ARG );					// STAGE,&value
	cb->putInstruction( POP << A_OP | REF_REG_REG << A_REF | AX << A_R1 );				// POP AX
	cb->putInstruction( MOV << A_OP | REF_REG_MEM << A_REF | GPF << A_R1 | AX << A_R2 );// MOV RP,[AX]
	cb->putInstruction( RET << A_OP );													// RET
	return method;
}

/**
 * Builds the code for the "ltrim" method
 */
Method* StringClass::ltrimMethod() {
	OFFSET_T offset;

	// create the method
	Method* method = new Method( this, "ltrim", "ltrim", new TypeReference( "void" ), PUBLIC );

	// define the byte code
	CodeBuffer* cb = method->getCodeBuffer();
	offset = cb->putInstruction( STAGE << A_OP | ARG_DIRECT << A_ARG );					// STAGE,&value
	cb->putInstruction( POP << A_OP | REF_REG_REG << A_REF | AX << A_R1 );				// POP AX
	cb->putInstruction( MOV << A_OP | REF_REG_MEM << A_REF | GPF << A_R1 | AX << A_R2 );// MOV RP,[AX]
	cb->putInstruction( RET << A_OP );													// RET
	return method;
}

/**
 * Builds the code for the "rtrim" method
 */
Method* StringClass::rtrimMethod() {
	OFFSET_T offset;

	// create the method
	Method* method = new Method( this, "rtrim", "rtrim", new TypeReference( "void" ), PUBLIC );

	// define the byte code
	CodeBuffer* cb = method->getCodeBuffer();
	offset = cb->putInstruction( STAGE << A_OP | ARG_DIRECT << A_ARG );					// STAGE,&value
	cb->putInstruction( POP << A_OP | REF_REG_REG << A_REF | AX << A_R1 );				// POP AX
	cb->putInstruction( MOV << A_OP | REF_REG_MEM << A_REF | GPF << A_R1 | AX << A_R2 );// MOV RP,[AX]
	cb->putInstruction( RET << A_OP );													// RET
	return method;
}

/**
 * Builds the code for the "toString" method
 */
Method* StringClass::toStringMethod() {
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
