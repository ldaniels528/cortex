/*
 * VoidClass.h
 *
 *  Created on: Mar 12, 2010
 *      Author: ldaniels
 */

#ifndef VOIDCLASS_H_
#define VOIDCLASS_H_

#include "../../class/Class.h"

/**
 * Represents a void (typeless) data type
 */
class VoidClass : public Class {
public:

	/**
	 * Constructor
	 */
	VoidClass();

	/**
	 * Destructor
	 */
	virtual ~VoidClass();
};

#endif /* VOIDCLASS_H_ */
