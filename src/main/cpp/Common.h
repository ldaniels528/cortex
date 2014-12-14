/*
 * Common.h
 *
 *  Created on: March 5, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef COMMON_H_
#define COMMON_H_

////////////////////////////////////////////////////////////
//		Constants
////////////////////////////////////////////////////////////

#define APP_NAME		"Cortex"
#define RELEASE			"Monaco"

#define MAJOR_VERSION	0
#define MINOR_VERSION 	2

// define the instruction type
#define INSTRUCTION_T	int

// define the register/memory sizes
#define REG_SIZE32		int
#define REG_SIZE64		long

// define the integer here so that we may switch between 32- and 64-bit
#define OFFSET_T 		long

////////////////////////////////////////////////////////////
//		MACROs
////////////////////////////////////////////////////////////

#define COALESCE(a,b) \
	( ( a != NULL ) ? a : b )

// MACRO for hash table declarations
#define HASH_MAP(key,value) \
	hash_map<key, value, hash<key>, equal_to<key> >

// MACRO for string-based hash table declarations
#define STRING_HASH_MAP(value) \
		hash_map<const char*, value, hash<const char*>, equal_to<std::string> >

// MACRO for parameter error messages
#define PARAMS_ERROR(received,expected,token) \
	printf( "Invalid parameters: too %s argument passed near %s on line %d\n", \
			( received < expected ) ? "few" : "many", token->getText(), token->getLineNumber() )

// MACRO to turn on/off a specific attribute bit
#define SET_ATTRIBUTE(attributes,enable,mask) \
	if( enable ) { \
		attributes |= mask; \
	} \
	else { \
		attributes &= ( 0xFF ^ mask ); \
	}

// MACRO for syntax error messages
#define SYNTAX_ERROR(message,token) \
	printf( "Syntax error: %s near '%s' on line %d\n", \
			message, token->getText(), token->getLineNumber() )

// MACRO to perform a swap of two elements
#define SWAP(T,var1,var2) \
{\
	T temp = *var1; \
	*var1 = *var2; \
	*var2 = temp; \
}

// MACRO for simulating throwing of exceptions
#define THROW(error) \
		error; return -1

////////////////////////////////////////////////////////////
//		Functions
////////////////////////////////////////////////////////////

/**
 * Returns the binary representation of the given value
 * @param value the given value
 */
const char* toBinary( long value );

#endif /* COMMON_H_ */
