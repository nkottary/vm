#define MAX_CODE_LEN 1000
#define N_INST 15

typedef unsigned char bytecode_t;

typedef enum {
  READ,
  WRT,
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
  PUSH,
  ERR
} symbol_t;

struct INS {
  char name[5];
  bytecode_t bytecode;
};

typedef struct INS INS;

const INS INST_SET[N_INST + 1];

symbol_t get_inst (bytecode_t bc);
bytecode_t get_bytecode (char *inst);
