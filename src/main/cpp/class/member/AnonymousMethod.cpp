/*
 * AnonymousMethod.cpp
 *
 *  Created on: March 17, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "AnonymousMethod.h"

/**
 * Constructor
 */
AnonymousMethod::AnonymousMethod( const char *name, Scope* scope ):
Method( scope, name, name, new TypeReference( "void" ), PRIVATE ) {
	// do nothing
}

/**
 * Destruction
 */
AnonymousMethod::~AnonymousMethod() {
	// do nothing
}

