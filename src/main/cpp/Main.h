/*
 * Serpent Virtual Machine (SVM)
 *
 *  Created on: February 28, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef MAIN_H_
#define MAIN_H_

/**
 * Main Class
 */
class Main {
public:

	/**
	 * Constructor
	 */
	Main();

	/**
	 * Destruction
	 */
	virtual ~Main();

	/**
	 * Builds the "sera.lang" assembly
	 */
	int build();

	/**
	 * Compiles the given source files
	 * argv - the given file arguments
	 * start - the starting index
	 * count - the ending index
	 */
	int compileSources( int start, int end, char *argv[] );

	/**
	 * Debugs a SVM binary
	 * @param argc the number of arguments passed
	 * @param argv the given file arguments
	 */
	int debug( int argc, char* argv[] );

	/**
	 * Executes a SVM binary
	 * @param argc the number of arguments passed
	 * @param argv the given file arguments
	 */
	int executeBinary( int argc, char *argv[] );

	/**
	 * Performs testing
	 */
	int test();

};

#endif /* MAIN_H_ */
