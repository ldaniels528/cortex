/*
 * CPUCore.cpp
 *
 *  Created on: February 28, 2010
 *      Author: lawrence.daniels@gmail.com
 */
#include <iostream.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "../OpCodes.h"

#include "CPUCore.h"
#include "ConstantValue.h"

/**
 * Default Constructor
 */
CPUCore::CPUCore( RuntimeState* state ) {
	this->state		= state;
	this->mem		= state->getMemory();
	this->callstack	= mem->getCallStack();
	this->datastack	= mem->getDataStack();
	this->alive		= true;
}

/**
 * Destruction
 */
CPUCore::~CPUCore() {
	// do nothing
}

/**
 * Dumps the contents of the registers to STDOUT
 */
void CPUCore::dump() {
	r.dump();
}

/**
 * Evaluates the next schedule instruction
 */
int CPUCore::execute() {
	// get the next instruction
	const INSTRUCTION_T instruction = mem->getInstruction( r.ip );

	// execute the instruction
	int errorCode = execute( instruction );
	if( !errorCode ) {
		// increment the instruction pointer
		r.ip += sizeof(INSTRUCTION_T);
	}

	// return the error code
	return errorCode;
}

/**
 * Evaluates the given instruction
 */
int CPUCore::execute( INSTRUCTION_T instruction ) {
	// determine the opCode
	// layout: 1111.1111.xxxx.xxxx.xxxx.xxxx.xxxx.xxxx
	const int opCode = ( instruction >> A_OP ) & MASK_OP;

	// determine the opCode
	switch( opCode ) {
		case ADD: 		add( instruction ); break;
		case AND:		_and( instruction ); break;
		case CALL:		call( instruction ); break;
		case CLRFLAG:	clearFlag( instruction ); break;
		case COPY:		copy( instruction ); break;
		case COS:		cosd( instruction ); break;
		case DEC:		dec( instruction ); break;
		case DIV:		div( instruction ); break;
		case DUMP:		dump(); break;
		case ECHO:		echo( instruction ); break;
		case FLIP:		flip( instruction ); break;
		case FLIPFLAG:	flipFlag( instruction ); break;
		case HLT:		halt(); break;
		case INVOKE:	invoke( instruction ); break;
		case INC:		inc( instruction ); break;
		case JMP:		jump( instruction ); break;
		case LOADCLASS:	loadClass( instruction ); break;
		case LOOP:		loop( instruction ); break;
		case MOV:		mov( instruction ); break;
		case MUL:		mul( instruction ); break;
		case NEWINST:	newInst( instruction ); break;
		case NOP:		break;
		case NOT:		_not( instruction ); break;
		case OR:		_or( instruction ); break;
		case PARAMS:	params( instruction ); break;
		case PEEK:		peek( instruction ); break;
		case PI:		pi( instruction ); break;
		case POP:		pop( instruction ); break;
		case POPALL:	r.popAll( datastack ); break;
		case POPFLAGS:	r.popFlags( datastack ); break;
		case PUSH:		push( instruction ); break;
		case PUSHALL:	r.pushAll( datastack ); break;
		case PUSHFLAGS:	r.pushFlags( datastack ); break;
		case RET:		ret( instruction ); break;
		case SETFLAG:	setFlag( instruction ); break;
		case SETPTR:	setPtr( instruction ); break;
		case SHL:		shl( instruction ); break;
		case SHR:		shr( instruction ); break;
		case SIN:		sind( instruction );
		case STAGE:		stage( instruction ); break;
		case TAN:		tand( instruction ); break;
		case SUB:		sub( instruction ); break;
		case XCHG:		xchg( instruction ); break;
		case XOR:		_xor( instruction ); break;
		default:
			printf( "Unrecognized opCode %02X (instruction %08X)\n", opCode, instruction );
			return -1;
	}
	return 0;
}

/**
 * Switches the current focus to the given scope
 * and executes it.
 */
void CPUCore::focus( Scope* newScope ) {
	// is there an existing scope?
	if( scope != NULL ) {
		// add the scope to the stack
		scopeStack.push_front( scope );

		// save the current pointer
		pushOffset( r.ip );
	}

	// remember this scope
	this->scope = newScope;

	// set the executable offset of the scope
	r.ip = newScope->getCodeOffset();
}

/**
 * Return the runtime state instance
 */
RuntimeState* CPUCore::getRuntime() {
	return state;
}

/**
 * Returns the current scope of execution
 */
Scope* CPUCore::getScope() {
	return scope;
}

/**
 * Halts the CPU
 */
void CPUCore::halt() {
	alive = false;
}

/**
 * Indicates whether the CPU is currently running
 */
bool CPUCore::isAlive() {
	return alive;
}

/**
 * Starts the CPU
 */
void CPUCore::startup() {
	alive = true;
}

/**
 * Calls a subroutine
 */
void CPUCore::call( INSTRUCTION_T instruction ) {
	// push the current offset
	pushOffset( r.ip );

	// perform the jump
	jumpN16( instruction );
}

/**
 * Clears the flag referenced in the given instruction
 * @param instruction the given instruction
 */
void CPUCore::clearFlag( INSTRUCTION_T instruction ) {
	const int flag = ( instruction >> A_COND ) & MASK_COND;
	switch( flag ) {
		case FLAG_CF:	r.flags.setCarry( false ); break;
		case FLAG_DF:	r.flags.setDirection( false ); break;
		case FLAG_FF:	r.flags.setFound( false ); break;
		case FLAG_OF:	r.flags.setOverflow( false ); break;
		case FLAG_PF:	r.flags.setParity( false ); break;
		case FLAG_SF:	r.flags.setSigned( false ); break;
		case FLAG_XF:	r.flags.setException( false ); break;
		case FLAG_XDF:	r.flags.setDivideByZero( false ); break;
		case FLAG_ZF:	r.flags.setZero( false ); break;
	}
}

/**
 * Moves a string of data from the source index (SI)
 * to the destination index (DI)
 */
void CPUCore::copy( INSTRUCTION_T instruction ) {
	// get the data quantifier
	const int dq = lookupQuantifier( instruction );

	// setup the start and end pointers
	mem->copy( *(r.gx), *(r.hx), *(r.cx), dq );

	// clear the count
	r.cx = 0;
}

/**
 * Computes the cosine of the given 64-bit floating point register
 * value, and stores it on the stack.
 * Flags affected: NONE
 */
void CPUCore::cosd( INSTRUCTION_T instruction ) {
	// lookup the floating point register (FP0-FP7)
	Register64FP* reg64 = lookupRegisterA64FP( instruction );

	// compute the cosine
	double value  = (*reg64);
	double result = cos( value );

	// put the result on the stack
	pushValueFP64( result );
}

/**
 * C-language formatter implementation
 */
void CPUCore::dumpString( const char* format ) {
	// format the string
	const char* data = formatter.format( format, 1 );

	// write the text to STDOUT
	fwrite( data, 1, strlen(data), stdout );
}

/**
 * Writes text to STDOUT
 */
void CPUCore::echo( INSTRUCTION_T instruction ) {
	// get the data/field offset
	OFFSET_T offset;
	if( ( offset = lookupArgumentOffset( instruction ) ) ) {
		// get the argument type
		const int argType = lookupArgumentType( instruction );

		// is it a pointer?
		if( argType == ARG_PTR ) {
			// get the pointer to the field
			OFFSET_T* fieldPtr = (OFFSET_T*)(*mem)[ offset ];

			// print the string data
			//printf( "CPUCore::echo: offset=%08lX fieldPtr=%08lX\n", offset, *fieldPtr );
			const char* s = mem->getString( *fieldPtr );
			dumpString( s );
		}

		// must be direct access
		else {
			const char* s = mem->getString( offset );
			dumpString( s );
		}
	}
}

/**
 * Reverses the span of bytes starting at register RDI
 * and ending with RDI + RCX
 */
void CPUCore::flip( INSTRUCTION_T instruction ) {
	// get the data quantifier
	int dq = lookupQuantifier( instruction );

	// reverses data from index RDI to index RDI+RCX in memory
	mem->flip( *(r.hx), *(r.cx), dq );
}

/**
 * Toggles the flag referenced in the given instruction
 * @param instruction the given instruction
 */
void CPUCore::flipFlag( INSTRUCTION_T instruction ) {
	const int flag = ( instruction >> A_COND ) & MASK_COND;
	switch( flag ) {
		case FLAG_CF:	r.flags.setCarry( !r.flags.isCarry() ); break;
		case FLAG_XDF:	r.flags.setDivideByZero( !r.flags.isDivideByZero() ); break;
		case FLAG_DF: 	r.flags.setDirection( !r.flags.isIncreasing() ); break;
		case FLAG_FF: 	r.flags.setFound( !r.flags.isFound() ); break;
		case FLAG_OF: 	r.flags.setOverflow( !r.flags.isOverflow() ); break;
		case FLAG_PF: 	r.flags.setParity( !r.flags.isParity() ); break;
		case FLAG_SF: 	r.flags.setSigned( !r.flags.isSigned() ); break;
		case FLAG_ZF: 	r.flags.setZero( !r.flags.isZero() ); break;
	}
}

/**
 * Invokes a method (retrieving the method name
 * and parameters from the stack)
 */
void CPUCore::invoke( INSTRUCTION_T instruction ) {
	// get the data/field offset
	OFFSET_T offset;
	if( ( offset = lookupArgumentOffset( instruction ) ) ) {
		// get the method name
		const char* methodName = mem->getString( offset );

		// lookup the method
		Method* method = state->lookupMethod( scope, methodName );
		if( method != NULL ) {
			// switch the "focus" to the new method
			focus( method );
		}
		else {
			printf( "CPUCore::invoke - Method '%s' not found\n", methodName );
			r.flags.setFound( false );
			r.flags.setException( true );
		}
	}
}

/**
 * Performs a Conditional Jump (16-bit offset) based on the criteria
 * found in the given instruction
 */
void CPUCore::jump( INSTRUCTION_T instruction ) {
	// if the condition has been satisfied, jump.
	if( satifiesCondition( instruction ) ) {
		return jumpN16( instruction );
	}
}

/**
 * Performs a "NEAR" JMP (16-bit offset) instruction
 */
void CPUCore::jumpN16( INSTRUCTION_T instruction ) {
	// get the current position
	OFFSET_T current = r.ip;

	// determine the offset delta
	const short delta = ( instruction & MASK_OFS16 );

	// get the new offset
	OFFSET_T offset = current + delta;
	//printf( "CPUCore::jumpNear: %08lX to %08lX (delta=%d)\n", current, offset, delta );

	// perform the jump
	r.ip = offset - sizeof(INSTRUCTION_T);
}

/**
 * Loads a class into memory
 */
void CPUCore::loadClass( INSTRUCTION_T instruction ) {
	// get the data/field offset
	OFFSET_T offset;
	if( ( offset = lookupArgumentOffset( instruction ) ) ) {
		// get the class name
		const char* classname = mem->getString( offset );

		// load the class
		Class* classInst = state->resolveClass( classname );
		if( classInst == NULL ) {
			r.flags.setFound( false );
			r.flags.setException( true );
		}
		else {
			printf( "CPUCore::loadClass: class '%s' has been loaded\n", classInst->getFullyQualifiedName() );
		}
	}
}

/**
 * Performs a loop if CX is greater than zero
 */
void CPUCore::loop( INSTRUCTION_T instruction ) {
	// lookup the register
	Register* reg = lookupRegisterA( instruction );

	// if the condition has been satisfied, jump.
	if( reg->getValue() > 0 ) {
		(*reg)--;
		jumpN16( instruction );
	}
}

/**
 * Moves an address into the specified 64-bit register
 */
void CPUCore::movaddr( INSTRUCTION_T instruction ) {

}

/**
 * Creates a new instance of a class; an object.
 */
void CPUCore::newInst( INSTRUCTION_T instruction ) {
	// get the data/field offset
	OFFSET_T offset;
	if( ( offset = lookupArgumentOffset( instruction ) ) ) {
		// TODO do something here
	}
}

/**
 * Loads the method parameter value onto the stack
 * for later execute (see invoke)
 */
void CPUCore::params( INSTRUCTION_T instruction ) {
	// get the data/field offset
	OFFSET_T offset;
	if( ( offset = lookupArgumentOffset( instruction ) ) ) {
		// TODO do something here
	}
}

/**
 * Peeks at a value from the stack
 * instruction - the given instruction
 */
void CPUCore::peek( INSTRUCTION_T instruction ) {
	// get the reference info
	// layout: xxxx.xxxx.11xx.xxxx.xxxx.xxxx.xxxx.xxxx
	const int ref = ( instruction >> A_REF ) & MASK_REF;
	ValueLocation* r0 = ( ref == REF_MEM_REG )
				? lookupMemoryRef( instruction )
				: (ValueLocation*)lookupRegisterA( instruction );

	// get the index of the element to peek at
	OFFSET_T index = r.spd - sizeof(OFFSET_T);

	// restore the value
	REG_SIZE64* p = (REG_SIZE64*)&datastack[index];
	r0->setLongValue( *p );
}

/**
 * Computes PI and stores it as a 64-bit floating point value on the stack.
 * Flags affected: NONE
 */
void CPUCore::pi( INSTRUCTION_T instruction ) {
	// put the result on the stack
	pushValueFP64( M_PI );
}

/**
 * Pops a value from the stack
 * instruction - the given instruction
 */
void CPUCore::pop( INSTRUCTION_T instruction ) {
	// get the reference info
	// layout: xxxx.xxxx.11xx.xxxx.xxxx.xxxx.xxxx.xxxx
	const int ref = ( instruction >> A_REF ) & MASK_REF;
	ValueLocation* r0 = ( ref == REF_MEM_REG )
				? lookupMemoryRef( instruction )
				: (ValueLocation*)lookupRegisterA( instruction );

	// pop the value from the stack
	r0->setLongValue( popValue64() );
}

/**
 * Pops an offset off the stack.
 */
OFFSET_T CPUCore::popOffset() {
	// decrement the stack
	r.spc -= sizeof(OFFSET_T);

	// get the offset delta
	OFFSET_T* offset = (OFFSET_T*)&callstack[r.spc];

	// return the new offset
	return *offset;
}

/**
 * Pops a 64-bit value off the stack
 */
REG_SIZE64 CPUCore::popValue64() {
	// decrement the stack
	r.spd -= sizeof(OFFSET_T);

	// get the offset delta
	REG_SIZE64* value = (REG_SIZE64*)&datastack[r.spd];

	// return the new offset
	return *value;
}

/**
 * Pushes a value onto the stack
 * instruction - the given instruction
 */
void CPUCore::push( INSTRUCTION_T instruction ) {
	// get the reference info
	// layout: xxxx.xxxx.11xx.xxxx.xxxx.xxxx.xxxx.xxxx
	const int ref = ( instruction >> A_REF ) & MASK_REF;
	ValueLocation* r0 = ( ref == REF_MEM_REG )
				? lookupMemoryRef( instruction )
				: (ValueLocation*)lookupRegisterA( instruction );

	// push the value onto the stack
	pushValue64( r0->longValue() );
}

/**
 * Saves the given offset onto the stack
 */
void CPUCore::pushOffset( OFFSET_T offset ) {
	// store the value
	OFFSET_T* p = (OFFSET_T*)&callstack[r.spc];
	*p = offset;

	// increment the stack
	r.spc += sizeof(OFFSET_T);
}

/**
 * Saves the given 64-bit integer value onto the stack
 */
void CPUCore::pushValue64( REG_SIZE64 value ) {
	// store the value
	REG_SIZE64* p = (REG_SIZE64*)&datastack[r.spd];
	*p = value;

	// increment the stack
	r.spd += sizeof(OFFSET_T);
}

/**
 * Saves the given 64-bit floating point value onto the stack
 */
void CPUCore::pushValueFP64( double value ) {
	// store the value
	double* p = (double*)&datastack[r.spd];
	*p = value;

	// increment the stack
	r.spd += sizeof(OFFSET_T);
}

/**
 * Returns from a subroutine
 */
void CPUCore::ret( INSTRUCTION_T instruction ) {
	// pop the former scope off the stack
	if( !scopeStack.empty() ) {
		scope = scopeStack.front();
		scopeStack.pop_front();
	}

	// if the stack is empty, halt.
	if( r.spc == 0 ) {
		halt();
	}

	// otherwise, get the new offset
	else {
		// get the return offset
		OFFSET_T offset = popOffset();

		// return to the caller
		r.ip = offset;
	}
}

/**
 * Scans the array, starting at RP and spanning RC elements, for
 * the quantity found in RS (e.g. "SCAN BYTE [RS]:RP,RC"). If
 * the value is found, the result is placed on the stack,
 * updates RP, RC, and Flag FF is set.
 */
void CPUCore::scan( INSTRUCTION_T instruction ) {
	// get the registers
	Register* rs = lookupRegisterA( instruction );
	Register* rp = lookupRegisterB( instruction );
	Register* rc = lookupRegisterC( instruction );

	// determine the data qualifier
	// layout: [op:8][dq:2][r1:5][r2:5][r3:5][...:7]
	int dq = lookupQuantifier( instruction );

	// scan the contents of memory
	SearchResult* result = mem->scan( rp->getValue(), rc->getValue(), rs, dq );

	// if a result was found, put its position on the stack
	if( result->found ) {
		pushValue64( result->position );
	}

	// update the count and the position register
	*(rp) += result->position;
	*(rc) -= result->position;

	// set the found/not found flag
	r.flags.setFound( result->found );

	// cleanup
	delete result;
}

/**
 * Sets the flag referenced in the given instruction
 * @param instruction the given instruction
 */
void CPUCore::setFlag( INSTRUCTION_T instruction ) {
	const int flag = ( instruction >> A_COND ) & MASK_COND;
	switch( flag ) {
		case FLAG_CF:	r.flags.setCarry( true ); break;
		case FLAG_DF:	r.flags.setDirection( true ); break;
		case FLAG_FF: 	r.flags.setFound( true ); break;
		case FLAG_OF: 	r.flags.setOverflow( true ); break;
		case FLAG_PF: 	r.flags.setParity( true ); break;
		case FLAG_SF: 	r.flags.setSigned( true ); break;
		case FLAG_XF: 	r.flags.setException( true ); break;
		case FLAG_XDF: 	r.flags.setDivideByZero( true ); break;
		case FLAG_ZF: 	r.flags.setZero( true ); break;
	}
}

/**
 * Retrieve the address of a data object/variable from
 * the stack and assigns it to a variable
 */
void CPUCore::setPtr( INSTRUCTION_T instruction ) {
	// get the data/field offset
	OFFSET_T fieldOffset;
	if( ( fieldOffset = lookupArgumentOffset( instruction ) ) ) {
		// pop the address off of the stack
		OFFSET_T dataOffset = popOffset();

		// point to the new data/value
		//printf( "CPUCore::setPtr - Field '%s' [%04lX] => '[%04lX]\n", field->getName(), field->getOffset(), dataOffset );
		OFFSET_T* fieldPtr = (OFFSET_T*)(*mem)[ fieldOffset ];
		*fieldPtr = dataOffset;
	}
}

/**
 * Computes the sine of the given 64-bit floating point register
 * value, and stores it on the stack.
 * Flags affected: NONE
 */
void CPUCore::sind( INSTRUCTION_T instruction ) {
	// lookup the floating point register (FP0-FP7)
	Register64FP* reg64 = lookupRegisterA64FP( instruction );

	// compute the cosine
	double value  = (*reg64);
	double result = sin( value );

	// put the result on the stack
	pushValueFP64( result );
}

/**
 * Sorts the contents of memory based on the
 * register selection.
 * (e.g. "SORT BYTE RAX,RCX")
 */
void CPUCore::sort( INSTRUCTION_T instruction ) {
	// get the registers
	ValueLocation* r0 = lookupRegisterA( instruction );
	ValueLocation* r1 = lookupRegisterB( instruction );

	// determine the data qualifier
	// layout: [op:8][dq:2][r1:5][r2:5][...:12]
	int quantifier = lookupReferenceType( instruction );

	// ascending order?
	bool ascending = r.flags.isIncreasing();

	// sort the contents of memory
	mem->sort( r0->longValue(), r1->longValue(), ascending, quantifier );
}

/**
 * Loads the address of a data object onto the stack
 * for later assignment (see setPtr)
 */
void CPUCore::stage( INSTRUCTION_T instruction ) {
	// get the data/field offset
	OFFSET_T offset;
	if( ( offset = lookupArgumentOffset( instruction ) ) ) {
		// push the offset
		pushOffset( offset );
	}
}

/**
 * Computes the tangent of the given 64-bit floating point register
 * value, and stores it on the stack.
 * Flags affected: NONE
 */
void CPUCore::tand( INSTRUCTION_T instruction ) {
	// lookup the floating point register (FP0-FP7)
	Register64FP* reg64 = lookupRegisterA64FP( instruction );

	// compute the cosine
	double value  = (*reg64);
	double result = tan( value );

	// put the result on the stack
	pushValueFP64( result );
}

/**
 * Returns the argument offset of the given instruction
 * instruction - the given instruction
 */
OFFSET_T CPUCore::lookupArgumentOffset( INSTRUCTION_T instruction ) {
	// get the reference data
	ReferenceData* refdata = lookupReferenceData( instruction );

	// get the argument type
	const int argType = lookupArgumentType( instruction );

	// is it a pointer reference?
	if( argType == ARG_PTR ) {
		// lookup the field
		Field* field = state->lookupField( scope, refdata->getContent() );
		if( field != NULL ) {
			//printf( "CPUCore::lookupArgumentOffset: [P] data = [%s] offset = %08lX\n", field->getName(), field->getOffset() );
			// return the offset of the actual pointer
			// NOTE: this is not the offset the field "points" to!
			return field->getOffset();
		}
		else {
			printf( "CPUCore::lookupArgumentOffset - Field '%s' not found\n", refdata->getContent() );
			r.flags.setFound( false );
			r.flags.setException( true );
			return 0;
		}
	}

	// otherwise, just put the offset onto the stack
	else {
		//printf( "CPUCore::lookupArgumentOffset: [D] data = [%s] offset = %08lX\n", refdata->getContent(), refdata->getDataOffset() );
		return refdata->getDataOffset();
	}
}

/**
 * Returns the argument type of the given instruction
 * instruction - the given instruction
 */
int CPUCore::lookupArgumentType( INSTRUCTION_T instruction ) {
	return ( instruction >> A_ARG ) & MASK_ARG;
}

/**
 * Returns a pointer to the register represented by the given register index
 * instruction - the given instruction
 */
MemoryReference* CPUCore::lookupMemoryRef( INSTRUCTION_T instruction ) {
	// lookup the register (e.g. "MOV AX,[SI]" => "SI")
	ValueLocation* r1 = lookupRegisterB( instruction );

	// get the reference info
	// layout: xxxx.xxxx.xxxx.xxxx.xxxx.xxxx.1111.1111
	OFFSET_T offset8  = instruction & 0xFF;
	OFFSET_T offset32 = r1->longValue() + offset8;

	// get a pointer to the data
	return mem->getPointer( offset32 );
}

/**
 * Returns the data quantifier for the given instruction
 * instruction - the given instruction
 */
int CPUCore::lookupQuantifier( INSTRUCTION_T instruction ) {
	// mask-off everything but data quantifier
	// layout: xxxx.xxxx.11xx.xxxx.xxxx.xxxx.xxxx.xxxx
	return ( instruction >> A_DQ ) & MASK_DQ;
}

/**
 * Returns the reference data indicated by the given instruction
 * instruction - the given instruction
 */
ReferenceData* CPUCore::lookupReferenceData( INSTRUCTION_T instruction ) {
	// get the reference data index
	int refId = instruction & MASK_OFS24;

	// lookup the reference data
	ReferenceData* data = scope->lookupReferenceData( refId );
	if( data == NULL ) {
		printf( "CPUCore::lookupReferenceData - reference #%d does not exist\n", refId );
	}

	return data;
}

/**
 * Returns a pointer to the memory reference/register represented by the given instruction
 * instruction - the given instruction
 */
ValueLocation* CPUCore::lookupReferenceA( INSTRUCTION_T instruction ) {
	// get the reference info
	// layout: xxxx.xxxx.11xx.xxxx.xxxx.xxxx.xxxx.xxxx
	const int ref = ( instruction >> A_REF ) & MASK_REF;

	// handle the reference type
	switch( ref) {
		case REF_MEM_REG:
			return lookupMemoryRef( instruction );
		case REF_REG_MEM:;
		case REF_REG_VAL:
			return lookupRegisterA( instruction );
		default:
			printf( "Reference A type (%d) was not handled\n", ref );
			return NULL;
	}
}

/**
 * Returns a pointer to the value reference represented by the given instruction
 * instruction - the given instruction
 */
ValueLocation* CPUCore::lookupReferenceB( INSTRUCTION_T instruction ) {
	// get the reference info
	// layout: xxxx.xxxx.11xx.xxxx.xxxx.xxxx.xxxx.xxxx
	const int ref = ( instruction >> A_REF ) & MASK_REF;

	// handle the reference type
	switch( ref ) {
		case REF_REG_REG:
			return lookupRegisterB( instruction );
		case REF_REG_MEM:
			return lookupMemoryRef( instruction );
		case REF_REG_VAL:
			return lookupValue16( instruction );
		default:
			printf( "Reference B type (%d) was not handled\n", ref );
			return NULL;
	}
}

/**
 * Returns the reference type information
 * instruction - the given instruction
 */
int CPUCore::lookupReferenceType( INSTRUCTION_T instruction ) {
	// mask-off everything but reference type
	// layout: xxxx.xxxx.11xx.xxxx.xxxx.xxxx.xxxx.xxxx
	return ( instruction >> A_REF ) & MASK_REF;
}

/**
 * Returns a pointer to the register represented by the given register index
 * instruction - the given instruction
 */
Register* CPUCore::lookupRegisterA( INSTRUCTION_T instruction ) {
	// mask-off everything but register A
	// layout: xxxx.xxxx.xx11.111x.xxxx.xxxx.xxxx.xxxx
	const int index = ( instruction >> A_R1 ) & MASK_REG;

	// return the appropriate register
	return r.indexOf( index );
}

/**
 * Returns a pointer to the 32-bit general purpose register represented
 * by the given register index
 * @param instruction the given instruction
 */
Register32GP* CPUCore::lookupRegisterA32GP( INSTRUCTION_T instruction ) {
	// mask-off everything but register A
	// layout: xxxx.xxxx.111x.xxxx.xxxx.xxxx.xxxx.xxxx
	const int index = AX + ( instruction >> A_REG_SUB & MASK_REG_SUB );

	// return the appropriate register
	return (Register32GP*)r.indexOf( index );
}

/**
 * Returns a pointer to the 64-bit general purpose register represented
 * by the given register index
 * @param instruction the given instruction
 */
Register64FP* CPUCore::lookupRegisterA64FP( INSTRUCTION_T instruction ) {
	// mask-off everything but register A
	// layout: xxxx.xxxx.111x.xxxx.xxxx.xxxx.xxxx.xxxx
	const int index = FP0 + ( instruction >> A_REG_SUB & MASK_REG_SUB );

	// return the appropriate register
	return (Register64FP*)r.indexOf( index );
}

/**
 * Returns a pointer to the 64-bit general purpose register represented
 * by the given register index
 * @param instruction the given instruction
 */
Register64GP* CPUCore::lookupRegisterA64GP( INSTRUCTION_T instruction ) {
	// mask-off everything but register A
	// layout: xxxx.xxxx.111x.xxxx.xxxx.xxxx.xxxx.xxxx
	const int index = GP0 + ( instruction >> A_REG_SUB & MASK_REG_SUB );

	// return the appropriate register
	return (Register64GP*)r.indexOf( index );
}

/**
 * Returns a pointer to the register represented by the given register index
 * instruction - the given instruction
 */
Register* CPUCore::lookupRegisterB( INSTRUCTION_T instruction ) {
	// mask-off everything but register B
	// layout: xxxx.xxxx.xxxx.xxx1.1111.xxxx.xxxx.xxxx
	const int index = ( instruction >> A_R2 ) & MASK_REG;

	// return the appropriate register
	return r.indexOf( index );
}

/**
 * Returns a pointer to the register represented by the given register index
 * instruction - the given instruction
 */
Register* CPUCore::lookupRegisterC( INSTRUCTION_T instruction ) {
	// mask-off everything but register C
	// layout: xxxx.xxxx.xxxx.xxxx.xxxx.1111.1xxx.xxxx
	const int index = ( instruction >> A_R3 ) & MASK_REG;

	// return the appropriate register
	return r.indexOf( index );
}

/**
 * Returns a pointer to the register represented by the given register index
 * instruction - the given instruction
 */
ValueLocation* CPUCore::lookupValue16( INSTRUCTION_T instruction ) {
	// get the 16-bit value
	// layout: xxxxx.xxxxx.xxxxx.xxxxx.1111.1111.1111.1111
	return new ConstantValue( instruction & 0xFFFF, DQ_SHORT );
}

/**
 * Indicates whether the condition portion of the
 * given instruction has been satisfied.
 * @see http://siyobik.info/index.php?module=x86&id=146
 */
bool CPUCore::satifiesCondition( INSTRUCTION_T instruction ) {
	// get the condition
	const int condition = ( instruction >> A_COND ) & MASK_COND;

	// has it been satisfied?
	switch( condition ) {
		case CND_NONE:	return true;
		case CND_C:		return r.flags.isCarry();
		case CND_NC:	return !r.flags.isCarry();
		case CND_CXZ:	return r.cx->getValue() == 0;
		case CND_CXNZ:	return r.cx->getValue() != 0;
		case CND_G: 	return r.flags.isGreater();
		case CND_GE: 	return r.flags.isGreater() || r.flags.isLess();
		case CND_NG: 	return !r.flags.isGreater();
		case CND_NGE: 	return !( r.flags.isGreater() || r.flags.isZero() );
		case CND_L: 	return r.flags.isLess();
		case CND_LE: 	return r.flags.isLess() && r.flags.isZero();
		case CND_NL: 	return !r.flags.isLess();
		case CND_NLE: 	return !( r.flags.isLess() && r.flags.isZero()  );
		case CND_D_INC:	return r.flags.isIncreasing();
		case CND_D_DEC:	return !r.flags.isIncreasing();
		case CND_F:		return r.flags.isFound();
		case CND_NF:	return !r.flags.isFound();
		case CND_O:		return r.flags.isOverflow();
		case CND_NO:	return !r.flags.isOverflow();
		case CND_P: 	return r.flags.isParity();
		case CND_NP: 	return !r.flags.isParity();
		case CND_S:		return r.flags.isSigned();
		case CND_NS:	return !r.flags.isSigned();
		case CND_X:		return r.flags.isException();
		case CND_NX:	return !r.flags.isException();
		case CND_XDZ:	return r.flags.isDivideByZero();
		case CND_NXDZ:	return !r.flags.isDivideByZero();
		case CND_Z: 	return r.flags.isZero();
		case CND_NZ: 	return !r.flags.isZero();
		default:
			printf( "WARNING: condition (%d) was not handled\n", condition );
			return false;
	}
}

/**
 * Adds registers or memory references based on the given instruction
 * @param instruction the given instruction
 */
void CPUCore::add( INSTRUCTION_T instruction ) {
	// lookup the value references
	ValueLocation* r1 = lookupReferenceA( instruction );
	ValueLocation* r2 = lookupReferenceB( instruction );

	// perform the operation
	switch( r1->getQuantifier() ) {
		case DQ_BYTE:	DO_ADD(char,charValue); break;
		case DQ_SHORT:	DO_ADD(short,shortValue); break;
		case DQ_INT:	DO_ADD(int,intValue); break;
		case DQ_LONG:	DO_ADD(long,longValue); break;
	}
}

/**
 * ANDs the given value to the given variable
 * var - the given variable
 * value - the given value
 */
void CPUCore::_and( INSTRUCTION_T instruction ) {
	// lookup the value references
	ValueLocation* r1 = lookupReferenceA( instruction );
	ValueLocation* r2 = lookupReferenceB( instruction );

	// perform the operation
	switch( r1->getQuantifier() ) {
		case DQ_BYTE:	DO_AND(char,charValue); break;
		case DQ_SHORT:	DO_AND(short,shortValue); break;
		case DQ_INT:	DO_AND(int,intValue); break;
		case DQ_LONG:	DO_AND(long,longValue); break;
	}
}

/**
 * Performs a comparison of two entities referenced
 * in the given instruction.
 * @param instruction the given instruction
 */
void CPUCore::cmp( INSTRUCTION_T instruction ) {
	// lookup the value references
	ValueLocation* r1 = lookupReferenceA( instruction );
	ValueLocation* r2 = lookupReferenceB( instruction );

	// perform the operation
	switch( r1->getQuantifier() ) {
		case DQ_BYTE:	DO_CMP(char,charValue); break;
		case DQ_SHORT:	DO_CMP(short,shortValue); break;
		case DQ_INT:	DO_CMP(int,intValue); break;
		case DQ_LONG:	DO_CMP(long,longValue); break;
	}
}

/**
 * Decrements the given variable by 1
 * var - the given variable
 */
void CPUCore::dec( INSTRUCTION_T instruction ) {
	// lookup the value references
	ValueLocation* r1 = lookupReferenceA( instruction );

	// perform the operation
	switch( r1->getQuantifier() ) {
		case DQ_BYTE:	DO_DEC(char,charValue); break;
		case DQ_SHORT:	DO_DEC(short,shortValue); break;
		case DQ_INT:	DO_DEC(int,intValue); break;
		case DQ_LONG:	DO_DEC(long,longValue); break;
	}
}

/**
 * Divides the given value to the given variable
 * var - the given variable
 * value - the given value
 */
void CPUCore::div( INSTRUCTION_T instruction ) {
	// lookup the value references
	ValueLocation* r1 = lookupReferenceA( instruction );
	ValueLocation* r2 = lookupReferenceB( instruction );

	// if the value is zero, exception.
	if( r2->longValue() == 0 ) {
		r.flags.setDivideByZero( true );
	}
	else {
		// perform the operation
		switch( r1->getQuantifier() ) {
			case DQ_BYTE:	DO_DIV(char,charValue); break;
			case DQ_SHORT:	DO_DIV(short,shortValue); break;
			case DQ_INT:	DO_DIV(int,intValue); break;
			case DQ_LONG:	DO_DIV(long,longValue); break;
		}
	}
}

/**
 * Increments the given variable by 1
 * var - the given variable
 */
void CPUCore::inc( INSTRUCTION_T instruction ) {
	// lookup the value references
	ValueLocation* r1 = lookupReferenceA( instruction );

	// perform the operation
	switch( r1->getQuantifier() ) {
		case DQ_BYTE:	DO_INC(char,charValue); break;
		case DQ_SHORT:	DO_INC(short,shortValue); break;
		case DQ_INT:	DO_INC(int,intValue); break;
		case DQ_LONG:	DO_INC(long,longValue); break;
	}
}

/**
 * Copies the given value to the given variable
 * var - the given variable
 * value - the given value
 */
void CPUCore::mov( INSTRUCTION_T instruction ) {
	ValueLocation* r1 = lookupReferenceA( instruction );
	ValueLocation* r2 = lookupReferenceB( instruction );

	// perform the operation
	switch( r1->getQuantifier() ) {
		case DQ_BYTE:	DO_MOV(char,charValue); break;
		case DQ_SHORT:	DO_MOV(short,shortValue); break;
		case DQ_INT:	DO_MOV(int,intValue); break;
		case DQ_LONG:	DO_MOV(long,longValue); break;
	}
}

/**
 * Multiplies the given value to the given variable
 * var - the given variable
 * value - the given value
 */
void CPUCore::mul( INSTRUCTION_T instruction ) {
	ValueLocation* r1 = lookupReferenceA( instruction );
	ValueLocation* r2 = lookupReferenceB( instruction );

	// perform the operation
	switch( r1->getQuantifier() ) {
		case DQ_BYTE:	DO_MUL(char,charValue); break;
		case DQ_SHORT:	DO_MUL(short,shortValue); break;
		case DQ_INT:	DO_MUL(int,intValue); break;
		case DQ_LONG:	DO_MUL(long,longValue); break;
	}
}

/**
 * Bitwise NOTs the given value to the given variable
 * var - the given variable
 * value - the given value
 */
void CPUCore::_not( INSTRUCTION_T instruction ) {
	ValueLocation* r1 = lookupReferenceA( instruction );

	// perform the operation
	switch( r1->getQuantifier() ) {
		case DQ_BYTE:	DO_NOT(char,charValue); break;
		case DQ_SHORT:	DO_NOT(short,shortValue); break;
		case DQ_INT:	DO_NOT(int,intValue); break;
		case DQ_LONG:	DO_NOT(long,longValue); break;
	}
}

/**
 * Bitwise ORs the given value to the given variable
 * var - the given variable
 * value - the given value
 */
void CPUCore::_or( INSTRUCTION_T instruction ) {
	ValueLocation* r1 = lookupReferenceA( instruction );
	ValueLocation* r2 = lookupReferenceB( instruction );

	// perform the operation
	switch( r1->getQuantifier() ) {
		case DQ_BYTE:	DO_OR(char,charValue); break;
		case DQ_SHORT:	DO_OR(short,shortValue); break;
		case DQ_INT:	DO_OR(int,intValue); break;
		case DQ_LONG:	DO_OR(long,longValue); break;
	}
}

/**
 * Shifts Left the given value to the given variable
 * var - the given variable
 * value - the given value
 */
void CPUCore::shl( INSTRUCTION_T instruction ) {
	// lookup the value references
	ValueLocation* r1 = lookupReferenceA( instruction );
	ValueLocation* r2 = lookupReferenceB( instruction );

	// perform the operation
	switch( r1->getQuantifier() ) {
		case DQ_BYTE:	DO_SHL(char,charValue); break;
		case DQ_SHORT:	DO_SHL(short,shortValue); break;
		case DQ_INT:	DO_SHL(int,intValue); break;
		case DQ_LONG:	DO_SHL(long,longValue); break;
	}
}

/**
 * Shifts Right the given value to the given variable
 * var - the given variable
 * value - the given value
 */
void CPUCore::shr( INSTRUCTION_T instruction ) {
	// lookup the value references
	ValueLocation* r1 = lookupReferenceA( instruction );
	ValueLocation* r2 = lookupReferenceB( instruction );

	// perform the operation
	switch( r1->getQuantifier() ) {
		case DQ_BYTE:	DO_SHR(char,charValue); break;
		case DQ_SHORT:	DO_SHR(short,shortValue); break;
		case DQ_INT:	DO_SHR(int,intValue); break;
		case DQ_LONG:	DO_SHR(long,longValue); break;
	}
}

/**
 * Subtracts the given value to the given variable
 * var - the given variable
 * value - the given value
 */
void CPUCore::sub( INSTRUCTION_T instruction ) {
	// lookup the value references
	ValueLocation* r1 = lookupReferenceA( instruction );
	ValueLocation* r2 = lookupReferenceB( instruction );

	// perform the operation
	switch( r1->getQuantifier() ) {
		case DQ_BYTE:	DO_SUB(char,charValue); break;
		case DQ_SHORT:	DO_SUB(short,shortValue); break;
		case DQ_INT:	DO_SUB(int,intValue); break;
		case DQ_LONG:	DO_SUB(long,longValue); break;
	}
}

/**
 * Exchanges the values of the given variables
 * var1 - the given variable 1
 * var2 - the given variable 2
 */
void CPUCore::xchg( INSTRUCTION_T instruction ) {
	// lookup the value references
	ValueLocation* r1 = lookupReferenceA( instruction );
	ValueLocation* r2 = lookupReferenceB( instruction );

	// perform the operation
	switch( r1->getQuantifier() ) {
		case DQ_BYTE:	DO_XCHG(char,r1,r2); break;
		case DQ_SHORT:	DO_XCHG(short,r1,r2); break;
		case DQ_INT:	DO_XCHG(int,r1,r2); break;
		case DQ_LONG:	DO_XCHG(long,r1,r2); break;
	}
}

/**
 * Bitwise XORs the given value to the given variable
 * var - the given variable
 * value - the given value
 */
void CPUCore::_xor( INSTRUCTION_T instruction ) {
	// lookup the value references
	ValueLocation* r1 = lookupReferenceA( instruction );
	ValueLocation* r2 = lookupReferenceB( instruction );

	// perform the operation
	switch( r1->getQuantifier() ) {
		case DQ_BYTE: 	DO_XOR(char,charValue); break;
		case DQ_SHORT:	DO_XOR(short,shortValue); break;
		case DQ_INT: 	DO_XOR(int,intValue); break;
		case DQ_LONG: 	DO_XOR(long,longValue); break;
	}
}
