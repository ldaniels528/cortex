/*
 * CompilerState.cpp
 *
 *  Created on: March 5, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "CompilerState.h"

/**
 * Default Constructor
 */
CompilerState::CompilerState() {
	this->codeBuffer 	= new CodeBuffer( 16384, 1024 );
	this->classLoader	= new ClassLoader( codeBuffer );

	// create the mapping of keywords
	keywords["abstract"] 	= KW_ABSTRACT;
	keywords["class"] 		= KW_CLASS;
	keywords["delete"] 		= KW_DELETE;
	keywords["do"] 			= KW_DO;
	keywords["else"] 		= KW_ELSE;
	keywords["extends"] 	= KW_EXTENDS;
	keywords["final"] 		= KW_FINAL;
	keywords["for"] 		= KW_FOR;
	keywords["foreach"] 	= KW_FOREACH;
	keywords["if"] 			= KW_IF;
	keywords["import"] 		= KW_IMPORT;
	keywords["namespace"] 	= KW_NAMESPACE;
	keywords["new"]	 		= KW_NEW;
	keywords["packaged"] 	= KW_PACKAGED;
	keywords["private"] 	= KW_PRIVATE;
	keywords["protected"] 	= KW_PROTECTED;
	keywords["public"]	 	= KW_PUBLIC;
	keywords["return"]	 	= KW_RETURN;
	keywords["static"]	 	= KW_STATIC;
	keywords["this"]	 	= KW_THIS;
	keywords["while"]	 	= KW_WHILE;
	keywords["virtual"]	 	= KW_VIRTUAL;
}

/**
 * Releases all resources
 */
CompilerState::~CompilerState() {
	keywords.clear();
	nsMap.clear();
	delete codeBuffer;
	delete classLoader;
}

/**
 * Adds a class reference to the class loader
 */
void CompilerState::addClass( Class* classinst ) {
	classLoader->addClass( classinst );
}

/**
 * Determines whether the given class name is a
 * resolvable class.
 */
bool CompilerState::isClass( const char *className ) {
	return classLoader->isClass( className );
}

/**
 * Indicates whether the class exists by loading
 * the class into memory if necessary
 */
Class* CompilerState::resolveClass( const char *className ) {
	return classLoader->resolveClass( className );
}

/**
 * Retrieves an identifier name
 */
const char* CompilerState::getIdentifierName( TokenStack* stack, const char *typeName ) {
	const char* name;
	Token* t;

	// the next token must be the identifier
	if( !stack->hasNext() || !( t = stack->next() ) || !( name = t->getText() ) ) {
		SYNTAX_ERROR( "unexpected end of statement", stack->last() );
		return NULL;
	}

	// the identifier can't be a keyword
	if( isKeyword( name ) ) {
		char* error = new char[128];
		sprintf( error, "unexpected keyword '%s'", name );
		SYNTAX_ERROR( error, t );
		delete error;
		return NULL;
	}

	// validate the identifier name
	if( t->getType() != tok::ALPHA_NUMERIC ) {
		char* error = new char[128];
		sprintf( error, "%s identifier expected", typeName );
		SYNTAX_ERROR( error, t );
		delete error;
		return NULL;
	}

	// return the identifier name
	return name;
}

/**
 * Retrieves a quoted identifier
 */
const char* CompilerState::getQuotedIdentifier( TokenStack* stack ) {
	const char* quotedText;
	Token* t;

	// the next token must be the identifier
	if( !stack->hasNext() || !( t = stack->next() ) || !( quotedText = t->getText() ) ) {
		SYNTAX_ERROR( "unexpected end of statement", stack->last() );
		return NULL;
	}

	// validate the identifier name
	if( t->getType() != tok::DQUOTE_TEXT ) {
		SYNTAX_ERROR( "quoted identifier expected", t );
		return NULL;
	}

	// return the identifier name
	return quotedText;
}

/**
 * Indicates whether the given word is a keyword
 */
bool CompilerState::isKeyword( const char* word ) {
	return keywords.count(word);
}

/**
 * Returns the index of the keyword
 */
int CompilerState::lookupKeyword( const char* keyword ) {
	return keywords[keyword];
}

/**
 * Adds a new name space to the state object
 */
bool CompilerState::addNameSpace( NameSpace* ns ) {
	// if name space already exists...
	if( nsMap.count( ns->getName() ) ) {
		return false;
	}

	// add the name space
	nsMap[ns->getName()] = ns;
	return true;
}

/**
 * Returns the map of all classes
 */
list<NameSpace*> CompilerState::getNameSpaces() {
	list<NameSpace*> list;
	for( STRING_HASH_MAP(NameSpace*)::iterator i = nsMap.begin(); i != nsMap.end(); i++ ) {
		list.push_back( i->second );
	}
	return list;
}

/**
 * Returns the next data reference sequence number
 */
int CompilerState::nextDataRefId() {
	return dataRefSeq++;
}

/**
 * Returns the next parameter sequence number
 */
int CompilerState::nextParamId() {
	return parmRefSeq++;
}

/**
 * Resets the class level counters
 */
void CompilerState::resetCounters() {
	dataRefSeq	= 0;
	parmRefSeq	= 0;
}
