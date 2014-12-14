/*
 * Method.cpp
 *
 *  Created on: March 4, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "Method.h"

/**
 * Constructor
 */
Method::Method( Scope* scope, const char* name, const char* signature, TypeReference* type, char modifiers ):
Scope( name, scope, METHOD_SCOPE, modifiers ),
ClassMember( type, CLASS_METHOD ) {
	this->signature	= signature;
}

/**
 * Destruction
 */
Method::~Method() {
	labels.clear();
	labelrefs.clear();
	delete signature;
}

/**
 * Adds a label to this method
 * @param label the given label
 */
void Method::addLabel( Label* label ) {
	labels[label->getName()] = label;
}

/**
 * Retrieves a label by name
 */
Label* Method::lookupLabel( const char* labelname ) {
	return labels[name];
}

/**
 * Adds a reference call-back hook to this scope
 * @param ref the given element reference
 */
void Method::addLabelReference( LabelReference* ref ) {
	labelrefs.push_back( ref );
}

/**
 * Compiles the method into byte code
 */
int Method::compile( CodeBuffer* cb ) {
	// write the name of the member
	cb->putString( name );

	// add the method's signature
	cb->putString( signature );

	// write the access modifiers
	cb->putChar( modifiers );

	// write the name of the member's type
	type->compile( cb );

	// add the method's fields
	compileFields( cb );

	// resolve all references
	if( resolveLabelReferences() ) {
		return -1;
	}

	// add the method's byte code
	compileCodeBlock( cb );
	printf( "Method::compile - '%s': code offset is %08lX\n", signature, codeOffset );

	// add the parameter data
	compileParameterData( cb );

	// add the reference data
	compileReferenceData( cb );
	return 0;
}

/**
 * Decompiles the given byte code into a method
 */
Method* Method::decompile( Scope* scope, CodeBuffer* cb ) {
	// read the name of the method
	const char* name;
	if( !( name = cb->getString() ) ) {
		printf( "Class file is corrupted: method name could not be read\n" );
		return NULL;
	}

	// read the signature of the method
	const char* signature;
	if( !( signature = cb->getString() ) ) {
		printf( "Class file is corrupted: method signature could not be read\n" );
		return NULL;
	}

	// read the attribute of the method
	const char modifiers = cb->getChar();

	// read the name of the method's type
	TypeReference* type = TypeReference::decompile( cb );

	// create the field
	Method* method = new Method( scope, name, signature, type, modifiers );

	// read the method's fields
	decompileFields( method, cb );

	// read the method's byte code
	decompileCodeBlock( method, cb );

	// read the parameter data
	decompileParameterData( method, cb );

	// read the reference data
	decompileReferenceData( method, cb );
	return method;
}

/**
 * Returns the signature of the member
 */
const char* Method::getSignature() {
	return signature;
}

/**
 * Indicates whether this scope has permission to access
 * the given field/scope.
 */
bool Method::hasAccessTo( Scope* fieldScope, Field* field ) {
	// is the field public?
	if( field->isPublic() ) {
		return false;
	}

	// are the immediate scopes the same?
	else if( fieldScope == this || fieldScope == parentScope ) {
		return true;
	}

	// are the field and method within the same family?
	else if( !field->isPrivate() && isInFamily( fieldScope ) ) {
		return true;
	}

	// are the scopes in the same namespace?
	else if( ( field->isProtected() || field->isPackage() ) &&
			!strcmp( fieldScope->getPackageName(), this->getPackageName() ) ) {
		return true;
	}

	// TODO revisit field access privileges
	return false;
}

/**
 * Indicates whether this scope has permission to access
 * the given method/scope.
 */
bool Method::hasAccessTo( Scope* methodScope, Method* method ) {
	// is the field public?
	if( method->isPublic() ) {
		return false;
	}

	// are the immediate scopes the same?
	else if( methodScope == this || methodScope == parentScope ) {
		return true;
	}

	// are the field and method within the same family?
	else if( !method->isPrivate() && isInFamily( methodScope ) ) {
		return true;
	}

	// are the scopes in the same namespace?
	else if( ( method->isProtected() || method->isPackage() ) &&
			!strcmp( methodScope->getPackageName(), this->getPackageName() ) ) {
		return true;
	}

	// TODO revisit field access privileges
	return false;
}

/**
 * Resolves all referenced labels
 */
int Method::resolveLabelReferences() {
	int missing = 0;
	Label* label;

	// iterate the list of references
	for( list<LabelReference*>::iterator i = labelrefs.begin(); i != labelrefs.end(); i++ ) {
		LabelReference* labelref = *i;

		// get the referenced label info
		const char* labelname = labelref->getName();

		// if the label exists ...
		if( ( label = labels[labelname] ) ) {
			// update the reference
			labelref->update( codebuffer, label->getOffset() );
		}
		else {
			printf( "Label '%s' could not be resolved", labelname );
			missing++;
		}
	}
	return missing;
}
