/*
 * RuntimeState.cpp
 *
 *  Created on: March 10, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "RuntimeState.h"

/**
 * Constructor
 */
RuntimeState::RuntimeState() {
	this->memory		= new VirtualMemory();
	this->classLoader	= new ClassLoader( memory );
}

/**
 * Destruction
 */
RuntimeState::~RuntimeState() {
	delete classLoader;
}

/**
 * Returns the class loader
 */
ClassLoader* RuntimeState::getClassLoader() {
	return classLoader;
}

/**
 * Returns a reference to the virtual machine's memory
 */
VirtualMemory* RuntimeState::getMemory() {
	return memory;
}

/**
 * Indicates whether the class exists by loading
 * the class into memory if necessary
 */
Class* RuntimeState::resolveClass( const char *classname ) {
	return classLoader->resolveClass( classname );
}

/**
 * Recursively searches for a field by its name
 */
Field* RuntimeState::lookupField( Scope* scope, const char* name ) {
	Scope* parentScope = NULL;
	Field* field = NULL;

	// if the field is within the scope, return it
	if( ( field = scope->lookupField( name ) ) ) {
		return field;
	}

	// otherwise, check the parent scope
	else if( ( parentScope = scope->getParentSope() ) != NULL ) {
		return lookupField( parentScope, name );
	}

	// otherwise, it's not found
	else {
		return NULL;
	}
}

/**
 * Recursively searches for a method by its signature
 */
Method* RuntimeState::lookupMethod( Scope* scope, const char* signature ) {
	Scope* parentScope = NULL;
	Class* classinst = NULL;
	Method* method = NULL;

	// is the scope a class?
	if( scope->getScopeType() == CLASS_SCOPE ) {
		// if the method is within the scope, return it
		classinst = (Class*)scope;
		if( ( method = classinst->lookupMethod( signature ) ) ) {
			return method;
		}
	}

	// otherwise, check the parent scope
	if( ( parentScope = scope->getParentSope() ) ) {
		return lookupMethod( parentScope, signature );
	}

	// otherwise, it's not found
	return method;
}
