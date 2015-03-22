/**
 * stack.h
 *
 * @author Nishanth H. Kottary
 */

#include "enums.h"

#ifndef STACK_H
#define STACK_H

#define MAX_STACK 100

struct STACK {
    void* elems[MAX_STACK];
    int top;
};

typedef struct STACK Stack;

Stack *newStack (void);
int isEmpty (Stack *stk);
int isFull (Stack *stk);
status_t push (Stack *stk, void *key);
void *pop (Stack *stk);
void *top (Stack *stk);
void freeStack(Stack *stk);

#endif
