/*
 * Field.h
 *
 *  Created on: March 4, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef FIELD_H_
#define FIELD_H_

#include <stdio.h>

#include "../../Common.h"
#include "../TypeReference.h"

#include "ClassMember.h"

/**
 * Represents a class field definition
 */
class Field : public ClassMember {
private:
	const char *name;
	OFFSET_T offset;
	char modifiers;

public:

	/**
	 * Constructor
	 * @param name the name of the field
	 * @param type the type of the field
	 * @param modifiers the modifiers of the field (e.g. "public")
	 */
	Field( const char *name, TypeReference *type, char modifiers = PACKAGE );

	/**
	 * Releases all resources
	 */
	virtual ~Field();

	/**
	 * Compiles the method into byte code
	 */
	int compile( CodeBuffer* );

	/**
	 * Decompiles a field from byte code
	 */
	static Field* decompile( CodeBuffer* );

	/**
	 * Returns the field's access modifiers
	 */
	char getAccessModifiers();

	/**
	 * Sets the field's access modifiers
	 */
	void setAccessModifiers( char );

	/**
	 * Returns the name of the field
	 */
	const char* getName();

	/**
	 * Returns the offset
	 */
	OFFSET_T getOffset();

	/**
	 * Sets the offset
	 */
	void setOffset( OFFSET_T );

	/**
	 * Indicates whether the member is private;
	 * meaning internal use only
	 */
	bool isPrivate();

	/**
	 * Indicates whether the member is protected;
	 * meaning internal use or sub-classes
	 */
	bool isProtected();

	/**
	 * Indicates whether the member's access level is package;
	 * meaning internal use, sub-classes, or packaged with
	 * (same name space as) the accessing class.
	 */
	bool isPackage();

	/**
	 * Indicates whether the member is public;
	 * meaning any class
	 */
	bool isPublic();

	/**
	 * Sets the private attribute
	 * @param access the access level (e.g. PUBLIC, PRIVATE, or PROTECTED)
	 */
	void setPrivacy( int access );

	/**
	 * Indicates whether the member is read-only
	 */
	bool isFinal();

	/**
	 * Sets the read-only attribute
	 */
	void setFinal( bool enable );

	/**
	 * Indicates whether the member is static
	 */
	bool isStatic();

	/**
	 * Sets the static attribute
	 */
	void setStatic( bool enable );

};

#endif /* FIELD_H_ */
