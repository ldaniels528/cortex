/*
 * Class.h
 *
 *  Created on: March 3, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef CLASSFILE_H_
#define CLASSFILE_H_

#include "../CodeBuffer.h"
#include "../Common.h"

#include "member/ClassMember.h"
#include "member/Field.h"
#include "member/Method.h"
#include "ScopeContainer.h"
#include "TypeReference.h"

/**
 * Represents a Class definition
 */
class Class : public ScopeContainer {
protected:
	TypeReference* typeref;

public:

	/**
	 * Constructor
	 */
	Class( const char* name, const char* packageName, Scope* parentScope = NULL, char modifiers = PACKAGE );

	/**
	 * Destruction
	 */
	virtual ~Class();

	/**
	 * Compiles the class into byte code
	 */
	int compile( CodeBuffer* cb );

	/**
	 * Decompiles a class from byte code
	 */
	static Class* decompile( CodeBuffer* cb );

	/**
	 * Returns the type of the class
	 */
	TypeReference* getTypeRef();

};

#endif /* CLASSFILE_H_ */
