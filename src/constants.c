/**
 * constants.c
 * Purpose: Defines the instruction set and conversion functions.
 *
 * @author Nishanth H. Kottary
 */

#include <assert.h>
#include <stdio.h>

#include "headers/constants.h"

const INS INST_SET[] = {
    BYTECODE_DEF(get_ins_tuple_macro)
};

/**
 * Get the instruction enum from the bytecode.
 *
 * @param  bytecode     
 *
 * @return               The instruction enum.
 */
symbol_t get_inst (bytecode_t bytecode) {
    int i;
    for (i = 0; i < N_INST; i ++) {
        if (INST_SET[i].bytecode == bytecode) {
            return (symbol_t)i;
        }
    }
    return ERR;
}

/**
 * Get the bytecode from the instruction's string representation.
 *
 * @param  inst          The string representation of the instruction.
 *
 * @return               The instruction bytecode.
 */
bytecode_t get_bytecode (char *inst) {
    int i;

    assert(inst != NULL);
    for (i = 0; i < N_INST; i ++) {
        if (strcmp(INST_SET[i].name, inst) == 0) {
            return INST_SET[i].bytecode;
        }
    }
    return INST_SET[ERR].bytecode;
}
