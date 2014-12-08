#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "headers/constants.h"
#include "headers/stack.h"

typedef enum {
  FALSE,
  TRUE
} bool_flag_t;

typedef enum {
  ERROR,
  NO_ERROR
} error_flag_t;

int main (int argc, char *argv[]) {  

  if (argc != 2) {
    printf("\nUSAGE: interpreter <vmc file>\n");
    return 0;
  }

  FILE *fp = fopen(argv[1], "rb");
  if (fp == NULL) {
    printf("\nERROR: could not open file %s\n", argv[1]);
    return 0;
  }

  bytecode_t compiled_code[MAX_CODE_LEN],
             n_insts;
  Stack *stk = newStack();
  bytecode_t pc = 0;
  int error_flag = NO_ERROR, bool_flag = 0, input;
  bytecode_t *byte, *num1, *num2;

  fread(&n_insts, sizeof (bytecode_t), 1, fp);
  fread(compiled_code, sizeof (bytecode_t), n_insts, fp);
  fclose(fp);
  
  for (pc = 0; pc < n_insts; pc ++) {
    symbol_t inst = get_inst(compiled_code[pc]);
    switch (inst) {
    case REAH:
      byte = (bytecode_t *)
	malloc(sizeof(bytecode_t));
      scanf("%02x", &input);
      *byte = input;
      if (push(stk, (void *)byte) == FAILURE) {
	error_flag = ERROR;
      } else {
	error_flag = NO_ERROR;
      }
      break;

    case READ:
      byte = (bytecode_t *)
	malloc(sizeof(bytecode_t));
      scanf("%d", &input);
      *byte = input;
      if (push(stk, (void *)byte) == FAILURE) {
	error_flag = ERROR;
      } else {
	error_flag = NO_ERROR;
      }
      break;

    case REAC:
      byte = (bytecode_t *)
	malloc(sizeof(bytecode_t));
      scanf("%c", (char *)&input);
      *byte = input;
      if (push(stk, (void *)byte) == FAILURE) {
	error_flag = ERROR;
      } else {
	error_flag = NO_ERROR;
      }
      break;

    case WRTH:
      byte = pop(stk);
      if (byte) {
	printf("%02x", *byte);
	error_flag = NO_ERROR;
	free(byte);
      } else {
	error_flag = ERROR;
      }
      break;

    case WRTD:
      byte = pop(stk);
      if (byte) {
	printf("%d", *byte);
	error_flag = NO_ERROR;
	free(byte);
      } else {
	error_flag = ERROR;
      }
      break;

    case WRTC:
      byte = pop(stk);
      if (byte) {
	printf("%c", *byte);
	error_flag = NO_ERROR;
	free(byte);
      } else {
	error_flag = ERROR;
      }
      break;

    case ADD:
      num1 = (bytecode_t *)pop(stk);
      num2 = (bytecode_t *)top(stk);
      if (num1 == 0 || num2 == 0) {
	error_flag = ERROR;
      } else {
	*num2 = (*num1) + (*num2);
	error_flag = NO_ERROR;
	free(num1);
      }
      break;

    case SUB:
      num1 = (bytecode_t *)pop(stk);
      num2 = (bytecode_t *)top(stk);
      if (num1 == 0 || num2 == 0) {
	error_flag = ERROR;
      } else {
	*num2 = (*num1) - (*num2);
	error_flag = NO_ERROR;
	free(num1);
      }
      break;

    case MUL:
      num1 = (bytecode_t *)pop(stk);
      num2 = (bytecode_t *)top(stk);
      if (num1 == 0 || num2 == 0) {
	error_flag = ERROR;
      } else {
	*num2 = (*num1) * (*num2);
	error_flag = NO_ERROR;
	free(num1);
      }
      break;

    case DIV:
      num1 = (bytecode_t *)pop(stk);
      num2 = (bytecode_t *)top(stk);
      if (num1 == 0 || num2 == 0) {
	error_flag = ERROR;
      } else {
	bytecode_t _num1, _num2;
	_num1 = *num1;
	_num2 = *num2;
	*num1 = _num1 / _num2;
	*num2 = _num1 % _num2;
	push(stk, (void *)num1);
	error_flag = NO_ERROR;
      }
      break;

    case POP:
      byte = pop(stk);
      if (byte == 0) {
	error_flag = ERROR;
      } else {
	error_flag = NO_ERROR;
	free(byte);
      }
      break;

    case EQU:
      num1 = (bytecode_t *)pop(stk);
      num2 = (bytecode_t *)top(stk);
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
      num1 = (bytecode_t *)pop(stk);
      num2 = (bytecode_t *)top(stk);
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
      num1 = (bytecode_t *)pop(stk);
      num2 = (bytecode_t *)top(stk);
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
      byte = pop(stk);
      if (byte == 0 || *byte > n_insts - 1) {
	error_flag = ERROR;
      } else {
	error_flag = NO_ERROR;
	pc = *byte - 1;
	free(byte);
      }
      break;

    case GOIF:
      byte = pop(stk);
      if (byte == 0 || *byte > n_insts - 1) {
	error_flag = ERROR;
      } else {
	error_flag = NO_ERROR;
	if (bool_flag == TRUE) {
	  pc = *byte - 1;
	}
	free(byte);
      }
      break;

    case GOUN:
      byte = pop(stk);
      if (byte == 0 || *byte > n_insts - 1) {
	error_flag = ERROR;
      } else {
	error_flag = NO_ERROR;
	if (bool_flag == FALSE) {
	  pc = *byte - 1;
	}
	free(byte);
      }
      break;

    case END:
      freeStack(stk);
      return 0;

    case DUP:
      num1 = top(stk);
      num2 = (bytecode_t *)
	malloc(sizeof(bytecode_t *));
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
      num1 = pop(stk);
      num2 = pop(stk);
      if (num1 == 0 || num2 == 0) {
	error_flag = ERROR;
      } else {
	push(stk, num1);
	push(stk, num2);
	error_flag = NO_ERROR;
      }
      break;

    case PUSH:
      byte = (bytecode_t *)
	malloc(sizeof(bytecode_t *));
      *byte = compiled_code[++ pc];
      if (push(stk, (void *)byte) == FAILURE) {
	error_flag = ERROR;
      } else {
	error_flag = NO_ERROR;
      }
      break;

    default:
      printf("\n Unexpected byte code error at"
	     " instruction %d .. exiting\n", pc);
      freeStack(stk);
      return 0;
    }
  }
  freeStack(stk);
  return 0;
}

