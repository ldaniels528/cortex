/*
 * ClassLoader.cpp
 *
 *  Created on: March 4, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "ClassLoader.h"

/**
 * Constructor
 */
ClassLoader::ClassLoader( CodeBuffer* cb ) {
	// capture the code buffer
	this->cb = cb;
}

/**
 * Destruction
 */
ClassLoader::~ClassLoader() {
	classes.clear();
}

/**
 * Adds a class reference to the class loader
 */
void ClassLoader::addClass( Class* classdef ) {
	classes[classdef->getName()] = classdef;
}

/**
 * Returns the set of all loaded classes
 */
vector<Class*> ClassLoader::getClasses() {
	vector<Class*> classList = vector<Class*>();
	for( STRING_HASH_MAP(Class*)::iterator i = classes.begin(); i != classes.end(); i++ ) {
		if( i->second != NULL ) {
			classList.push_back( i->second );
		}
	}
	return classList;
}

/**
 * Determines whether the given class name is a
 * resolvable class.
 */
bool ClassLoader::isClass( const char* className ) {
	// is the class already loaded?
	if( classes.count( className ) ) {
		return true;
	}

	// otherwise ...
	else {
		// attempt to load the class
		return resolveClass( className ) != NULL;
	}
}

/**
 * Loads a class by name
 * @param classFilename the given class file name
 */
Class* ClassLoader::loadClass( const char* classFilename ) {
	// open the file for reading
	FILE* in = fopen( classFilename, "rb" );
	if( in == NULL ) {
		printf( "Class file '%s' could not be opened\n", classFilename );
		return NULL;
	}

	// obtain file size:
	fseek( in, 0, SEEK_END );
	long fileSize = ftell( in );
	//printf( "Class file '%s' is %ld bytes\n", classFilename, fileSize );
	rewind( in );

	// allocate a block of memory for the class
	OFFSET_T offset = cb->allocate( fileSize );

	// get a pointer to the block
	char* buf = (char*)(*cb)[ offset ];

	// read the content
	int count = fread( buf, 1, fileSize, in );
	if( count != fileSize ) {
		printf( "Error retrieving data from class file '%s'\n", classFilename );
		return NULL;
	}

	// close the file
	fclose( in );

	// point to the memory block
	cb->setPosition( offset );

	// return the class
	return Class::decompile( cb );;
}

/**
 * Inserts a class into memory
 */
void ClassLoader::insertClass( Class* theClass ) {
	// allocate memory for the class


	// add the class to the cache
	addClass( theClass );
}

/**
 * Indicates whether the class exists by loading
 * the class into memory if necessary
 */
Class* ClassLoader::resolveClass( const char *classname ) {
	// locate the class
	Class* theClass = findClass( classname );

	if( theClass != NULL ) {
		// if the class was found, and it to the cache
		if( !classes.count( classname )  ) {
			addClass( theClass );
		}
	}

	// return the class
	return theClass;
}

/**
 * Returns the class matching the given class name
 * loading the class into memory if necessary.
 */
Class* ClassLoader::findClass( const char* classname ) {
	// if the class is already loaded ...
	if( classes.count( classname ) ) {
		return classes[classname];
	}

	// create the file name
	char *filename = new char[ strlen( classname ) + 5 ];
	sprintf( filename, "%s.scc", classname );

	// otherwise, load the class
	return loadClass( filename );
}

