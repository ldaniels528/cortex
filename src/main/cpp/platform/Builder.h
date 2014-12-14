/*
 * Builder.h
 *
 *  Created on: March 21, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef BUILDER_H_
#define BUILDER_H_

#include <vector.h>

#include "../Common.h"
#include "../class/Class.h"

/**
 * SVM Platform Builder
 */
class Builder {
private:
	vector<Class*> classes;

public:

	/**
	 * Constructor
	 */
	Builder();

	/**
	 * Destruction
	 */
	virtual ~Builder();

	/**
	 * Builds the class library
	 */
	int build();

};

#endif /* BUILDER_H_ */
