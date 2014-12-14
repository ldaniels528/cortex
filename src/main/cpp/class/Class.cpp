/*
 * Class.cpp
 *
 *  Created on: March 3, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "Class.h"

#define CLASS_ID 	"DOLCE"

/**
 * Constructor
 */
Class::Class( const char* name, const char* packageName, Scope* parentScope, char modifiers ) :
ScopeContainer( name, parentScope, CLASS_SCOPE, modifiers ) {
	this->packageName	= packageName;
	this->typeref		= new TypeReference( name );
	this->datarefgen	= 0;
}

/**
 * Destruction
 */
Class::~Class() {
	delete typeref;
}

/**
 * Compiles the class file into byte code
 */
int Class::compile( CodeBuffer* cb ) {
	int errorCode = 0;

	// add the header information
	cb->putString( CLASS_ID );

	// add the version information
	cb->putChar( MAJOR_VERSION );
	cb->putChar( MINOR_VERSION );

	// add the class name
	cb->putString( name );

	// add the package name
	cb->putString( packageName );

	// add the access modifier
	cb->putChar( modifiers );

	// include the fields
	if( ( errorCode = compileFields( cb ) ) ) {
		return errorCode;
	}

	// include the methods
	if( ( errorCode = compileMethods( cb ) ) ) {
		return errorCode;
	}

	// include the class initialization code
	if( ( errorCode = compileCodeBlock( cb ) ) ) {
		return errorCode;
	}

	// include the parameter data
	if( ( errorCode = compileParameterData( cb ) ) ) {
		return errorCode;
	}

	// include the reference data
	if( ( errorCode = compileReferenceData( cb ) ) ) {
		return errorCode;
	}

	// return success code
	return errorCode;
}

/**
 * Decompiles a class from byte code
 */
Class* Class::decompile( CodeBuffer* cb ) {
	// read the header information
	const char *classid = cb->getString();
	if( classid == NULL || strcmp( classid, CLASS_ID ) ) {
		printf( "Class file is corrupted: missing or invalid header\n" );
		return NULL;
	}

	// read the version information
	const char majorVersion = cb->getChar();
	const char minorVersion = cb->getChar();
	if( majorVersion > MAJOR_VERSION ) {
		printf( "Class version (%d.%02d) is incompatible\n", majorVersion, minorVersion );
		return NULL;
	}

	// read the class name
	const char *classname = cb->getString();
	if( classname == NULL ) {
		printf( "Class file is corrupted: no class name\n" );
		return NULL;
	}

	// read the package name
	const char *pkgname = cb->getString();
	if( pkgname == NULL ) {
		printf( "Class file is corrupted: no package name\n" );
		return NULL;
	}

	// add the access modifier
	const char modifiers = cb->getChar();

	// create the class instance
	Class* classdef = new Class( classname, pkgname, NULL, modifiers );

	// retrieve the fields
	decompileFields( classdef, cb );

	// retrieve the methods
	decompileMethods( classdef, cb );

	// retrieve the class initialization code
	decompileCodeBlock( classdef, cb );

	// retrieve the parameter data
	decompileParameterData( classdef, cb );

	// retrieve the reference data
	decompileReferenceData( classdef, cb );

	// return the class
	return classdef;
}

/**
 * Returns the type of the class
 */
TypeReference* Class::getTypeRef() {
	return typeref;
}
