/**
 * lexer.c
 * Purpose: Lexical analysis and generation of tokens.
 *
 * @author: Nishanth H. Kottary
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <malloc.h>

#include "headers/lexer.h"
#include "headers/enums.h"
#include "headers/constants.h"

/**
 * Initialize a token_t node with string token, line number and a NULL next_tk
 * pointer.
 *
 * @param[out]   tk_node     A pointer to the token_t pointer to be initialized.
 * @param[in]    token       The token string.
 * @param[in]    line_num    The line number.
 *
 * @return                   The error status.
 */
static status_t vm_init_token_node (token_t **tk_node, const char *token, 
                                    const unsigned int line_num) {
    assert(token != NULL);
    assert(tk_node != NULL);

    *tk_node = (token_t *)malloc(sizeof(token_t));
    token_t *node = *tk_node;
    if (node == NULL) {
        return FAILURE;
    }
    
    strncpy(node->token, token, MAX_LINE_LEN - 1);
    node->line_num = line_num;
    node->next_tk = NULL;
    return SUCCESS;
}

/**
 * Gets a table of token_t's from a file.
 *
 * @param[in]   fp        The file pointer of the file to be lexed.
 * @param[out]  tok_list  A pointer to the head of the token list.
 * @param[out]  tt_len    The token table length.
 *
 * @return                The error status.
 */
status_t vm_get_token_list (FILE *fp, token_t **tok_list)
{
    assert(fp != NULL);
    assert(tok_list != NULL);

    char line[MAX_LINE_LEN],
        *line_ptr =   NULL,
        *checkpoint = NULL,
        *token =      NULL;

    const char delim[] = " \n";

    unsigned int line_num  = 0,
                 tok_len   = 0;

    token_t *next_tk  = NULL,
            *prev_tk  = NULL;

    vm_init_token_node(tok_list, "Dummy", 0);
    prev_tk = *tok_list;
    
    if (fgets(line, MAX_LINE_LEN, fp) == NULL) {
        return SUCCESS;
    }

    do {
        line_num++;
        line_ptr = line;
        while ( (token = strtok_r(line_ptr, delim, &checkpoint)) != NULL) {
            line_ptr = NULL;
            tok_len = strlen(token);
            if (tok_len == 0) {
                continue;
            }
            if (token[0] == '#') {
                break;
            }
            vm_init_token_node(&next_tk, token, line_num);
            prev_tk->next_tk = next_tk;
            prev_tk = next_tk;
        }
    } while (fgets(line, MAX_LINE_LEN, fp) != NULL);

    return SUCCESS;
}

/**
 * Free the token list.
 *
 * @param  token_list
 *
 * @return               The error status.
 */
status_t vm_free_token_list (token_t *tok_list)
{
    token_t *prev_tk = NULL,
            *next_tk = NULL;

    prev_tk = tok_list;
    next_tk = NULL;

    while (prev_tk) {
        next_tk = prev_tk->next_tk;
        free(prev_tk);
        prev_tk = next_tk;
    }

    return SUCCESS;
}
