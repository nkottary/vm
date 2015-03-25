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

int main (int argc, char *argv[]) 
{  
    if (argc != 2) {
        printf("\nUSAGE: interpreter <vmc file>\n");
        return 0;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        printf("\nERROR: could not open file %s\n", argv[1]);
        return 0;
    }

    bytecode_t compiled_code[MAX_CODE_LEN], n_insts;
    Stack *stk = newStack();
    bytecode_t pc = 0;
    int error_flag = NO_ERROR, bool_flag = 0, input;
    int *stack_val, *num1, *num2;

    fread(&n_insts, sizeof (bytecode_t), 1, fp);
    fread(compiled_code, sizeof (bytecode_t), n_insts, fp);
    fclose(fp);
  
    for (pc = 0; pc < n_insts; pc ++) {
        symbol_t inst = get_inst(compiled_code[pc]);
        switch (inst) {
        case REAH:
            stack_val = (int *)malloc(sizeof(int));
            scanf("%08x", &input);
            *stack_val = input;
            if (push(stk, (void *)stack_val) == FAILURE) {
                error_flag = ERROR;
            } else {
                error_flag = NO_ERROR;
            }
            break;

        case READ:
            stack_val = (int *)malloc(sizeof(int));
            scanf("%d", &input);
            *stack_val = input;
            if (push(stk, (void *)stack_val) == FAILURE) {
                error_flag = ERROR;
            } else {
                error_flag = NO_ERROR;
            }
            break;

        case REAC:
            stack_val = (int *)malloc(sizeof(int));
            scanf("%c", (char *)&input);
            *stack_val = input;
            if (push(stk, (void *)stack_val) == FAILURE) {
                error_flag = ERROR;
            } else {
                error_flag = NO_ERROR;
            }
            break;

        case WRTH:
            stack_val = pop(stk);
            if (stack_val) {
                printf("%08x", *stack_val);
                error_flag = NO_ERROR;
                free(stack_val);
            } else {
                error_flag = ERROR;
            }
            break;

        case WRTD:
            stack_val = pop(stk);
            if (stack_val) {
                printf("%d", *stack_val);
                error_flag = NO_ERROR;
                free(stack_val);
            } else {
                error_flag = ERROR;
            }
            break;

        case WRTC:
            stack_val = pop(stk);
            if (stack_val) {
                printf("%c", *stack_val);
                error_flag = NO_ERROR;
                free(stack_val);
            } else {
                error_flag = ERROR;
            }
            break;

        case ADD:
            num1 = (int *)pop(stk);
            num2 = (int *)top(stk);
            if (num1 == 0 || num2 == 0) {
                error_flag = ERROR;
            } else {
                *num2 = (*num1) + (*num2);
                error_flag = NO_ERROR;
                free(num1);
            }
            break;

        case SUB:
            num1 = (int *)pop(stk);
            num2 = (int *)top(stk);
            if (num1 == 0 || num2 == 0) {
                error_flag = ERROR;
            } else {
                *num2 = (*num1) - (*num2);
                error_flag = NO_ERROR;
                free(num1);
            }
            break;

        case MUL:
            num1 = (int *)pop(stk);
            num2 = (int *)top(stk);
            if (num1 == 0 || num2 == 0) {
                error_flag = ERROR;
            } else {
                *num2 = (*num1) * (*num2);
                error_flag = NO_ERROR;
                free(num1);
            }
            break;

        case DIV:
            num1 = (int *)pop(stk);
            num2 = (int *)top(stk);
            if (num1 == 0 || num2 == 0) {
                error_flag = ERROR;
            } else {
                int _num1, _num2;
                _num1 = *num1;
                _num2 = *num2;
                *num1 = _num1 / _num2;
                *num2 = _num1 % _num2;
                push(stk, (void *)num1);
                error_flag = NO_ERROR;
            }
            break;

        case POP:
            stack_val = pop(stk);
            if (stack_val == 0) {
                error_flag = ERROR;
            } else {
                error_flag = NO_ERROR;
                free(stack_val);
            }
            break;

        case EQU:
            num1 = (int *)pop(stk);
            num2 = (int *)top(stk);
            if (num1 == 0 || num2 == 0) {
                error_flag = ERROR;
            } else {
                if (*num1 == *num2) {
                    bool_flag = TRUE;
                } else {
                    bool_flag = FALSE;
                }
                push(stk, (void *)num1);
                error_flag = NO_ERROR;
            }
            break;

        case GRT:
            num1 = (int *)pop(stk);
            num2 = (int *)top(stk);
            if (num1 == 0 || num2 == 0) {
                error_flag = ERROR;
            } else {
                if (*num1 > *num2) {
                    bool_flag = TRUE;
                } else {
                    bool_flag = FALSE;
                }
                push(stk, (void *)num1);
                error_flag = NO_ERROR;
            }
            break;

        case LST:
            num1 = (int *)pop(stk);
            num2 = (int *)top(stk);
            if (num1 == 0 || num2 == 0) {
                error_flag = ERROR;
            } else {
                if (*num1 < *num2) {
                    bool_flag = TRUE;
                } else {
                    bool_flag = FALSE;
                }
                push(stk, (void *)num1);
                error_flag = NO_ERROR;
            }
            break;

        case GOTO:
            stack_val = (int *)pop(stk);
            if (stack_val == 0 || *stack_val > n_insts - 1) {
                error_flag = ERROR;
            } else {
                error_flag = NO_ERROR;
                pc = *stack_val - 1;
                free(stack_val);
            }
            break;

        case GOIF:
            stack_val = (int *)pop(stk);
            if (stack_val == 0 || *stack_val > n_insts - 1) {
                error_flag = ERROR;
            } else {
                error_flag = NO_ERROR;
                if (bool_flag == TRUE) {
                    pc = *stack_val - 1;
                }
                free(stack_val);
            }
            break;

        case GOUN:
            stack_val = (int *)pop(stk);
            if (stack_val == 0 || *stack_val > n_insts - 1) {
                error_flag = ERROR;
            } else {
                error_flag = NO_ERROR;
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
                error_flag = ERROR;
            } else {
                *num2 = *num1;
                error_flag = NO_ERROR;
                if (push(stk, num2) == FAILURE) {
                    error_flag = ERROR;
                }
            }
            break;

        case FLIP:
            num1 = (int *)pop(stk);
            num2 = (int *)pop(stk);
            if (num1 == 0 || num2 == 0) {
                error_flag = ERROR;
            } else {
                push(stk, num1);
                push(stk, num2);
                error_flag = NO_ERROR;
            }
            break;

        case PUSH:
            stack_val = (int *)malloc(sizeof(int *));
            vm_get_integer_from_bytecode(&compiled_code[pc + 1], stack_val);
            pc += 4;
            assert(pc < n_insts);
            if (push(stk, (void *)stack_val) == FAILURE) {
                error_flag = ERROR;
            } else {
                error_flag = NO_ERROR;
            }
            break;
            
        case NOP:
            break;

        default:
            printf("\n Unexpected or invalid byte code at"
                   " instruction %d .. exiting\n", pc);
            freeStack(stk);
            return 0;
        }
    }
    freeStack(stk);
    return 0;
}

