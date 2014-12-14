/*
 * ComplexToken.h
 *
 *  Created on: Mar 6, 2010
 *      Author: ldaniels
 */

#ifndef COMPLEXTOKEN_H_
#define COMPLEXTOKEN_H_

#include "Token.h"
#include "TokenStack.h"

class ComplexToken : public Token {
private:
	TokenStack *stack;

public:

	/**
	 * Default Constructor
	 */
	ComplexToken( tok::TokenType type, int position, int lineNumber );

	/**
	 * Destruction
	 */
	virtual ~ComplexToken();

	/**
	 * Returns a list of the child nodes
	 */
	TokenStack* getChildren();

	/**
	 * Indicates whether child nodes exist
	 */
	bool hasChildren();

protected:

	/**
	 * Adds a child token node to this parent node
	 */
	void add( Token *node );

	/**
	 * Tokenizer is a friend...
	 */
	friend class Tokenizer;

};

#endif /* COMPLEXTOKEN_H_ */
