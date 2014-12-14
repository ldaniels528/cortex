/*
 * NameSpace.h
 *
 *  Created on: March 11, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef NAMESPACE_H_
#define NAMESPACE_H_

#include "../CodeBuffer.h"
#include "Class.h"
#include "Scope.h"

/**
 * Represents a name space
 */
class NameSpace : public Scope {
private:
	STRING_HASH_MAP(Class*) classes;

public:

	/**
	 * Constructor
	 */
	NameSpace( const char* name, Scope* parentScope = NULL );

	/**
	 * Destruction
	 */
	virtual ~NameSpace();

	/**
	 * Adds a class to the name space
	 */
	bool addClass( Class* classInst );

	/**
	 * Returns the map of all classes
	 */
	list<Class*> getClasses();

	/**
	 * Compiles the name space into byte code
	 */
	int compile( CodeBuffer* cb );

	/**
	 * Decompiles a name space from byte code
	 */
	static NameSpace* decompile( ScopeContainer* scope, CodeBuffer *cb );

};

#endif /* NAMESPACE_H_ */
