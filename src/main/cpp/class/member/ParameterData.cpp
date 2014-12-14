/*
 * ParameterData.cpp
 *
 *  Created on: March 20, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include "ParameterData.h"

/**
 * Constructor
 */
ParameterData::ParameterData( int parameterId, list<Parameter*> params, OFFSET_T offset ) {
	this->parameterId 	= parameterId;
	this->params		= params;
	this->offset		= offset;
}

/**
 * Destruction
 */
ParameterData::~ParameterData() {
	params.clear();
}

/**
 * Compiles the data into byte code
 */
int ParameterData::compile( CodeBuffer* cb ) {
	// write the reference ID & parameter count
	cb->putInt( parameterId );
	cb->putChar( params.size() );

	// write each parameter in the list
	for( list<Parameter*>::iterator param = params.begin(); param != params.end(); param++ ) {
		// compile the parameter
		if( (*param)->compile( cb ) ) {
			return -1;
		}
	}
	return 0;
}

/**
 * Decompiles the byte code into parameter data
 */
ParameterData* ParameterData::decompile( CodeBuffer* cb ) {
	// get the reference ID & parameter count
	int referenceId = cb->getInt();
	int n_params 	= cb->getChar();
	OFFSET_T offset	= cb->getPosition();

	// create the parameter list
	list<Parameter*> params;

	// retrieve the parameters
	for( int n = 0; n < n_params; n++ ) {
		Parameter* param = Parameter::decompile( cb );
		if( param == NULL ) {
			return NULL;
		}
		params.push_back( param );
	}
	return new ParameterData( referenceId, params, offset );
}

/**
 * Returns the parameter reference identifier
 */
int ParameterData::getParameterId() {
	return parameterId;
}

/**
 * Returns the code/data offset
 */
OFFSET_T ParameterData::getOffset() {
	return offset;
}

/**
 * Sets the code/data offer
 */
void ParameterData::setOffset( OFFSET_T offset ) {
	this->offset = offset;
}

