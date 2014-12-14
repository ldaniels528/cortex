/*
 * AssemblyCompiler.cpp
 *
 *  Created on: March 5, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include <stdio.h>
#include <string.h>
#include <sstream>

#include "AssemblyParser.h"

/**
 * Constructor
 */
AssemblyParser::AssemblyParser( CompilerState* state ) {
	// cache the compiler state
	this->state = state;

	// initialize the parser
	tok = new Tokenizer();
	tok->setIgnoreDelimiters( true );
	tok->setNewLineSignificance( true );

	// build the condition array
	COND_MAP["NONE"]	= CND_NONE;
	COND_MAP["C"] 		= CND_C;
	COND_MAP["NC"] 		= CND_NC;
	COND_MAP["CXZ"] 	= CND_CXZ;
	COND_MAP["CXNZ"] 	= CND_CXNZ;
	COND_MAP["DBZ"] 	= CND_XDZ;
	COND_MAP["NDBZ"] 	= CND_NXDZ;
	COND_MAP["F"] 		= CND_F;
	COND_MAP["NF"] 		= CND_NF;
	COND_MAP["HF"] 		= CND_D_INC;
	COND_MAP["HB"] 		= CND_D_DEC;
	COND_MAP["G"] 		= CND_G;
	COND_MAP["NG"] 		= CND_NG;
	COND_MAP["GE"] 		= CND_GE;
	COND_MAP["NGE"] 	= CND_NGE;
	COND_MAP["L"] 		= CND_L;
	COND_MAP["NL"] 		= CND_NL;
	COND_MAP["LE"] 		= CND_LE;
	COND_MAP["NLE"]	 	= CND_NLE;
	COND_MAP["O"] 		= CND_O;
	COND_MAP["NO"] 		= CND_NO;
	COND_MAP["P"] 		= CND_P;
	COND_MAP["NP"]		= CND_NP;
	COND_MAP["S"]		= CND_S;
	COND_MAP["NS"]		= CND_NS;
	COND_MAP["X"]		= CND_X;
	COND_MAP["NX"]		= CND_NX;
	COND_MAP["Z"]		= CND_Z;
	COND_MAP["NZ"]		= CND_NZ;

	// build the reverse condition mapping
	COND_LIST = new const char*[ COND_MAP.size() ];
	importOpCodes( COND_MAP, COND_LIST );

	// build the flag mapping
	FLAG_MAP["C"]		= FLAG_CF;
	FLAG_MAP["D"]		= FLAG_DF;
	FLAG_MAP["F"]		= FLAG_FF;
	FLAG_MAP["G"]		= FLAG_GF;
	FLAG_MAP["L"]		= FLAG_LF;
	FLAG_MAP["O"]		= FLAG_OF;
	FLAG_MAP["P"]		= FLAG_PF;
	FLAG_MAP["S"]		= FLAG_SF;
	FLAG_MAP["X"]		= FLAG_XF;
	FLAG_MAP["XD"]		= FLAG_XDF;
	FLAG_MAP["Z"]		= FLAG_ZF;

	// build the reverse flag mapping
	FLAG_LIST = new const char*[ FLAG_MAP.size() ];
	importOpCodes( FLAG_MAP, FLAG_LIST );

	// create the register mapping
	RegisterSet r;
	for( int n = 0; n < REGISTERS; n++ ) {
		const char* name = r.regs[n]->getName();
		REG_CODES[name] = n;
	}

	// build the reverse register mapping
	REG_NAMES = new const char*[REG_CODES.size()];
	importOpCodes( REG_CODES, REG_NAMES );

	// create the mapping of the Type 0 instructions
	CMDS_TYPE0["dump"]			= DUMP;
	CMDS_TYPE0["hlt"] 			= HLT;
	CMDS_TYPE0["nop"] 			= NOP;
	CMDS_TYPE0["pi"]			= PI;
	CMDS_TYPE0["popall"]		= POPALL;
	CMDS_TYPE0["popflags"]		= POPFLAGS;
	CMDS_TYPE0["pushall"]		= PUSHALL;
	CMDS_TYPE0["pushflags"]		= PUSHFLAGS;
	CMDS_TYPE0["ret"]			= RET;

	// create the mapping of the Type 1A instructions
	CMDS_TYPE1A["dec"]			= DEC;
	CMDS_TYPE1A["inc"]			= INC;
	CMDS_TYPE1A["not"]			= NOT;
	CMDS_TYPE1A["peek"]			= PEEK;
	CMDS_TYPE1A["pop"]			= POP;
	CMDS_TYPE1A["push"]			= PUSH;

	// create the mapping of the Type 1B instructions
	CMDS_TYPE1B["abs"]			= ABS;
	CMDS_TYPE1B["cos"]			= COS;
	CMDS_TYPE1B["sin"]			= SIN;

	// create the mapping of the Type 2A instructions
	CMDS_TYPE2A["add"]			= ADD;
	CMDS_TYPE2A["and"]			= AND;
	CMDS_TYPE2A["cmp"]			= CMP;
	CMDS_TYPE2A["div"]			= DIV;
	CMDS_TYPE2A["mod"]			= MOD;
	CMDS_TYPE2A["mov"]			= MOV;
	CMDS_TYPE2A["or"]			= OR;
	CMDS_TYPE2A["sub"]			= SUB;
	CMDS_TYPE2A["shl"]			= SHL;
	CMDS_TYPE2A["shr"]			= SHR;
	CMDS_TYPE2A["xor"]			= XOR;

	// create the mapping of the Type 2B instructions
	CMDS_TYPE2A["movaddr"]		= MOVADDR_GP0;

	// create the mapping of the Type 3A instructions
	CMDS_TYPE3A["call"]			= CALL;
	CMDS_TYPE3A["jmp"]			= JMP;
	CMDS_TYPE3A["loop"]			= LOOP;

	// create the mapping of the Type 3C instructions
	CMDS_TYPE3B["clrflag"]		= CLRFLAG;
	CMDS_TYPE3B["flipflag"]		= FLIPFLAG;
	CMDS_TYPE3B["setflag"]		= SETFLAG;

	// create the mapping of the Type 4A instructions
	CMDS_TYPE4A["flip"]			= FLIP;
	CMDS_TYPE4A["sort"] 		= SORT;

	// create the mapping of the Type 4B instructions
	CMDS_TYPE4B["copy"] 		= COPY;
	CMDS_TYPE4B["fill"]			= FILL;
	CMDS_TYPE4B["scan"] 		= SCAN;

	// create the mapping of the Type 5 instructions
	CMDS_TYPE5["echo"]			= ECHO;
	CMDS_TYPE5["invoke"]		= INVOKE;
	CMDS_TYPE5["loadclass"]		= LOADCLASS;
	CMDS_TYPE5["params"]		= PARAMS;
	CMDS_TYPE5["setptr"]		= SETPTR;
	CMDS_TYPE5["stage"]			= STAGE;

	// build the reverse mapping
	OP_NAMES = new const char*[256];
	importOpCodes( CMDS_TYPE0, OP_NAMES );
	importOpCodes( CMDS_TYPE1A, OP_NAMES );
	importOpCodes( CMDS_TYPE2A, OP_NAMES );
	importOpCodes( CMDS_TYPE2B, OP_NAMES );
	importOpCodes( CMDS_TYPE3A, OP_NAMES );
	importOpCodes( CMDS_TYPE3B, OP_NAMES );
	importOpCodes( CMDS_TYPE4A, OP_NAMES );
	importOpCodes( CMDS_TYPE4B, OP_NAMES );
	importOpCodes( CMDS_TYPE5, OP_NAMES );

	// build the quantifier mapping
	QUANTIFIERS = new const char*[6];
	QUANTIFIERS[DQ_BYTE]		= "byte";
	QUANTIFIERS[DQ_SHORT]		= "short";
	QUANTIFIERS[DQ_INT]			= "int";
	QUANTIFIERS[DQ_LONG]		= "long";
	QUANTIFIERS[DQ_FLOAT]		= "float";
	QUANTIFIERS[DQ_DOUBLE]		= "double";
}

/**
 * Destructor
 */
AssemblyParser::~AssemblyParser() {
	// clear all deference data
	CMDS_TYPE0.clear();
	CMDS_TYPE1A.clear();
	CMDS_TYPE2A.clear();
	CMDS_TYPE3A.clear();
	CMDS_TYPE3B.clear();
	CMDS_TYPE4A.clear();
	CMDS_TYPE4B.clear();
	CMDS_TYPE5.clear();
	REG_CODES.clear();

	// release all resources
	delete COND_LIST;
	delete OP_NAMES;
	delete QUANTIFIERS;
	delete REG_NAMES;
	delete tok;
}

/**
 * Compiles an assembly language block into byte code
 */
int AssemblyParser::parse( Scope* scope, TokenStack* stack ) {
	int errorCode = 0;
	Token* block;
	Token* t;

	// get the assembly block token
	block = stack->next();

	// if not a method scope, it's an error
	if( scope->getScopeType() != METHOD_SCOPE ) {
		SYNTAX_ERROR( "Assembly code can only be defined within a method", block );
		return -1;
	}

	// cast as a method
	Method* method = (Method*)scope;

	// get the assembly code
	string code;
	code.append( block->getText() );
	code = code.substr( 2, code.length() - 4 );

	// parse the assembly block
	tok->setText( code.data() );
	tok->setLineNumber( block->getLineNumber() );

	// create a new token stack
	TokenStack* substack = new TokenStack();

	// iterate the tokens
	while( !errorCode && tok->hasNext() ) {
		// populate the stack
		bool ok;
		do {
			ok = ( ( t = tok->next() ) != NULL ) && ( t->getType() != tok::END_OF_STATEMENT );
			if( ok ) {
				//printf( "AssemblyParser::compile - |%s|\n", t->getText() );
				substack->pushFront( t );
			}
		} while( ok );

		//printf( "AssemblyParser::compile - substack.size = %d, hasMore = %s\n", stack->size(), stack->hasNext() ? "yes" : "no" );

		// evaluate the assembly code
		if( substack->hasNext() ) {
			errorCode = parseAssembly( method, substack );
		}
	}

	// delete the stack
	delete substack;

	return errorCode;
}

/**
 * Returns the string representation of the given instruction
 */
const char* AssemblyParser::disassemble( Scope* scope, OFFSET_T offset, INSTRUCTION_T instruction ) {
	// create the buffer
	char* code = new char[255];

	// decipher the opCode portion of the instruction
	const int opCode = instruction >> A_OP & MASK_OP;

	// is it a TYPE 0 instruction? (e.g. "HLT")
	if( opCode >= TYPE0_START && opCode <= TYPE0_END ) {
		sprintf( code, "%s", OP_NAMES[opCode] );
	}

	// is it a TYPE 1A instruction? (e.g. "POP AX")
	else if( opCode >= TYPE1A_START && opCode <= TYPE1A_END ) {
		const int refType	= instruction >> A_REF & MASK_REF;
		const int reg 		= instruction >> A_R1 & MASK_REG;
		sprintf( code, ( refType == REF_MEM_REG ) ? "%s [%s]" : "%s %s", OP_NAMES[opCode], REG_NAMES[reg] );
	}

	// is it a TYPE 1B instruction? (e.g. "COS FP0" or "COS [GP0]")
	else if( opCode >= TYPE1B_START && opCode <= TYPE1B_END ) {
		const int refType	= instruction >> A_REF & MASK_REF;
		const int reg 		= instruction >> A_R1 & MASK_REG;
		sprintf( code, ( refType == REF_MEM_REG ) ? "%s [%s]" : "%s %s", OP_NAMES[opCode], REG_NAMES[reg] );
	}

	// is it a TYPE 2A instruction? (e.g. "MOV AX,CX")
	else if( opCode >= TYPE2A_START && opCode <= TYPE2A_END ) {
		const short ofs16	= instruction & MASK_OFS16;
		const int refType	= instruction >> A_REF & MASK_REF;
		const int reg1		= instruction >> A_R1 & MASK_REG;
		const int reg2		= instruction >> A_R2 & MASK_REG;

		switch( refType ) {
			case REF_REG_REG:
				sprintf( code, "%s %s,%s", OP_NAMES[opCode], REG_NAMES[reg1], REG_NAMES[reg2] );
				break;
			case REF_REG_MEM:
				sprintf( code, "%s %s,[%s]", OP_NAMES[opCode], REG_NAMES[reg1], REG_NAMES[reg2] );
				break;
			case REF_MEM_REG:
				sprintf( code, "%s [%s],%s", OP_NAMES[opCode], REG_NAMES[reg1], REG_NAMES[reg2] );
				break;
			case REF_REG_VAL:
				sprintf( code, "%s %s,%0X", OP_NAMES[opCode], REG_NAMES[reg1], ofs16 );
				break;
		}
	}

	// is it a TYPE 2B instruction? (e.g. "MOVADDR GP0,field")
	else if( opCode >= TYPE2B_START && opCode <= TYPE2B_END ) {
		const int r64 = GP0 + ( instruction >> A_REG_SUB & MASK_REG_SUB );
		const char* refdata = disassembleReferenceData( scope, instruction );
		sprintf( code, "%s %s,%s", OP_NAMES[opCode], REG_NAMES[r64], refdata );
	}

	// is it a TYPE 3A instruction? (e.g. "CALL:NC label")
	else if( opCode >= TYPE3A_START && opCode <= TYPE3A_END ) {
		const int condition = instruction >> A_COND & MASK_COND;
		const short ofs16	= instruction & MASK_OFS16;
		if( condition == CND_NONE ) {
			sprintf( code, "%s %05lX", OP_NAMES[opCode], offset + ofs16 );
		}
		else {
			sprintf( code, "%s:%s %05lX", OP_NAMES[opCode], COND_LIST[condition], offset + ofs16 );
		}
	}

	// is it a TYPE 3B instruction? (e.g. "SETFLAG:C")
	else if( opCode >= TYPE3B_START && opCode <= TYPE3B_END ) {
		const int flag = ( instruction >> A_COND ) & MASK_COND;
		sprintf( code, "%s:%s", OP_NAMES[opCode], FLAG_LIST[flag] );
	}

	// is it a TYPE 4A instruction? (e.g. "FLIP BYTE GP0:GP1")
	else if( opCode >= TYPE4A_START && opCode <= TYPE4A_END ) {
		const int refType	= instruction >> A_REF & MASK_REF;
		const int reg1 		= instruction >> A_R1 & MASK_REG;
		const int reg2		= instruction >> A_R2 & MASK_REG;
		sprintf( code, "%s %s %s:%s", OP_NAMES[opCode], QUANTIFIERS[refType], REG_NAMES[reg1], REG_NAMES[reg2] );
	}

	// is it a TYPE 4B instruction? (e.g. "SCAN BYTE [REX] RAX:RCX")
	else if( opCode >= TYPE4B_START && opCode <= TYPE4B_END ) {
		const int refType	= instruction >> A_REF & MASK_REF;
		const int reg1 		= instruction >> A_R1 & MASK_REG;
		const int reg2	 	= instruction >> A_R2 & MASK_REG;
		const int reg3 		= instruction >> A_R3 & MASK_REG;
		sprintf( code, "%s %s [%s] %s:%s", OP_NAMES[opCode], QUANTIFIERS[refType], REG_NAMES[reg1], REG_NAMES[reg2], REG_NAMES[reg3] );
	}

	// is it a TYPE 5 instruction? (e.g. "STAGE field")
	else if( opCode >= TYPE5_START && opCode <= TYPE5_END ) {
		const int accessType = instruction >> A_ARG & MASK_ARG;
		const char* refdata	 = disassembleReferenceData( scope, instruction );
		sprintf( code, accessType ? "%s %s" : "%s \"%s\"", OP_NAMES[opCode], refdata );
	}

	// for all others ...
	else {
		strcpy( code, "????" );
	}

	// return the instruction
	return code;
}

/**
 * Disassembles the given instruction's reference data into a string representation
 */
const char* AssemblyParser::disassembleReferenceData( Scope* scope, INSTRUCTION_T instruction ) {
	// retrieve the reference index
	const int refIndex = ( instruction & MASK_OFS24 );

	// lookup the reference data
	ReferenceData* refdata = scope->lookupReferenceData( refIndex );

	// return the data
	return ( refdata != NULL ) ? refdata->getContent() : "????";
}

/**
 * Resolves an assembly language command
 */
int AssemblyParser::parseAssembly( Method* method, TokenStack* stack ) {
	int errorCode = 0;

	// process the stack
	while( !errorCode && stack->hasNext() ) {
		// the first argument is the command
		const char *command = stack->peek()->getText();
		//printf( "command = |%s|\n", command );

		// is the command is a 'Type 0' command? (e.g. "HLT")
		if( CMDS_TYPE0.count( command ) ) {
			errorCode = parseAssemblyType0( method, stack );
		}

		// is the command is a 'Type 1A' command? (e.g. "PUSH AX")
		else if( CMDS_TYPE1A.count( command ) ) {
			errorCode = parseAssemblyType1A( method, stack );
		}

		// is the command is a 'Type 1B' command? (e.g. "COS FP0")
		else if( CMDS_TYPE1B.count( command ) ) {
			errorCode = parseAssemblyType1B( method, stack );
		}

		// is the command is a 'Type 2A' command? (e.g. "MOV AX,CX")
		else if( CMDS_TYPE2A.count( command ) ) {
			errorCode = parseAssemblyType2A( method, stack );
		}

		// is the command is a 'Type 2B' command? (e.g. "MOVADDR EAX,field")
		else if( CMDS_TYPE2B.count( command ) ) {
			errorCode = parseAssemblyType2B( method, stack );
		}

		// is the command is a 'Type 3A' command? (e.g. "JMP:NC @here")
		else if( CMDS_TYPE3A.count( command ) ) {
			errorCode = parseAssemblyType3A( method, stack );
		}

		// is the command is a 'Type 3B' command? (e.g. "CLRFLAG:C")
		else if( CMDS_TYPE3B.count( command ) ) {
			errorCode = parseAssemblyType3B( method, stack );
		}

		// is the command is a 'Type 4A' command? (e.g. "FLIP BYTE RAX:RBX")
		else if( CMDS_TYPE4A.count( command ) ) {
			errorCode = parseAssemblyType4A( method, stack );
		}

		// is the command is a 'Type 4B' command? (e.g. "COPY WORD RCX RAX:RBX")
		else if( CMDS_TYPE4B.count( command ) ) {
			errorCode = parseAssemblyType4B( method, stack );
		}

		// is the command is a 'Type 5' command? (e.g. "ECHO data")
		else if( CMDS_TYPE5.count( command ) ) {
			errorCode = parseAssemblyType5( method, stack );
		}

		// is it a label? (e.g. "@here")
		else if( !strcmp( command, "@" ) ) {
			errorCode = parseLabel( method, stack );
		}

		// must be an error ...
		else {
			SYNTAX_ERROR( "unrecognized assembly instruction", stack->next() );
			return -1;
		}
	}

	return errorCode;
}

/**
 * Parses a command and converts it to byte code (e.g. "HLT")
 */
int AssemblyParser::parseAssemblyType0( Method* method, TokenStack* stack ) {
	// get the command token
	Token* command = stack->next();

	// there should be no arguments
	if( stack->hasNext() ) {
		PARAMS_ERROR( 1, 1, command );
		return 1;
	}

	// get the code buffer
	CodeBuffer* cb = method->getCodeBuffer();

	// commit the byte code
	INSTRUCTION_T instruction = CMDS_TYPE0[ command->getText() ] << A_OP;
	cb->putInstruction( instruction );
	return 0;
}

/**
 * Parses a command and converts it to byte code (e.g. "PUSH RAX" or "PUSH [RAX]")
 */
int AssemblyParser::parseAssemblyType1A( Method* method, TokenStack* stack ) {
	int errorCode = 0;

	// get the command token
	Token* command = stack->next();

	// get the code buffer
	CodeBuffer* cb = method->getCodeBuffer();

	// if there is no next element ...
	if( !stack->hasNext() ) {
		SYNTAX_ERROR( "register or reference expected", command );
		return -1;
	}

	// get the operand
	Parameters *p = lookupParameters( stack );
	if( p->operand1 == NULL ) {
		SYNTAX_ERROR( "unrecognized argument", stack->last() );
		errorCode = 4;
	}

	else {
		// lookup the opCode
		INSTRUCTION_T opCode = CMDS_TYPE1A[ command->getText() ];

		// commit the byte code
		cb->putInstruction( getInstruction( opCode, p ) );
	}

	// release resources
	if( p != NULL ) {
		delete p;
	}

	// return the code
	return errorCode;
}

/**
 * Parses a command and converts it to byte code (e.g. "COS FP0")
 */
int AssemblyParser::parseAssemblyType1B( Method* method, TokenStack* stack ) {
	int errorCode = 0;

	// get the command token
	Token* command = stack->next();

	// get the code buffer
	CodeBuffer* cb = method->getCodeBuffer();

	// if there is no next element ...
	if( !stack->hasNext() ) {
		SYNTAX_ERROR( "register or reference expected", command );
		return -1;
	}

	// get the operand
	Parameters *p = lookupParameters( stack );
	if( p->operand1 == NULL ) {
		SYNTAX_ERROR( "unrecognized argument", stack->last() );
		errorCode = 4;
	}

	else {
		// lookup the opCode
		INSTRUCTION_T opCode = CMDS_TYPE1B[ command->getText() ];

		// commit the byte code
		cb->putInstruction( getInstruction( opCode, p ) );
	}

	// release resources
	if( p != NULL ) {
		delete p;
	}

	// return the code
	return errorCode;
}

/**
 * Parses a command and converts it to byte code (e.g. "ADD AX,[BX]")
 */
int AssemblyParser::parseAssemblyType2A( Method* method, TokenStack* stack ) {
	int errorCode = 0;

	// get the command token
	Token* command = stack->next();

	// if there is no next element ...
	if( !stack->hasNext() ) {
		SYNTAX_ERROR( "register or reference expected", command );
		return -1;
	}

	// validate the format (e.g. "MOV RCX,RAX" or "MOV RAX,[RBX]" or "MOV [RBX],RAX")
	// argument index 1 should be a register or memory location
	// argument index 2 should be a register or memory location
	Parameters *p = lookupParameters( stack );

	// validate operand #1
	if( p->operand1 == NULL ) {
		SYNTAX_ERROR( "unrecognized identifier", stack->last() );
		errorCode = 1;
	}

	// validate operand #2
	else if( p->operand2 == NULL ) {
		SYNTAX_ERROR( "unrecognized identifier", stack->last() );
		errorCode = 2;
	}

	// passed all tests ...
	else {
		// get the code buffer
		CodeBuffer* cb = method->getCodeBuffer();

		// lookup the opCode
		int opCode = CMDS_TYPE2A[ command->getText() ];

		// commit the byte code
		cb->putInstruction( getInstruction( opCode, p ) );
	}

	// release resources
	if( p != NULL ) {
		delete p;
	}

	// return the code
	return errorCode;
}

/**
 * Parses a command and converts it to byte code (e.g. "MOVADDR GP0,field")
 */
int AssemblyParser::parseAssemblyType2B( Method* method, TokenStack* stack ) {
	int errorCode = 0;
	Token* t;

	// get the command token
	Token* command = stack->next();

	// validate the format (e.g. "MOVADDR GP0,field")
	// argument index 1 should be a 64-bit general purpose register
	// argument index 2 should be a field

	// lookup the 64-bit general purpose register
	int reg64;
	if( !( t = stack->next() ) ||
		!REG_CODES.count(t->getText()) ||
		!( reg64 = REG_CODES[t->getText()] ) ||
		!( reg64 >= GP0 && reg64 <= GP7 ) ) {
			// report the error
			SYNTAX_ERROR( "64-bit general purpose register expected", stack->last() );
			return -1;
	}

	// lookup the field name
	const char* fieldname;
	if( !( t = stack->next() ) ||
		!( fieldname = t->getText() ) ||
		!method->lookupField( fieldname ) ) {
			// report the error
			char* message = new char[256];
			sprintf( message, "Field '%s' is not defined in scope", fieldname );
			SYNTAX_ERROR( message, stack->last() );
			delete message;
			return -2;
	}

	// get the code buffer
	CodeBuffer* cb = method->getCodeBuffer();

	// lookup the opCode
	const int opCode = CMDS_TYPE2B[ command->getText() ];

	// commit the byte code
	INSTRUCTION_T instruction = opCode << A_OP | ( reg64 & MASK_REG_SUB ) << A_REG_SUB;
	OFFSET_T offset = cb->putInstruction( instruction );
	method->addReferenceData( new ReferenceData( state->nextDataRefId(), fieldname, offset ) );

	// return the code
	return errorCode;
}

/**
 * Parses a Type 3B command (e.g. "JMP:C @there") and converts it to byte code
 */
int AssemblyParser::parseAssemblyType3A( Method* method, TokenStack* stack ) {
	int condition = 0;
	int errorCode = 0;

	// get the command token
	Token* command = stack->next();
	Token* t;

	// is there a condition?
	if( stack->hasNext() && ( t = stack->peek() ) && strcmp( t->getText(), "@" ) ) {
		// get the condition
		const char* flag = state->getIdentifierName( stack, "flag" );
		if( flag == NULL || !COND_MAP.count( flag ) ) {
			SYNTAX_ERROR( "flag identifier expected", stack->last() );
			return -1;
		}
		// lookup the condition
		condition = COND_MAP[flag];
	}

	// if there is no next element ...
	if( !stack->hasNext() ||
		strcmp( stack->next()->getText(), "@" ) ||
		!stack->hasNext() ||
		stack->peek()->getType() != tok::ALPHA_NUMERIC ) {
		SYNTAX_ERROR( "label expected", stack->last() );
		return -1;
	}

	// get the label text
	const char *label = stack->next()->getText();
	OFFSET_T offset = method->getCodeBuffer()->getPosition();

	// create the instruction stub
	INSTRUCTION_T instruction = ( CMDS_TYPE3A[ command->getText() ] << A_OP ) | ( condition << A_COND );

	// write the instruction, and
	// create the call-back hook
	CodeBuffer* cb = method->getCodeBuffer();
	cb->putInstruction( instruction );
	method->addLabelReference( new LabelReference( label, offset ) );

	// return the code
	return errorCode;
}

/**
 * Parses a Type 3B command (e.g. "CLRFLAG:C") and converts it to byte code
 */
int AssemblyParser::parseAssemblyType3B( Method* method, TokenStack* stack ) {
	int errorCode = 0;

	// get the command token
	Token* command = stack->next();

	// get the condition
	const char* flagname = state->getIdentifierName( stack, "flag" );
	if( flagname == NULL || !FLAG_MAP.count( flagname ) ) {
		SYNTAX_ERROR( "flag identifier expected", stack->last() );
		return -1;
	}
	const int flag = FLAG_MAP[flagname];

	// create the instruction stub
	INSTRUCTION_T instruction = ( CMDS_TYPE3B[ command->getText() ] << A_OP ) | ( flag << A_COND );

	// write the instruction
	method->getCodeBuffer()->putInstruction( instruction );

	// return the code
	return errorCode;
}

/**
 * Parses a Type 4A command (e.g. "FLIP BYTE RAX:RBX") and converts it to byte code
 */
int AssemblyParser::parseAssemblyType4A( Method* method, TokenStack* stack ) {
	// TODO add logic here
	SYNTAX_ERROR( "Assembly code is not implemented", stack->last() );
	return -1;
}

/**
 * Parses a Type 4B command (e.g. "COPY WORD RCX RAX:RBX") and converts it to byte code
 */
int AssemblyParser::parseAssemblyType4B( Method* method, TokenStack* stack ) {
	// TODO add logic here
	SYNTAX_ERROR( "Assembly code is not implemented", stack->last() );
	return -1;
}


/**
 * Parses a Type 5 command (e.g. "ECHO data") and converts it to byte code
 */
int AssemblyParser::parseAssemblyType5( Method* method, TokenStack* stack ) {
	Token* t;
	int errorCode = 0;

	// get the command token
	Token* command = stack->next();

	// there must be a next element...
	if( !stack->hasNext() ||
		!( t = stack->next() ) ) {
		SYNTAX_ERROR( "unexpected end of statement", stack->last() );
		return -1;
	}

	// gather the properties
	const char* data = t->getText();
	int accessMethod = 0;

	// is the token quoted text?
	switch( t->getType() ) {
		// is it quoted text?
		case tok::DQUOTE_TEXT:
			accessMethod = ARG_DIRECT;
			break;

		// is it a variable?
		case tok::ALPHA_NUMERIC:
			accessMethod = ARG_PTR;
			if( !method->lookupField( data ) ) {
				char *message = new char[256];
				sprintf( message, "Field '%s' is not defined in scope", data );
				SYNTAX_ERROR( message, command );
				delete message;
				return -1;
			}
			break;

		// anything else is an error ...
		default:
			SYNTAX_ERROR( "unexpected token", stack->last() );
			return -1;
	}

	// create the instruction stub
	// example: ECHO argument
	INSTRUCTION_T instruction = ( CMDS_TYPE5[ command->getText() ] << A_OP | accessMethod << A_ARG );
	OFFSET_T offset = method->getCodeBuffer()->putInstruction( instruction );
	method->addReferenceData( new ReferenceData( state->nextDataRefId(), data, offset ) );

	// return the code
	return errorCode;
}

/**
 * Parses a label and prepares it for CALL, JUMP, and LOOP instructions
 * @param method the given method instance
 * @param stack the given token stack
 */
int AssemblyParser::parseLabel( Method* method, TokenStack* stack ) {
	Token *t;

	// skip the label symbol (@)
	stack->next();

	// if there's not at least one more token ...
	if( !stack->hasNext() || ( t = stack->next() )->getType() != tok::ALPHA_NUMERIC ) {
		SYNTAX_ERROR( "label identifier expected", t );
		return -1;
	}

	// build the label
	Label* label = new Label(
			t->getText(),
			method->getCodeBuffer()->getPosition()
	);

	// add the label to the method
	method->addLabel( label );
	return 0;
}

/**
 * Parses the given memory reference expression (e.g. "[BX+ABh]")
 * into byte code, and place the value into result.
 * @param expression the given expression
 */
int AssemblyParser::parseReferenceExpression( string expression ) {
	// setup the parser
	Tokenizer tok;

	// remove the '[' and ']' characters
	tok.setText( expression.substr( 1, expression.length() - 2 ).data() );

	// declare the return code
	int returnCode = 0;

	// create an array for the operands
	Operand** ops = new Operand*[3];

	// walk through the arguments
	int index = 0;
	int count = 0;
	Token *t, *last;
	while( ( t = tok.next() ) ) {
		// the maximum number of arguments is 3
		if( count >= 3 ) {
			delete ops;
			THROW( SYNTAX_ERROR( "the maximum number of arguments (3) was exceeded", t ) );
		}

		// handle the argument
		switch( count++ % 2 ) {
			// register or value
			case 0:
				// lookup the operand
				ops[index++] = lookupOperand( t );
				break;

			// operator
			case 1:
				if( t->getType() != tok::OPERATOR ) {
					SYNTAX_ERROR( "operator expected", t );
					returnCode = -1;
				}
				break;
		}

		// record the last token
		last = t;
	}

	// is the expression balanced?
	// there must be an even # of arguments
	if( count % 2 != 0 ) {
		SYNTAX_ERROR( "identifier expected", last );
		returnCode = -1;
	}

	return returnCode;
}


/**
 * Returns the instruction generated from the operational code
 * and the given parameters.
 * @param opCode the given operation code
 * @param p the given parameters
 */
INSTRUCTION_T AssemblyParser::getInstruction( INSTRUCTION_T opCode, Parameters *p ) {
	// include the opCode and reference type
	INSTRUCTION_T instruction = ( opCode << A_OP ) | ( p->reference << A_REF );

	// include operand1
	if( p->operand1 != NULL ) {
		instruction |= ( p->operand1->index & MASK_REG ) << A_R1;
	}

	// include operand2
	if( p->operand2 != NULL ) {

		switch( p->operand2->type ) {
			case assy::REGISTER:
				instruction |= ( p->operand2->index & MASK_REG ) << A_R2;
				break;

			case assy::VALUE:
				instruction |= ( p->operand2->intValue & MASK_OFS16 );
				break;

			case assy::MEMORY_REF:
				instruction |= ( p->operand2->index & MASK_REG ) << A_R2;
				instruction |= ( p->offset & ( ( p->reference == REF_REG_VAL ) ? 0xFFFF : 0xFF ) );
				break;
		}
	}

	return instruction;
}

/**
 * Returns the reference type
 */
int AssemblyParser::getReferenceType( Operand *op1, Operand *op2 ) {
	// determine the reference types for both operands
	int rt1 = ( op1 != NULL && op1->type == assy::REGISTER ) ? 0 : 1;
	int rt2 = ( op2 != NULL && op2->type == assy::REGISTER ) ? 0 : 1;

	// return the composite reference type
	return ( op2 != NULL && op2->type == assy::VALUE ) ? REF_REG_VAL : ( ( rt1 << 1 ) | rt2 );
}

/**
 * Retrieve the parameters for the given arguments
 * argc - the given number of arguments
 * args - the given arguments
 */
Parameters* AssemblyParser::lookupParameters( TokenStack* stack ) {
	// create a new parameter instance
	Parameters *p = new Parameters();

	// there must be at least 1 token
	if( !stack->hasNext() ) {
		SYNTAX_ERROR( "operand expected", stack->last() );
		return p;
	}

	// get the first operand
	p->operand1 = lookupOperand( stack->next() );

	// is there second operand?
	if( ( p->operand1 != NULL ) && stack->hasNext()  ) {
		p->operand2 = lookupOperand( stack->next() );
	}

	// determine the reference type
	p->reference = getReferenceType( p->operand1, p->operand2 );
	return p;
}

/**
 * Retrieve the operand for the given argument
 * @param stack the given token stack
 */
Operand* AssemblyParser::lookupOperand( Token* t ) {
	// cache the token's text
	const char *arg = t->getText();

	// create a new operand
	Operand *op = NULL;

	// handle the token
	switch( t->getType() ) {
		case tok::INTEGER:
			op = new Operand();
			op->type = assy::VALUE;
			op->valueType = assy::INT_VALUE;
			op->intValue = atoi( arg );
			break;

		case tok::DECIMAL:
			SYNTAX_ERROR( "decimal values are not allowed", t );
			break;

		case tok::PARENTHESIS_BLOCK:
			op = new Operand();
			op->type = assy::MEMORY_REF;
			op->index = parseReferenceExpression( arg );
			if( op->index < 0 ) {
				delete op;
				op = NULL;
			}
			break;

		default:
			// is it a register?
			if( REG_CODES.count( arg ) ) {
				op = new Operand();
				op->type = assy::REGISTER;
				op->index = REG_CODES[arg];
				return op;
			}

			// otherwise, it's an error
			else {
				SYNTAX_ERROR( "invalid or illegal identifier", t );
			}
			break;
	}

	return op;
}

/**
 * Imports the instruction to opCode mapping from the source, and puts it
 * in dest as opCode to instruction (reverse mapping).
 */
void AssemblyParser::importOpCodes( STRING_HASH_MAP(int) src, const char** dst ) {
	// build the reverse mapping
	for( STRING_HASH_MAP(int)::iterator i = src.begin(); i != src.end(); i++ ) {
		dst[i->second] = i->first;
	}
}
