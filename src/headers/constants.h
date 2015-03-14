#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_CODE_LEN 1000
#define N_INST 21

typedef unsigned char bytecode_t;

typedef enum {
  REAH,
  READ,
  REAC,
  WRTH,
  WRTD,
  WRTC,
  ADD,
  SUB,
  MUL,
  DIV,
  POP,
  EQU,
  GRT,
  LST,
  GOTO,
  GOIF,
  GOUN,
  END,
  DUP,
  FLIP,
  PUSH,
  ERR
} symbol_t;

struct INS {
  char name[5];
  bytecode_t bytecode;
};

typedef enum {
    FALSE,
    TRUE
} bool_flag_t;

typedef struct INS INS;

const INS INST_SET[N_INST + 1];

symbol_t get_inst (bytecode_t bc);
bytecode_t get_bytecode (char *inst);

#endif
