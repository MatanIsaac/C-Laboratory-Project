# <div align="center">Assembler Project - C-Laboratory #20465 </div>

This project is the final assignment (#14) for the Open University’s C-Laboratory Course #20465.
The objective is to implement an assembler for an imaginary computer, translating assembly source code into a binary representation suitable for machine execution.
Development was done on Linux Ubuntu 16.04, per course requirements.

### <div align="center"> Project Objectives </div>

    Parse and analyze a given assembly source file.
    Translate assembly instructions into corresponding machine code.
    Generate corresponding output files: am, object, externals, entries 

### <div align="center">Project Overview</div>

This assembler converts human-readable assembly language into machine code, making it executable by the imaginary computer model. 
The assembler does not handle linking or loading; it simply translates assembly instructions into their binary representation.


### <div align="center"> Course Development Requirements </div>


    Strict ANSI C compliance (C89/C90)
    No external libraries – pure C standard library usage
    Fully self-contained implementation
    Structured, modular design with multiple source files

### <div align="center">Features </div>

    Multi-pass assembler for proper label resolution.
    Symbol table management for labels and variables.
    Error detection with descriptive messages.
    Optimized machine code generation.
    Strict memory constraints for compliance with the imaginary computer's specs.
    Small testing framework included (used to test different areas of this assembler project) 

### <div align="center">The Imaginary Computer Model </div>

This project simulates an imaginary CPU architecture with the following specifications:
System Components

    CPU: 
        Contains 8 general-purpose registers: r0, r1, r2, r3, r4, r5, r6, r7
    Register Size: 24 bits
        LSB (Least Significant Bit): Bit 0
        MSB (Most Significant Bit): Bit 23
    PSW (Program Status Word) Register: 
        Stores CPU status flags.
    RAM: 
        Addressable memory space of 2^21 (2,097,152 locations), each 24-bit wide.
    Number Representation:
        Only natural (positive) numbers are supported.
        Arithmetic operations use 2's complement for negative numbers.
        ASCII character representation is supported.


### <div align="center"> Building the Assembler </div>

This project is written in ANSI C and adheres to strict C89/C90 standards.

Ensure you have:

    GCC Compiler (or any ANSI C-compatible compiler)
    GNU Make (for compilation automation)

To build the assembler, run:

    make

To run the assembler test with an assembly source file:

    ./build/assembler source.as

The output machine code file will be generated in:  
    
    build/output_files/

### <div align="center"> Example Input & Output </div>
Assembly Code Example:

    MAIN:   add r3, LIST
    LOOP:   prn #48
            lea STR, r6
            inc r6

Generated Machine Code:

    Address | Source Code        | Explanation               | Binary Machine Code
    -----------------------------------------------------------------------------------
    0000100 | MAIN: add r3, LIST | First word of instruction | 000010110110100000001100
    0000101 |                    | Address of label LIST     | 000000000000001111111010
    -----------------------------------------------------------------------------------
    0000102 | LOOP: prn #48      |                           | 001101000000000000000100
    0000103 |                    | Immediate value 48        | 000000000000000110000100
    -----------------------------------------------------------------------------------
    0000104 | lea STR, r6        |                           | 000100010001111000000100
    0000105 |                    | Address of label STR      | 000000000000001111010010
    -----------------------------------------------------------------------------------
    0000106 | inc r6             |                           | 000000000000001111010010

Generated Object File:
    
    0000100 0b680c
    0000101 000412
    0000102 340004
    0000103 000184
    0000104 111e04
    0000105 000001
    0000106 141e1c

### <div align="center"> Contact </div>
For any questions, suggestions, or issues, feel free to send me a message !
