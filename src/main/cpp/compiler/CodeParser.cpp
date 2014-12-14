/*
 * CodeParser.cpp
 *
 *  Created on: March 12, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "CodeParser.h"

/**
 * Constructor
 */
CodeParser::CodeParser( CompilerState* state ) {
	this->state				= state;
	this->assemblyParser 	= new AssemblyParser( state );
	this->anonSeq			= 0;
}

/**
 * Destruction
 */
CodeParser::~CodeParser() {
	delete assemblyParser;
}

/**
 * Parses the source code
 */
int CodeParser::parse( Scope* scope, TokenStack* stack ) {
	int errorCode = 0;
	Token *t;

	// get the command arguments
	while( !errorCode && stack->hasNext() && ( t = stack->peek() ) ) {
		//printf( "t = %s[%s]\n", t->getTypeName(), t->getText() );

		// check situationally
		switch( t->getType() ) {
			case tok::CODE_BLOCK:
				errorCode = parseAnonymuousCodeBlock( scope, ( t = stack->next() ) );
				break;

			case tok::ASSEMBLY_BLOCK:
				errorCode = assemblyParser->parse( scope, stack );
				break;

			case tok::END_OF_STATEMENT:
				t = stack->next();
				this->modifiers = 0;
				break;

			case tok::ALPHA_NUMERIC:
				// is the token a keyword?
				if( state->isKeyword( t->getText() ) ) {
					const char *keyword = stack->next()->getText();
					errorCode = parseKeyword( scope, stack, keyword );
				}

				// is it a field/method declaration?
				else if( state->isClass( t->getText() ) ) {
					errorCode = parseClassMember( scope, stack );
				}

				// must be an assignment or method invocation ...
				else {
					errorCode = parseExpression( scope, stack );
				}
				break;

			// anything else ...
			default:
				SYNTAX_ERROR( "unrecognized command", t );
				errorCode = -1;
				break;
		}
	}

	return errorCode;
}

/**
 * Parses the text contained within the given token stack
 */
int CodeParser::parseKeyword( Scope* scope, TokenStack* stack, const char* keyword ) {
	int errorCode = 0;

	// TODO delete, do-while, for, foreach
	// TODO if-else, try-catch, throw, while

	// handle the keyword
	switch( state->lookupKeyword( keyword ) ) {
		case KW_ABSTRACT:	errorCode = updateModifiers( ABSTRACT, stack ); break;
		case KW_CLASS:		errorCode = parseKeyword_Class( scope, stack ); break;
		case KW_FINAL:		errorCode = updateModifiers( FINAL, stack ); break;
		case KW_IMPORT:		errorCode = parseKeyword_Import( scope, stack ); break;
		case KW_NAMESPACE:	errorCode = parseKeyword_NameSpace( scope, stack ); break;
		case KW_NEW:		errorCode = parseKeyword_New( scope, stack ); break;
		case KW_PACKAGED:	errorCode = updateModifiers( PACKAGE, stack ); break;
		case KW_PRIVATE:	errorCode = updateModifiers( PRIVATE, stack ); break;
		case KW_PROTECTED:	errorCode = updateModifiers( PROTECTED, stack ); break;
		case KW_PUBLIC:		errorCode = updateModifiers( PUBLIC, stack ); break;
		case KW_RETURN:		errorCode = parseKeyword_Return( scope, stack ); break;
		case KW_STATIC:		errorCode = updateModifiers( STATIC, stack ); break;
		case KW_VIRTUAL:	errorCode = updateModifiers( VIRTUAL, stack ); break;
		default:
			char *message = new char[256];
			sprintf( message, "Keyword '%s' was not handled", keyword );
			SYNTAX_ERROR( message, stack->last() );
			delete message;
			errorCode = -1;
	}

	// success
	return errorCode;
}

/**
 * Parses the keyword "class" from the given token stack
 */
int CodeParser::parseKeyword_Class( Scope* scope, TokenStack* stack ) {
	Class* classdef;
	const char* name;
	Token* t;

	// validate incoming scope
	if( scope->getScopeType() != NAMESPACE_SCOPE ) {
		SYNTAX_ERROR( "Classes cannot be defined in this scope", stack->last() );
		return -1;
	}

	// the next token must be the identifier
	if( !( name = state->getIdentifierName( stack, "class" ) ) ) {
		return -2;
	}

	// get the name space
	NameSpace* ns = (NameSpace*)scope;
	printf( "class '%s' in namespace '%s' modifiers = %s\n", name, ns->getName(), toBinary( modifiers ) );

	// TODO check for inheritance! multiple inheritance?
	// - class MyChild extends MyParent { ... }
	// - class StrobeLamp extends Lamp, Strobe { ... }

	// create the class
	classdef = new Class( name, ns->getName(), ns, modifiers );
	this->modifiers = 0;

	// add the name space to the state
	if( !ns->addClass( classdef ) ) {
		char* error = new char[256];
		sprintf( error, "Class '%s' already exists in namespace '%s'\n", name, ns->getName() );
		SYNTAX_ERROR( error, stack->last() );
		delete error;
		return -4;
	}

	// the next element must be a code block
	if( !stack->hasNext() || !( t = stack->peek() ) || ( t->getType() != tok::CODE_BLOCK ) ) {
		SYNTAX_ERROR( "unexpected token", t );
		return -5;
	}

	// process the code block
	int errorCode = parse( classdef, ((ComplexToken*)t)->getChildren() );

	// register the class
	state->addClass( classdef );

	// reset the class-level counters
	state->resetCounters();
	return errorCode;
}

/**
 * Parses the keyword "import" from the given token stack
 */
int CodeParser::parseKeyword_Import( Scope* scope, TokenStack* stack ) {
	// gather the imports: e.g. [import "Test1"])
	// the next identifier is a class
	const char *classname = state->getQuotedIdentifier( stack );
	if( classname == NULL ) {
		return -1;
	}

	// load the class
	printf( "Importing class '%s'...\n", classname );
	Class* loadedClass = state->resolveClass( classname );
	if( loadedClass == NULL ) {
		return -2;
	}

	return 0;
}

/**
 * Parses the keyword "namespace" from the given token stack
 */
int CodeParser::parseKeyword_NameSpace( Scope* scope, TokenStack* stack ) {
	const char* name;
	Token* t;

	// validate incoming scope
	if( scope->getScopeType() != NAMESPACE_SCOPE ) {
		SYNTAX_ERROR( "Namespaces cannot be defined in this scope", stack->last() );
		return -1;
	}

	// the next token must be the identifier
	if( !( name = state->getIdentifierName( stack, "namespace" ) ) ) {
		return -1;
	}

	// create the name space
	NameSpace* ns = new NameSpace( name, scope );

	// add the name space to the state
	if( !state->addNameSpace( ns ) ) {
		printf( "Namespace '%s' already exists\n", name );
		return -4;
	}

	// the next element must be a code block
	if( !stack->hasNext() || !( t = stack->peek() ) || ( t->getType() != tok::CODE_BLOCK ) ) {
		SYNTAX_ERROR( "unexpected token", t );
		return -5;
	}

	// process the code block
	return parse( ns, ((ComplexToken*)t)->getChildren() );
}

/**
 * Parses the "new" keyword
 */
int CodeParser::parseKeyword_New( Scope* scope, TokenStack* stack ) {
	int errorCode = 0;

	// parses the object definition: new string("hello");
	// get the object type
	TypeReference* type = parseType( scope, stack );

	// parse the method parameters
	list<Parameter*> params;
	if( ( errorCode = parseMethodParameters( scope, stack, params ) ) ) {
		return errorCode;
	}

	// setup code to create the object on the stack
	//	PARAMS params
	//	NEWINST type
	CodeBuffer* cb = scope->getCodeBuffer();
	OFFSET_T offset1 = cb->putInstruction( PARAMS << A_OP );
	OFFSET_T offset2 = cb->putInstruction( NEWINST << A_OP );

	// add the parameter and reference data
	scope->addParameterData( new ParameterData( state->nextParamId(), params, offset1 ) );
	scope->addReferenceData( new ReferenceData( state->nextDataRefId(), type->getName(), offset2 ) );
	return errorCode;
}

/**
 * Parses the keyword "return" from the given token stack
 */
int CodeParser::parseKeyword_Return( Scope* scope, TokenStack* stack ) {
	// get the scope type
	ScopeType type = scope->getScopeType();
	if( type != METHOD_SCOPE ) {
		SYNTAX_ERROR( "Statement is only permissible within a method block", stack->last() );
		return -1;
	}

	// get the method instance
	Method* method = (Method*)scope;

	// TODO support "return value" syntax

	// add the instruction
	INSTRUCTION_T instruction = RET << A_OP;
	method->getCodeBuffer()->putInstruction( instruction );
	return 0;
}

/**
 * Parses an anonymous method from the given token stack
 */
int CodeParser::parseAnonymuousCodeBlock( Scope* scope, Token *block ) {
	// get the child stack
	TokenStack* stack = ((ComplexToken*)block)->getChildren();

	// generate a unique name
	char* name = new char[31];
	sprintf( name, "$anon%02X", anonSeq++ );

	// create a local scope
	AnonymousMethod* method = new AnonymousMethod( name, scope );

	// parse the block
	return parse( method, stack );
}

/**
 * Parses the class member (field or method) contained within the given token stack
 */
int CodeParser::parseClassMember( Scope* scope, TokenStack* stack ) {
	int errorCode = 0;
	TypeReference* type;
	const char* name;

	// the following types are parsed:
	// 	method layout: "[static] [final] type|[]| methodName(...) { ... }"
	//	field layout:  "[static] [final] type|[]| fieldName| = ....|"

	int step = 0;
	bool done = false;
	while( !done && stack->hasNext() ) {
		switch( ++step ) {
			// step 1: get the type of the class member (e.g. "string" or "string[]")
			case 1:
				// extract the name of the identifier
				if( !( type = parseType( scope, stack ) ) ) {
					return -1;
				}
				break;

			// step 2: get the class member name
			case 2:
				// get the type name
				if( !( name = state->getIdentifierName( stack, "class member" ) ) ) {
					return -2;
				}
				break;

			// step 3: get the class member instance (field or method)
			// 			method layout: "[static] [final] type methodName(...) { ... }"
			//			field layout:  "[static] [final] type fieldName[ = ....]"
			case 3:
				// each child should be a method (e.g. "type methodName(...)") or field (e.g. "type fieldName")
				// if there is a parenthesis block, it's likely to be a method
				errorCode = ( stack->peek()->getType() == tok::PARENTHESIS_BLOCK )
						? parseMethod( scope, name, type, stack )
						: parseField( scope, name, type, stack );

				// was there an error?
				if( errorCode ) {
					return errorCode;
				}
				done = true;
				break;
		}
	}

	// did it complete?
	if( !done ) {
		SYNTAX_ERROR( "unexpected end of statement", stack->last() );
		return -4;
	}

	// return the error code
	return 0;
}

/**
 * Compiles a field declaration
 */
int CodeParser::parseField( Scope* scope, const char* name, TypeReference* type, TokenStack* stack ) {
	int errorCode = 0;
	Token *t1, *t2;
	const char* data;

	// create the field, and attach it to the class
	Field* field = new Field( name, type, this->modifiers );
	printf( "field '%s' type='%s' modifiers = %s\n", name, type->getName(), toBinary( this->modifiers ) );

	// reset the modifier for the current scope
	this->modifiers = 0;

	// is there also an assignment?
	if( stack->hasNext() && !strcmp( stack->peek()->getText(), "=" ) ) {
		// capture the
		t1 = stack->next();

		// there must be another token
		if( !( t2 = stack->next() ) ) {
			SYNTAX_ERROR( "Unexpected end of statement", t1 );
			return NULL;
		}

		switch( t2->getType() ) {
			case tok::SQUOTE_TEXT:;
			case tok::DQUOTE_TEXT:
				data = t2->getText();
				setupAssignment( scope, field, data );
				break;

			default:
				parseExpression( scope, stack );
		}
	}

	// add the field to the scope
	if( !errorCode ) {
		scope->addField( field );
	}

	// TODO there may be an expression: int x = 5 * y + 1;
	return errorCode;
}

/**
 * Parses a method definition
 */
int CodeParser::parseMethod( Scope* scope, const char* name, TypeReference* type, TokenStack* stack ) {
	int errorCode = 0;
	const char* signature;
	list<Parameter*> params;
	Method *method;

	// step through the tokens ...
	int step = 0;
	bool done = false;
	while( !errorCode && !done && stack->hasNext() ) {
		switch( ++step ) {
			// step 1: create the method
			case 1:
				// get the method parameters
				if( !( errorCode = parseMethodParameters( scope, stack, params ) ) ) {
					// create the method signature
					signature = generateSignature( name, params );

					// create the method
					printf( "CodeParser::parseMethod - method '%s' signature = '%s' modifiers = %s\n", name, signature, toBinary( this->modifiers ) );
					method = new Method( scope, name, signature, type, this->modifiers );
				}

				// reset the modifier for the current scope
				this->modifiers = 0;
				break;

			// step 2: process the method code block
			case 2:
				// must be a code block
				if( stack->peek()->getType() != tok::CODE_BLOCK ) {
					SYNTAX_ERROR( "symbol '{' expected", stack->next() );
					return NULL;
				}

				// process the method code block
				errorCode = parse( method, ((ComplexToken*)stack->next())->getChildren() );
				method->getCodeBuffer()->putInstruction( RET << A_OP ); // RET
				done = true;
				break;
		}
	}

	// is there already an error code?
	if( errorCode ) {
		return errorCode;
	}

	// did it complete?
	if( !done ) {
		SYNTAX_ERROR( "unexpected end of statement", stack->last() );
		return -1;
	}

	// add the method to the scope
	if( scope->getScopeType() != CLASS_SCOPE ) {
		SYNTAX_ERROR( "method cannot be defined here", stack->last() );
		return -1;
	}

	// add the method to the scope
	ScopeContainer* container = (ScopeContainer*)scope;
	container->addMethod( method );
	return 0;
}

/**
 * Parses method call parameters
 */
int CodeParser::parseMethodParameters( Scope* scope, TokenStack* stack, list<Parameter*> params ) {
	Token* t;

	// the next token must be a parenthesis block (e.g. "( .. )")
	if( !( t = stack->next() ) || ( t->getType() != tok::PARENTHESIS_BLOCK ) ) {
		SYNTAX_ERROR( "Symbol '(' expected", stack->last() );
		return -1;
	}

	// get the sub-stack
	TokenStack* substack = ((ComplexToken*)t)->getChildren();

	// gather the parameters
	while( ( t = substack->next() ) ) {
		printf( "CodeParser::parseMethodParameters token: %s\n", t->getText() );
		// TODO add method parameter parsing logic here

	}

	return 0;
}

/**
 * Parses the expression from the given token stack
 */
int CodeParser::parseExpression( Scope* scope, TokenStack* stack ) {
	int errorCode = 0;
	Token* t;

	// gather the arguments up to the end of the statement
	TokenStack* substack = getStackToEndOfStatement( stack );

	// get the argument list
	while( !errorCode && substack->hasNext() && ( t = substack->next() ) ) {
		printf( "CodeParser::parseExpression: '%s' [%s]\n", t->getText(), t->getTypeName() );

		// handle the token by type
		switch( t->getType() ) {
			case tok::ALPHA_NUMERIC:
				// is the argument a field?
				if( scope->lookupField( t->getText() ) ) {
					printf( "CodeParser::parseExpression: Field - [%s]\n", t->getText() );
				}

				// is it a method call?
				else if( substack->hasNext() && substack->peek()->getType() == tok::PARENTHESIS_BLOCK ) {
					errorCode = parseExpression_MethodCall( t->getText(), scope, substack );
				}
				break;

			case tok::DECIMAL:
			case tok::INTEGER:
				break;

			case tok::DQUOTE_TEXT:
				break;

			case tok::SQUOTE_TEXT:
				break;

			case tok::OPERATOR:
				break;

			case tok::PARENTHESIS_BLOCK:
				break;

			default:
				errorCode = -1;
				SYNTAX_ERROR( "unexpected token", substack->last() );
		}
	}

	// delete the stack
	delete substack;

	// return the error code
	return errorCode;
}

/**
 * Parses the method call within an expression
 */
int CodeParser::parseExpression_MethodCall( const char* methodName, Scope* scope, TokenStack* stack ) {
	int errorCode = 0;
	list<Parameter*> params;

	// parse the method parameters
	if( ( errorCode = parseMethodParameters( scope, stack, params ) ) ) {
		return errorCode;
	}

	// generate the method signature
	const char* signature = generateSignature( methodName, params );

	// lookup the method
	Method* method = lookupMethod( scope, signature );
	if( method == NULL ) {
		// generate the error message
		char* errorMessage = new char[256];
		sprintf( errorMessage, "Method '%s' could not be found", signature );

		// display the error message
		SYNTAX_ERROR( errorMessage, stack->last() );
		delete errorMessage;
		return -1;
	}
	else {
		printf( "CodeParser::parseExpression: Method call - [%s]\n", signature );

		// get the code buffer
		CodeBuffer* cb = scope->getCodeBuffer();

		// parse the method parameter


		// setup the method call
		// instruction: PARAMS methodParams
		// instruction: INVOKE methodSig
		OFFSET_T offset1 = cb->putInstruction( PARAMS << A_OP );
		OFFSET_T offset2 = cb->putInstruction( INVOKE << A_OP );

		// add the parameter and reference data
		scope->addParameterData( new ParameterData( state->nextParamId(), params, offset1 ) );
		scope->addReferenceData( new ReferenceData( state->nextDataRefId(), signature, offset2 ) );
		return 0;
	}
}

/**
 * Parses a type definition
 */
TypeReference* CodeParser::parseType( Scope* scope, TokenStack* stack ) {
	const char* typeName;
	TypeReference* type;
	Token *t;

	// the next element must be alphanumeric
	if( !( t = stack->next() ) ||
		 ( t->getType() != tok::ALPHA_NUMERIC ) ||
		 !( typeName = t->getText() ) ||
		 !state->isClass( typeName ) ) {
		SYNTAX_ERROR( "type identifier expected", stack->last() );
		return NULL;
	}

	// create the type
	type = new TypeReference( typeName );

	// is the type an array? (e.g. "int[] field")
	if( stack->hasNext() &&
		( stack->peek()->getType() == tok::BRACKET_BLOCK ) &&
		( t = stack->next() ) ) {
		// TODO finish adding logic here
		type->setArray( true );
		printf( "array = '%s'\n", t->getText() );
	}

	// return the class reference
	return type;
}

/**
 * Recursively searches the scope hierarchy for the instance of
 * a method having the given name.
 */
Method* CodeParser::lookupMethod( Scope* scope, const char* signature ) {
	// is the scope a class scope?
	if( scope->getScopeType() == CLASS_SCOPE ) {
		ScopeContainer* container = (ScopeContainer*)scope;
		Method* method = container->lookupMethod( signature );
		if( method != NULL ) {
			return method;
		}
	}

	// look for the method via the parent scope
	Scope* parentScope = scope->getParentSope();
	return ( parentScope != NULL ) ? lookupMethod( parentScope, signature ) : NULL;
}

/**
 * Generates the method signature
 */
const char* CodeParser::generateSignature( const char *name, list<Parameter*> params ) {
	string* signature = new string();

	// append the method name
	signature->append( name );

	// if there are method parameters, include them
	if( !params.empty() ) {
		// add the starting parenthesis
		signature->append( "(" );

		// add the parameters
		int n = 0;
		for( list<Parameter*>::iterator i = params.begin(); i != params.end(); i++ ) {
			Parameter* p = *i;

			// add a comma
			if( n++ > 0 ) {
				signature->append( "," );
			}

			// append the data type
			signature->append( p->getType()->getName() );
		}

		// add the ending parenthesis
		signature->append( ")" );
	}

	// return the signature
	const char *data = strdup( signature->data() );
	delete signature;
	return data;
}

/**
 * Returns a sub-stack of elements from the given stack, starting
 * at the current position of the stack until the end of statement.
 */
TokenStack* CodeParser::getStackToEndOfStatement( TokenStack* stack ) {
	Token* t;

	// gather the arguments up to the end of the statement
	TokenStack* substack = new TokenStack();
	while( ( t = stack->next() ) && ( t->getType() != tok::END_OF_STATEMENT ) ) {
		substack->pushFront( t );
	}
	return substack;
}

/**
 * Sets up code to copy the address of the given data into
 * the given field at runtime
 */
void CodeParser::setupAssignment( Scope* scope, Field* field, const char* value ) {
	// get scope's code buffer
	CodeBuffer* cb = scope->getCodeBuffer();

	// setup the assignment: field = data (e.g. name = "results")
	OFFSET_T offset1 = cb->putInstruction( STAGE << A_OP | ARG_DIRECT << A_ARG );	// STAGE data
	OFFSET_T offset2 = cb->putInstruction( SETPTR << A_OP | ARG_PTR << A_ARG );		// SETPTR field

	// setup the reference data
	scope->addReferenceData( new ReferenceData( state->nextDataRefId(), value, offset1 ) );
	scope->addReferenceData( new ReferenceData( state->nextDataRefId(), field->getName(), offset2 ) );
}

/**
 * Updates the current access modifier
 */
int CodeParser::updateModifiers( char newModifier, TokenStack* stack ) {
	// capture the unmodified state
	const char oldModifier = modifiers;

	// update the modifiers
	modifiers |= newModifier;

	// error if no change
	if( ( newModifier != 0 ) && ( modifiers == oldModifier ) ) {
		SYNTAX_ERROR( "duplicate modifier", stack->last() );
		return -1;
	}

	return 0;
}
