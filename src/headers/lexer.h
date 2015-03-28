/**
 * lexer.h
 * Purpose: Lexical analysis and token generation.
 *
 * @author Nishanth H. Kottary
 */

#ifndef LEXER_H
#define LEXER_H

#include "constants.h"

struct _token_t{
    char token[MAX_LINE_LEN];
    unsigned int line_num;
    struct _token_t *next_tk;
};

typedef struct _token_t token_t;

status_t vm_get_token_list (FILE *fp, token_t **tok_list);
status_t vm_free_token_list (token_t *tok_list);

#endif
