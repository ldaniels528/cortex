/*
 * Token.cpp
 *
 *  Created on: February 28, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include <stdio.h>

#include "Token.h"

/**
 * Default Constructor
 */
Token::Token( tok::TokenType type, int position, int lineNumber ) {
	this->type 			= type;
	this->position		= position;
	this->lineNumber	= lineNumber;
}

/**
 * Constructor
 *	_text: the host string that is to be parsed.
 *	_position: the position of the token within the string
 */
Token::Token( tok::TokenType type, int position, int lineNumber, std::string text ) {
	this->type			= type;
	this->position		= position;
	this->lineNumber	= lineNumber;
	this->text 			= text;
}

/**
 * Destruction
 */
Token::~Token() {
	//delete text;
}

/**
 * Returns: the line number of the text substring from the original host text
 */
int Token::getLineNumber() {
	return lineNumber;
}

/**
 * Returns: the position of the text substring from the original host string
 */
int Token::getPosition() {
	return position;
}

/**
 * Returns: the text contained within the token
 */
const char* Token::getText() {
	return text.data();
}

/**
 * Returns: the text contained within the token
 */
std::string Token::getString() {
	return text;
}

/**
 * Set the text contained within the token
 */
void Token::setText( std::string text ) {
	this->text = text;
}

/**
 * Returns: the token type
 */
tok::TokenType Token::getType() {
	return type;
}

/**
 * Returns: the token type name
 */
const char *Token::getTypeName() {
	static const char *TYPE_NAMES[] = {
		"ALPHA_NUMERIC", "INTEGER", "DECIMAL", "DQUOTE_TEXT", "SQUOTE_TEXT",
		"OPERATOR", "DELIMITER", "END_OF_STATEMENT",
		"ASSEMBLY_BLOCK", "CODE_BLOCK", "BRACKET_BLOCK", "PARENTHESIS_BLOCK"
	};
	return TYPE_NAMES[type];
}
