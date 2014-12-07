#include "constants.h"

const INS INST_SET[] = {
  {"READ", 0x00},
  {"WRT", 0x01},
  {"ADD", 0x02},
  {"SUB", 0x03},
  {"MUL", 0x04},
  {"DIV", 0x05},
  {"POP", 0x06},
  {"EQU", 0x07},
  {"GRT", 0x08},
  {"LST", 0x09},
  {"GOTO", 0x0A},
  {"GOIF", 0x0B},
  {"GOUN", 0x0C},
  {"END", 0x0D},
  {"PUSH", 0x0E},
  {"ERR", 0x0F}
};

symbol_t get_inst (bytecode_t bytecode) {
  int i;
  for (i = 0; i < N_INST; i ++) {
    if (INST_SET[i].bytecode == bytecode) {
      return (symbol_t)i;
    }
  }
  return ERR;
}

bytecode_t get_bytecode (char *inst) {
  int i;
  for (i = 0; i < N_INST; i ++) {
    if (strcmp(INST_SET[i].name, inst) == 0) {
      return INST_SET[i].bytecode;
    }
  }
  return INST_SET[ERR].bytecode;
}

