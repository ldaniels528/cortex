/*
 * RegisterSet.cpp
 *
 *  Created on: March 19, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#include <stdio.h>

#include "RegisterSet.h"

/**
 * Constructor
 */
RegisterSet::RegisterSet() {
	// define the 64-bit pseudo registers
	this->ip		= 0;
	this->rrp 		= 0;
	this->spc		= 0;
	this->spd 		= 0;

	// define the 64-bit counter registers
	this->rc0		= 0;
	this->rc1		= 0;
	this->rc2		= 0;
	this->rc3		= 0;

	// define the 32-bit general purpose registers
	this->ax 		= new Register32GP( "ax" );
	this->bx 		= new Register32GP( "bx" );
	this->cx 		= new Register32GP( "cx" );
	this->dx 		= new Register32GP( "dx" );
	this->ex		= new Register32GP( "ex" );
	this->fx		= new Register32GP( "fx" );
	this->gx 		= new Register32GP( "gx" );
	this->hx 		= new Register32GP( "hx" );

	// define the 64-bit general purpose registers
	this->gp0 		= new Register64GP( "gp0" );
	this->gp1 		= new Register64GP( "gp1" );
	this->gp2 		= new Register64GP( "gp2" );
	this->gp3 		= new Register64GP( "gp3" );
	this->gp4		= new Register64GP( "gp4" );
	this->gp5		= new Register64GP( "gp5" );
	this->gp6 		= new Register64GP( "gp6" );
	this->gp7 		= new Register64GP( "gp7" );
	this->gp8		= new Register64GP( "gp8" );
	this->gp9		= new Register64GP( "gp9" );
	this->gpa 		= new Register64GP( "gpa" );
	this->gpb 		= new Register64GP( "gpb" );
	this->gpc 		= new Register64GP( "gpc" );
	this->gpd 		= new Register64GP( "gpd" );
	this->gpe 		= new Register64GP( "gpe" );
	this->gpf 		= new Register64GP( "gpf" );

	// define the 64-bit floating point registers
	this->fp0		= new Register64FP( "fp0" );
	this->fp1		= new Register64FP( "fp1" );
	this->fp2		= new Register64FP( "fp2" );
	this->fp3		= new Register64FP( "fp3" );
	this->fp4		= new Register64FP( "fp4" );
	this->fp5		= new Register64FP( "fp5" );
	this->fp6		= new Register64FP( "fp6" );
	this->fp7		= new Register64FP( "fp7" );

	// create the array of all registers
	this->regs 		= new Register*[REGISTERS];

	// define the 32-bit general purpose registers
	this->regs[AX] 	= ax;
	this->regs[BX]	= bx;
	this->regs[CX]	= cx;
	this->regs[DX]	= dx;
	this->regs[EX]	= ex;
	this->regs[FX]	= fx;
	this->regs[GX]	= gx;
	this->regs[HX]	= hx;

	// define the 64-bit floating pointer registers
	this->regs[FP0] = fp0;
	this->regs[FP1] = fp1;
	this->regs[FP2] = fp2;
	this->regs[FP3] = fp3;
	this->regs[FP4] = fp4;
	this->regs[FP5] = fp5;
	this->regs[FP6] = fp6;
	this->regs[FP7] = fp7;

	// define the 64-bit general purpose registers
	this->regs[GP0] = gp0;
	this->regs[GP1]	= gp1;
	this->regs[GP2]	= gp2;
	this->regs[GP3]	= gp3;
	this->regs[GP4]	= gp4;
	this->regs[GP5]	= gp5;
	this->regs[GP6]	= gp6;
	this->regs[GP7]	= gp7;
	this->regs[GP8]	= gp8;
	this->regs[GP9]	= gp9;
	this->regs[GPA]	= gpa;
	this->regs[GPB]	= gpb;
	this->regs[GPC]	= gpc;
	this->regs[GPD]	= gpd;
	this->regs[GPE]	= gpe;
	this->regs[GPF]	= gpf;
}

/**
 * Destruction
 */
RegisterSet::~RegisterSet() {
	// delete the individual registers
	for( int n = 0; n < REGISTERS; n++ ) {
		delete regs[n];
	}

	// delete the array
	delete[] regs;
}

/**
 * Dumps the contents of the registers to STDOUT
 */
void RegisterSet::dump() {
	// create the output buffer
	char* buf = new char[256];

	// 32-bit general purpose registers
	dumpRange( buf, 4, AX, HX );
	printf( "\n" );

	// 32-bit floating point registers
	dumpRange( buf, 2, FP0, FP7 );
	printf( "\n" );

	// 64-bit general purpose registers
	dumpRange( buf, 2, GP0, GPF );
	printf( "\n" );

	// 64-bit counter registers
	printf( "rc0 %016lX%6src1 %016lX\n", rc0, "", rc1 );
	printf( "rc2 %016lX%6src3 %016lX\n", rc2, "", rc3 );
	printf( "\n" );

	// display the instruction pointer and flags register
	printf( "spc %016lX%6sspd %016lX\n", spc, "", spd );
	printf( "rrp %016lX%6srip %016lX\n", rrp, "", ip );
	printf( "FLAGS %s\n", flags.toString() );

	// free resources
	delete buf;
}

/**
 * Dumps the contents of the given range of registers to STDOUT
 */
void RegisterSet::dumpRange( char* buf, int columns, int start, int end ) {
	int m = 0;

	// dump the registers
	for( int n = start; n <= end; n++ ) {
		sprintf( buf, ( ( columns == 4 ) ? "%s%s " : "%s%s%6s" ), buf, regs[n]->toString(), "" );

		// at every specified interval, start a new line
		m = (n - start);
		if( ( m > 0 ) && ( (m+1) % columns == 0 ) ) {
			printf( "%s\n", buf );
			*buf = '\0';
		}
	}

	// were any left in the buffer?
	if( (m+1) % columns > 0 ) {
		printf( "%s\n", buf );
		*buf = '\0';
	}
}

/**
 * Returns the register based on the given index
 */
Register* RegisterSet::indexOf( int index ) {
	return regs[index];
}

/**
 * Restores all registers from the stack
 */
void RegisterSet::popAll( char* stack ) {
	// don't forget the flags!
	popFlags( stack );

	// restore all values from the stack
	for( int n = REGISTERS; n >= 0; n-- ) {
		// decrement the stack
		spd -= sizeof(OFFSET_T);

		// restore the value
		REG_SIZE64* p = (REG_SIZE64*)&stack[spd];
		regs[n]->setValue( *p );
	}
}

/**
 * Restores the flags register from the stack
 */
void RegisterSet::popFlags( char* stack ) {
	// decrement the stack
	spd -= sizeof(OFFSET_T);

	// restore the value
	REG_SIZE64* p = (REG_SIZE64*)&stack[spd];
	flags.setValue( *p );
}

/**
 * Saves all registers onto the stack
 */
void RegisterSet::pushAll( char* stack ) {
	// save all values to the stack
	for( int n = 0; n < REGISTERS; n++ ) {
		// store the value
		REG_SIZE64* p = (REG_SIZE64*)&stack[spd];
		*p = regs[n]->getValue();

		// increment the stack
		spd += sizeof(OFFSET_T);
	}

	// don't forget the flags!
	pushFlags( stack );
}

/**
 * Saves the flags registers onto the stack
 */
void RegisterSet::pushFlags( char* stack  ) {
	// store the value
	REG_SIZE64* p = (REG_SIZE64*)&stack[spd];
	*p = flags.getValue();

	// increment the stack
	spd += sizeof(OFFSET_T);
}
