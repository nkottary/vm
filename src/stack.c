/**
 * stack.c
 * Purpose: Stack implementation.
 *
 * @author Nishanth H. Kottary
 */

#include <malloc.h>
#include <assert.h>

#include "headers/stack.h"

Stack *newStack ()
{
    int i;
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    for (i = 0; i < MAX_STACK; i ++) {
        stack->elems[i] = NULL;
    }
    stack->top = -1;
}

int isEmpty (Stack *s)
{
    assert(s != NULL);
    return s->top == -1;
}

int isFull (Stack *s)
{
    assert(s != NULL);
    return s->top >= MAX_STACK - 1;
}

status_t push (Stack *s, void *key)
{
    assert(s != NULL);
    if (isFull(s)) {
        return FAILURE;
    }
    s->elems[++s->top] = key;
    return SUCCESS;
}

void *pop (Stack *s)
{
    if (isEmpty(s)) {
        return 0;
    }
    return s->elems[s->top--];
}

void *top (Stack *s)
{
    assert(s != NULL);
    if (isEmpty(s)) {
        return 0;
    }
    return s->elems[s->top];
}

void freeStack(Stack *s)
{
    assert(s != NULL);
    void *poped = pop(s);
    while (poped) {
        free(poped);
        poped = pop(s);
    }
    free(s);
}
    
