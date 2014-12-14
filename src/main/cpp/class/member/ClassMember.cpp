/*
 * ClassMember.cpp
 *
 *  Created on: March 5, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include <stdio.h>

#include "ClassMember.h"

/**
 * Constructor
 */
ClassMember::ClassMember( TypeReference* type,
						  const ClassMemberType memberType ) {
	this->type			= type;
	this->memberType	= memberType;
}

/**
 * Destruction
 */
ClassMember::~ClassMember() {
	// do nothing
}

/**
 * Returns the class member type
 */
ClassMemberType ClassMember::getMemberType() {
	return memberType;
}

/**
 * Returns the type of the class member
 */
TypeReference* ClassMember::getType() {
	return type;
}


