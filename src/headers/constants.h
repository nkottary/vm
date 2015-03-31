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

#define BYTECODE_DEF(list_macro) list_macro(REAH),        \
        list_macro(READ),                                 \
        list_macro(REAC),                                 \
        list_macro(WRTH),                                 \
        list_macro(WRTD),                                 \
        list_macro(WRTC),                                 \
        list_macro(ADD),                                  \
        list_macro(SUB),                                  \
        list_macro(MUL),                                  \
        list_macro(DIV),                                  \
        list_macro(POP),                                  \
        list_macro(EQU),                                  \
        list_macro(GRT),                                  \
        list_macro(LST),                                  \
        list_macro(GOTO),                                 \
        list_macro(GOIF),                                 \
        list_macro(GOUN),                                 \
        list_macro(END),                                  \
        list_macro(DUP),                                  \
        list_macro(FLIP),                                 \
        list_macro(PUSH),                                 \
        list_macro(ERR),                                  \
        list_macro(NOP),                                  \
/* The following two byte codes are only used as compiler \
 * hints. They are not interpreted by the vm.             \
 */                                                       \
        list_macro(LAB),                                  \
        list_macro(IND),                                  \
/*******************************************************/ \
        list_macro(GET),                                  \
        list_macro(PUT),

#define get_symbol_macro(symbol) symbol
#define get_ins_tuple_macro(symbol) {#symbol, symbol}

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
