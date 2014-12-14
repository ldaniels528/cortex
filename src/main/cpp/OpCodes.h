/*
 * OpCodes.h
 *
 *  Created on: February 28, 2010
 *      Author: lawrence.daniels@gmail.com
 */

#ifndef OPCODES_H_
#define OPCODES_H_

// constants for settings for access modifiers
#define ABSTRACT		0x01 // 0000.0001
#define FINAL			0x02 // 0000.0010
#define PRIVACY_MASK	0x0C // 0000.1100
#define PACKAGE			0x00 // 0000.xx00 : xx = 00
#define PROTECTED		0x04 // 0000.xx00 : xx = 01
#define PRIVATE 		0x08 // 0000.xx00 : xx = 10
#define PUBLIC			0x0C // 0000.xx00 : xx = 11
#define STATIC			0x10 // 0001.0000
#define VIRTUAL			0x20 // 0010.0000 TODO remove virtual (using final instead)

// registers (32 registers = 5 bits, x2 = 10 bits)
#define REGISTERS	32

// define the (16) 64-bit general purpose registers
#define GP0			0
#define GP1			1
#define GP2			2
#define GP3			3
#define GP4			4
#define GP5			5
#define GP6			6
#define	GP7			7
#define GP8			8
#define GP9			9
#define GPA			10
#define GPB			11
#define GPC			12
#define GPD			13
#define GPE			14
#define GPF			15

// define the (8) 64-bit floating point registers
#define FP0			16
#define FP1			17
#define FP2			18
#define FP3			19
#define FP4			20
#define FP5			21
#define FP6			22
#define FP7			23

// define the (8) 32-bit general purpose registers
#define AX			24
#define BX			25
#define CX			26
#define DX			27
#define EX			28
#define FX			29
#define GX			30
#define	HX			31

// define the register mask
#define MASK_OP			0xFF	// (8-bits)  1111.1111
#define MASK_ARG		0x01	// (1-bits)			 1
#define MASK_DQ			0x07	// (3-bits)        111
#define MASK_REG		0x1F	// (5-bits)     1.1111
#define MASK_REG_SUB	0x0F	// (4-bits)       1111
#define MASK_REF		0x03	// (2-bits)         11
#define MASK_COND		0xFF	// (8-bits)	 1111.1111
#define MASK_OFS16		0xFFFF	// (24-bits) 1111.1111.1111.1111.1111.1111
#define MASK_OFS24		0x7FFFFF// (24-bits) 0111.1111.1111.1111.1111.1111

// OpCode binary shifting
#define A_OP			24	// 1111.1111.xxxx.xxxx.xxxx.xxxx.xxxx.xxxx
#define A_REG_SUB		24	// xxxx.1111.xxxx.xxxx.xxxx.xxxx.xxxx.xxxx
#define A_ARG			23	// xxxx.xxx1.xxxx.xxxx.xxxx.xxxx.xxxx.xxxx
#define A_REF			22	// xxxx.xxxx.11xx.xxxx.xxxx.xxxx.xxxx.xxxx
#define A_COND			16	// xxxx.xxxx.1111.1111.xxxx.xxxx.xxxx.xxxx
#define A_DQ			9	// xxxx.xxxx.xxxx.xxxx.xxxx.111x.xxxx.xxxx
#define A_R1			17	// xxxx.xxxx.xx11.111x.xxxx.xxxx.xxxx.xxxx
#define A_R2			12	// xxxx.xxxx.xxxx.xxx1.1111.xxxx.xxxx.xxxx
#define A_R3			7 	// xxxx.xxxx.xxxx.xxxx.xxxx.1111.1xxx.xxxx

// argument type identifiers
#define ARG_DIRECT		0
#define ARG_PTR			1

// data quantifiers
#define DQ_BYTE			0x00 // 000 = 1 byte  (char)
#define DQ_SHORT		0x01 // 001 = 2 bytes (short)
#define DQ_INT			0x02 // 010 = 4 bytes (int)
#define DQ_LONG			0x03 // 011 = 8 bytes (long)
#define DQ_FLOAT		0x04 // 100 = 4 bytes (float)
#define DQ_DOUBLE		0x05 // 101 = 8 bytes (double)

// references
#define REF_REG_REG		0x00 // 00 = A and B are both registers 				(e.g. "ADD AX,BX+00")
#define REF_REG_MEM		0x01 // 01 = A is a register, B is a memory reference	(e.g. "ADD AX,[BX+00]")
#define REF_MEM_REG		0x02 // 10 = A is a memory reference, B is a register	(e.g. "ADD [BX+EX+00],AX")
#define REF_REG_VAL		0x03 // 11 = A is a register, B is a 16-bit signed int	(e.g. "ADD AX,1234")

// FLAGS breakdown
#define FLAG_CF			0 	// Carry Flag
#define FLAG_FF			1 	// Found/Not Found Flag
#define FLAG_DF			2 	// Direction Flag
#define FLAG_GF			3 	// Greater Than Flag
#define FLAG_LF			4 	// Less Than Flag
#define FLAG_OF			5 	// Overflow Flag
#define FLAG_PF			6 	// Parity Flag
#define FLAG_SF			7 	// Sign Flag
#define FLAG_XF			8 	// Exception Flag
#define FLAG_XDF		9	// Division By Zero Flag
#define FLAG_ZF			10	// Zero Flag

// conditional constants (for JMP, LOOP and other flow instructions)
// see http://siyobik.info/index.php?module=x86&id=146
#define CND_NONE		0x00	// CALL label
#define CND_C			0x01	// CALL:C label
#define CND_NC			0x02	// CALL:NC label
#define CND_CXZ			0x03	// CALL:CXZ label
#define CND_CXNZ		0x04	// CALL:CXNZ label
#define CND_XDZ			0x05	// CALL:DBZ label
#define CND_NXDZ		0x06	// CALL:NDBZ label
#define CND_F			0x07	// CALL:F label
#define CND_NF			0x08	// CALL:NF label
#define CND_D_INC		0x09	// CALL:HF label
#define CND_D_DEC		0x0A	// CALL:HB label
#define CND_G			0x0B	// CALL:G label
#define CND_NG			0x0C	// CALL:NG label
#define CND_GE			0x0D	// CALL:GE label
#define CND_NGE			0x0E	// CALL:NGE label
#define CND_L			0x0F	// CALL:L label
#define CND_NL			0x10	// CALL:NL label
#define CND_LE			0x11	// CALL:LE label
#define CND_NLE			0x12	// CALL:NLE label
#define CND_O			0x13	// CALL:O label
#define CND_NO			0x14	// CALL:NO label
#define CND_P			0x15	// CALL:P label
#define CND_NP			0x16	// CALL:NP label
#define CND_S			0x17	// CALL:S label
#define CND_NS			0x18	// CALL:NS label
#define CND_X			0x19	// CALL:X label
#define CND_NX			0x1A	// CALL:NX label
#define CND_Z			0x1B	// CALL:Z label
#define CND_NZ			0x1C	// CALL:NZ label

// Instructions (8 bits = 256 distinct ops)
//
// MOV AX,DX = 0x4538EC00
//				0000.0101b 	= MOV
//				00b 		= A and B are registers
//				11.1000b	= AX
//				11.1011b	= DX
//				0000.0000	= 00h
// 				Layout: [op:8][rt:2][r0:5][r1:5][msc:4][ofs:8] = 32-bits
//
// SUB AX,[BX+AA] = 0x417AE0AA
//				0000.0001b 	= SUB
//				01b 		= A is a register, B is a memory reference
//				11.1010b	= CX
//				11.1000b	= AX
//				1010.1010	= AAh
// 				Layout: [op:8][rt:2][r0:5][r1:5][msc:4][ofs:8] = 32-bits
//
// ADD AX,1234 = 0x40F81234
//				0000.0000b 	= ADD
//				11b 		= A is a register, B is a 16-bit value
//				11.1000b	= AX
//	 0001.0010.0011.0100b	= 1234h
//				Layout: [op:8][rt:2][r0:5][msc:1][ofs:16] = 32-bits
//
// SORT LONG RAX:RCX = 0x37
//				0011.0111b	= SORT
//				11b			= DWORD (LONG)
//				1.1000b		= AX
//				1.1010b		= CX
//				Layout: [op:8][rt:2][r0:5][r1:5][msc:12] = 32-bits

/////////////////////////////////////
// Type 0 system instructions
/////////////////////////////////////
#define TYPE0_START		0x00
#define TYPE0_END		0x08

#define	HLT				0x00	// HLT
#define	NOP				0x01	// NOP
#define DUMP			0x02	// DUMP
#define PI				0x03	// PI - outputs to stack
#define	POPALL			0x04	// POPALL
#define POPFLAGS		0x05	// POPFLAGS
#define	PUSHALL			0x06	// PUSHALL
#define PUSHFLAGS		0x07	// PUSHFLAGS
#define	RET				0x08	// RET

/////////////////////////////////////
// Type 1A math/stack instructions
/////////////////////////////////////
#define TYPE1A_START	0x10
#define TYPE1A_END		0x15

#define	INC				0x10	// INC BYTE [BX]
#define	DEC				0x11	// DEC WORD [BX]
#define	NOT				0x12	// NOT AX

#define	PEEK			0x13	// PEEK R0
#define	POP				0x14	// POP R0
#define	PUSH			0x15	// PUSH DWORD [BX]

/////////////////////////////////////
// Type 1B math functions
/////////////////////////////////////
#define TYPE1B_START	0x16
#define TYPE1B_END		0x1A

#define ABS				0x16	// ABS FP0	- outputs to stack
#define ATAN			0x17	// ATAN FP0	- outputs to stack
#define COS				0x18	// COS FP1	- outputs to stack
#define SIN				0x19	// SIN FP2 	- outputs to stack
#define TAN				0x1A	// TAN FP3	- outputs to stack

/////////////////////////////////////
// Type 2A data instructions
/////////////////////////////////////
#define TYPE2A_START	0x20
#define TYPE2A_END		0x2E

#define	ADD				0x20	// ADD AX,[BX]	(think about "ADC")
#define	SUB				0x21	// SUB AX,[BX]	(think about "SBC")
#define	MUL				0x22	// MUL AX,CX
#define	DIV				0x23	// DIV AX,CX
#define	MOD				0x24	// MOD AX,CX
#define	MOV				0x25	// MOV AX,[&field] = field address is by index

#define	AND				0x26	// AND AX,CX
#define	OR				0x27	// OR  AX,CX
#define	XOR				0x28	// XOR AX,CX

#define	SHL				0x29	// SHL R0,R1
#define	SHR				0x2A	// SHR R0,R1
#define	ROL				0x2B	// ROL R0,R1
#define	ROR				0x2C	// ROR R0,R1

#define	CMP				0x2D	// CMP RAX,RCX
#define	XCHG			0x2E	// XCHG RAX,RCX

/////////////////////////////////////
// Type 2B data instructions
//	layout: iiii.irrr.xxxx.xxxx.xxxx.xxxx.xxxx.xxxx
/////////////////////////////////////
#define TYPE2B_START	0x30
#define TYPE2B_END		0x3F

#define MOVADDR_GP0		0x30	// MOVADDR GP0,field
#define MOVADDR_GP1		0x31	// MOVADDR GP1,field
#define MOVADDR_GP2		0x32	// MOVADDR GP2,field
#define MOVADDR_GP3		0x33	// MOVADDR GP3,field
#define MOVADDR_GP4		0x34	// MOVADDR GP4,field
#define MOVADDR_GP5		0x35	// MOVADDR GP5,field
#define MOVADDR_GP6		0x36	// MOVADDR GP6,field
#define MOVADDR_GP7		0x37	// MOVADDR GP7,field
#define MOVADDR_GP8		0x38	// MOVADDR GP8,field
#define MOVADDR_GP9		0x39	// MOVADDR GP9,field
#define MOVADDR_GPA		0x3A	// MOVADDR GPA,field
#define MOVADDR_GPB		0x3B	// MOVADDR GPB,field
#define MOVADDR_GPC		0x3C	// MOVADDR GPC,field
#define MOVADDR_GPD		0x3D	// MOVADDR GPD,field
#define MOVADDR_GPE		0x3E	// MOVADDR GPE,field
#define MOVADDR_GPF		0x3F	// MOVADDR GPF,field

/////////////////////////////////////
// Type 3A flow control instructions
/////////////////////////////////////
#define TYPE3A_START	0x40
#define TYPE3A_END		0x42

#define	CALL			0x40	// CALL @here
#define	JMP				0x41	// JMP:C @here
#define	LOOP			0x42	// LOOP @here

/////////////////////////////////////
// Type 3B flag instructions
/////////////////////////////////////
#define TYPE3B_START	0x43
#define TYPE3B_END		0x45

#define CLRFLAG			0x43	// CLRFLAG:C
#define FLIPFLAG		0x44	// FLIPFLAG:C
#define SETFLAG			0x45	// SETFLAG:C

/////////////////////////////////////
// Type 4A massive data instructions
/////////////////////////////////////
#define TYPE4A_START	0x50
#define TYPE4A_END		0x51

#define FLIP			0x50	// FLIP BYTE RAX:RBX
#define SORT			0x51	// SORT BYTE RAX:RBX

/////////////////////////////////////
// Type 4B massive data instructions
/////////////////////////////////////
#define TYPE4B_START	0x52
#define TYPE4B_END		0x54

#define	COPY			0x52	// COPY WORD RCX RAX:RBX
#define	FILL			0x53	// FILL BYTE [REX] RAX:RBX
#define	SCAN			0x54	// SCAN BYTE [REX] RAX:RBX - outputs to stack

/////////////////////////////////////
// Type 5A pointer instructions
/////////////////////////////////////
#define TYPE5_START		0x60
#define TYPE5_END		0x66

#define ECHO			0x60	// ECHO <IDX24>
#define INVOKE			0x61	// INVOKE <IDX24>
#define LOADCLASS		0x62	// LOADCLASS <IDX24>
#define NEWINST			0x63	// NEWINST <IDX24>
#define PARAMS			0x64	// PARAMS <IDX24>
#define SETPTR			0x65	// SETPTR <IDX24> - performs a field value assignment using the stack for the data address
#define STAGE			0x66	// STAGE <IDX24> - stages a data address on the stack

#endif /* OPCODES_H_ */
