/*
 * AssemblyCompiler.h
 *
 *  Created on: March 5, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef ASSEMBLYPARSER_H_
#define ASSEMBLYPARSER_H_

#include <hash_map.h>
#include <string.h>

#include "../Common.h"
#include "../CodeBuffer.h"
#include "../OpCodes.h"
#include "../class/Class.h"
#include "../class/ClassLoader.h"
#include "../class/Scope.h"
#include "../class/member/Label.h"
#include "../class/member/Method.h"
#include "../token/Token.h"
#include "../token/Tokenizer.h"
#include "../token/TokenStack.h"
#include "../runtime/RegisterSet.h"

#include "CompilerState.h"

/**
 * Assembly name space
 */
namespace assy {

	/**
	 * Operand Type enumeration
	 */
	enum OperandTypes {
		REGISTER, MEMORY_REF, VALUE
	};

	/**
	 * Operand Value Type enumeration
	 */
	enum ValueTypes {
		INT_VALUE, LONG_VALUE, DEC_VALUE
	};

}

/**
 * Operand
 */
struct Operand {
	assy::OperandTypes type;
	assy::ValueTypes valueType;
	int index;
	int intValue;
	long longValue;
	double doubleValue;
};

/**
 * Operand Parameters
 */
struct Parameters {
	Operand *operand1;
	Operand *operand2;
	int offset;
	int reference;
};

/**
 * This compiler is responsible for building binary code
 * based on assembly language commands (e.g. "MOV AX,CX")
 */
class AssemblyParser {
private:
	STRING_HASH_MAP(int) REG_CODES;
	STRING_HASH_MAP(int) CMDS_TYPE0;
	STRING_HASH_MAP(int) CMDS_TYPE1A;
	STRING_HASH_MAP(int) CMDS_TYPE1B;
	STRING_HASH_MAP(int) CMDS_TYPE2A;
	STRING_HASH_MAP(int) CMDS_TYPE2B;
	STRING_HASH_MAP(int) CMDS_TYPE3A;
	STRING_HASH_MAP(int) CMDS_TYPE3B;
	STRING_HASH_MAP(int) CMDS_TYPE4A;
	STRING_HASH_MAP(int) CMDS_TYPE4B;
	STRING_HASH_MAP(int) CMDS_TYPE5;
	STRING_HASH_MAP(int) COND_MAP;
	STRING_HASH_MAP(int) FLAG_MAP;
	const char** COND_LIST;
	const char** FLAG_LIST;
	const char** QUANTIFIERS;
	const char** REG_NAMES;
	const char** OP_NAMES;
	CompilerState* state;
	Tokenizer* tok;

public:

	/**
	 * Object Creation
	 */
	AssemblyParser( CompilerState* state );

	/**
	 * Object Deletion
	 */
	virtual ~AssemblyParser();

	/**
	 * Parses an assembly language block into byte code
	 */
	int parse( Scope* scope, TokenStack* stack );

	/**
	 * Returns the string representation of the given instruction
	 */
	const char* disassemble( Scope*, OFFSET_T, INSTRUCTION_T );

private:

	/**
	 * Disassembles the given instruction's reference data into a string representation
	 */
	const char* disassembleReferenceData( Scope*, INSTRUCTION_T );

	/**
	 * Parses an assembly language command
	 */
	int parseAssembly( Method* method, TokenStack* );

	/**
	 * Parses a Type 0 command (e.g. "HLT") and converts it to byte code
	 */
	int parseAssemblyType0( Method* method, TokenStack* );

	/**
	 * Parses a Type 1A command (e.g. "PUSH AX") and converts it to byte code
	 */
	int parseAssemblyType1A( Method* method, TokenStack* );

	/**
	 * Parses a Type 1B command (e.g. "COS FP0") and converts it to byte code
	 */
	int parseAssemblyType1B( Method* method, TokenStack* );

	/**
	 * Parses a Type 2A command (e.g. "MOV AX,CX") and converts it to byte code
	 */
	int parseAssemblyType2A( Method* method, TokenStack* );

	/**
	 * Parses a Type 2B command (e.g. "MOVADDR EAX,field") and converts it to byte code
	 */
	int parseAssemblyType2B( Method* method, TokenStack* );

	/**
	 * Parses a Type 3B command (e.g. "JMPIF:CARRY @there") and converts it to byte code
	 */
	int parseAssemblyType3A( Method* method, TokenStack* );

	/**
	 * Parses a Type 3C command (e.g. "CLRFLAG CARRY") and converts it to byte code
	 */
	int parseAssemblyType3B( Method* method, TokenStack* );

	/**
	 * Parses a Type 4A command (e.g. "FLIP BYTE RAX:RBX") and converts it to byte code
	 */
	int parseAssemblyType4A( Method* method, TokenStack* );

	/**
	 * Parses a Type 4B command (e.g. "COPY WORD RCX RAX:RBX") and converts it to byte code
	 */
	int parseAssemblyType4B( Method* method, TokenStack* );

	/**
	 * Parses a Type 5 command (e.g. "ECHO data") and converts it to byte code
	 */
	int parseAssemblyType5( Method* method, TokenStack* );

	/**
	 * Parses a label and prepares it for CALL, JUMP, and LOOP instructions
	 */
	int parseLabel( Method* method, TokenStack* );

	/**
	 * Parses the given memory reference expression (e.g. "[BX+EX+ABh]")
	 * into byte code, and place the value into result.
	 */
	int parseReferenceExpression( string expression );

	/**
	 * Returns the instruction generated from the operational code
	 * and the given parameters.
	 */
	INSTRUCTION_T getInstruction( INSTRUCTION_T opCode, Parameters* p );

	/**
	 * Returns the reference type
	 */
	int getReferenceType( Operand* op1, Operand* op2  );

	/**
	 * Retrieve the element type of the argument
	 */
	Operand* lookupOperand( Token* t );

	/**
	 * Retrieve the element type of the argument
	 */
	Parameters* lookupParameters( TokenStack* );

	/**
	 * Imports the instruction to opCode mapping from the source, and puts it
	 * in dest as opCode to instruction (reverse mapping).
	 */
	void importOpCodes( STRING_HASH_MAP(int) src, const char** dst );

};

#endif /* ASSEMBLYPARSER_H_ */
