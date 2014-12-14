/*
 * StringFormatter.cpp
 *
 *  Created on: March 29, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include <iostream.h>
#include <stdio.h>
#include <string.h>

#include "StringFormatter.h"

using namespace std;

/**
 * Constructor
 */
StringFormatter::StringFormatter() {
	// do nothing
}

/**
 * Destruction
 */
StringFormatter::~StringFormatter() {
	// do nothing
}

/**
 * C-language formatter implementation
 */
const char* StringFormatter::format( const char* format, ... ) {
	std::string buffer( format );

	// process the escape characters (e.g. "\n")
	processEscapeCharacters( buffer );

	// get the argument list
	int length;
	va_list args;
	va_start( args, length );

	// process the format characters (e.g. "%d")
	processFormatCharacters( buffer, args, length );

	// cleanup
	va_end( args );

	// return the string
	return buffer.data();
}

/**
 * Processing the escape characters (e.g. "\n")
 */
void StringFormatter::processEscapeCharacters( string& buffer ) {
	size_t index = 0;

	// expand the escape characters;
	do {
		// find the next escape character
		index = buffer.find( '\\', index );

		// did we find one?
		if( index != std::string::npos ) {
			if( buffer.length() > index+1 ) {
				// get the escape character
				const char c = buffer[index+1];

				// process the escape character
				switch( c ) {
					case '0': buffer.replace( index, 2, "\0" ); break;
					case 'n': buffer.replace( index, 2, "\n" ); break;
					case 't': buffer.replace( index, 2, "\t" ); break;
					default:
						printf( "Error in format unexpected '%c' at %d\n", c, (int)index );
				}
			}
			index++;
		}
	}
	while( index != std::string::npos );
}

/**
 * Processing the format characters (e.g. "%d")
 */
void StringFormatter::processFormatCharacters( string& buffer, va_list args, int argc ) {
	size_t index = 0;

	// expand the escape characters;
	do {
		// find the next escape character
		index = buffer.find( '%', index );

		// did we find one?
		if( index != std::string::npos ) {
			if( buffer.length() > index+1 ) {
				// get the escape character
				const char c = buffer[index+1];
				//printf( "Found escape character (%c) at %d\n", c, (int)index );

				// process the escape character
				switch( c ) {
					// binary
					case 'b': buffer.replace( index, 2, toBinary( va_arg( args, unsigned ) ) ); break;
					// character
					case 'c': buffer.replace( index, 2, formatData( "%c", va_arg( args, int ) ) ); break;
					// decimal/integer
					case 'd':;
					case 'i': buffer.replace( index, 2, formatData( "%d", va_arg( args, int ) )  ); break;
					// floating point
					case 'f': buffer.replace( index, 2, formatData( "%d", va_arg( args, double ) ) ); break;
					// octal
					case 'o': buffer.replace( index, 2, formatData( "%o", va_arg( args, unsigned ) ) ); break;
					// signed
					case 's': buffer.replace( index, 2, formatData( "%s", va_arg( args, const char* ) ) ); break;
					// unsigned
					case 'u': buffer.replace( index, 2, formatData( "%d", va_arg( args, unsigned ) ) ); break;
					// hexadecimal
					case 'X': buffer.replace( index, 2, formatData( "%X", va_arg( args, unsigned ) ) ); break;
					case 'x': buffer.replace( index, 2, formatData( "%x", va_arg( args, unsigned ) ) ); break;
					// error
					default:
						printf( "Error in format unexpected '%c' at %d\n", c, (int)index );
				}
			}
			index++;
		}
	}
	while( index != std::string::npos );
}

/**
 * Returns the binary representation of the given value
 * @param value the given value
 */
const char* StringFormatter::toBinary( const long value ) {
	// if the value is zero ...
	if( value == 0 ) {
		return "0";
	}

	// build the string
	std::string s;
	long factor = 1;
	while( value >= factor ) {
		s.insert( 0, value & factor ? "1" : "0" );
		factor <<= 1;

	}
	return s.data();
}

/**
 * Returns the numeric representation of the given value
 * @param value the given value
 */
const char* StringFormatter::formatData( const char* format, const size_t value ) {
	char* buf = new char[32];
	sprintf( buf, format, value );
	return buf;
}

/**
 * Returns the string representation of the given value
 * @param value the given value
 */
const char* StringFormatter::formatData( const char* format, const char* value ) {
	char* buf = new char[32];
	sprintf( buf, format, value );
	return buf;
}

