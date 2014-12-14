/*
 * Tokenizer.h
 *
 *  Created on: February 28, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <string.h>

#include "Token.h"

using namespace std;

class Tokenizer {
private:
	char *text;
	bool newLineIsEOL;
	bool ignoreDelimiters;
	int position;
	int lineNumberAdjust;
	int length;

public:
	/**
	 * Default Constructor
	 */
	Tokenizer();

	/**
	 * Destruction
	 */
	virtual ~Tokenizer();

	/**
	 * Indicates whether there is at least one more
	 * element in the stream.
	 */
	bool hasNext();

	/**
	 * Indicates whether the parse is ignoring delimiters (',' and ':')
	 */
	bool ignoresDelimiters();

	/**
	 * Tells the parser whether or not to ignore delimiters
	 */
	void setIgnoreDelimiters( bool state );

	/**
	 * Indicates whether the newline (\n) character is to
	 * be considered the end of an expression.
	 */
	bool isNewLineSignificant();

	/**
	 * Tells the parser whether newline (\n) characters
	 * are to be considered as the end of an expression.
	 */
	void setNewLineSignificance( bool state );

	/**
	 * Returns the next token from the underlying
	 * text string or NULL if none was found.
	 */
	Token* next();

	/**
	 * Returns the next token from the underlying
	 * text string or NULL if none was found.
	 */
	Token* _next();

	/**
	 * Returns the next token from the underlying
	 * text stream without changing the position
	 * of the cursor or NULL if at the end of the stream.
	 */
	Token* peek();

	/**
	 * Sets the starting line number
	 */
	void setLineNumber( int lineNumberAdjust );

	/**
	 * Sets the text that is to be parsed
	 */
	void setText( const char *text );

protected:

	/**
	 * Indicates whether the given character is alphabetic
	 */
	bool isAlpha( char ch );

	/**
	 * Indicates whether the given character is alphanumeric
	 */
	bool isAlphaOrDigit( char ch );

	/**
	 * Indicates whether the given character is alphanumeric
	 */
	bool isDigit( char ch );

	/**
	 * Returns a chunk of alphanumeric text or NULL
	 */
	Token* parseAlphaNumericText();

	/**
	 * Returns a chunk of assembly language block or NULL
	 */
	Token* parseAssemblyBlock();

	/**
	 * Returns a block or NULL
	 */
	Token* parseBlock( char open, char close, tok::TokenType type );

	/**
	 * Returns a bracket block or NULL
	 */
	Token* parseBracketBlock();

	/**
	 * Returns a code block (e.g. "{ .... }") or NULL
	 */
	Token* parseCodeBlock();

	/**
	 * Returns a delimiter or NULL
	 */
	Token* parseDelimiter();

	/**
	 * Returns an end of statement (;) token or NULL
	 */
	Token* parseEndOfStatement();

	/**
	 * Returns a chunk of numeric text or NULL
	 */
	Token* parseNumericText();

	/**
	 * Returns an operator or NULL
	 */
	Token* parseOperator();

	/**
	 * Returns a parenthesis block or NULL
	 */
	Token* parseParenthesisBlock();

	/**
	 * Returns a chunk of double quoted (") data or NULL
	 */
	Token* parseDoubleQuotedText();

	/**
	 * Returns a chunk of single quoted (') data or NULL
	 */
	Token* parseSingleQuotedText();

	/**
	 * Returns a substring from the given string from
	 * the given start to end positions.
	 */
	Token* substring( tok::TokenType type, int start, int end );

	/**
	 * Returns a substring from the given string from
	 * the given start to end positions.
	 */
	string substring( int start, int end );

	/**
	 * Skips over spurious tokens
	 */
	void skipIgnoredSequences();

	/**
	 * Skips over block comments
	 */
	void skipCommentBlock();

	/**
	 * Skips over line comments
	 */
	void skipCommentLine();

	/**
	 * Skips over delimiters: comma(,) and colon(:)
	 */
	void skipDelimiters();

	/**
	 * Skips over whitespace
	 */
	void skipWhiteSpace();

private:

	/**
	 * Returns the line number at the given position
	 */
	int determineLineNumber( int position );

};

#endif /* TOKENIZER_H_ */
