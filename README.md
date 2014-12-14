Cortex
======

A multi-paradigm virtual machine-based experimental programming language

# Objective
To build a language that combines everything
that developers want into a single language. It will borrow
syntax and features from C++, C#, Java, Python, Perl, PHP, and Lisp.

The language will also combine many different programming methodologies
including Object-Oriented Programming (OOP), Functional Programming,
and Aspect-Oriented Programming (AOP).

# Focus
Syntactic sugar, and maximizing performance and flexibility

# Features
* 64-bit?
* Multiple Inheritance
* Metadata (Annotations)
* Operator Overloading
* built-in sorting algorithms
* Support for Assembly language
* Multi-platform. Write once, run anywhere.

## Syntax Ideas
1. The language will feature built-in support for lists (ordered), sets (unique) and maps (key-value pairs).
2. The language will also support a built-in Lisp-like map function.
3. All values are objects including primitive values like integers.
4. Function calls without parameters do not need parenthesis "()"; however,
	function definitions require them.
5. Functions can be used as read-only variables
6. The language will be strongly typed like Java, but will feature
 	conventions found in languages like PHP, Perl, Python, and Ruby.
7. The language will feature a C/C++ line define statement (preprocessor)
8. Specifying the variable type is optional for all built-in types.

## Design and Internals
Cortex Instruction Set (8 bits = 256 distinct ops)

This document describes work-in-progress ideas for building the 
assembly language component of Cortex.

## Type System
### Types - Integer
* sint8			    - 8-bit signed integer
* byte/uint8		- 8-bit unsigned integer
* sint16		 	- 16-bit signed integer
* uint16		 	- 16-bit unsigned integer
* sint32		 	- 32-bit signed integer
* uint32		 	- 32-bit unsigned integer
* sint64		 	- 64-bit signed integer
* uint64		 	- 64-bit unsigned integer

### Types - Floating Point
* float32		 	- 32-bit decimal/floating point
* float64		 	- 64-bit decimal/floating point

### Types - Others
* char			- 16-bit Unicode character
* dynamic		- pointer to a typed value
* string 		- string of characters
* list 			- list of objects
* set			- unique set of objects (also enum)
* hash			- hash table
* object		- base type for all types

## Modifiers
### Field/Method Modifiers
* final
* package?
* private
* protected
* public
* static

### Method Modifiers
* abstract

### Aliases
* alias sint32 int;


### Assembly Language

	public class test {
	
		public static void main() {
			// let's put random values in "a" and "b"
			a = random(200);
			b = random(200);
			
			// get the absolute value of the difference
			c = getAbsDifference( a, b );
			printf( "difference = %d\n", c );
		}
		
		static int getAbsDifference( int number1, int number2 ) {
			return ( number1 < number2 ) ? number2 - number1 : number1 - number2;
		}
		
		static int getAbsDifferenceAssy( int number1, int number2 ) {
			int diff = 0;
			
			/* now let's use assembly to determine the 
			   difference "a" and 100 */
			%%
				// setup the values
				mov cx,number1	
				mov ax,number2
				
				// perform the comparison			
				cmp cx,ax
				jge @bigger
				
				// if a < 100
				xchg ax,cx
					
				// if a > 100
				@bigger
					sub cx,ax
				
				// record the difference in b
				mov &diff,rcx
			%%
			// at this point, diff = abs( a - b )
			return diff;
		}
	}

### Lists (@), Sets (#), and Hashes (%)

Example 1:

	cbo = {@ {% "hello":1 "bon jour":2, "buenos dias":3 %} {@ 1 2 3 4 @} {# apple banana orange peach #} @}
			// or
	hsh = {% "hello":1 "bon jour":2, "buenos dias":3 %}
	lst = {@ 1 2 3 4 @}
	sts = {# apple banana orange peach #}
	cbo = {@ hsh lst sts @}
	
	? cbo[0] => {% "hello":1 "bon jour":2, "buenos dias":3 %}
	? cbo[1] => {@ 1 2 3 4 @}
	? cbo[2] => {$ apple banana orange peach $}

	a = {@ 1, 2, 3, 4 @};
	print a;

    => ( 1 2 3 4 )

	list a = {@@};
	a.add(1);
	a.add(2);
	a += ( 3 );
	print a; 

    => ( 1 2 3 )

Example 2:

	list b = {@ 5, 3, 2, 1 @};
	b.sort();
	print b; 

    =>	( 1 2 3 4 5 )

	b.pop();
	print b; 

    => ( 1 2 3 4 )

	print a - b; 

    => ( 4 )

Example 3:

	list a = {@ 1 2 3};
	print map(cos, a ); // (cos(1),cos(2),cos(3))
    // 	or
	print( a -> cos ); // (cos(1),cos(2),cos(3))
	
Example 4:

	a = {@ 1, 1, 2, 3, 4, 5, 6, 6, 7 @}
	? set(a) => ( 1 2 3 4 5 6 7 )
	
Example 5:

	a = {% "good day":1 "bon jour":2 "buenos dias":3 %}; 
	if( a isa hash ) print "Yes" else print "No"; // Yes
	if( a isa set ) print "Yes" else print "No"; // No
	if( a isa list ) print "Yes" else print "No"; // No
	
	print a.keys;

    => ( "good day" "bon jour" "buenos dias" )

	print a.values;

    => ( "1" "2" "3" )

### Objects

Example 1:

	print (-1).abs; // 1
	print ++1; // 2

Example 2:

	class Dog {
		string _name;
		
		name() { return _name; }
	
		name( string _name ) { this._name = _name }
	
		string speak() { ... }
	}

	myDog = Dog.new;
	myDog.name = "Fido"
	puts "My dogs name is ${myDog.name}.\n";
	
    => My dogs name is Fido.
	
### Functions Properties

    // If a functions has no parameters, it can
    // be accessed like a variable.

    int value() { ... }
    x = value + 5;	// x = value() + 5

    // If a function accepts a single parameter, then it
    // can be set like a variable

    void value( int value ) { ... }
    value = 5; 		// value(5)

    // If a function has multiple parameters, then it
    // can be set using a list.

    void info( string name, int age ) { ... }
    info = {@ "Lawrence", 35 @};

    void printall( list items ) {
        n = 0;
        foreach item : items {
            n++;
            print "${n} ${item}\n";
        }
    }

    printall( "so long", "far well", "alveitisei", "goodbye" ),

    printall = {@ "so long" "far well", "alveitisei", "goodbye" @}

### Function Mapping

In Lisp:

    (define lst (list 2 3 5 7 11))

    (map double lst)
    => (4 6 10 14 22)


In Cortex:

    lst = {@ 2,3,5,7,11 @};
    int double(int x) { return x + x; }
    map(double, lst);
    => (4 6 10 14 22)

###	Keyword "switch"

    switch name {
        case "Hello":;
            break;

        case "Goodbye":;
            break;
    }

    switch score {
        case 1:;
            break;

        case score > 5:;
            break;

        case score < 5:;
            break;
    }

### Math

Variables whose types are not explicitly defined
are considered a "dynamic" pointer to a value, therefore
no conversion is performed.

    x = 5.1 * y + 7.1;
        or
    dynamic x = 5.1 * y + 7.1;


    int x = 5.1 * y + 7.1 // => 5*y+7

### Structures

    struct Customer {
        uint32 customerID;
        string first;
        string last;
        string middle;
        char[9] ssn;
    }

### Stores

    SQLConnection sql = SQL::connect( "oracle", "localhost", "user", "password" );

    customerID = 5;
    Customer customer <- sql.query( "SELECT customerID, first, last, middle, ssn FROM Customers WHERE customerID = ${customerID}" );
    //		or
    Customer customer <- ( sql.query <- "SELECT customerID, first, last, middle, ssn FROM Customers WHERE customerID = ${customerID}" );
    count = sql.insert <- "INSERT INTO Customers ( first, middle, last ) VALUES ( ${first}, ${middle}, ${last} )";


    store<Customer> customeFile = File::store( "C:\temp\dummy.txt" );

### Streams

    DataOutputStream out = DataOutputStream.new( FileOutputStream.new( "C:\\temp\\dummyOut.txt" ) );
    "This is the first line\n" -> out;

    // copy the file
    string line;
    DataInputStream in = DataInputStream.new( FileInputStream.new( "C:\\temp\\dummyIn.txt" ) );
    while( line <- in ) { line -> out }
    // or
    while( ( line <- in ) -> out ) { }

    string line;

    // get a line from STDIN and write it to STDOUT
    line <- System::STDIN;
    line -> System:STDOUT;

    // get a line from STDIN and write it to STDOUT
    ( line <- System::STDIN ) -> System:STDOUT;

## Registers
There are 32 registers available 16 64-bit general purpose registers (GP0 through GPF)
and 8 32-bit general purpose registers (AX, BX, CX, DX, EX, FX, GX, and HX). There are also
special registers: 4 counters (RC0-RC3), 8 positional (RP0-RP7)

WIP #1: Perhaps set up multiple counters (e.g. RC1 - RC4), and allow the instruction 
to indicate which counter is being used.  This will add greater flexibility when writing
code that loops or moves data about. 

WIP #2: Perhaps use 1 additional bit to represent 128 registers; 64 for the user
and 64 for the system. This will help prevent issues when directly modifying 
register values and flags. ** RESOLVED ** --> this will be handled via a register state object.

WIP #3: Consider creating 8 floating point registers, and associated functions like
sin(x), cos(x), etc.

WIP #4: Consider creating 32 long integer registers (64-bit), which can be used as
longs or doubles interchangeably. Associated functions like sin(x) and cos(x) could
work at a system level.

    64 registers = 16 x 4 (INT,LONG,FLOAT,DOUBLE)

    INT		AX-HX (8), FL (9), RI9-RI16
    LONG	AY-HY (8), SP (9), RP (10),

    FLOAT	AA-HA
    DOUBLE	AB-HB

## Instruction Set
Cortex uses fixed length instructions (32-bit) to promote parallelism and
high speed decoding of the instructions. There are four types of instructions.

* WIP #1: Switch to 64-bit instructions in order to be able to directly access
** 32-bit quantities:
** Ex1: ADD RAX,1234ABCDh 			-> [op:8][rt:2][r0:6][msc:16][ofs:32] = 64-bits
** Ex2: MOV RAX,[R00+1234ABCDh]	-> [op:8][rt:2][r0:6][r1:6][msc:10][ofs:32] = 64-bits

* WIP #2: Consider limiting all offsets to references from memory locations.
* WIP #3: Consider adding a bit that indicates that the next instruction depends on the current instruction.
	

### TYPE 0: System instructions
    POPALL
    HLT

### TYPE 1: Math/Stack instructions
    POP RAX
    INC RDX
    DEC BYTE[BX] 		-> qualifiers include BYTE/8, SWORD/16, WORD/32, DWORD/64

    COS [GP0]			-> Computes the cosine using the float value found at pointer GP0
    SIN FP0				-> Computes the sine using the double value found in FP0
    ABS GP1				-> Absolute value of the integer found in GP1

### TYPE 2: Basic Data instructions
    SUB AX,CX
    ADD BX,DX

    MOVADDR GP0,[0x7E]

### TYPE 3: Flow instructions
    CALL label
    JMP label
    LOOP label
    RET

    CALL:C label
    CALL:NC label
    CALL:DBZ label
    CALL:NDBZ label
    CALL:F label
    CALL:NF label
    CALL:HF label
    CALL:HB label
    CALL:G label
    CALL:NG label
    CALL:GE label
    CALL:NGE label
    CALL:L label
    CALL:NL label
    CALL:LE label
    CALL:NLE label
    CALL:O label
    CALL:NO label
    CALL:P label
    CALL:NP label
    CALL:S label
    CALL:NS label
    CALL:X label
    CALL:NX label
    CALL:Z label
    CALL:NZ label

### TYPE 4: Massive Data instructions
    COPY BYTE RC1 RP0:RP1 			-> Copies RC1 bytes (8-bit quantity) from RP0 to RP1
    SORT LONG RC2 RP3:RP4			-> Sorts a memory array of longs (64-bit quantity) from RP3 to RP4

    FILL BYTE GP0 RP1:RP2			-> Fills the memory from RP1 to RP2 with the byte (8-bit quantity) found in GP0
    SCAN WORD GP1 RP2:RP3 	 		-> Scans the memory from RP2 to RP3 for the word (16-bit quantity) found in GP1

MAP?

### TYPE 5: Data instructions
    STAGEPARMS <address>

    ECHO "Hello World"

    INVOKE "main()"

    LOADCLASS "sera.util.LinkedList"

    // data1 = "Hello World"
    STAGE "Hello World"
    SETPTR data1

    // data2 = data1
    STAGE data1
    SETPTR data2

## Instruction Layout

    Layout 1: [op:8][rt:2][r0:5][r1:5][dq:2][msc:2][ofs:8]	= 32-bits -> MOV RAX,RCX | MOV RAX,[RBX+n8]
    Layout 2: [op:8][rt:2][r0:5][r1:5][dq:2][r2:5][msc:5]	= 32-bits -> SCAN BYTE [RAX] RCX RDX

## Thinking out loud

    Layout 3: [op:8|dq:2|r0:5|msc:1|ofs:16]	= 32-bits -> MOV RAX,[n16]

    Experiment 1: [op:8|dq:2|msc:22][10]				= 32-bits -> LOAD [n32] *requires 2 extra bytes
    Experiment 2: [op:8|dq:2|msc:22][42]				= 32-bits -> LOAD [n64] *requires 6 extra bytes

    Experiment 3: [op:8|dq:2|r0:5|msc:1|ofs:16]	= 32-bits -> MOV RAX,[n64]
    Experiment 4: [op:8|dq:2|r0:5|msc:1|ofs:16]	= 32-bits -> MOV RAX,[n64]

    Experiment 5: [op:8|dq:2|r0:5|msc:1|ofs:16]	= 32-bits -> MOV RAX,[n64]

    [op:8][ref1:2][ref2:2]        [dq:2][reg1:5][*:13]	- [q32]				= MOV ,RAX
    [op:8][ref1:2][ref2:2]        [dq:2][reg1:5][*:13]	- [q32]				= MOV LONG [1000],RAX
    [op:8][ref1:2][ref2:2]        [dq:2][*:18] 			- [q32][q32]		= MOV LONG [1000],0
    [op:8][ref1:2][ref2:2]        [dq:2][*:18] 			- [q32][q08]		= MOV BYTE [1000],0
    [op:8][ref1:2][ref2:2][ref3:2][dq:2][*:16] 			- [q32][q32][q32]	= SCAN BYTE [1000] 0000:FFFF

_Need an instruction for retrieving/accessing the address of variables_

    Layout: [op:8][rt:2][r0:6][r1:6][msc:2][ofs:8] = 32-bits

     MOV AX,DX = 0x0538EC00
                    0000.0101b 	= MOV
                    00b 		= A and B are registers
                    11.1000b	= AX
                    11.1011b	= DX
                    0000.0000	= 00h

    Layout: [op:8][rt:2][r0:5][r1:5][msc:4][ofs:8] = 32-bits

     SUB AX,[BX+AA] = 0x017AE0AA
                    0000.0001b 	= SUB
                    01b 		= A is a register, B is a memory reference
                    11.1010b	= CX
                    11.1000b	= AX
                    1010.1010	= AAh

     Layout: [op:8][rt:2][r0:5][msc:1][ofs:16] = 32-bits

     ADD AX,1234 = 0x00F81234
                    0000.0000b 	= ADD
                    11b 		= A is a register, B is a 16-bit value
                    1.1000b		= AX
                    0001.0010b	= 12h
                    0011.0100b	= 34h
				
### Requirements

    MOV Q32,Q32
    MOV Q64,Q64
    CMP Q32,Q32
    CMP Q64,Q64
    XCHG Q32,Q32
    XCHG Q64,Q64

### Arrays

Array objects have a header that is the length of the array, 

Thus "char[] data = new char[128]" looks like:

[header:32=128][..(128 bytes of data)...]

So the actual "address" of an array is &data + 4.

