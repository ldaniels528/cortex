/*
 * ClassLoader.h
 *
 *  Created on: March 4, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef CLASSLOADER_H_
#define CLASSLOADER_H_

#include <vector.h>

#include "../Common.h"

#include "Class.h"
#include "TypeReference.h"
#include "member/Field.h"
#include "member/Method.h"

/**
 * Represents the SVM Class Loader
 */
class ClassLoader {
private:
	STRING_HASH_MAP(Class*) classes;
	CodeBuffer* cb;

public:

	/**
	 * Default Constructor
	 */
	ClassLoader( CodeBuffer* );

	/**
	 * Destruction
	 */
	virtual ~ClassLoader();

	/**
	 * Adds a class reference to the class loader
	 */
	void addClass( Class* );

	/**
	 * Returns the set of all loaded classes
	 */
	vector<Class*> getClasses();

	/**
	 * Determines whether the given class name is a
	 * resolvable class.
	 */
	bool isClass( const char* );

	/**
	 * Loads a class by names
	 * className - the given class name
	 */
	Class* loadClass( const char* );

	/**
	 * Returns the class matching the given class name
	 * loading the class into memory if necessary.
	 */
	Class* resolveClass( const char* );

private:

	/**
	 * Returns the class matching the given class name
	 * loading the class into memory if necessary.
	 */
	Class* findClass( const char* );

	/**
	 * Inserts a class into memory
	 */
	void insertClass( Class* theClass );

};

#endif /* CLASSLOADER_H_ */
