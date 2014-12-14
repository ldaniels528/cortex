/*
 * ComplexToken.cpp
 *
 *  Created on: Mar 6, 2010
 *      Author: ldaniels
 */

#include "ComplexToken.h"
#include "TokenStack.h"

/**
 * Default Constructor
 */
ComplexToken::ComplexToken( tok::TokenType type, int position, int lineNumber ) : Token( type, position, lineNumber ) {
	this->stack = new TokenStack();
}

/**
 * Destruction
 */
ComplexToken::~ComplexToken() {
	delete stack;
}

/**
 * Adds a child token node to this parent node
 */
void ComplexToken::add( Token* node ) {
	stack->pushFront( node );
}

/**
 * Returns a list of the child nodes
 */
TokenStack* ComplexToken::getChildren() {
	return stack;
}

/**
 * Indicates whether child nodes exist
 */
bool ComplexToken::hasChildren() {
	return stack->size() > 0;
}
