/*
 * CPUCore.h
 *
 *  Created on: February 28, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef CPUCORE_H_
#define CPUCORE_H_

#include <list.h>

#include "../CodeBuffer.h"
#include "../OpCodes.h"
#include "../class/Scope.h"

#include "MemoryReference.h"
#include "RegisterFlags.h"
#include "RegisterSet.h"
#include "RuntimeState.h"
#include "StringFormatter.h"
#include "ValueLocation.h"
#include "VirtualMemory.h"

#define DO_ADD(type,typeValue) \
{ \
	type* p = (type*)r1->getPointer(); \
	type value1 = *p; \
	type value2 = r2->typeValue(); \
	*p += value2; \
	r.flags.setCarry( ( value1 + value2 ) != ( value1 | value2 ) ); \
}

#define DO_AND(type,typeValue) \
{ \
	type* p = (type*)r1->getPointer(); \
	type value = r2->typeValue(); \
	*p &= value; \
}

#define DO_CMP(type,typeValue) \
{ \
	type value1 = r1->typeValue(); \
	type value2 = r2->typeValue(); \
	\
	r.flags.setGreater( value1 > value2 ); \
	r.flags.setLess( value1 < value2 ); \
	r.flags.setZero( value1 == value2 ); \
}

#define DO_DEC(type,typeValue) \
{ \
	type value = r1->typeValue(); \
	type* p = (type*)r1->getPointer(); \
	(*p)--; \
	r.flags.setCarry( ( value - 1 ) != ( value & 1 ) ); \
}

#define DO_DIV(type,typeValue) \
{ \
	type* p = (type*)r1->getPointer(); \
	type value2 = r2->typeValue(); \
	*p /= value2; \
}

#define DO_INC(type,typeValue) \
{ \
	type value = r1->typeValue(); \
	type* p = (type*)r1->getPointer(); \
	(*p)++; \
	r.flags.setCarry( ( value + 1 ) != ( value | 1 ) ); \
}

#define DO_MOV(type,typeValue) \
{ \
	type* p = (type*)r1->getPointer(); \
	type value2 = r2->typeValue(); \
	*p = value2; \
}

#define DO_MUL(type,typeValue) \
{ \
	type* p = (type*)r1->getPointer(); \
	type value2 = r2->typeValue(); \
	*p *= value2; \
}

#define DO_NOT(type,typeValue) \
{ \
	type value = r1->typeValue(); \
	type* p = (type*)r1->getPointer(); \
	*p = ~value; \
}

#define DO_OR(type,typeValue) \
{ \
	type* p = (type*)r1->getPointer(); \
	type value = r2->typeValue(); \
	*p |= value; \
}

#define DO_SHL(type,typeValue) \
{ \
	type* p = (type*)r1->getPointer(); \
	type value = r2->typeValue(); \
	*p <<= value; \
}

#define DO_SHR(type,typeValue) \
{ \
	type* p = (type*)r1->getPointer(); \
	type value = r2->typeValue(); \
	*p >>= value; \
}

#define DO_SUB(type,typeValue) \
{ \
	type* p = (type*)r1->getPointer(); \
	type value1 = *p; \
	type value2 = r2->typeValue(); \
	*p += value2; \
	r.flags.setCarry( ( value1 - value2 ) != ( value1 & value2 ) ); \
}

#define DO_XCHG(type,r1,r2) \
{ \
	type* v1 = (type*)r1->getPointer(); \
	type* v2 = (type*)r2->getPointer(); \
	type temp = *v1; \
	*v1 = *v2; \
	*v2 = temp; \
}

#define DO_XOR(type,typeValue) \
{ \
	type* p = (type*)r1->getPointer(); \
	type value = r2->typeValue(); \
	*p ^= value; \
}

/**
 * Represents a Virtual CPU Core
 */
class CPUCore {
private:
	list<Scope*> scopeStack;
	StringFormatter formatter;
	RuntimeState* state;
	VirtualMemory* mem;
	Scope* scope;
	char* callstack;
	char* datastack;
	bool alive;

public:
	RegisterSet r;			// CPU register set

	/**
	 * Constructor
	 */
	CPUCore( RuntimeState* );

	/**
	 * Destruction
	 */
	virtual ~CPUCore();

	/**
	 * Dumps the contents of the registers to STDOUT
	 */
	void dump();

	/**
	 * Evaluates the next schedule instruction
	 */
	int execute();

	/**
	 * Evaluates the given instruction
	 */
	int execute( INSTRUCTION_T );

	/**
	 * Focus on the given scope for execution
	 */
	void focus( Scope* );

	/**
	 * Return the runtime state instance
	 */
	RuntimeState* getRuntime();

	/**
	 * Returns the current scope of execution
	 */
	Scope* getScope();

	/**
	 * Halts the CPU
	 */
	void halt();

	/**
	 * Indicates whether the CPU is currently running
	 */
	bool isAlive();

	/**
	 * Starts the CPU
	 */
	void startup();

private:

	/**
	 * Calls a subroutine
	 */
	void call( INSTRUCTION_T );

	/**
	 * Clears the flag referenced in the given instruction
	 * @param instruction the given instruction
	 */
	void clearFlag( INSTRUCTION_T );

	/**
	 * Copies data from one memory location to another
	 */
	void copy( INSTRUCTION_T );

	/**
	 * Computes the cosine of the given floating point register
	 * value, and stores it on the stack.
	 * Flags affected: NONE
	 */
	void cosd( INSTRUCTION_T );

	/**
	 * Writes text to STDOUT
	 */
	void echo( INSTRUCTION_T );

	/**
	 * Flips the span of bytes starting at register RDI
	 * and ending with RDI + RCX
	 */
	void flip( INSTRUCTION_T );

	/**
	 * Toggles the flag referenced in the given instruction
	 * @param instruction the given instruction
	 */
	void flipFlag( INSTRUCTION_T );

	/**
	 * Invokes a method (retrieving the method name
	 * and parameters from the stack)
	 */
	void invoke( INSTRUCTION_T );

	/**
	 * Performs a Conditional Jump (16-bit offset) based on the criteria
	 * found in the given instruction
	 */
	void jump( INSTRUCTION_T );

	/**
	 * Performs a "NEAR" JMP (16-bit offset) instruction
	 */
	void jumpN16( INSTRUCTION_T );

	/**
	 * Loads a class into memory
	 */
	void loadClass( INSTRUCTION_T );

	/**
	 * Performs a loop if CX/ECX is greater than zero
	 */
	void loop( INSTRUCTION_T );

	/**
	 * Moves an address into the specified 64-bit register
	 */
	void movaddr( INSTRUCTION_T );

	/**
	 * Creates a new instance of a class; an object.
	 */
	void newInst( INSTRUCTION_T );

	/**
	 * Loads the method parameter value onto the stack
	 * for later execute (see invoke)
	 */
	void params( INSTRUCTION_T );

	/**
	 * Peeks at a value from the stack
	 * instruction - the given instruction
	 */
	void peek( INSTRUCTION_T );

	/**
	 * Computes PI and stores it as a 64-bit floating point value on the stack.
	 * Flags affected: NONE
	 */
	void pi( INSTRUCTION_T );

	/**
	 * Pops a value from the stack
	 * instruction - the given instruction
	 */
	void pop( INSTRUCTION_T );

	/**
	 * Pops an offset off the stack
	 */
	OFFSET_T popOffset();

	/**
	 * Pops a 64-bit value off the stack
	 */
	REG_SIZE64 popValue64();

	/**
	 * Customized printf-like function
	 */
	void dumpString( const char* format );

	/**
	 * Pushes the given value onto the stack
	 * instruction - the given instruction
	 */
	void push( INSTRUCTION_T );

	/**
	 * Saves the given offset onto the stack
	 */
	void pushOffset( OFFSET_T );

	/**
	 * Saves the given 64-bit value onto the stack
	 */
	void pushValue64( REG_SIZE64 );

	/**
	 * Saves the given 64-bit floating point value onto the stack
	 */
	void pushValueFP64( double value );

	/**
	 * Returns from a subroutine
	 */
	void ret( INSTRUCTION_T );

	/**
	 * Scans the array, starting at RA and spanning RC elements, for
	 * the quantity found in RD (e.g. "SCAN BYTE [RD]:RA,RC"). If
	 * the value is found, the result is placed on the stack,
	 * and Flag FF is set.
	 */
	void scan( INSTRUCTION_T );

	/**
	 * Sets the flag referenced in the given instruction
	 * @param instruction the given instruction
	 */
	void setFlag( INSTRUCTION_T );

	/**
	 * Retrieve the address of a data object/variable from
	 * the stack and assigns it to a variable
	 */
	void setPtr( INSTRUCTION_T );

	/**
	 * Computes the sine of the given 64-bit floating point register
	 * value, and stores it on the stack.
	 * Flags affected: NONE
	 */
	void sind( INSTRUCTION_T );

	/**
	 * Performs a sort on the underlying data
	 */
	void sort( INSTRUCTION_T );

	/**
	 * Loads the address of a data object onto the stack
	 * for later assignment (see setPtr)
	 */
	void stage( INSTRUCTION_T );

	/**
	 * Computes the tangent of the given 64-bit floating point register
	 * value, and stores it on the stack.
	 * Flags affected: NONE
	 */
	void tand( INSTRUCTION_T );

private:

	/**
	 * Indicates whether the condition portion of the
	 * given instruction has been satisfied.
	 */
	bool satifiesCondition( INSTRUCTION_T );

	/**
	 * Returns the argument offset of the given instruction
	 * instruction - the given instruction
	 */
	OFFSET_T lookupArgumentOffset( INSTRUCTION_T );

	/**
	 * Returns the argument type of the given instruction
	 * instruction - the given instruction
	 */
	int lookupArgumentType( INSTRUCTION_T );

	/**
	 * Returns a memory reference
	 * instruction - the given instruction
	 */
	MemoryReference* lookupMemoryRef( INSTRUCTION_T );

	/**
	 * Returns the data quantifier for the given instruction
	 * instruction - the given instruction
	 */
	int lookupQuantifier( INSTRUCTION_T );

	/**
	 * Returns a pointer to the memory reference/register represented by the given instruction
	 * instruction - the given instruction
	 */
	ValueLocation* lookupReferenceA( INSTRUCTION_T );

	/**
	 * Returns a pointer to the value reference represented by the given instruction
	 * instruction - the given instruction
	 */
	ValueLocation* lookupReferenceB( INSTRUCTION_T );

	/**
	 * Returns the reference data indicated by the given instruction
	 * instruction - the given instruction
	 */
	ReferenceData* lookupReferenceData( INSTRUCTION_T );

	/**
	 * Returns the reference type information
	 * instruction - the given instruction
	 */
	int lookupReferenceType( INSTRUCTION_T );

	/**
	 * Returns a pointer to the register represented by the given register index
	 * instruction - the given instruction
	 */
	Register* lookupRegisterA( INSTRUCTION_T );

	/**
	 * Returns a pointer to the 32-bit general purpose register represented
	 * by the given register index
	 * @param instruction the given instruction
	 */
	Register32GP* lookupRegisterA32GP( INSTRUCTION_T instruction );

	/**
	 * Returns a pointer to the 64-bit general purpose register represented
	 * by the given register index
	 * @param instruction the given instruction
	 */
	Register64FP* lookupRegisterA64FP( INSTRUCTION_T instruction );

	/**
	 * Returns a pointer to the 64-bit general purpose register represented
	 * by the given register index
	 * @param instruction the given instruction
	 */
	Register64GP* lookupRegisterA64GP( INSTRUCTION_T );

	/**
	 * Returns a pointer to the register represented by the given register index
	 * instruction - the given instruction
	 */
	Register* lookupRegisterB( INSTRUCTION_T );

	/**
	 * Returns a pointer to the register represented by the given register index
	 * instruction - the given instruction
	 */
	Register* lookupRegisterC( INSTRUCTION_T );

	/**
	 * Returns the 16-bit value/offset
	 * instruction - the given instruction
	 */
	ValueLocation* lookupValue16( INSTRUCTION_T );

	/**
	 * Adds registers or memory references based on the given instruction
	 * @param instruction the given instruction
	 */
	void add( INSTRUCTION_T );

	/**
	 * ANDs the given value to the given variable
	 * var - the given variable
	 * value - the given value
	 */
	void _and( INSTRUCTION_T );

	/**
	 * Performs a comparison of two entities referenced
	 * in the given instruction.
	 * @param instruction the given instruction
	 */
	void cmp( INSTRUCTION_T );

	/**
	 * Decrements the given variable by 1
	 * var - the given variable
	 */
	void dec( INSTRUCTION_T ) ;

	/**
	 * Divides the given value to the given variable
	 * var - the given variable
	 * value - the given value
	 */
	void div( INSTRUCTION_T );

	/**
	 * Increments the given variable by 1
	 * var - the given variable
	 */
	void inc( INSTRUCTION_T );

	/**
	 * Copies the given value to the given variable
	 * var - the given variable
	 * value - the given value
	 */
	void mov( INSTRUCTION_T );

	/**
	 * Multiplies the given value to the given variable
	 * var - the given variable
	 * value - the given value
	 */
	void mul( INSTRUCTION_T );

	/**
	 * Bitwise NOTs the given value to the given variable
	 * var - the given variable
	 * value - the given value
	 */
	void _not( INSTRUCTION_T );

	/**
	 * Bitwise ORs the given value to the given variable
	 * var - the given variable
	 * value - the given value
	 */
	void _or( INSTRUCTION_T );
	/**
	 * Shifts Left the given value to the given variable
	 * var - the given variable
	 * value - the given value
	 */
	void shl( INSTRUCTION_T );

	/**
	 * Shifts Right the given value to the given variable
	 * var - the given variable
	 * value - the given value
	 */
	void shr( INSTRUCTION_T );

	/**
	 * Subtracts the given value to the given variable
	 * var - the given variable
	 * value - the given value
	 */
	void sub( INSTRUCTION_T );

	/**
	 * Exchanges the values of the given variables
	 * var1 - the given variable 1
	 * var2 - the given variable 2
	 */
	void xchg( INSTRUCTION_T );

	/**
	 * Bitwise XORs the given value to the given variable
	 * var - the given variable
	 * value - the given value
	 */
	void _xor( INSTRUCTION_T );

};

#endif /* CPUCORE_H_ */
