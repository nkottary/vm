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
#include "headers/lexer.h"

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
 * Perform a one pass through the token list and build label table.
 *
 * @param[out] label_table  The table to be populated from file.
 * @param[out] len          The length of the table after being populated.
 * @param[in]  tok_list        
 *
 * @return                  Returns a status_t.
 */
status_t vm_build_label_table (label_t *label_table, int *len, 
                               const token_t *tok_list)
{
    assert(label_table != NULL);
    assert(len != NULL);
    assert(tok_list != NULL);

    const char *token = NULL;
    int line_num = 0;

    bytecode_t label_count = 0;

    *len = 0;

    while (tok_list != NULL) {

        token = tok_list->token;
        line_num = tok_list->line_num;

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
        }
        tok_list = tok_list->next_tk;
    } 
    *len = label_count;
    return SUCCESS;
}

/**
 * Get the argument given to push. Assumes token is valid.
 *
 * @param[out]  arg        The integer argument to push.
 * @param[in]   token      The valid token from which to parse.
 *
 * @return                 The error status.
 */
status_t vm_get_coded_arg (int *arg, const char *token) {

    assert(arg != NULL);
    assert(token != NULL);

    const int len = strlen(token);
    if (token[0] == '\'') {
        if (len != 3 || token[2] != '\'') {
            return FAILURE;
        } 
        *arg = (int)token[1];
    } else if (token[len - 1] == 'h') {
        char scan_token[MAX_LINE_LEN];
        strncpy(scan_token, token, len);
        scan_token[len - 1] = '\0';
        sscanf(scan_token, "%x", arg);
    } else {
        sscanf(token, "%d", arg);                    
    }
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
                                int *code_start, token_t *tok_list, 
                                const label_t *label_table, const int lt_len)
{
    assert(compiled_code != NULL);
    assert(len           != NULL);
    assert(tok_list      != NULL);
    assert(code_start    != NULL);

    const char *token = NULL;
    const char delim[] = " \n";

    bytecode_t pc = 0;
    unsigned int line_num = 0;

    bool_flag_t  code_flag    = FALSE;

    tok_list = tok_list->next_tk; /* Ignore the dummy token */

    while (tok_list && !code_flag) {
        line_num = tok_list->line_num;
        token = tok_list->token;
        if (token[0] == ':') {
            /* 
             * The following will be replaced by NOP once the labels are 
             * resolved.
             */
            compiled_code[pc++] = INST_SET[LAB].bytecode;
        } else if (strcmp(token, "__CODE__") == 0) {
            code_flag = TRUE;
        } else {
            int arg = 0;
            if (vm_get_coded_arg(&arg, token) == FAILURE) {
                fprintf(stderr, 
                        "\nError: Syntax error in push argument in "
                        "line number %d.", line_num);
                return FAILURE;
            }
            vm_put_integer_to_bytecode(&compiled_code[pc], arg);
            pc += 4;
        }
        tok_list = tok_list->next_tk;
    }

    if (!code_flag) {
        fprintf(stderr, "\nError: Code segment not declared!");
        return FAILURE;
    }
    *code_start = pc;

    while (tok_list) {
        line_num = tok_list->line_num;
        token = tok_list->token;
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

            tok_list = tok_list->next_tk;
            line_num = tok_list->line_num;
            token = tok_list->token;

            const label_t *found_label = 
                vm_search_label_table(token, label_table, lt_len);

            if (found_label == NULL) {
                fprintf(stderr, "\nERROR: Jump label not found"
                        " in line number %d", line_num);
                return FAILURE;
            }

            compiled_code[pc++] = INST_SET[JMP].bytecode;
            vm_put_integer_to_bytecode(&compiled_code[pc], found_label->id);
            pc += 4;

            if (flag == JUMPIF) {
                compiled_code[pc] = INST_SET[GOIF].bytecode;
            } else if (flag == JUMPUN) {
                compiled_code[pc] = INST_SET[GOUN].bytecode;
            } else {
                compiled_code[pc] = INST_SET[GOTO].bytecode;
            }
            pc++;
        } else if (strcmp(token, "mem_get") == 0) {
            compiled_code[pc++] = INST_SET[MEM].bytecode;
            tok_list = tok_list->next_tk;
            line_num = tok_list->line_num;
            token = tok_list->token;

            const label_t *found_label = 
                vm_search_label_table(token, label_table, lt_len);

            if (found_label == NULL) {
                fprintf(stderr, "\nERROR: memget label not found"
                        " in line number %d", line_num);
                return FAILURE;
            }
            vm_put_integer_to_bytecode(&compiled_code[pc], found_label->id);
            pc += 4;
            compiled_code[pc++] = INST_SET[GET].bytecode;
        } else if (strcmp(token, "mem_put") == 0) {
            compiled_code[pc++] = INST_SET[MEM].bytecode;
            tok_list = tok_list->next_tk;
            line_num = tok_list->line_num;
            token = tok_list->token;

            const label_t *found_label = 
                vm_search_label_table(token, label_table, lt_len);

            if (found_label == NULL) {
                fprintf(stderr, "\nERROR: Memput label not found"
                        " in line number %d", line_num);
                return FAILURE;
            }
            vm_put_integer_to_bytecode(&compiled_code[pc], found_label->id);
            pc += 4;
            compiled_code[pc++] = INST_SET[PUT].bytecode;
        } else if (strcmp(token, "PUSH") == 0) {
            compiled_code[pc++] = INST_SET[PUSH].bytecode;

            int arg = 0;
            tok_list = tok_list->next_tk;
            line_num = tok_list->line_num;
            token = tok_list->token;

            if (vm_get_coded_arg(&arg, token) == FAILURE) {
                fprintf(stderr, 
                        "\nError: Syntax error in push argument in "
                        "line number %d.", line_num);
                return FAILURE;
            }
            vm_put_integer_to_bytecode(&compiled_code[pc], arg);
            pc += 4;
        } else {
            bytecode_t bc = get_bytecode(token);
            if (bc == INST_SET[ERR].bytecode) {
                fprintf(stderr, "\nERROR: unrecognized instruction %s in"
                        " line number %d\n", token, line_num);
                return FAILURE;
            }
            compiled_code[pc++] = bc;
        }
        tok_list = tok_list->next_tk;
    }

    compiled_code[pc++] = INST_SET[END].bytecode; /* This is required when there
                                                   * is label at the very end
                                                   * because the pc will go to
                                                   * the instruction after that
                                                   * label.
                                                   */
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

    token_t *tok_list = NULL;
    bytecode_t compiled_code[MAX_CODE_LEN];
    label_t label_table[N_LABELS];    
    int code_len = 0, 
        code_start = 0,
        label_count = 0;

    memset(compiled_code, MAX_CODE_LEN, sizeof(bytecode_t));
    memset(label_table, N_LABELS, sizeof(label_t));

    if (vm_get_token_list(fp, &tok_list) == FAILURE) {
        fprintf(stderr, "\nERROR: Failed to tokenize.");
        exit(EXIT_FAILURE);
    }

    if (vm_build_label_table(label_table, &label_count, tok_list) == FAILURE) {
        fprintf(stderr, "\nERROR: Failed to build label table.");
        exit(EXIT_FAILURE);
    }

    rewind(fp);
    if (vm_compile_first_pass(compiled_code, &code_len, &code_start,
                              tok_list, label_table, label_count) == FAILURE) {
        fprintf(stderr, "\nERROR: Compilation failed in first pass.");
        exit(EXIT_FAILURE);
    }
    fclose(fp);
    vm_free_token_list(tok_list);

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
