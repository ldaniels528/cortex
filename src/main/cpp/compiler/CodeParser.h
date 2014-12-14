/*
 * CodeParser.h
 *
 *  Created on: March 12, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef CODEPARSER_H_
#define CODEPARSER_H_

#include "../OpCodes.h"
#include "../class/Class.h"
#include "../class/Scope.h"
#include "../class/member/AnonymousMethod.h"
#include "../class/member/Method.h"
#include "../class/member/Parameter.h"
#include "../class/member/ParameterData.h"
#include "../token/ComplexToken.h"
#include "../token/TokenStack.h"

#include "AssemblyParser.h"
#include "CompilerState.h"

/**
 * Master parser for the all source code
 */
class CodeParser {
private:
	AssemblyParser* assemblyParser;
	CompilerState* state;
	unsigned int anonSeq;
	char modifiers;

public:

	/**
	 * Constructor
	 */
	CodeParser( CompilerState* state );

	/**
	 * Destruction
	 */
	virtual ~CodeParser();

	/**
	 * Parses the source code
	 */
	int parse( Scope* scope, TokenStack* stack );

private:

	/**
	 * Parses the text contained within the given token stack
	 */
	int parseKeyword( Scope*, TokenStack*, const char* );

	/**
	 * Parses the keyword "class" from the given token stack
	 */
	int parseKeyword_Class( Scope*, TokenStack* );

	/**
	 * Parses the keyword "import" from the given token stack
	 */
	int parseKeyword_Import( Scope*, TokenStack* stack );

	/**
	 * Parses the keyword "namespace" from the given token stack
	 */
	int parseKeyword_NameSpace( Scope*, TokenStack* );

	/**
	 * Parses the "new" keyword
	 */
	int parseKeyword_New( Scope*, TokenStack* );

	/**
	 * Parses the keyword "return" from the given token stack
	 */
	int parseKeyword_Return( Scope*, TokenStack* );

	/**
	 * Parses the text contained within the given token stack
	 */
	int parseClassMember( Scope*, TokenStack* );

	/**
	 * Parses the expression from the given token stack
	 */
	int parseExpression( Scope*, TokenStack* );

	/**
	 * Parses the method call within an expression
	 */
	int parseExpression_MethodCall( const char*, Scope*, TokenStack* );

	/**
	 * Compiles a field definition
	 */
	int parseField( Scope*, const char* name, TypeReference*, TokenStack* );

	/**
	 * Parses a method definition
	 */
	int parseMethod( Scope*, const char* name, TypeReference*, TokenStack* );

	/**
	 * Parses method call parameters
	 */
	int parseMethodParameters( Scope*, TokenStack*, list<Parameter*> );

	/**
	 * Parses an anonymous method code block from the given token
	 */
	int parseAnonymuousCodeBlock( Scope*, Token *block );

	/**
	 * Parses a type definition
	 */
	TypeReference* parseType( Scope*, TokenStack* );

	/**
	 * Generates the method signature
	 */
	const char* generateSignature( const char*, list<Parameter*> params );

	/**
	 * Recursively searches the hierarchy for the instance of
	 * a method having the given name.
	 */
	Method* lookupMethod( Scope*, const char* );

	/**
	 * Returns a sub-stack of elements from the given stack, starting
	 * at the current position of the stack until the end of statement.
	 */
	TokenStack* getStackToEndOfStatement( TokenStack* );

	/**
	 * Sets up code to copy the address of the given data into
	 * the given field at runtime
	 */
	void setupAssignment( Scope*, Field*, const char* );

	/**
	 * Updates the current access modifier
	 */
	int updateModifiers( char, TokenStack* );

};

#endif /* CODEPARSER_H_ */
