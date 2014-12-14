/*
 * Tokenizer.cpp
 *
 *  Created on: February 28, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include <string.h>

#include "ComplexToken.h"
#include "Tokenizer.h"

using namespace tok;

//#define DEBUG true

/**
 * Default Constructor
 */
Tokenizer::Tokenizer() {
	this->ignoreDelimiters	= false;
	this->newLineIsEOL 		= false;
}

/**
 * Destruction
 */
Tokenizer::~Tokenizer() {
	// do nothing
}

/**
 * Indicates whether there is at least one more
 * element in the stream.
 */
bool Tokenizer::hasNext() {
	// fail safe
	if( text == NULL ) {
		return false;
	}

	// skip pass the stuff we can ignore
	skipIgnoredSequences();

	// is there still something there?
	return ( position < length );
}

/**
 * Returns the next token from the underlying
 * text string or NULL if none was found.
 */
Token* Tokenizer::next() {
	Token* t = _next();
	if( t != NULL ) {
		//printf( "Tokenizer::next - |%s|\n", t->getText() );
	}
	return t;
}

/**
 * Returns the next token from the underlying
 * text string or NULL if none was found.
 */
Token* Tokenizer::_next() {
	// skip spurious tokens
	skipIgnoredSequences();

	// are we at the end of the string
	if( position >= length ) {
		return NULL;
	}

	// determine the next token
	Token* t;
	if( ( t = parseEndOfStatement() ) ) return t;
	else if( ( t = parseAssemblyBlock() ) ) return t;
	else if( ( t = parseBracketBlock() ) ) return t;
	else if( ( t = parseCodeBlock() ) ) return t;
	else if( ( t = parseParenthesisBlock() ) ) return t;
	else if( ( t = parseDoubleQuotedText() ) ) return t;
	else if( ( t = parseSingleQuotedText() ) ) return t;
	else if( ( t = parseOperator() ) ) return t;
	else if( ( t = parseDelimiter() ) ) return t;
	else if( ( t = parseNumericText() ) ) return t;
	else return parseAlphaNumericText();
}

/**
 * Returns the next token from the underlying
 * text stream without changing the position
 * of the cursor or NULL if at the end of the stream.
 */
Token* Tokenizer::peek() {
	// save the current position
	const int current = position;

	// get the next token
	Token* t = next();

	// restore the old position
	position = current;

	// return the token
	return t;
}

/**
 * Indicates whether the parse is ignoring delimiters (',' and ':')
 */
bool Tokenizer::ignoresDelimiters() {
	return ignoreDelimiters;
}

/**
 * Tells the parser whether or not to ignore delimiters
 */
void Tokenizer::setIgnoreDelimiters( bool ignoreDelimiters ) {
	this->ignoreDelimiters = ignoreDelimiters;
}

/**
 * Returns whether the newline (\n) character is to
 * be considered the end of an expression.
 */
bool Tokenizer::isNewLineSignificant() {
	return newLineIsEOL;
}

/**
 * Indicates to the parser whether newline (\n) characters
 * are to be considered as the end of an expression.
 */
void Tokenizer::setNewLineSignificance( bool newLineIsEOL ) {
	this->newLineIsEOL = newLineIsEOL;
}

/**
 * Sets the starting line number
 */
void Tokenizer::setLineNumber( int lineNumber ) {
	this->lineNumberAdjust = lineNumber;
}

/**
 * Sets the text that is to be parsed
 */
void Tokenizer::setText( const char *textdata ) {
	this->text 				= strdup( textdata );
	this->length 			= strlen( text );
	this->lineNumberAdjust 	= 0;
	this->position 			= 0;
}

/**
 * Indicates whether the given character is alphabetic
 */
bool Tokenizer::isAlpha( char ch ) {
	return ( ch >= 'A' && ch <= 'Z' ) || ( ch >= 'a' && ch <= 'z' );
}

/**
 * Indicates whether the given character is alphanumeric
 */
bool Tokenizer::isAlphaOrDigit( char ch ) {
	return isAlpha( ch ) || isDigit( ch );
}

/**
 * Indicates whether the given character is alphanumeric
 */
bool Tokenizer::isDigit( char ch ) {
	return ( ch >= '0' && ch <='9' ) ;
}

/**
 * Returns a chunk of alphanumeric string data or NULL
 */
Token* Tokenizer::parseAlphaNumericText() {
	// capture the starting position
	int start = position;

	// while there is alphanumeric text, capture it
	while( ( position < length ) &&
		   ( isAlphaOrDigit( text[position] ) || text[position] == '_' ) ) position++;

	// return the outcome
	return ( start != position ) ? substring( ALPHA_NUMERIC, start, position ) : NULL;
}

/**
 * Returns a chunk of assembly language or NULL
 */
Token* Tokenizer::parseAssemblyBlock() {
	// does it start with an assembly language indicator ("%%")?
	if( ( position + 1 < length ) &&
			( text[position] == '%' && text[position+1] == '%' ) ) {
		// capture the starting position
		int start = position;

		// skip ahead
		position += 2;

		// continue scanning for the end block
		while( ( position + 1 < length ) &&
				( text[position] != '%' || text[position+1] != '%' ) ) position++;

		// adjust the ending position
		position += 2;

		// return the token
		return substring( ASSEMBLY_BLOCK, start, position++ );
	}
	return NULL;
}

/**
 * Returns a parameter block or NULL
 */
Token* Tokenizer::parseBlock( char open, char close, TokenType type ) {
	// is the next token a parenthesis block?
	if( text[position] == open ) {
		// capture the start of the block
		int start = position++;

		// create a new node
		ComplexToken *block = new ComplexToken( type, start, determineLineNumber( position) );

		// search until the closing block is found
		while( ( position < length ) && ( text[position] != close ) ) {
			// evaluate the next token
			Token *node = next();

			// attach the next node to this one
			if( node != NULL ) {;
				block->add( node );
			}
		}

		// capture the ending position
		int end = ++position;

		// set the content of this block
		block->setText( substring( start, end ) );
		return block;
	}
	return NULL;
}

/**
 * Returns a bracket block or NULL
 */
Token* Tokenizer::parseBracketBlock() {
	return parseBlock( '[', ']', BRACKET_BLOCK );
}

/**
 * Returns a code block (e.g. "{ .... }") or NULL
 */
Token* Tokenizer::parseCodeBlock() {
	return parseBlock( '{', '}', CODE_BLOCK );
}

/**
 * Returns a delimiter or NULL
 */
Token* Tokenizer::parseDelimiter() {
	if( !ignoreDelimiters ) {
		// capture the starting position
		int start = position;

		// is it a delimiter?
		switch( text[position] ) {
			case ',':;
			case ':':
				position++;
				break;
		}

		// return the delimiter
		if( start != position ) {
			return substring( DELIMITER, start, position );
		}
	}
	return NULL;
}

/**
 * Returns an end of statement (';' or '\n') token or NULL
 */
Token* Tokenizer::parseEndOfStatement() {
	// capture the starting position
	int start = position;

	// find all consecutive EOS characters
	while( ( position < length ) &&
		   ( ( text[position] == ';' ) ||
			 ( newLineIsEOL && text[position] == '\n' ) ) ) {
		position++;
	}

	// return the result
	return ( start != position ) ? substring( END_OF_STATEMENT, start, position ) : NULL;
}

/**
 * Returns a chunk of numeric text or NULL
 */
Token* Tokenizer::parseNumericText() {
	// capture the starting position
	int start = position;

	// while there is numeric text, capture it
	while( ( position < length ) &&
			isDigit( text[position] ) ) position++;

	// let's assume it's an integer value
	TokenType type = INTEGER;

	// was something captured?
	if( start != position  ) {
		////////////// DECIMAL? /////////////////
		// is the next character a '.' (decimal)?
		if( text[position] == '.' ) {
			type = DECIMAL;

			// while there is numeric text, capture it
			while( ( ++position < length ) &&
					isDigit( text[position] ) ) { }
		}

		///////////// NO MORE ALPHANUMERIC VALUES //////
		// from this point no alphanumeric text is allow
		if( ( position < length ) && isAlphaOrDigit( text[position] ) ) {
			// look like we've found an exception...
			// let's back out
			position = start;
		}
	}

	// return the outcome
	return ( start != position ) ? substring( type, start, position ) : NULL;
}

/**
 * Returns an operator or NULL
 */
Token* Tokenizer::parseOperator() {
	bool composite = false;
	int start = position;

	// is it a bitwise, math, or logical operator?
	switch( text[position] ) {
		// composite operators
		case '=':;	// equal
		case '!':;	// not equal
		case '+':;	// addition
		case '-':;	// subtraction
		case '*':;	// multiplication
		case '/':;	// division
		case '%':;	// modulus
		case '>':;	// greater
		case '<':;	// lesser
		case '&':;	// bitwise AND
		case '|':	// bitwise OR
			position++;
			composite = true;
			break;

		// non-composite operators
		case '\\':;
		case '~':;
		case '@':;
		case '$':;
		case '^':;
		case '#':;
		case '?':
			position++;
			break;

		// not an operator
		default:
			return NULL;
	}

	// is it a composite-capable operator?
	if( composite && ( position < length ) ) {
		// it is a special case composite operator ('==', '&&', '||', '>>', '<<', '++', or '--' )?
		if( text[position-1] == text[position] ) {
			switch( text[position] ) {
				case '=':;	// equality
				case '&':;	// logical OR
				case '|':;	// logical AND
				case '>':;	// bitwise SHIFT RIGHT
				case '<':;	// bitwise SHIFT LEFT
				case '+':;	// increment
				case '-':;	// decrement
					position++;
					break;
			}
		}

		// is it a composite assignment (e.g. '+=', '-=', '*=', '/=', '!=', etc )?
		else if( text[position] == '=' ) { position++; }
	}

	// return the operator string
	return substring( OPERATOR, start, position );
}

/**
 * Returns a chunk of double quoted (") data or NULL
 */
Token* Tokenizer::parseDoubleQuotedText() {
	// is it a double quote?
	if( text[position] == '"' ) {
		// capture the starting position
		int start = ++position;

		// capture everything in the middle
		while( ( position < length ) && ( text[position] != '"' ) ) {
			position++;
		}

		// return the token
		return substring( DQUOTE_TEXT, start, position++ );
	}
	return NULL;
}

/**
 * Returns a chunk of single quoted (') data or NULL
 */
Token* Tokenizer::parseSingleQuotedText() {
	// is it a single quote?
	if( text[position] == '\'' ) {
		// capture the starting position
		int start = ++position;

		// capture everything in the middle
		while( ( position < length ) && ( text[position] != '\'' ) ) {
			position++;
		}

		// return the token
		return substring( SQUOTE_TEXT, start, position++ );
	}
	return NULL;
}

/**
 * Returns a parenthesis block or NULL
 */
Token* Tokenizer::parseParenthesisBlock() {
	return parseBlock( '(', ')', PARENTHESIS_BLOCK );
}

/**
 * Returns a substring from the given string from
 * the given start to end positions.
 */
Token* Tokenizer::substring( TokenType type, int start, int end ) {
	return new Token( type, start, determineLineNumber( end ), substring( start, end ) );
}

/**
 * Returns a substring from the given string from
 * the given start to end positions.
 */
std::string Tokenizer::substring( int start, int end ) {
	// determine the size of the span
	int size = end - start;
	if( size < 0 ) {
		size = 0;
	}

	// get a pointer to the source string
	const char* src = &text[start];

	// create a destination string
	char* dst = new char[size + 1];

	// copy the string
	memcpy( dst, src, size );

	// terminate the string
	dst[size] ='\0';

#ifdef DEBUG
	cout << "substring=|" << temp << '|' << endl;
#endif
	return dst;
}

/**
 * Skips over any sequence of tokens that are to be
 * ignored, including white space and comments
 */
void Tokenizer::skipIgnoredSequences() {
	if( position < length ) {
		int current;
		do {
			// capture the current position
			current = position;

			// skip whitespace & comments
			skipWhiteSpace();
			skipDelimiters();
			skipCommentLine();
			skipCommentBlock();
		} while( ( position < length ) && ( current != position ) );
	}
}

/**
 * Returns a line comment or NULL
 */
void Tokenizer::skipCommentLine() {
	// does it start with a block quote ("//")?
	if( ( position + 1 < length ) &&
			( text[position] == '/' && text[position+1] == '/' ) ) {

		// skip the entire line
		while( ( position < length ) &&
			( text[position] != '\n' ) ) position++;

		// adjust the final position
		if( !newLineIsEOL ) {
			position++;
		}
	}
}

/**
 * Returns a block comment or NULL
 */
void Tokenizer::skipCommentBlock() {
	// does it start with a block quote ("/*")?
	if( ( position + 1 < length ) &&
			( text[position] == '/' && text[position+1] == '*' ) ) {

		// continue scanning for the end block ("*/")
		while( ( position + 1 < length ) &&
				( text[position] != '*' || text[position+1] != '/' ) ) position++;

		// adjust the ending position
		position += 2;
	}
}

/**
 * Skips over delimiters: comma(,) and colon(:)
 */
void Tokenizer::skipDelimiters() {
	if( ignoreDelimiters ) {
		// skip all delimiters
		while( ( position < length ) &&
				( text[position] == ',' || text[position] == ':' ) ) position++;
	}
}

/**
 * Skips over whitespace
 */
void Tokenizer::skipWhiteSpace() {
	// skip all whitespace
	while( ( position < length ) &&
			( text[position] == ' ' ||
			  text[position] == '\t' ||
			  ( !newLineIsEOL && text[position] == '\n' ) ) ) position++;
}

/**
 * Returns the line number at the given position
 */
int Tokenizer::determineLineNumber( int position ) {
	int lineNumber = 1 + lineNumberAdjust;
	for( int n = 0; n < position; n++ ) {
		if( text[n] == '\n' ) lineNumber++;
	}
	return lineNumber;
}

