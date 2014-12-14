/*
 * Label.cpp
 *
 *  Created on: Mar 6, 2010
 *      Author: ldaniels
 */

#include "Label.h"

/**
 * Constructor
 */
Label::Label( const char* name, OFFSET_T offset ) {
	this->name 		= name;
	this->offset	= offset;
}

/**
 * Destructor
 */
Label::~Label() {
	delete name;
}

/**
 * Returns the name of the method
 */
const char* Label::getName() {
	return name;
}

/**
 * Returns the offset of the label
 */
OFFSET_T Label::getOffset() {
	return offset;
}
