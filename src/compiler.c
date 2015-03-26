/**
 * compiler.c
 * Purpose: Compile a .vm file to a .vmc file
 *
 * @author Nishanth H. Kottary
 */
  
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "headers/constants.h"
#include "headers/enums.h"

#define INST_LEN       5
#define N_LABELS      10
#define LABEL_LEN     10
#define MAX_LINE_LEN  80

typedef enum JUMP_T {
    JUMP,
    JUMPIF,
    JUMPUN
} jump_t;

typedef struct LABEL_T {
    char label[LABEL_LEN];
    bytecode_t line_num;
    bytecode_t pc;
    bytecode_t id;
} label_t;

/**
 * Search for a label_t struct in label_table of length len given a string key.
 *
 * @param  key         The label string to be searched for.
 * @param  label_table Array of labels in the code.
 * @param  len         Length of the label_table.
 *
 * @return             The label struct if found else NULL.
 */
const label_t* vm_search_label_table (const char *key, 
                                      const label_t *label_table, const int len) 
{
    int i = 0;
    const label_t *this_label = NULL;

    assert(key != NULL);
    assert(label_table != NULL);

    for (i = 0; i < len; i ++) {
        this_label = &label_table[i];
        if (strcmp(key, this_label->label) == 0) {
            return this_label;
        }
    }
    return NULL;
}

/**
 * Perform a one pass through the file and build label table.
 *
 * @param[out] label_table  The table to be populated from file.
 * @param[out] len          The length of the table after being populated.
 * @param[in]  fp           The source file.
 *
 * @return                  Returns a status_t.
 */
status_t vm_build_label_table (label_t *label_table, int *len, FILE *fp)
{
    assert(label_table != NULL);
    assert(len != NULL);
    assert(fp != NULL);

    char line[MAX_LINE_LEN],
        *line_ptr = NULL,
        *checkpoint = NULL,
        *token = NULL;
    const char delim[] = " \n";

    bytecode_t line_num = 0,
               label_count = 0;

    if (fgets(line, MAX_LINE_LEN, fp) == NULL) { /* Nothing to do. */
        return SUCCESS;
    }

    do {
        line_num++;
        line_ptr = line;
        while ( (token = strtok_r(line_ptr, delim, &checkpoint)) != NULL) {
            line_ptr = NULL;
            if (token[0] == ':') {
                token++; /* forget the ':' */
                if (strlen(token) == 0) {
                    fprintf(stderr, 
                            "\nERROR: unnamed label in line number %d\n", 
                            line_num);
                    return FAILURE;
                }

                label_t *this_lbl = &label_table[label_count];
                const label_t *found_label = vm_search_label_table(token, 
                                                                   label_table, 
                                                                   label_count);
                if (found_label != NULL) {
                    fprintf(stderr, 
                            "\nERROR: Label redefined in line number %d, "
                            "earlier definition was here %d\n", 
                            line_num, found_label->line_num);
                    return FAILURE;
                }
                memset(this_lbl->label, 0, LABEL_LEN);
                strncpy(this_lbl->label, token, LABEL_LEN);
                this_lbl->line_num = line_num;
                this_lbl->id = label_count;

                label_count++;
            } else if (token[0] == '#') {
                break;
            }
        }
    } while (fgets(line, MAX_LINE_LEN, fp) != NULL);

    *len = label_count;
    return SUCCESS;
}

/**
 * Performs a one pass through the file and compiles to bytecode, but does not 
 * resolve labels and labelled jumps to GOTO's. Labels and jumps are only 
 * compiled to intermediate code. They will be replaced for proper bytecode
 * in a different pass of compilation. Label table should be built before 
 * calling this function.
 *
 * @param[out] compiled_code   The compiled bytecode array.
 * @param[out] len             The number of bytes in the compiled_code array.
 * @param[out] code_start      The offset at which the code segment starts.
 * @param[in]  fp              The source file.
 * @param[in]  label_table     
 * @param[in]  lt_len          The length of the label table.
 *
 * @return                     Returns a status_t.
 */
status_t vm_compile_first_pass (bytecode_t *compiled_code, int *len, 
                                int *code_start, FILE *fp, 
                                const label_t *label_table, const int lt_len)
{
    assert(compiled_code != NULL);
    assert(len           != NULL);
    assert(fp            != NULL);
    assert(code_start    != NULL);

    char line[MAX_LINE_LEN],
        *line_ptr =   NULL,
        *checkpoint = NULL,
        *token =      NULL;
    const char delim[] = " \n";

    bytecode_t line_num =    0,
               pc =          0;

    bool_flag_t  code_flag = FALSE;

    if (fgets(line, MAX_LINE_LEN, fp) == NULL) { /* Nothing to do. */
        return SUCCESS;
    }

    do {
        if (code_flag) {
            break;
        }

        line_num++;
        line_ptr = line;
        while ( (token = strtok_r(line_ptr, delim, &checkpoint)) != NULL) {
            line_ptr = NULL;
            if (token[0] == ':') {
                /* 
                 * The following will be replaced by NOP once the labels are 
                 * resolved.
                 */
                compiled_code[pc++] = INST_SET[LAB].bytecode;
            }
            else if (token[0] == '#') {
                break;
            } else if (strcmp(token, "__CODE__") == 0) {
                code_flag = TRUE;
                break;
            } else {
                int arg = 0;
                int len = 0;
                while (token == NULL) {
                    if (fgets(line, MAX_LINE_LEN, fp) == NULL) {
                        fprintf(stderr, 
                                "\nError: Unexpected end of file- PUSH not"
                                " given an argument in line number %d", 
                                line_num);
                        return FAILURE;
                    }
                    line_num++;
                    token = strtok_r(line, delim, &checkpoint);
                }
                len = strlen(token);
                if (token[len - 1] == 'h') {
                    token[len - 1] = '\0';
                    sscanf(token, "%x", &arg);
                } else {
                    sscanf(token, "%d", &arg);                    
                }
                vm_put_integer_to_bytecode(&compiled_code[pc], arg);
                pc += 4;
            }
        }
    } while (fgets(line, MAX_LINE_LEN, fp) != NULL);

    if (!code_flag) {
        fprintf(stderr, "\nError: Code segment not declared!");
        return FAILURE;
    }
    *code_start = pc;

    do {
        line_num++;
        line_ptr = line;
        while ( (token = strtok_r(line_ptr, delim, &checkpoint)) != NULL) {
            line_ptr = NULL;
            if (token[0] == ':') {
                /* 
                 * The following will be replaced by NOP once the labels are 
                 * resolved.
                 */
                compiled_code[pc++] = INST_SET[LAB].bytecode;
            }
            else if (strncmp(token, "jump", 4) == 0) {
                jump_t flag = JUMP;
                if (strcmp(token, "jumpif") == 0) {
                    flag = JUMPIF;
                } else if (strcmp(token, "jumpun") == 0) {
                    flag = JUMPUN;
                }

                token = strtok_r(NULL, delim, &checkpoint);
                if (token == NULL) {
                    fprintf(stderr, "\nERROR: Jump not given a label in line "
                            "number %d", line_num);
                    return FAILURE;
                }
                if (strlen(token) == 0)  {
                    fprintf(stderr, "\nERROR: Argument to jump is not a label"
                            " in line number %d", line_num);
                    return FAILURE;
                }
                const label_t *found_label = 
                    vm_search_label_table(token, label_table, lt_len);

                if (found_label == NULL) {
                    fprintf(stderr, "\nERROR: Jump label not found"
                            " in line number %d", line_num);
                    return FAILURE;
                }

                compiled_code[pc++] = INST_SET[JMP].bytecode;
                compiled_code[pc++] = found_label->id;
                /*
                 * Making room for 32-bit integer argument to push
                 */
                compiled_code[pc++] = 0;
                compiled_code[pc++] = 0;
                compiled_code[pc++] = 0;
                if (flag == JUMPIF) {
                    compiled_code[pc] = INST_SET[GOIF].bytecode;
                } else if (flag == JUMPUN) {
                    compiled_code[pc] = INST_SET[GOUN].bytecode;
                } else {
                    compiled_code[pc] = INST_SET[GOTO].bytecode;
                }
                pc++;
            }
            else if (token[0] == '#') {
                break;
            } else if (strcmp(token, "mem_get") == 0) {
                compiled_code[pc++] = INST_SET[MEM].bytecode;
                token = strtok_r(NULL, delim, &checkpoint);
                if (token == NULL) {
                    fprintf(stderr, "\nERROR: Memget not given a label in line "
                            "number %d", line_num);
                    return FAILURE;
                }
                if (strlen(token) == 0)  {
                    fprintf(stderr, "\nERROR: Argument to memget is not a label"
                            " in line number %d", line_num);
                    return FAILURE;
                }
                const label_t *found_label = 
                    vm_search_label_table(token, label_table, lt_len);

                if (found_label == NULL) {
                    fprintf(stderr, "\nERROR: memget label not found"
                            " in line number %d", line_num);
                    return FAILURE;
                }
                compiled_code[pc++] = found_label->id;
                /*
                 * Making room for 32-bit integer argument to push
                 */
                compiled_code[pc++] = 0;
                compiled_code[pc++] = 0;
                compiled_code[pc++] = 0;
                compiled_code[pc++] = INST_SET[GET].bytecode;
            } else if (strcmp(token, "mem_put") == 0) {
                compiled_code[pc++] = INST_SET[MEM].bytecode;
                token = strtok_r(NULL, delim, &checkpoint);
                if (token == NULL) {
                    fprintf(stderr, "\nERROR: Memput not given a label in line "
                            "number %d", line_num);
                    return FAILURE;
                }
                if (strlen(token) == 0)  {
                    fprintf(stderr, "\nERROR: Argument to memput is not a label"
                            " in line number %d", line_num);
                    return FAILURE;
                }
                const label_t *found_label = 
                    vm_search_label_table(token, label_table, lt_len);

                if (found_label == NULL) {
                    fprintf(stderr, "\nERROR: Memput label not found"
                            " in line number %d", line_num);
                    return FAILURE;
                }
                compiled_code[pc++] = found_label->id;
                /*
                 * Making room for 32-bit integer argument to push
                 */
                compiled_code[pc++] = 0;
                compiled_code[pc++] = 0;
                compiled_code[pc++] = 0;
                compiled_code[pc++] = INST_SET[PUT].bytecode;
            } else if (strcmp(token, "PUSH") == 0) {
                compiled_code[pc++] = INST_SET[PUSH].bytecode;
                if (!feof(fp)) {
                    int arg = 0;
                    int len = 0;
                    token = strtok_r(NULL, delim, &checkpoint);
                    while (token == NULL) {
                        if (fgets(line, MAX_LINE_LEN, fp) == NULL) {
                            fprintf(stderr, 
                                    "\nError: Unexpected end of file- PUSH not"
                                    " given an argument in line number %d", 
                                    line_num);
                            return FAILURE;
                        }
                        line_num++;
                        token = strtok_r(line, delim, &checkpoint);
                    }
                    len = strlen(token);
                    if (token[len - 1] == 'h') {
                        token[len - 1] = '\0';
                        sscanf(token, "%x", &arg);
                    } else {
                        sscanf(token, "%d", &arg);                    
                    }
                    vm_put_integer_to_bytecode(&compiled_code[pc], arg);
                    pc += 4;
                } else {
                    fprintf(stderr, "\nERROR: PUSH not given an argument in"
                            " line number %d\n", line_num);
                    return FAILURE;
                }
            } else {
                bytecode_t bc = get_bytecode(token);
                if (bc == INST_SET[ERR].bytecode) {
                    fprintf(stderr, "\nERROR: unrecognized instruction %s in"
                            " line number %d\n", token, line_num);
                    return FAILURE;
                }
                compiled_code[pc++] = bc;
            }
        }
    } while (fgets(line, MAX_LINE_LEN, fp) != NULL);

    *len = pc;
    return SUCCESS;
}

/**
 * A second pass of compilation, replace LAB with a NOP, 
 * replace JMP and label id with PUSH <pc> GOTO.
 *
 * @param  compiled_code      Compiled code from first pass.
 * @param  len                Length of the compiled code.
 * @param  code_start         The offset where the code starts.
 * @param  label_table
 * @param  lt_len             Length of label_table.
 * 
 * @return                    Returns a status_t.
 */
status_t vm_compile_second_pass (bytecode_t *compiled_code, const int code_len,
                                 const int code_start,
                                 label_t *label_table, const int lt_len)
{
    int i = 0;
    int label_count = 0;

    assert(compiled_code != NULL);
    assert(label_table != NULL);

    for (i = 0; i < code_len; i++) {
        if (compiled_code[i] == INST_SET[PUSH].bytecode) {
            i += 4; /* Skip the argument to PUSH. */
        }
        else if (compiled_code[i] == INST_SET[LAB].bytecode) {
            compiled_code[i] = INST_SET[NOP].bytecode;

            assert(label_count < lt_len);
            label_table[label_count].pc = (bytecode_t)(i + 1);
            label_count++;
            i++;
        } 
    }

    for (i = code_start; i < code_len; i++) {
        if (compiled_code[i] == INST_SET[PUSH].bytecode) {
            i += 4; /* Skip the argument to PUSH. */
        }
        else if (compiled_code[i] == INST_SET[JMP].bytecode ||
                 compiled_code[i] == INST_SET[MEM].bytecode) {
            compiled_code[i++] = INST_SET[PUSH].bytecode;
            assert(i < code_len);
            assert(compiled_code[i] < lt_len);
            compiled_code[i] = label_table[compiled_code[i]].pc;
            i += 4;  /* +3 because stack elem is 4 byts and in above
                      * line we have already added 1 byte of pc, 
                      * +1 because we have a GOTO, GOIF, GOUN, GET or PUT
                      * instruction anyway.
                      */
        }
    }

    return SUCCESS;
}

int main (int argc, char *argv[]) 
{
    if (argc != 2 && argc != 3) {
        fprintf(stderr, "\nUSAGE: interpreter <vm file>\n");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(argv[1], "r");

    if (fp == NULL) {
        fprintf(stderr, "\nERROR: could not open file %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    bytecode_t compiled_code[MAX_CODE_LEN];
    label_t label_table[N_LABELS];    
    int code_len = 0, 
        code_start = 0,
        label_count = 0;

    memset(compiled_code, MAX_CODE_LEN, sizeof(bytecode_t));
    memset(label_table, N_LABELS, sizeof(label_t));

    if (vm_build_label_table(label_table, &label_count, fp) == FAILURE) {
        fprintf(stderr, "\nERROR: Failed to build label table.");
        exit(EXIT_FAILURE);
    }

    rewind(fp);
    if (vm_compile_first_pass(compiled_code, &code_len, &code_start,
                              fp, label_table, label_count) == FAILURE) {
        fprintf(stderr, "\nERROR: Compilation failed in first pass.");
        exit(EXIT_FAILURE);
    }
    fclose(fp);

    if (vm_compile_second_pass(compiled_code, code_len, code_start, label_table,
                               label_count) == FAILURE) {
        fprintf(stderr, "\nERROR: Compilation failed in second pass.");
        exit(EXIT_FAILURE);
    }

    char vmc_fn[20];
    if (argc == 3) {
        strcpy(vmc_fn, argv[2]);
    } else {
        strcpy(vmc_fn, argv[1]);
        strcat(vmc_fn, "c");
    }
    fp = fopen(vmc_fn, "wb");
    if (fp == NULL) {
        fprintf(stderr, "\nERROR: could not create output file %s\n", vmc_fn);
        exit(EXIT_FAILURE);
    }
    fwrite(&code_start, sizeof (bytecode_t), 1, fp);
    fwrite(&code_len, sizeof (bytecode_t), 1, fp);
    fwrite(compiled_code, sizeof (bytecode_t), code_len, fp);
    fclose(fp);  

    return 0;
}
