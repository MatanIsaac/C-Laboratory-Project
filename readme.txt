Assembler Project mmn14

Main Project Goals:
    1. Parse and analyze a given assembly source code file.
    2. Translate the assembly instructions into corresponding machine code.
    3. Generate a machine code output file that can be executed or loaded by the target system.

Purpose:
    This project involves creating an assembler for a specific assembly language, converting human-readable 
    assembly instructions into the machine-readable format required by the target hardware or virtual machine.

    Assmebler's Role:
    The role of the assembler is to build a machine code file, based on a program written in assembly language. 
    This is the first step in the process of transferring the program to create code that can be executed on the hardware. 
    The following stages are linking (linkage) and loading, but we do not deal with these in this context.

The Imaginary Computer and Assembly Language:
    Computer Consists of: 
        CPU, registers, and RAM memory(some part of the memory's for the stack)
    CPU:
        Has 8 general registers called: r0,r1,r2,r3,r4,r5,r6,r7 (register names always starts with 'r')
        Size of Every Register: 24 bits
        Least Significant Bit(LSB): bit no 0
        Most  Significant Bit(LSB): bit no 23
        PSW Register(Program Status Word): Contains several flags indicating the processor's status at any given moment. 
        (For more information about the PSW register, see the machine instructions description, where the flags' usage is explained)
    RAM:
        The memory size is 2^21 locations, in addresses ranging from 0 to 2^21-1
        Each address is 24 bits in size. 
        A memory location is also called a "מילה" 
        The bits in each "מילה" are numbered like in the registers
    The Computer's Number Support:
        The Computer Supports only natural and whole numbers, negative numbers are not allowed.
        Arithmetic is performed using the 2's complement method. 
        There is support for characters, which are represented in ASCII code.
    Machine Instruction Structure:
        Every machine instruction in our model consists of an operation and operands. 
        The number of operands is between 0 and 2, depending on the type of operation. 
        The function of each operand is determined by its role, distinguishing between a source operand and a destination operand.
        


    
