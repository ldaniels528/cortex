/*
 * NameSpace.cpp
 *
 *  Created on: March 11, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "NameSpace.h"

/**
 * Constructor
 */
NameSpace::NameSpace( const char* name, Scope* parentScope ) : Scope( name, parentScope, NAMESPACE_SCOPE ) {
	// do nothing
}

/**
 * Destructor
 */
NameSpace::~NameSpace() {
	// delete all class files
	for( STRING_HASH_MAP(Class*)::iterator i = classes.begin(); i != classes.end(); i++ ) {
		delete i->first;
		delete i->second;
	}
	classes.clear();
}

/**
 * Adds a class to the name space
 */
bool NameSpace::addClass( Class* classInst ) {
	const char* classname = classInst->getName();

	// if the class already exists ...
	if( classes.count( classname ) ) {
		return false;
	}

	// add the class
	classes[classname] = classInst;
	return true;
}

/**
 * Returns the map of all classes
 */
list<Class*> NameSpace::getClasses() {
	list<Class*> list;
	for( STRING_HASH_MAP(Class*)::iterator i = classes.begin(); i != classes.end(); i++ ) {
		list.push_back( i->second );
	}
	return list;
}

/**
 * Compiles the name space into byte code
 */
int NameSpace::compile( CodeBuffer* cb ) {
	cb->putString( name );
	return 0;
}

/**
 * Decompiles a name space from byte code
 */
NameSpace* NameSpace::decompile( ScopeContainer* scope, CodeBuffer *cb ) {
	const char* name = cb->getString();
	return new NameSpace( name, scope );
}
