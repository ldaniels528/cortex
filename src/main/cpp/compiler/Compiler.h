/*
 * Compiler.h
 *
 *  Created on: February 28, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef COMPILER_H_
#define COMPILER_H_

#include <hash_map.h>
#include <string.h>

#include "../Common.h"
#include "../CodeBuffer.h"
#include "../OpCodes.h"
#include "../class/Class.h"
#include "../class/ClassLoader.h"

#include "CodeParser.h"
#include "CompilerState.h"


using namespace std;

/**
 * SERA Compiler
 */
class Compiler {
private:
	CompilerState *state;
	CodeParser* parser;

public:

	/**
	 * Default Constructor
	 */
	Compiler();

	/**
	 * Destruction
	 */
	virtual ~Compiler();

	/**
	 * Compiles the given source file
	 */
	int compile( const char *sourcePath );

	/**
	 * Writes the class file to disk as the given output path
	 */
	bool writeFile( CodeBuffer *sb, const char *outputPath );

private:

	int compile_pass1( const char *sourcePath );

	/**
	 * This is the resolution part of the compile process
	 * where all class files must be resolved, and all
	 * declared classes are written to disk.
	 */
	int compile_pass2();

};

#endif /* COMPILER_H_ */
