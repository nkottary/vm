/**
 * decompiler.c
 * Purpose: Convert the bytecode in a .vmc file to their equivalent string
 *          representation.
 *
 * @author Nishanth H. Kottary
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "headers/constants.h"

int main (int argc, char *argv[]) 
{ 
    if (argc != 2 && argc != 3) {
        printf("\nUSAGE: interpreter <vmc file>\n");
        return 0;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        printf("\nERROR: could not open file %s\n", argv[1]);
        return 0;
    }

    bytecode_t compiled_code[MAX_CODE_LEN],
               code_len = 0,
               code_start = 0;

    fread(&code_start, sizeof (bytecode_t), 1, fp);
    fread(&code_len, sizeof (bytecode_t), 1, fp);
    fread(compiled_code, sizeof (bytecode_t), code_len, fp);
    fclose(fp);

    char src[1000];
    int pc = 0;

    src[0] = '\0';

    for (pc = 0; pc < code_start; pc ++) {
        symbol_t inst = get_inst(compiled_code[pc]);
        if (inst == NOP) {
            strcat(src, INST_SET[inst].name);
        } else {
            char hex_num[20];
            int push_arg = 0;
            assert( (pc + 4) < code_len);
            vm_get_integer_from_bytecode(&compiled_code[pc], &push_arg);
            pc += 3;
            strcat(src, " ");
            sprintf(hex_num, "%08xh # %d \n", push_arg, push_arg);
            strcat(src, hex_num);
        }
        strcat(src, "\n");
    }

    strcat(src, "__CODE__\n");

    for (pc = code_start; pc < code_len; pc ++) {
        symbol_t inst = get_inst(compiled_code[pc]);
        if (inst == ERR) {
            printf("\nERROR: unrecognizable byte code at"
                   "byte number %d", pc);
            return 0;
        }
        strcat(src, INST_SET[inst].name);
        if (inst == PUSH) {
            char hex_num[20];
            int push_arg = 0;
            assert( (pc + 4) < code_len);
            vm_get_integer_from_bytecode(&compiled_code[pc + 1], &push_arg);
            pc += 4;
            strcat(src, " ");
            sprintf(hex_num, "%08xh # %d \n", push_arg, push_arg);
            strcat(src, hex_num);
        } else {
            strcat(src, "\n");
        }
    }

    char vm_fn[20];
    if (argc == 3) {
        strcpy(vm_fn, argv[2]);
    } else {
        strcpy(vm_fn, argv[1]);
        vm_fn[strlen(argv[1]) - 1] = '\0';
    }
    fp = fopen(vm_fn, "w");
    if (fp == NULL) {
        printf("\nERROR: could not create output file %s\n", vm_fn);
        return 0;
    }
    fprintf(fp, "%s", src);
    fclose(fp);  

    return 0;
}

