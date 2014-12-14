/*
 * Field.cpp
 *
 *  Created on: March 4, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "Field.h"

/**
 * Constructor
 * @param name the name of the field
 * @param type the type of the field
 * @param modifiers the access modifiers
 */
Field::Field( const char *name, TypeReference *type, char modifiers ):
ClassMember( type, CLASS_FIELD ) {
	this->name 		= name;
	this->modifiers	= modifiers;
}

/**
 * Releases all resources
 */
Field::~Field() {
	// do nothing
}

/**
 * Compiles the method into byte code
 */
int Field::compile( CodeBuffer *cb ) {
	// write the name of the member
	cb->putString( name );

	// write the name of the member's type
	type->compile( cb );

	// write the access modifiers
	cb->putChar( modifiers );

	// capture the current offset
	offset = cb->getPosition();
	printf( "Field::compile - assigned '%s' memory offset %08lX\n", name, offset );

	// let's allow space for a pointer here the pointer
	cb->move( sizeof( OFFSET_T ) );
	return 0;
}

/**
 * Decompiles a field from byte code
 */
Field* Field::decompile( CodeBuffer* cb ) {
	// read the name of the member
	char *name = cb->getString();
	if( name == NULL ) {
		printf( "Class file is corrupted: field name could not be read\n" );
		return NULL;
	}

	// read the name of the member's type
	TypeReference* type = TypeReference::decompile( cb );

	// read the attribute of the member
	char attributes = cb->getChar();

	// read the field's pointer
	OFFSET_T offset = cb->getPosition();

	// skip field's the pointer
	cb->move( sizeof( OFFSET_T ) );

	// create the field
	Field* field = new Field( name, type, attributes );
	field->setOffset( offset );
	return field;
}

/**
 * Returns the field's access modifiers
 */
char Field::getAccessModifiers() {
	return modifiers;
}

/**
 * Sets the field's access modifiers
 */
void Field::setAccessModifiers( char modifiers ) {
	this->modifiers = modifiers;
}

/**
 * Returns the name of the field
 */
const char* Field::getName() {
	return name;
}

/**
 * Returns the offset
 */
OFFSET_T Field::getOffset() {
	return offset;
}

/**
 * Sets the offset
 */
void Field::setOffset( OFFSET_T offset ) {
	this->offset = offset;
}

/**
 * Indicates whether the field is private;
 * meaning internal use only
 */
bool Field::isPrivate() {
	return ( modifiers & PRIVACY_MASK ) == PRIVATE;
}

/**
 * Indicates whether the field is protected;
 * meaning internal use or sub-classes
 */
bool Field::isProtected() {
	return ( modifiers & PRIVACY_MASK ) == PROTECTED;
}

/**
 * Indicates whether the field is public;
 * meaning any class
 */
bool Field::isPublic() {
	return ( modifiers & PRIVACY_MASK ) == PUBLIC;
}

/**
 * Indicates whether the field is shared;
 * meaning internal use, sub-classes, and same name space
 */
bool Field::isPackage() {
	return ( modifiers & PRIVACY_MASK ) == PACKAGE;
}

/**
 * Sets the private attribute
 * @param access the access level (e.g. PRIVATE, PROTECTED, PACKAGE or PUBLIC)
 */
void Field::setPrivacy( int mask ) {
	this->modifiers &= 0xCF | mask;
}

/**
 * Indicates whether the field is read-only
 */
bool Field::isFinal() {
	return modifiers & FINAL;
}

/**
 * Sets the read-only attribute
 */
void Field::setFinal( bool enable ) {
	SET_ATTRIBUTE(modifiers,enable,FINAL)
}

/**
 * Indicates whether the field is static
 */
bool Field::isStatic() {
	return modifiers & STATIC;
}

/**
 * Sets the static attribute
 */
void Field::setStatic( bool enable ) {
	SET_ATTRIBUTE(modifiers,enable,STATIC)
}
