/*
 * Method.h
 *
 *  Created on: March 4, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef METHOD_H_
#define METHOD_H_

#include <hash_map.h>
#include <string.h>

#include "../../Common.h"
#include "../../CodeBuffer.h"

#include "../Scope.h"
#include "../TypeReference.h"

#include "ClassMember.h"
#include "Field.h"
#include "Label.h"
#include "LabelReference.h"

/**
 * Represents a class method
 */
class Method : public Scope, public ClassMember {
private:
	STRING_HASH_MAP(Label*) labels;
	list<LabelReference*> labelrefs;
	const char* signature;

public:

	/**
	 * Constructor
	 */
	Method( Scope* scope, const char* name, const char* signature, TypeReference* type, char modifiers );

	/**
	 * Destruction
	 */
	virtual ~Method();

	/**
	 * Adds a label to this method
	 */
	void addLabel( Label* );

	/**
	 * Retrieves a label by name
	 */
	Label* lookupLabel( const char* );

	/**
	 * Adds a label reference call-back hook to this scope
	 */
	void addLabelReference( LabelReference* );

	/**
	 * Compiles the method into byte code
	 */
	int compile( CodeBuffer* cb );

	/**
	 * Decompiles the given byte code into a method
	 */
	static Method* decompile( Scope*, CodeBuffer* );

	/**
	 * Returns the signature of the member
	 */
	const char* getSignature();

	/**
	 * Indicates whether this scope has permission to access
	 * the given field.
	 */
	bool hasAccessTo( Scope*, Field* );

	/**
	 * Indicates whether this scope has permission to access
	 * the given scope.
	 */
	bool hasAccessTo( Scope*, Method* );

	/**
	 * Resolves all referenced entities (e.g. labels, fields, etc.)
	 */
	int resolveLabelReferences();

};

#endif /* METHOD_H_ */
