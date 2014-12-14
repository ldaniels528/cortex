/*
 * Scope.h
 *
 *  Created on: March 10, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef SCOPE_H_
#define SCOPE_H_

#include <hash_map.h>
#include <vector.h>

#include "../CodeBuffer.h"

#include "member/Field.h"
#include "member/ParameterData.h"
#include "ReferenceData.h"

/**
 * Represents an enumeration of Scope types
 */
enum ScopeType {
	NAMESPACE_SCOPE, CLASS_SCOPE, METHOD_SCOPE
};

/**
 * Represents a scope where objects and method may be defined.
 */
class Scope {
protected:
	STRING_HASH_MAP(Field*) fields;
	vector<ParameterData*> parameterdata;
	vector<ReferenceData*> referencedata;
	CodeBuffer* codebuffer;
	Scope* parentScope;
	ScopeType scopeType;
	OFFSET_T codeOffset;
	const char* name;
	const char* fullName;
	const char* packageName;
	char modifiers;
	int datarefgen;

public:

	/**
	 * Default Constructor
	 */
	Scope( const char* name, Scope* parentScope, ScopeType type, char modifiers = PUBLIC );

	/**
	 * Destruction
	 */
	virtual ~Scope();

	/**
	 * Indicates whether the given scope falls within
	 * the ancestry of the current scope
	 */
	bool isInFamily( Scope* );

	/**
	 * Adds the field to the method
	 * @param field - the given method argument
	 */
	virtual bool addField( Field* );

	/**
	 * Returns the fields contained with this scope
	 */
	vector<Field*> getFields();

	/**
	 * Retrieves a field by name
	 * @param name the name of the desired field
	 */
	Field* lookupField( const char* );

	/**
	 * Adds the given parameter data to the class
	 * @param data the parameter data
	 */
	void addParameterData( ParameterData* );

	/**
	 * Adds the given reference data to the class
	 * @param data the reference data
	 */
	void addReferenceData( ReferenceData* );

	/**
	 * Returns the set of all reference data
	 */
	vector<ReferenceData*> getReferenceDataList();

	/**
	 * Lookups a specific piece of data via the given parameter ID
	 */
	ParameterData* lookupParameterData( unsigned int );

	/**
	 * Lookups a specific piece of data via the given reference ID
	 */
	ReferenceData* lookupReferenceData( unsigned int );

	/**
	 * Writes the compiled code to the buffer
	 */
	int compileCodeBlock( CodeBuffer* );

	/**
	 * Compiles the fields into byte code
	 */
	int compileFields( CodeBuffer* );

	/**
	 * Compiles the parameter data into byte code
	 */
	int compileParameterData( CodeBuffer* );

	/**
	 * Compiles the reference data into byte code
	 */
	int compileReferenceData( CodeBuffer* );

	/**
	 * Imports the byte code block from the buffer, and attaches
	 * it to the given scope.
	 */
	static int decompileCodeBlock( Scope*, CodeBuffer* );

	/**
	 * Decompiles the byte code into fields and attaches
	 * them to the given scope.
	 */
	static int decompileFields( Scope*, CodeBuffer* );

	/**
	 * Decompiles the byte code into parameter data and attaches
	 * it to the given scope.
	 */
	static int decompileParameterData( Scope*, CodeBuffer* );

	/**
	 * Decompiles the byte code into reference data and attaches
	 * it to the given scope.
	 */
	static int decompileReferenceData( Scope*, CodeBuffer* );

	/**
	 * Returns the method's byte code
	 */
	CodeBuffer* getCodeBuffer();

	/**
	 * Returns the offset of this method in the code
	 */
	OFFSET_T getCodeOffset();

	/**
	 * Sets the offset of this method in the code
	 */
	void setCodeOffset( OFFSET_T );

	/**
	 * Returns the access modifiers
	 */
	char getAccessModifiers();

	/**
	 * Sets the access modifiers
	 */
	void setAccessModifiers( char );

	/**
	 * Returns the name of the name space
	 */
	const char* getName();

	/**
	 * Returns the fully qualified name
	 */
	const char* getFullyQualifiedName();

	/**
	 * Returns the full name of the name space
	 */
	const char* getPackageName();

	/**
	 * Returns the parent scope
	 */
	Scope* getParentSope();

	/**
	 * Returns the type of scope
	 */
	ScopeType getScopeType();

	/**
	 * Indicates whether the scope is abstract
	 */
	bool isAbstract();

	/**
	 * Sets the abstract attribute
	 */
	void setAbstract( bool );

	/**
	 * Indicates whether the scope is private;
	 * meaning internal use only
	 */
	bool isPrivate();

	/**
	 * Indicates whether the scope is protected;
	 * meaning internal use or sub-classes
	 */
	bool isProtected();

	/**
	 * Indicates whether the scope's access level is package;
	 * meaning internal use, sub-classes, or packaged with
	 * (same name space as) the accessing class.
	 */
	bool isPackage();

	/**
	 * Indicates whether the scope is public;
	 * meaning any class
	 */
	bool isPublic();

	/**
	 * Sets the private attribute
	 * @param access the access level (e.g. PUBLIC, PRIVATE, PROTECTED, or PACKAGED)
	 */
	void setPrivacy( int access );

	/**
	 * Indicates whether the scope is read-only
	 */
	bool isFinal();

	/**
	 * Sets the read-only attribute
	 */
	void setFinal( bool enable );

	/**
	 * Indicates whether the scope is static
	 */
	bool isStatic();

	/**
	 * Sets the static attribute
	 */
	void setStatic( bool enable );

private:

	/**
	 * Builds the fully qualified name of the scope
	 */
	const char* buildFullyQualifiedName( const char*, const char* );

	/**
	 * Builds the full name of the scope
	 */
	const char* buildScopeName( Scope* scope );

};

#endif /* SCOPE_H_ */
