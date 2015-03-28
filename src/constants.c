/**
 * constants.c
 * Purpose: Defines the instruction set and conversion functions.
 *
 * @author Nishanth H. Kottary
 */

#include <assert.h>
#include <stdio.h>

#include "headers/constants.h"
#include "headers/enums.h"

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
symbol_t get_inst (const bytecode_t bytecode) {
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
bytecode_t get_bytecode (const char *inst) {
    int i;

    assert(inst != NULL);
    for (i = 0; i < N_INST; i ++) {
        if (strcmp(INST_SET[i].name, inst) == 0) {
            return INST_SET[i].bytecode;
        }
    }
    return INST_SET[ERR].bytecode;
}

/**
 * Get an integer from the compiled code array
 *
 * @param[in]       compiled_code_ptr   Pointer to the place in the 
 *                                      compiled code array from where the
 *                                      integer is to be read from.
 * @param[out]      int_val             The integer made from the 4 bytes of 
 *                                      compiled_code.
 *
 * @return                              status_t
 */
status_t vm_get_integer_from_bytecode (const bytecode_t *compiled_code_ptr,
                                       int *int_val)
{
    int value = 0;
    assert(compiled_code_ptr != NULL);
    assert(int_val != NULL);

    value = compiled_code_ptr[0];
    value |= compiled_code_ptr[1] << 8;
    value |= compiled_code_ptr[2] << 16;
    value |= compiled_code_ptr[3] << 24;

    *int_val = value;
    return SUCCESS;
}

/**
 * Put an integer to the compiled code array
 *
 * @param[out]      compiled_code_ptr   Pointer to the place in compiled code
 *                                      where an integer is to be put.
 * @param[in]       int_val             The integer to be inserted to the 
 *                                      compiled code array.
 *
 * @return                              status_t
 */
status_t vm_put_integer_to_bytecode (bytecode_t *compiled_code_ptr,
                                     const int int_val)
{
    int value = 0;
    assert(compiled_code_ptr != NULL);

    compiled_code_ptr[0] = (bytecode_t)int_val;
    compiled_code_ptr[1] = (bytecode_t)(int_val >> 8);
    compiled_code_ptr[2] = (bytecode_t)(int_val >> 16);
    compiled_code_ptr[3] = (bytecode_t)(int_val >> 24);

    return SUCCESS;
}
                                       
