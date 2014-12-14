/*
 * LabelReference.h
 *
 *  Created on: March 16, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef LABELREFERENCE_H_
#define LABELREFERENCE_H_

#include "../../CodeBuffer.h"
#include "../../Common.h"

#include "Label.h"

/**
 * Represents a relative reference to a label
 */
class LabelReference : public Label {
public:

	/**
	 * Constructor
	 */
	LabelReference( const char* label, OFFSET_T offset );

	/**
	 * Destruction
	 */
	virtual ~LabelReference();

	/**
	 * Updates the instruction that references the label
	 */
	void update( CodeBuffer*, OFFSET_T );

};

#endif /* LABELREFERENCE_H_ */
