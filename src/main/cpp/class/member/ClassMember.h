/*
 * ClassMember.h
 *
 *  Created on: March 5, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef CLASSMEMBER_H_
#define CLASSMEMBER_H_

#include "../../OpCodes.h"
#include "../../CodeBuffer.h"
#include "../TypeReference.h"

/**
 * Represents a type of class member
 */
enum ClassMemberType {
	CLASS_METHOD, CLASS_FIELD
};

/**
 * Represents a member of a class (e.g. field or method)
 */
class ClassMember {
protected:
	ClassMemberType memberType;
	TypeReference *type;

public:

	/**
	 * Creates a new class member instance
	 * name - the given member name
	 */
	ClassMember( TypeReference *type, const ClassMemberType memberType );

	/**
	 * Releases all resources
	 */
	virtual ~ClassMember();

	/**
	 * Returns the class member type
	 */
	ClassMemberType getMemberType();

	/**
	 * Returns the type of the class member
	 */
	TypeReference* getType();

};

#endif /* CLASSMEMBER_H_ */
