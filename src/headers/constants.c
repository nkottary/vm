#include "constants.h"

const INS INST_SET[] = {
  {"REAH", 0x00},
  {"READ", 0x01},
  {"REAC", 0x02},
  {"WRTH", 0x03},
  {"WRTD", 0x04},
  {"WRTC", 0x05},
  {"ADD", 0x06},
  {"SUB", 0x07},
  {"MUL", 0x08},
  {"DIV", 0x09},
  {"POP", 0x0A},
  {"EQU", 0x0B},
  {"GRT", 0x0C},
  {"LST", 0x0D},
  {"GOTO", 0x0E},
  {"GOIF", 0x0F},
  {"GOUN", 0x10},
  {"END", 0x11},
  {"DUP", 0x12},
  {"FLIP", 0x13},
  {"PUSH", 0x14},
  {"ERR", 0x15}
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

