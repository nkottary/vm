/**
 * vm.c
 * Purpose: Interpret a .vmc file.
 *
 * @author Nishanth H. Kottary
 */

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>

#include "headers/constants.h"
#include "headers/stack.h"
#include "headers/enums.h"

#define CHECK_NOT_ENOUGH_MEMORY_ERROR(ptr)                        \
    if (ptr == NULL) {                                            \
        fprintf(stderr, "\nError: Not enough memory for malloc"); \
        return -1;                                                \
    }

int main (int argc, char *argv[]) 
{  
    if (argc != 2) {
        printf("\nUSAGE: vm <vmc file>\n");
        return 0;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        fprintf(stderr, "\nERROR: could not open file %s\n", argv[1]);
        return -1;
    }

    bytecode_t compiled_code[MAX_CODE_LEN], 
               code_len   = 0, 
               code_start = 0,
               pc         = 0;
    Stack *stk = newStack();
    bool_flag_t bool_flag   = FALSE;
    error_flag_t error_flag = NO_ERROR;
    int input = 0;
    int *stack_val = NULL, 
        *num1      = NULL, 
        *num2      = NULL;

    fread(&code_start, sizeof (bytecode_t), 1, fp);
    fread(&code_len, sizeof (bytecode_t), 1, fp);
    fread(compiled_code, sizeof (bytecode_t), code_len, fp);
    fclose(fp);
  
    for (pc = code_start; pc < code_len; pc ++) {
        symbol_t inst = get_inst(compiled_code[pc]);
        switch (inst) {
        case REAH:
            stack_val = (int *)malloc(sizeof(int));
            CHECK_NOT_ENOUGH_MEMORY_ERROR(stack_val);
            scanf("%08x", &input);
            *stack_val = input;
            if (push(stk, (void *)stack_val) == FAILURE) {
                fprintf(stderr, "\nError: Stack overflow error."
                        " in byte number %d, instruction REAH", pc);
                error_flag = ERROR;
            }
            break;

        case READ:
            stack_val = (int *)malloc(sizeof(int));
            CHECK_NOT_ENOUGH_MEMORY_ERROR(stack_val);
            scanf("%d", &input);
            *stack_val = input;
            if (push(stk, (void *)stack_val) == FAILURE) {
                fprintf(stderr, "\nError: Stack overflow error."
                        " in byte number %d, instruction READ", pc);
                error_flag = ERROR;
            }
            break;

        case REAC:
            stack_val = (int *)malloc(sizeof(int));
            CHECK_NOT_ENOUGH_MEMORY_ERROR(stack_val);
            scanf("%c", (char *)&input);
            *stack_val = input;
            if (push(stk, (void *)stack_val) == FAILURE) {
                fprintf(stderr, "\nError: Stack overflow error."
                        " in byte number %d, instruction REAC", pc);
                error_flag = ERROR;
            }
            break;

        case WRTH:
            stack_val = pop(stk);
            if (stack_val) {
                printf("%08x", *stack_val);
                free(stack_val);
            } else {
                fprintf(stderr, "\nError: Stack underflow error."
                        " in byte number %d, instruction WRTH", pc);
                error_flag = ERROR;
            }
            break;

        case WRTD:
            stack_val = pop(stk);
            if (stack_val) {
                printf("%d", *stack_val);
                free(stack_val);
            } else {
                fprintf(stderr, "\nError: Stack underflow error."
                        " in byte number %d, instruction WRTD", pc);
                error_flag = ERROR;
            }
            break;

        case WRTC:
            stack_val = pop(stk);
            if (stack_val) {
                printf("%c", *stack_val);
                free(stack_val);
            } else {
                fprintf(stderr, "\nError: Stack underflow error."
                        " in byte number %d, instruction WRTC", pc);
                error_flag = ERROR;
            }
            break;

        case ADD:
            num1 = (int *)pop(stk);
            num2 = (int *)top(stk);
            if (num1 == 0 || num2 == 0) {
                fprintf(stderr, "\nError: Stack underflow error."
                        " in byte number %d, instruction ADD", pc);
                error_flag = ERROR;
            } else {
                *num2 = (*num1) + (*num2);
                free(num1);
            }
            break;

        case SUB:
            num1 = (int *)pop(stk);
            num2 = (int *)top(stk);
            if (num1 == 0 || num2 == 0) {
                fprintf(stderr, "\nError: Stack underflow error."
                        " in byte number %d, instruction SUB", pc);
                error_flag = ERROR;
            } else {
                *num2 = (*num1) - (*num2);

                free(num1);
            }
            break;

        case MUL:
            num1 = (int *)pop(stk);
            num2 = (int *)top(stk);
            if (num1 == 0 || num2 == 0) {
                fprintf(stderr, "\nError: Stack underflow error."
                        " in byte number %d, instruction MUL", pc);
                error_flag = ERROR;
            } else {
                *num2 = (*num1) * (*num2);

                free(num1);
            }
            break;

        case DIV:
            num1 = (int *)pop(stk);
            num2 = (int *)top(stk);
            if (num1 == 0 || num2 == 0) {
                fprintf(stderr, "\nError: Stack underflow error."
                        " in byte number %d, instruction DIV", pc);
                error_flag = ERROR;
            } else {
                int _num1, _num2;
                _num1 = *num1;
                _num2 = *num2;
                *num1 = _num1 / _num2;
                *num2 = _num1 % _num2;
                push(stk, (void *)num1);

            }
            break;

        case POP:
            stack_val = pop(stk);
            if (stack_val == 0) {
                fprintf(stderr, "\nError: Stack underflow error."
                        " in byte number %d, instruction POP", pc);
                error_flag = ERROR;
            } else {
                free(stack_val);
            }
            break;

        case EQU:
            num1 = (int *)pop(stk);
            num2 = (int *)top(stk);
            if (num1 == 0 || num2 == 0) {
                fprintf(stderr, "\nError: Stack underflow error."
                        " in byte number %d, instruction EQU", pc);
                error_flag = ERROR;
            } else {
                if (*num1 == *num2) {
                    bool_flag = TRUE;
                } else {
                    bool_flag = FALSE;
                }
                push(stk, (void *)num1);
            }
            break;

        case GRT:
            num1 = (int *)pop(stk);
            num2 = (int *)top(stk);
            if (num1 == 0 || num2 == 0) {
                fprintf(stderr, "\nError: Stack underflow error."
                        " in byte number %d, instruction GRT", pc);
                error_flag = ERROR;
            } else {
                if (*num1 > *num2) {
                    bool_flag = TRUE;
                } else {
                    bool_flag = FALSE;
                }
                push(stk, (void *)num1);
            }
            break;

        case LST:
            num1 = (int *)pop(stk);
            num2 = (int *)top(stk);
            if (num1 == 0 || num2 == 0) {
                fprintf(stderr, "\nError: Stack underflow error."
                        " in byte number %d, instruction LST", pc);
                error_flag = ERROR;
            } else {
                if (*num1 < *num2) {
                    bool_flag = TRUE;
                } else {
                    bool_flag = FALSE;
                }
                push(stk, (void *)num1);
            }
            break;

        case GOTO:
            stack_val = (int *)pop(stk);
            if (stack_val == 0) {
                fprintf(stderr, "\nError: Stack underflow error."
                        " in byte number %d, instruction GOTO", pc);
                error_flag = ERROR;
            } else if (*stack_val > code_len - 1) {
                fprintf(stderr, "\nError: GOTO instruction given"
                        " out of bounds address in byte number %d", pc);
                error_flag = ERROR;
            } else {
                pc = *stack_val - 1;
                free(stack_val);
            }
            break;

        case GOIF:
            stack_val = (int *)pop(stk);
            if (stack_val == 0) {
                fprintf(stderr, "\nError: Stack underflow error."
                        " in byte number %d, instruction GOIF", pc);
                error_flag = ERROR;
            } else if (*stack_val > code_len - 1) {
                fprintf(stderr, "\nError: GOIF instruction given"
                        " out of bounds address in byte number %d", pc);
                error_flag = ERROR;
            } else {
                if (bool_flag == TRUE) {
                    pc = *stack_val - 1;
                }
                free(stack_val);
            }
            break;

        case GOUN:
            stack_val = (int *)pop(stk);
            if (stack_val == 0) {
                fprintf(stderr, "\nError: Stack underflow error."
                        " in byte number %d, instruction GOUN", pc);
                error_flag = ERROR;
            } else if (*stack_val > code_len - 1) {
                fprintf(stderr, "\nError: GOUN instruction given"
                        " out of bounds address in byte number %d", pc);
                error_flag = ERROR;
            } else {
                if (bool_flag == FALSE) {
                    pc = *stack_val - 1;
                }
                free(stack_val);
            }
            break;

        case END:
            freeStack(stk);
            return 0;

        case DUP:
            num1 = (int *)top(stk);
            num2 = (int *)malloc(sizeof(int *));
            if (num1 == 0) {
                fprintf(stderr, "\nError: Stack underflow error."
                        " in byte number %d, instruction DUP", pc);
                error_flag = ERROR;
            } else {
                *num2 = *num1;

                if (push(stk, num2) == FAILURE) {
                    fprintf(stderr, "\nError: Stack underflow error."
                        " in byte number %d, instruction DUP", pc);
                    error_flag = ERROR;
                }
            }
            break;

        case FLIP:
            num1 = (int *)pop(stk);
            num2 = (int *)pop(stk);
            if (num1 == 0 || num2 == 0) {
                fprintf(stderr, "\nError: Stack underflow error."
                        " in byte number %d, instruction FLIP", pc);
                error_flag = ERROR;
            } else {
                push(stk, num1);
                push(stk, num2);
            }
            break;

        case PUSH:
            stack_val = (int *)malloc(sizeof(int *));
            vm_get_integer_from_bytecode(&compiled_code[pc + 1], stack_val);
            pc += 4;
            assert(pc < code_len);
            if (push(stk, (void *)stack_val) == FAILURE) {
                fprintf(stderr, "\nError: Stack overflow error."
                        " in byte number %d, instruction PUSH", pc);
                error_flag = ERROR;
            }
            break;

        case GET:
            num1 = (int *)top(stk);
            if (num1 == 0) {
                fprintf(stderr, "\nError: Stack underflow error."
                        " in byte number %d, instruction GET", pc);
                error_flag = ERROR;
            } else {
                vm_get_integer_from_bytecode(&compiled_code[(bytecode_t)*num1], 
                                             num1);
            }
            break;

        case PUT:
            num1 = (int *)pop(stk);
            num2 = (int *)pop(stk);
            if (num1 == 0 || num2 == 0) {
                fprintf(stderr, "\nError: Stack underflow error."
                        " in byte number %d, instruction PUT", pc);
                error_flag = ERROR;
            } else {
                vm_put_integer_to_bytecode(&compiled_code[(bytecode_t)*num1], 
                                           *num2);
            }
            break;

        case NOP:
            break;

        default:
            fprintf(stderr, "\n Unexpected or invalid byte code at"
                    " instruction %d .. exiting\n", pc);
            error_flag = ERROR;
        }
        if (error_flag == ERROR) {
            break;
        }
    }
    freeStack(stk);
    return 0;
}

