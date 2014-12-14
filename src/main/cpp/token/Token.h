/*
 * Token.h
 *
 *  Created on: February 28, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef TOKEN_H_
#define TOKEN_H_

#include <iostream.h>

namespace tok {

	/**
	 * Token Type Enumeration
	 */
	enum TokenType {
		ALPHA_NUMERIC, INTEGER, DECIMAL, DQUOTE_TEXT, SQUOTE_TEXT,
		OPERATOR, DELIMITER, END_OF_STATEMENT,
		ASSEMBLY_BLOCK, CODE_BLOCK, BRACKET_BLOCK, PARENTHESIS_BLOCK
	};
}

/**
 * Token Parser Framework Token
 */
class Token {
protected:
	tok::TokenType type;
	std::string text;
	int lineNumber;
	int position;

public:

	/**
	 * Constructor
	 *	 text: the host string that is to be parsed.
	 *	 position: the position of the token within the string
	 */
	Token( tok::TokenType type, int position, int lineNumber, std::string text );

	/**
	 * Releases all resources
	 */
	virtual ~Token();

	/**
	 * Returns: the line number of the text substring from the original host text
	 */
	int getLineNumber();

	/**
	 * Returns: the position of the text substring from the original host text
	 */
	int getPosition();

	/**
	 * Returns: the text contained within the token
	 */
	const char* getText();

	/**
	 * Returns: the text contained within the token
	 */
	std::string getString();

	/**
	 * Returns: the token type
	 */
	tok::TokenType getType();

	/**
	 * Returns: the token type name
	 */
	const char *getTypeName();

protected:

	/**
	 * Default Constructor
	 */
	Token( tok::TokenType type, int position, int lineNumber );

	/**
	 * Set the text contained within the token
	 */
	void setText( std::string text );

	// friendships
	friend class Tokenizer;
};

#endif /* TOKEN_H_ */
