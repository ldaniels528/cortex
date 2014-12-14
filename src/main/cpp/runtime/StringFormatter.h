/*
 * StringFormatter.h
 *
 *  Created on: March 29, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef STRINGFORMATTER_H_
#define STRINGFORMATTER_H_

/**
 * JUX String Formatter
 */
class StringFormatter {
public:

	/**
	 * Constructor
	 */
	StringFormatter();

	/**
	 * Destructor
	 */
	virtual ~StringFormatter();

	/**
	 * Customized printf-like function
	 */
	const char* format( const char* format, ... );

private:

	/**
	 * Processing the escape characters (e.g. "\n")
	 */
	void processEscapeCharacters( std::string& );

	/**
	 * Processing the format characters (e.g. "%d")
	 */
	void processFormatCharacters( std::string&, va_list, int  );

	/**
	 * Returns the binary representation of the given value
	 * @param value the given value
	 */
	const char* toBinary( const long value );

	/**
	 * Returns the numeric representation of the given value
	 * @param value the given value
	 */
	const char* formatData( const char* format, const size_t value );

	/**
	 * Returns the string representation of the given value
	 * @param value the given value
	 */
	const char* formatData( const char* format, const char* value );

};

#endif /* STRINGFORMATTER_H_ */
