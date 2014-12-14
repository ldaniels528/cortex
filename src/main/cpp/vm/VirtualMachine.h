/*
 * VirtualMachine.h
 *
 *  Created on: February 28, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef VIRTUALMACHINE_H_
#define VIRTUALMACHINE_H_

#include <vector.h>

#include "../class/ClassLoader.h"
#include "../class/TypeReference.h"
#include "../class/member/Method.h"
#include "../class/member/Parameter.h"
#include "../runtime/CPUCore.h"
#include "../runtime/RuntimeState.h"

#define MAIN_METHOD		"main"

using namespace std;

/**
 * Cortex Virtual Machine (SVM)
 */
class VirtualMachine {
private:
	RuntimeState* runtime;
	CPUCore *cpu;

public:
	/**
	 * Constructor
	 */
	VirtualMachine();

	/**
	 * Destruction
	 */
	virtual ~VirtualMachine();

	/**
	 * Starts the "main()" method of the given class file
	 */
	int run( const char* classFilePath, int argc, char* args[] );

protected:

	/**
	 * Builds a list of method arguments
	 */
	vector<Parameter*> getMethodArguments( int argc, char* args[] );

	/**
	 * Executes the method
	 * @param parameters the method argument parameters
	 */
	int execute( Method* method, vector<Parameter*> parameters );

};

#endif /* VIRTUALMACHINE_H_ */
