/*
 * TokenStack.h
 *
 *  Created on: March 6, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef TOKENSTACK_H_
#define TOKENSTACK_H_

#include <list.h>

#include "Token.h"
#include "Tokenizer.h"

/**
 * Represents a token-based stack
 */
class TokenStack {
private:
	list<Token*> stack;
	Tokenizer* tok;
	Token* latest;

public:

	/**
	 * Default Constructor
	 */
	TokenStack();

	/**
	 * Default Constructor
	 */
	TokenStack( Tokenizer* );

	/**
	 * Release resources
	 */
	virtual ~TokenStack();

	/**
	 * Removes all tokens from the stack
	 */
	void clear();

	/**
	 * Indicates whether there is at least
	 * one more element in the token parser
	 * or on the stack.
	 */
	bool hasNext();

	/**
	 * Returns the last encountered non-NULL token
	 */
	Token* last();

	/**
	 * Returns the next token either from the stack
	 * or underlying token parser.
	 */
	Token* next();

	/**
	 * Returns the next token either from the stack
	 * or underlying token parser.
	 */
	TokenStack* operator ++ ( int );

	/**
	 * Pushes a token onto the stack
	 */
	void pushBack( Token* );

	/**
	 * Pushes the given token onto the stack
	 * in such a way that it is first.
	 */
	void pushFront( Token* );

	/**
	 * Pops a token off the stack
	 */
	Token* pop();

	/**
	 * Retrieves a token from the stack without removing it
	 */
	Token* peek();

	/**
	 * Resets the stack to empty
	 */
	void reset();

	/**
	 * Returns the number of tokens currently on the stack
	 */
	int size();

	/**
	 * Returns a sub-stack of all elements up to the given type
	 */
	TokenStack* subStack( tok::TokenType );

};

#endif /* TOKENSTACK_H_ */
