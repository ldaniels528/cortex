/*
 * Scope.cpp
 *
 *  Created on: March 10, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include <iostream.h>
#include <string.h>

#include "../Common.h"
#include "Scope.h"

/**
 * Constructor
 */
Scope::Scope( const char* name, Scope* parentScope, ScopeType type, char modifiers ) {
	this->name			= name;
	this->parentScope	= parentScope;
	this->scopeType 	= type;
	this->modifiers		= modifiers;

	// allocate the code block
	this->codebuffer 	= new CodeBuffer( 128, 64 );

	// generate the package and fully qualified names
	this->packageName	= buildScopeName( parentScope );
	this->fullName		= buildFullyQualifiedName( name, packageName );
}

/**
 * Destruction
 */
Scope::~Scope() {
	delete codebuffer;
	delete fullName;
	delete packageName;

	// cleanup the collections
	fields.clear();
	parameterdata.clear();
	referencedata.clear();
}

/**
 * Indicates whether the given scope falls within
 * the ancestry of the current scope
 */
bool Scope::isInFamily( Scope* scope ) {
	// did we find the scope?
	if( scope == this ) {
		return true;
	}

	// look for it at the parent level ...
	return ( parentScope != NULL ) ? parentScope->isInFamily( scope ) : false;
}

/**
 * Adds the method argument to the method
 * @param field the given field
 */
bool Scope::addField( Field* field ) {
	// reject duplicate fields
	if( fields.count( field->getName() ) ) {
		return false;
	}

	// add the field
	fields[ field->getName() ] = field;
	return true;
}

/**
 * Returns the fields contained with this scope
 */
vector<Field*> Scope::getFields() {
	vector<Field*> fieldList;
	for( STRING_HASH_MAP(Field*)::iterator i = fields.begin(); i != fields.end(); i++ ) {
		Field* field = i->second;
		if( field != NULL ) {
			fieldList.push_back( field );
		}
	}
	return fieldList;
}

/**
 * Retrieves a field by name
 * @param name the name of the desired field
 */
Field* Scope::lookupField( const char* name ) {
	// does this scope contain the field?
	if( fields.count( name ) ) {
		return fields[name];
	}

	// if not, check the parent
	else if( parentScope != NULL ) {
		return parentScope->lookupField( name );
	}

	// otherwise, abandon the search
	else {
		return NULL;
	}
}

/**
 * Adds the given data to the class
 * @param data the parameter data
 */
void Scope::addParameterData( ParameterData* data ) {
	parameterdata.push_back( data );
}

/**
 * Lookups a specific piece of data via the given parameter ID
 */
ParameterData* Scope::lookupParameterData( unsigned int paramId ) {
	return ( paramId < parameterdata.size() ) ? parameterdata[ paramId ] : NULL;
}

/**
 * Adds the given data to the class
 * @param data the reference data
 */
void Scope::addReferenceData( ReferenceData* data ) {
	referencedata.push_back( data );
}

/**
 * Returns the set of all reference data
 */
vector<ReferenceData*> Scope::getReferenceDataList() {
	return referencedata;
}

/**
 * Lookups a specific piece of data via the given reference ID
 */
ReferenceData* Scope::lookupReferenceData( unsigned int refId ) {
	return ( refId < referencedata.size() ) ? referencedata[ refId ] : NULL;
}

/**
 * Writes the compiled code to the buffer
 */
int Scope::compileCodeBlock( CodeBuffer* cb ) {
	// write the length of the code block
	cb->putInt( codebuffer->getLength() );

	// capture the code offset
	this->codeOffset = cb->getPosition();

	// append the byte code
	cb->putBuffer( *codebuffer );
	return 0;
}

/**
 * Compiles the fields into byte code
 */
int Scope::compileFields( CodeBuffer* cb ) {
	int errors = 0;
	cb->putShort( fields.size() );
	for( STRING_HASH_MAP(Field*)::iterator i = fields.begin(); i != fields.end(); i++ ) {
		Field* field = i->second;
		errors += field->compile( cb );
	}
	return errors;
}

/**
 * Compiles the parameter data into byte code
 */
int Scope::compileParameterData( CodeBuffer* cb ) {
	int errors = 0;
	cb->putInt( parameterdata.size() );
	for( vector<ParameterData*>::iterator i = parameterdata.begin(); i != parameterdata.end(); i++ ) {
		ParameterData* data = *i;
		errors += data->compile( cb );
	}
	return errors;
}

/**
 * Compiles the reference data into byte code
 */
int Scope::compileReferenceData( CodeBuffer* cb ) {
	int errors = 0;
	cb->putInt( referencedata.size() );
	for( vector<ReferenceData*>::iterator i = referencedata.begin(); i != referencedata.end(); i++ ) {
		ReferenceData* data = *i;
		errors += data->compile( cb, codeOffset );
	}
	return errors;
}

/**
 * Imports the byte code block from the buffer, and attaches
 * it to the given scope.
 */
int Scope::decompileCodeBlock( Scope* scope, CodeBuffer* cb ) {
	// read the method's byte length
	int codesize = cb->getInt();
	if( codesize < 0 ) {
		printf( "Class file is corrupted: method byte code length could not be read\n" );
		return -1;
	}

	// capture the entry point of this method
	scope->codeOffset = cb->getPosition();

	// read the method's byte code
	char* bytecode = cb->getChars( codesize );
	if( bytecode == NULL ) {
		printf( "Class file is corrupted: method byte code (%d bytes) could not be read\n", codesize );
		return -2;
	}

	// add the code block
	scope->getCodeBuffer()->putChars( bytecode, codesize );
	return 0;
}

/**
 * Decompiles the byte code into fields and attaches
 * them to the given scope.
 */
int Scope::decompileFields( Scope* scope, CodeBuffer* cb ) {
	int n_fields = cb->getShort();
	for( int n = 0; n < n_fields; n++ ) {
		Field* field = Field::decompile( cb );
		if( field == NULL ) {
			printf( "Class file is corrupted: (%d) fields could not be retrieved\n", n_fields );
			return NULL;
		}

		// add it to the scope
		scope->addField( field );
	}
	return 0;
}

/**
 * Decompiles the byte code into parameter data and attaches
 * it to the given scope.
 */
int Scope::decompileParameterData( Scope* scope, CodeBuffer* cb ) {
	int n_params = cb->getInt();
	for( int n = 0; n < n_params; n++ ) {
		ParameterData* data = ParameterData::decompile( cb );
		if( data == NULL ) {
			printf( "Class file is corrupted: [%d] parameter data could not be retrieved\n", n+1 );
			return NULL;
		}

		// add the parameter data
		scope->addParameterData( data );
	}

	return 0;
}

/**
 * Decompiles the byte code into reference data and attaches
 * it to the given scope.
 */
int Scope::decompileReferenceData( Scope* scope, CodeBuffer* cb ) {
	int n_classdata = cb->getInt();
	for( int n = 0; n < n_classdata; n++ ) {
		ReferenceData* data = ReferenceData::decompile( cb );
		if( data == NULL ) {
			printf( "Class file is corrupted: [%d] reference data could not be retrieved\n", n+1 );
			return NULL;
		}

		// add the reference data
		scope->addReferenceData( data );
	}

	return 0;
}

/**
 * Returns the method's byte code
 */
CodeBuffer* Scope::getCodeBuffer() {
	return codebuffer;
}

/**
 * Returns the offset of this method in the code
 */
OFFSET_T Scope::getCodeOffset() {
	return codeOffset;
}

/**
 * Sets the offset of this method in the code
 */
void Scope::setCodeOffset( OFFSET_T offset ) {
	this->codeOffset = offset;
}

/**
 * Returns the access modifiers
 */
char Scope::getAccessModifiers() {
	return modifiers;
}

/**
 * Sets the access modifiers
 */
void Scope::setAccessModifiers( char modifiers ) {
	this->modifiers = modifiers;
}

/**
 * Returns the name of the scope
 */
const char* Scope::getName() {
	return name;
}

/**
 * Returns the fully qualified name
 */
const char* Scope::getFullyQualifiedName() {
	return fullName;
}

/**
 * Returns the full name of the scope
 */
const char* Scope::getPackageName() {
	return packageName;
}

/**
 * Returns the parent scope
 */
Scope* Scope::getParentSope() {
	return parentScope;
}

/**
 * Returns the type of scope
 */
ScopeType Scope::getScopeType() {
	return scopeType;
}

/**
 * Indicates whether the scope is abstract
 */
bool Scope::isAbstract() {
	return ( modifiers & ABSTRACT );
}

/**
 * Sets the abstract attribute
 */
void Scope::setAbstract( bool enable ) {
	SET_ATTRIBUTE(modifiers,enable,ABSTRACT)
}

/**
 * Indicates whether the field is private;
 * meaning internal use only
 */
bool Scope::isPrivate() {
	return ( modifiers & PRIVACY_MASK ) == PRIVATE;
}

/**
 * Indicates whether the field is protected;
 * meaning internal use or sub-classes
 */
bool Scope::isProtected() {
	return ( modifiers & PRIVACY_MASK ) == PROTECTED;
}

/**
 * Indicates whether the field is public;
 * meaning any class
 */
bool Scope::isPublic() {
	return ( modifiers & PRIVACY_MASK ) == PUBLIC;
}

/**
 * Indicates whether the field is shared;
 * meaning internal use, sub-classes, and same name space
 */
bool Scope::isPackage() {
	return ( modifiers & PRIVACY_MASK ) == PACKAGE;
}

/**
 * Sets the private attribute
 * @param access the access level (e.g. PRIVATE, PROTECTED, SHARED or PUBLIC)
 */
void Scope::setPrivacy( int mask ) {
	this->modifiers &= 0xCF | mask;
}

/**
 * Indicates whether the field is read-only
 */
bool Scope::isFinal() {
	return modifiers & FINAL;
}

/**
 * Sets the read-only attribute
 */
void Scope::setFinal( bool enable ) {
	SET_ATTRIBUTE(modifiers,enable,FINAL)
}

/**
 * Indicates whether the field is static
 */
bool Scope::isStatic() {
	return modifiers & STATIC;
}

/**
 * Sets the static attribute
 */
void Scope::setStatic( bool enable ) {
	SET_ATTRIBUTE(modifiers,enable,STATIC)
}

/**
 * Builds the fully qualified name of the scope
 */
const char* Scope::buildFullyQualifiedName( const char* name, const char* packageName ) {
	int count;
	if( ( count = strlen( packageName ) ) ) {
		char* data = new char[ strlen(name) + count + 1 ];
		sprintf( data, "%s.%s", packageName, name );
		return data;
	}
	else {
		return name;
	}
}

/**
 * Builds the name of the scope
 */
const char* Scope::buildScopeName( Scope* scope ) {
	std::string pkgname = "";
	for( Scope* p = scope; p != NULL; p = p->getParentSope() ) {
		if( pkgname.length() && strlen( p->getName() ) ) {
			pkgname.append( "." );
		}
		pkgname.append( p->getName() );
	}
	return pkgname.data();
}
