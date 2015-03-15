#include <stdio.h>
#include <string.h>

#include "headers/constants.h"

int main (int argc, char *argv[]) 
{
    if (argc != 2 && argc != 3) {
        printf("\nUSAGE: interpreter <vm file>\n");
        return 0;
    }

    FILE *fp = fopen(argv[1], "r");
    char inst[5];

    if (fp == NULL) {
        printf("\nERROR: could not open file %s\n", argv[1]);
        return 0;
    }

    bytecode_t compiled_code[MAX_CODE_LEN],
        pc = 0;

    while (!feof(fp)) {
        if (fscanf(fp, "%s", inst) == -1) {
            continue;
        }
        else if (strcmp(inst, "/*") == 0) {
            while (!feof(fp)) {
                fscanf(fp, "%s", inst);
                if (strcmp(inst, "*/") == 0) {
                    break;
                }
            }
        }
        else if (strcmp(inst, "PUSH") == 0) {
            compiled_code[pc++] = INST_SET[PUSH].bytecode;
            if (!feof(fp)) {
                unsigned int arg;
                int len = 0;
                fscanf(fp, "%s", inst);
                len = strlen(inst);
                if (inst[len - 1] == 'h') {
                    inst[len - 1] = '\0';
                    sscanf(inst, "%02x", &arg);
                } else {
                    sscanf(inst, "%u", &arg);                    
                }
                compiled_code[pc++] = (bytecode_t) arg;
            } else {
                printf("\nERROR: PUSH not given an argument in"
                       " byte number %d\n", pc);
                return 0;
            }
        } else {
            bytecode_t bc = get_bytecode(inst);
            if (bc == INST_SET[ERR].bytecode) {
                printf("\nERROR: unrecognized instruction %s in"
                       " byte number %d\n", inst, pc);
                return 0;
            }
            compiled_code[pc++] = bc;
        }
    }
    fclose(fp);

    char vmc_fn[20];
    if (argc == 3) {
        strcpy(vmc_fn, argv[2]);
    } else {
        strcpy(vmc_fn, argv[1]);
        strcat(vmc_fn, "c");
    }
    fp = fopen(vmc_fn, "wb");
    if (fp == NULL) {
        printf("\nERROR: could not create output file %s\n", vmc_fn);
        return 0;
    }
    fwrite(&pc, sizeof (bytecode_t), 1, fp);
    fwrite(compiled_code, sizeof (bytecode_t), pc, fp);
    fclose(fp);  

    return 0;
}
