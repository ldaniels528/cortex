/*
 * RegisterSet.h
 *
 *  Created on: March 19, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef REGISTERSET_H_
#define REGISTERSET_H_

#include "../OpCodes.h"

#include "Register32GP.h"
#include "Register64FP.h"
#include "Register64GP.h"
#include "RegisterFlags.h"

/**
 * Represents a set of CPU registers
 */
class RegisterSet {
public:
	// 64-bit pseudo registers
	RegisterFlags flags;// 64-bit flags register
	OFFSET_T rrp;		// 64-bit return pointer register
	OFFSET_T spd;		// 64-bit data stack pointer register
	OFFSET_T spc;		// 64-bit code stack pointer register
	OFFSET_T ip;		// 64-bit instruction pointer

	// 64-bit counters
	REG_SIZE64 rc0;
	REG_SIZE64 rc1;
	REG_SIZE64 rc2;
	REG_SIZE64 rc3;

	// set of user accessible registers (e.g. AX, CX, etc.)
	Register** regs;

	// 32-bit general purpose registers
	Register32GP* ax;
	Register32GP* bx;
	Register32GP* cx;	// 32-bit counter register
	Register32GP* dx;
	Register32GP* ex;
	Register32GP* fx;
	Register32GP* gx;
	Register32GP* hx;

	// 64-bit general purpose registers
	Register64GP* gp0;
	Register64GP* gp1;	// 64-bit general base register
	Register64GP* gp2;	// 64-bit counter register
	Register64GP* gp3;	// 64-bit data register
	Register64GP* gp4;	// 64-bit offset extension register
	Register64GP* gp5;	// 64-bit offset extension register
	Register64GP* gp6;
	Register64GP* gp7;
	Register64GP* gp8;
	Register64GP* gp9;
	Register64GP* gpa;
	Register64GP* gpb;
	Register64GP* gpc;
	Register64GP* gpd;
	Register64GP* gpe;
	Register64GP* gpf;

	// 64-bit floating point registers
	Register64FP* fp0;
	Register64FP* fp1;
	Register64FP* fp2;
	Register64FP* fp3;
	Register64FP* fp4;
	Register64FP* fp5;
	Register64FP* fp6;
	Register64FP* fp7;

	/**
	 * Constructor
	 */
	RegisterSet();

	/**
	 * Destruction
	 */
	virtual ~RegisterSet();

	/**
	 * Dumps the contents of the registers to STDOUT
	 */
	void dump();

	/**
	 * Returns the register based on the given index
	 */
	Register* indexOf( int );

	/**
	 * Restores all registers from the stack
	 */
	void popAll( char* stack );

	/**
	 * Restores the flags register from the stack
	 */
	void popFlags( char* stack );

	/**
	 * Saves all registers onto the stack
	 */
	void pushAll( char* stack );

	/**
	 * Saves the flags registers onto the stack
	 */
	void pushFlags( char* stack );

private:

	/**
	 * Dumps the contents of the given range of registers to STDOUT
	 */
	void dumpRange( char* buf, int columns, int start, int end );

};

#endif /* REGISTERSET_H_ */
