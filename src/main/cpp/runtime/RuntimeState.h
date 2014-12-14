/*
 * RuntimeState.h
 *
 *  Created on: March 10, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef RUNTIMESTATE_H_
#define RUNTIMESTATE_H_

#include "../class/ClassLoader.h"
#include "../class/Scope.h"
#include "../class/member/Field.h"
#include "../class/member/Method.h"

#include "ValueLocation.h"
#include "VirtualMemory.h"

/**
 * Represents the runtime state of the virtual machine
 */
class RuntimeState {
private:
	OFFSET_T* dataOffsets;
	ClassLoader* classLoader;
	VirtualMemory* memory;

public:

	/**
	 * Constructor
	 */
	RuntimeState();

	/**
	 * Destruction
	 */
	virtual ~RuntimeState();

	/**
	 * Returns the class loader
	 */
	ClassLoader* getClassLoader();

	/**
	 * Returns a reference to the virtual machine's memory
	 */
	VirtualMemory* getMemory();

	/**
	 * Indicates whether the class exists by loading
	 * the class into memory if necessary
	 */
	Class* resolveClass( const char *classname );

	/**
	 * Recursively searches for a field by its name
	 */
	Field* lookupField( Scope*, const char* );

	/**
	 * Recursively searches for a method by its signature
	 */
	Method* lookupMethod( Scope*, const char* );

};

#endif /* RUNTIMESTATE_H_ */
