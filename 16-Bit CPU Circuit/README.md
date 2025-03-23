#                   Single Cycle CPU 

## Overview
This project build a 16-bit, RISC processor capable of fully executing 16 unique R, I, and J MIPS assembly instructions. 
The processor is word-addressed, with each word equalling 2 bytes, and follows a single-cycle architecture. The design is built through Logisim using only AND, OR, XOR, NOT, MUX, ROM, RAM, decoders, and D FLip-Flop Registers. 
All of these components, exluding ROM and RAM, can be created on breadboards. Multiplexer (MUX), decoders, and D Flip-Flop Registers can all be created using the basic gates (AND, NAND, NOR, etc), which in turn can all be created on a breadboard with only diodes, wires, and resistors. 
Considering I have built all of these gates on breadboards before, I could build a fully-functioning CPU with only a breadboard and the 3 most basic electrical components. However, it would require tens of thousands of diodes, resistors, and wires each, making it theoretically, but practically, impossible.

## Processor Architecture
1) 16-bit word-addressed design
2) 8 general purpose registers ($r0 - $r7) with each register having 16 D Flip-Flop registers 
3) $r0 always hardwired to 0, $r7 link register for jal, $r6 is typically stack pointer 
4) Single-cycle execution: each instruction completes in one cycle (there is a clock simulator included where each click on the simulator acts as one cycle)
5) Supports all of the following MIPS instructions: 
    - R-Type: add, sub, sll, srl, and, not, jr
    - I-Type: addi, lw, sw, bne, ble, input, output
    - J-Type: j, jal

## Skills Used/Key Features in Each File
*Multiple files use similar skills/features, i.e. RegisterFile primarily uses 8 SixteenBitRegister files in its implementation* 
*Example Outputs has pictures of each circuit file, every black box with a name at the bottom is a representation of another file. For example, in main, the box named
BitAdder16 is a representation of the file BitAdder16. The wires going into the left are inputs, the wires coming out are outputs. So the wire going into in0 corresponds to*



<big>**Main**</big> - Uses the other files to create the entire CPU logic
- <big>**ROM**</big> - Uses ROM to store instructions from a program 
- <big>**Instruction Decoding**</big> - Splits intructions into opcode to determine instruction type, then splits further based on result (ie if there is rt, rs, and rt for R-type, rt and rs for I-type, or just address for J-type)
- <big>**RAM**</big> - for loading and storing data from memory

<big>**RegisterFile & SixteenBitRegister**</big> - Takes in 16 bit number and stores it in appropriate register
- <big>**8 16 Bit Registers**</big> - there are 8 general purpose registers, each capable of holding 16-bit numbers
- <big>**Write/Read Operations**</big> - Control signals determine whether instruction is read/write, decoder determines which register to activate (actives specific write enabled or read enabled control signals), data is either committed to register or data is read and passed on

<big>**ALU (16-bit Arithmetic Logic Unit)**</big> - Takes 16 bit numbers and performs calculations based on control signals
- <big>**Arithmetic**</big> - addition, subtraction, add immediate, multiplication, division
- All <big>**logical operations**</big> - AND, OR, NOT, NOR, etc. 
- <big>**Shift Operations**</big> - left and right bit shifts
- <big>**Comparison**</big> - includes <, >, <=, >=, !=, == 
- <big>**Address Calculations**</big> - calculate address for load/store/jump and return
- <big>**I/O**</big> - can print keyboard input to TTY display

<big>**BitAdder16 & BitAdder1**</big> - BitAdder16 takes in 16 bit numbers, used in the ALU unit to perform addition calculations that can be used for memory address, offset, multiplcation, etc. 

<big>**LeftShit & RightShift (Barrel bit shifters)**</big> - Takes in 16 bit number and can do left or right shift by 0-7 bits in a single operation (all bits move at once) using 3 shift stages

<big>**Control Path**</big> - Takes in opcode to determine specific instruction and activate control signals
- <big>**Opcode Decoder**</big> - converts 4-bit binary opcode into output signals by activating the relevant control signals based on the logic of multiple AND, OR, and NOT gates
- <big>**Control Signals**</big> - Supports Rwe, ALUin2, rdst, ALUop, Shift, Rwd, DMwe, BRne, BRnelt, JP, JR, jal, KeyIn, and OutKey control signals

<big>**Memory Latch**</big> - to ensure data, addresses, and control signals remain synchronized and stable during read/write
- <big>**Address Latch**</big> - stores address in 16 D Flip Flops
- <big>**Data Latch**</big> - stores data in 16 D Flip-Flops
- <big>**MUX**</big> - chooses latched or new address based on control signals
 
<big>**RIJ Decide**</big> - Takes 16 bit instruction and determines whether the instruction is R, I, or J type 

##               How to test/run program

### Inputs
- Can click on lines in ROM to directly set instructions (can use binary or hex)
- Can disconnect wire exiting from ROM (before it goes into RIJ decide component) and add a new 16 bit input component, which will allow manual flipping of all 16 bits to simulate instructions 

### Outputs 
- Hover/click on wires after input instructions and clock cycle flip to see the values in the wires
- Use keyboard instruction and clock cycle flip to see values on TTY 

### Important Usage Notes
- Click the clock in the top left of main file to simulate up/down electrical signals
- Flip reset to reset all values in the entire CPU (registers, ROM, RAM)


##                       Next Steps
- Implement a multi-stage piplining system so the CPU can execute different stages of instructions simultaneously
- Implement a interrupt handling system where the CPU can be stopped by user input
- Implement a cache (wrote logic for it in HW5)
- Make the CPU 32-bit so it can handle 32 instructions
- Make it multi-core