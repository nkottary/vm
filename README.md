# Assembly Language for a stack machine

## Resources

1. Stack of size 1000 elements each 4 bytes.
2. A boolean bit.
3. A program counter.

## Example hello world program

```
# Hello world program 
__CODE__                # start code segment
# PUSH the string Hello World! in reverse order.
# 0 indicates end of string.
PUSH   0                
PUSH  10                # new line.
PUSH '!'
PUSH 'D'
PUSH 'L'
PUSH 'R'
PUSH 'O'
PUSH 'W'
PUSH 32                 # space
PUSH 'O'
PUSH 'L'
PUSH 'L'
PUSH 'E'
PUSH 'H'
PUSH  Ah                # new line. (in hexadecimal).

:print                  # A label for decision making/looping.
PUSH 0
EQU
PUSH &end               # If top of stack is zero stop printing.
GOIF
POP                     # pop the 0.
WRTC                    # print the character.
PUSH &print             # loop.
GOTO

:end
```

## Instructions without args

```
REAH        - read a hexadecimal byte from stdin 
              to top of stack
READ        - read a decimal to top of stack.
REAC        - read a character to top of stack.
WRTH        - write a hexadecimal byte to stdout.
WRTD        - write a decimal to stdout.
WRTC        - write a character to stdout.
ADD         - add top 2 elems of stack and put sum
              in stack
SUB         - subtract topmost element by 2nd 
              topmost element from stack and put
              difference in top of stack
MUL         - multiply top 2 elems os stack and 
              put product in top of stack
DIV         - Divide topmost elem by 2nd topmost
              elem and put quotient in top of 
              stack and remainder below top.
POP         - pop topmost elem from stack
EQU         - if top 2 elems are equal boolean 
              flag set to 1 else 0
GRT         - is top is greater than second top 
              boolean flag set to 1 else 0
LST         - is top is smaller than second top 
              boolean flag set to 1 else 0
GOTO        - go to instruction number given by 
               top of stack.
GOIF        - like GOTO but only if boolean flag 
              set.
GOUN        - like GOTO but only if boolean flag 
              not set.
END         - end execution.
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
IND         - Indirection, defines that next 4 bytes
              is an address.
```

In a second pass of compilation LAB is replaced by NOP
and IND is compiled to PUSH <instruction-to-jump-to>.

## Labels

A label can be defined with the ':' character, and can be 
jumped to with GOTO, GOIF or GOUN instructions, for example-

```
....code....
:start  \# Definition of a label.
....code....
GOTO &start \# Jump to label 
```
To push data from data segment to stack, first push the 
address to the stack then use GET, for example-

```
:my_data 123
__CODE__      
PUSH &my_data
GET
```

Similarly, PUT can be used to write data from the stack
to the data segment.

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
