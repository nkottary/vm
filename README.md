# Bytecode Interpreter for a Stack Machine

I am going to build a high level language that 
compiles to this bytecode. This is a stack machine
 so arguments are pushed to a stack and consumed 
by the instructions.

## Resources

1. Stack of size 1000 elements each a byte.
2. A boolean bit
4. A program counter

## Instructions without args

```
REAH	    - read a hexadecimal byte from stdin 
	      to top of stack
READ	    - read a decimal to top of stack.
REAC	    - read a character to top of stack.
WRTH 	    - write a hexadecimal byte to stdout.
WRTD	    - write a decimal to stdout.
WRTC	    - write a character to stdout.
ADD 	    - add top 2 elems of stack and put sum
	      in stack
SUB 	    - subtract topmost element by 2nd 
	      topmost element from stack and put
	      difference in top of stack
MUL 	    - multiply top 2 elems os stack and 
	      put product in top of stack
DIV 	    - Divide topmost elem by 2nd topmost
	      elem and put quotient in top of 
	      stack and remainder below top.
POP 	    - pop topmost elem from stack
EQU 	    - if top 2 elems are equal boolean 
	      flag set to 1 else 0
GRT 	    - is top is greater than second top 
	      boolean flag set to 1 else 0
LST 	    - is top is smaller than second top 
	      boolean flag set to 1 else 0
GOTO 	    - go to instruction number given by 
	      top of stack.
GOIF        - like GOTO but only if boolean flag 
	      set.
GOUN        - like GOTO but only if boolean flag 
	      not set.
END	    - end execution.
DUP         - Duplicate the topmost element on 
	      the stack.
FLIP        - Flip the top two elements on the 
	      stack.
GET         - Push the element at data segment 
              address given by the top of stack.
PUT         - Put the second in top of stack to 
              the location given by top of stack.
NOP         - No operation.
```
## Instructions with arguments

```
PUSH        - push a byte to top of stack. Byte may
              be entered as a decimal or a hexadecimal 
              with a suffix 'h'.
```

## Bytecodes used for compiler hints

```
LAB         - Defines a label at this point.
JMP         - Defines a jump to a label indexed by the
              next byte.
MEM         - Used for indicating mem_get and mem_put.
```

In a second pass of compilation LAB is replaced by NOP
and JMP is compiled to PUSH <instruction-to-jump-to> 
GOTO.

## High level instructions

These instructions are compiled to byte code

### Labels:

A label can be defined with the ':' character, and can be 
jumped to with jump <label-name>, for example-

```
....code....
:start  \# Definition of a label.
....code....
jump start \# Jump to label 
```
Conditional jumps (jumpif and jumpun) are also available.

### Data segment get and put

To push a data segment value into stack use-
```
mem_get label_name
```
where label_name is the label in data segment where the 
value is stored.

To put top of stack value into data segment use-
```
mem_put label_name
```

## comments

Use the \# character for comments.

## How to run the example programs

First compile file to byte code.
```
./compiler hw.vm
```
Then run the compiled .vmc file in the vm.
```
./vm hw.vmc
```
To decompile the code to a .vm file use decompiler.
```
./decompiler hw.vmc
```
