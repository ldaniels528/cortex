/*
 * TestClassGen.cpp
 *
 *  Created on: Mar 2, 2010
 *      Author: ldaniels
 */

#include <stdio.h>

#include "../../Sucre/src/OpCodes.h"

void bitTesting() {
	int x = 0x80; 			// 1000.0000 = 80h
	int y = ( 0xFF ^ x ); 	// 0111.1111 = 7Fh

	printf( "x = %02X, y = %02X\n", x, y );
}

void shiftTest() {
	char* ca = new char[20];
	ca[0] = 0xAB;
	ca[1] = 0xCD;
	ca[2] = 0xEF;
	ca[3] = 0xFF;

	int i = *((int*)&ca[0]);
	printf( "i = %04X\n", i );
}

/**
 * For stand alone operation
 * argc - the argument count
 * argv - the arguments
 */
int main( int argc, char *argv[] ) {
	shiftTest();
	return 0;
}
