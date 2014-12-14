/*
 * TokenStack.cpp
 *
 *  Created on: March 6, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "TokenStack.h"

/**
 * TokenStack Constructor
 */
TokenStack::TokenStack() {
	this->tok = NULL;
}

/**
 * TokenStack Constructor
 */
TokenStack::TokenStack( Tokenizer* tok ) {
	this->tok = tok;
}

/**
 * Release resources
 */
TokenStack::~TokenStack() {
	stack.clear();
}

/**
 * Removes all tokens from the stack
 */
void TokenStack::clear() {
	stack.clear();
}

/**
 * Indicates whether there is at least
 * one more element in the token parser
 * or on the stack.
 */
bool TokenStack::hasNext() {
	return ( stack.size() > 0 ) || ( tok != NULL && tok->hasNext() );
}

/**
 * Returns the last encountered non-NULL token
 */
Token* TokenStack::last() {
	if( latest == NULL ) {
		latest = peek();
	}
	return latest;
}

/**
 * Returns the next token either from the stack
 * or underlying token parser.
 */
Token* TokenStack::next() {
	// get the next token
	Token *t = ( stack.size() > 0 ) ? pop() :  ( tok != NULL ? tok->next() : NULL );

	// record the latest token
	if( t != NULL ) {
		latest = t;
	}
	return t;
}

/**
 * Returns the next token either from the stack
 * or underlying token parser.
 */
TokenStack* TokenStack::operator ++ ( int index ) {
	next();
	return this;
}

/**
 * Pushes a token onto the stack
 */
void TokenStack::pushBack( Token* token ) {
	stack.push_back( token );
}

/**
 * Pushes the given token onto the stack
 * in such a way that it is first.
 */
void TokenStack::pushFront( Token* token ) {
	stack.push_front( token );
}

/**
 * Pops a token off the stack
 */
Token* TokenStack::pop() {
	// get the last value on the stack
	Token *token = stack.back();

	// pop it off
	stack.pop_back();

	// if not null, hold on it to
	if( token != NULL ) {
		latest = token;
	}

	return token;
}

/**
 * Retrieves a token from the stack without removing it
 */
Token* TokenStack::peek() {
	// is there a token on the stack?
	if( stack.size() > 0 ) {
		latest = stack.back();
		return latest;
	}

	// is there a token in the parser?
	else if( tok->hasNext() ) {
		latest = tok->next();
		pushBack( latest );
		return latest;
	}

	// nothing ...
	else return NULL;
}

/**
 * Resets the stack to empty
 */
void TokenStack::reset() {
	clear();
}

/**
 * Returns the number of tokens currently on the stack
 */
int TokenStack::size() {
	return stack.size();
}

/**
 * Returns a sub-stack of all elements up to the given type
 */
TokenStack* TokenStack::subStack( tok::TokenType type ) {
	Token *t;
	TokenStack* t_stack = new TokenStack();
	printf( "TokenStack::subStack - gathering data\n" );
	while( ( t = this->next() ) && ( t->getType() != type ) ) {
		printf( "TokenStack::subStack - |%s|\n", t->getText() );
		t_stack->pushFront( t );
	}
	printf( "TokenStack::subStack - leaving\n" );
	return t_stack;
}
