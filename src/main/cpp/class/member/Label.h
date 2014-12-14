/*
 * Label.h
 *
 *  Created on: March 6, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef LABEL_H_
#define LABEL_H_

#include "../../Common.h"

/**
 * Represents an Assembly Language label
 */
class Label {
protected:
	const char* name;
	OFFSET_T offset;

public:

	/**
	 * Constructor
	 */
	Label( const char* name, OFFSET_T offset );

	/**
	 * Destructor
	 */
	virtual ~Label();

	/**
	 * Returns the name of the method
	 */
	const char* getName();

	/**
	 * Returns the position of the label
	 */
	OFFSET_T getOffset();
};

#endif /* LABEL_H_ */
