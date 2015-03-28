/**
 * constants.h
 * Purpose: Defines the bytecode enum and values.
 *
 * @author Nishanth H. Kottary
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "enums.h"

#define MAX_CODE_LEN 1000
#define INST_LEN       5
#define N_LABELS      10
#define LABEL_LEN     10
#define MAX_LINE_LEN  80

typedef unsigned char bytecode_t;

#define BYTECODE_DEF(list_macro) list_macro(REAH, 0x00),        \
        list_macro(READ, 0x01),                                 \
        list_macro(REAC, 0x02),                                 \
        list_macro(WRTH, 0x03),                                 \
        list_macro(WRTD, 0x04),                                 \
        list_macro(WRTC, 0x05),                                 \
        list_macro(ADD, 0x06),                                  \
        list_macro(SUB, 0x07),                                  \
        list_macro(MUL, 0x08),                                  \
        list_macro(DIV, 0x09),                                  \
        list_macro(POP, 0x0A),                                  \
        list_macro(EQU, 0x0B),                                  \
        list_macro(GRT, 0x0C),                                  \
        list_macro(LST, 0x0D),                                  \
        list_macro(GOTO, 0x0E),                                 \
        list_macro(GOIF, 0x0F),                                 \
        list_macro(GOUN, 0x10),                                 \
        list_macro(END, 0x11),                                  \
        list_macro(DUP, 0x12),                                  \
        list_macro(FLIP, 0x13),                                 \
        list_macro(PUSH, 0x14),                                 \
        list_macro(ERR, 0x15),                                  \
        list_macro(NOP, 0x16),                                  \
/* The following two byte codes are only used as compiler hints.\
 * They are not interpreted by the vm.                          \
 */                                                             \
        list_macro(LAB, 0x17),                                  \
        list_macro(JMP, 0x18),                                  \
        list_macro(MEM, 0x19),                                  \
/*************************************************************/ \
        list_macro(GET, 0x1A),                                  \
        list_macro(PUT, 0x1B),

#define get_symbol_macro(symbol, hex) symbol
#define get_ins_tuple_macro(symbol, hex) {#symbol, hex}

typedef enum {
    BYTECODE_DEF(get_symbol_macro)
    N_INST
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

const INS INST_SET[N_INST];

symbol_t get_inst (const bytecode_t bc);
bytecode_t get_bytecode (const char *inst);
status_t vm_get_integer_from_bytecode (const bytecode_t *compiled_code_ptr,
                                       int *int_val);
status_t vm_put_integer_to_bytecode (bytecode_t *compiled_code_ptr,
                                     const int int_val);

#endif
