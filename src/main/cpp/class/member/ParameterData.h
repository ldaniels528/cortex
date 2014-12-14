/*
 * ParameterData.h
 *
 *  Created on: March 20, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef PARAMETERDATA_H_
#define PARAMETERDATA_H_

#include <list.h>

#include "../../CodeBuffer.h"
#include "Parameter.h"

/**
 * Represents method parameter data
 */
class ParameterData {
private:
	list<Parameter*> params;
	OFFSET_T offset;
	int parameterId;

public:

	/**
	 * Constructor
	 */
	ParameterData( int paramId, list<Parameter*> params, OFFSET_T offset );

	/**
	 * Destruction
	 */
	virtual ~ParameterData();

	/**
	 * Compiles the data into byte code
	 */
	int compile( CodeBuffer* );

	/**
	 * Decompiles the byte code into parameter data
	 */
	static ParameterData* decompile( CodeBuffer* );

	/**
	 * Returns the parameter reference identifier
	 */
	int getParameterId();

	/**
	 * Returns the code/data offset
	 */
	OFFSET_T getOffset();

	/**
	 * Sets the code/data offset
	 */
	void setOffset( OFFSET_T );

};

#endif /* PARAMETERDATA_H_ */
