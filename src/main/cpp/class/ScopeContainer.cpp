/*
 * ScopeContainer.cpp
 *
 *  Created on: March 17, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "ScopeContainer.h"

/**
 * Constructor
 */
ScopeContainer::ScopeContainer( const char* name, Scope* parentScope, ScopeType type, char modifiers ):
Scope( name, parentScope, type, modifiers ) {
	// do nothing
}

/**
 * Destruction
 */
ScopeContainer::~ScopeContainer() {
	methods.clear();
}

/**
 * Compiles the class' methods into byte code
 */
int ScopeContainer::compileMethods( CodeBuffer* cb ) {
	cb->putShort( methods.size() );
	for( STRING_HASH_MAP(Method*)::iterator i = methods.begin(); i != methods.end(); i++ ) {
		Method* method = i->second;
		method->compile( cb );
	}
	return 0;
}

/**
 * Decompiles into byte code into methods and attaches them
 * to the given class
 */
int ScopeContainer::decompileMethods( ScopeContainer* scope, CodeBuffer* cb ) {
	int n_methods = cb->getShort();
	for( int n = 0; n < n_methods; n++ ) {
		Method* method = Method::decompile( scope, cb );
		if( method == NULL ) {
			printf( "Class file is corrupted: (%d) methods could not be retrieved\n", n_methods );
			return NULL;
		}

		// add it to the class
		scope->addMethod( method );
	}
	return 0;
}

/**
 * Adds the given member to the class
 * member - the given member
 */
bool ScopeContainer::addMember( ClassMember *member ) {
	switch( member->getMemberType() ) {
		case CLASS_FIELD: return addField( (Field*)member );
		case CLASS_METHOD: return addMethod( (Method*)member );
	}
	return false;
}

/**
 * Looks up a method by its signature
 */
ClassMember* ScopeContainer::lookupMember( const char* name ) {
	if( methods.count( name ) ) {
		return  methods[name];
	}
	else {
		return fields[name];
	}
}

/**
 * Adds the given method to the class
 * @param method the given method
 */
bool ScopeContainer::addMethod( Method *method ) {
	// if the method already exists...
	if( methods.count( method->getSignature() ) ) {
		return false;
	}

	// add the method
	methods[method->getSignature()] = method;
	return true;
}

/**
 * Returns the methods contained with this scope
 */
vector<Method*> ScopeContainer::getMethods() {
	vector<Method*> methodList;

	for( STRING_HASH_MAP(Method*)::iterator i = methods.begin(); i != methods.end(); i++ ) {
		Method* method = i->second;
		if( method != NULL ) {
			methodList.push_back( method );
		}
	}
	return methodList;
}

/**
 * Looks up a class method by its signature
 */
Method* ScopeContainer::lookupMethod( const char* signature ) {
	return methods[signature];
}

