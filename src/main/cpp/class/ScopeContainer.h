/*
 * ScopeContainer.h
 *
 *  Created on: March 17, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef SCOPECONTAINER_H_
#define SCOPECONTAINER_H_

#include <list.h>
#include <vector.h>

#include "Scope.h"
#include "member/ClassMember.h"
#include "member/Method.h"

/**
 * Represents a cascading scope.
 */
class ScopeContainer : public Scope {
protected:
	STRING_HASH_MAP(Method*) methods;

public:

	/**
	 * Constructor
	 */
	ScopeContainer( const char* name, Scope* parentScope, ScopeType type, char modifiers );

	/**
	 * Destruction
	 */
	virtual ~ScopeContainer();

	/**
	 * Compiles the scope's methods into byte code
	 */
	int compileMethods( CodeBuffer* );

	/**
	 * Decompiles into byte code into methods and attaches them
	 * to the given scope
	 */
	static int decompileMethods( ScopeContainer*, CodeBuffer* );

	/**
	 * Adds the given member to the class
	 * @param member the class member
	 */
	bool addMember( ClassMember* );

	/**
	 * Looks up a class member by its name
	 */
	ClassMember* lookupMember( const char* );

	/**
	 * Adds the given method to the class
	 * @param method the given method
	 */
	bool addMethod( Method* );

	/**
	 * Returns the methods contained with this scope
	 */
	vector<Method*> getMethods();

	/**
	 * Looks up a class method by its signature
	 */
	Method* lookupMethod( const char* );

};

#endif /* SCOPECONTAINER_H_ */
