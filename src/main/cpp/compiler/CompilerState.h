/*
 * CompilerState.h
 *
 *  Created on: March 5, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef COMPILERSTATE_H_
#define COMPILERSTATE_H_

#include <hash_map.h>
#include <list.h>

#include "../CodeBuffer.h"
#include "../Common.h"
#include "../class/ClassLoader.h"
#include "../class/NameSpace.h"
#include "../class/Scope.h"
#include "../token/TokenStack.h"

// keyword definitions
#define KW_ABSTRACT		0
#define KW_CLASS		1
#define KW_DELETE		2
#define KW_DO			3
#define KW_ELSE			4
#define KW_EXTENDS		5
#define KW_FINAL		6
#define KW_FOR			7
#define KW_FOREACH		8
#define KW_IF			9
#define KW_IMPORT		10
#define KW_NAMESPACE	11
#define KW_NEW			12
#define KW_PACKAGED		13
#define KW_PRIVATE		14
#define KW_PROTECTED	15
#define KW_PUBLIC		16
#define KW_RETURN		17
#define KW_STATIC		18
#define KW_THIS			19
#define KW_VIRTUAL		20
#define KW_WHILE		21

/**
 * Represents the "state" information that is used
 * by the various parsers to compile the code
 */
class CompilerState {
private:
	STRING_HASH_MAP(NameSpace*) nsMap;
	STRING_HASH_MAP(int) keywords;
	ClassLoader* classLoader;
	int dataRefSeq;
	int parmRefSeq;

public:
	CodeBuffer *codeBuffer;

	/**
	 * Default Constructor
	 */
	CompilerState();

	/**
	 * Releases all resources
	 */
	virtual ~CompilerState();

	/**
	 * Adds a class reference to the class loader
	 */
	void addClass( Class* );

	/**
	 * Determines whether the given class name is a
	 * resolvable class.
	 */
	bool isClass( const char* );

	/**
	 * Indicates whether the class exists by loading
	 * the class into memory if necessary
	 */
	Class* resolveClass( const char* );

	/**
	 * Returns the identifier name from the given token stack
	 */
	const char* getIdentifierName( TokenStack*, const char* );

	/**
	 * Retrieves a quoted identifier
	 */
	const char* getQuotedIdentifier( TokenStack* );

	/**
	 * Indicates whether the given word is a keyword
	 */
	bool isKeyword( const char* );

	/**
	 * Returns the index of the keyword
	 */
	int lookupKeyword( const char* );

	/**
	 * Adds a new name space to the state object
	 */
	bool addNameSpace( NameSpace* nameSpace );

	/**
	 * Returns the currently defined name space
	 */
	list<NameSpace*> getNameSpaces();

	/**
	 * Returns the next data reference sequence
	 */
	int nextDataRefId();

	/**
	 * Returns the next parameter sequence number
	 */
	int nextParamId();

	/**
	 * Resets the class level counters
	 */
	void resetCounters();

};

#endif /* COMPILERSTATE_H_ */
