/*
 * AnonymousMethod.h
 *
 *  Created on: March 17, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef ANONYMOUSMETHOD_H_
#define ANONYMOUSMETHOD_H_

#include "Method.h"

/**
 * Represents an anonymous method
 */
class AnonymousMethod : public Method {
public:

	/**
	 * Constructor
	 */
	AnonymousMethod( const char *name, Scope* scope );

	/**
	 * Destruction
	 */
	virtual ~AnonymousMethod();

};

#endif /* ANONYMOUSMETHOD_H_ */
