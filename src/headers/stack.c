#include <malloc.h>

#include "stack.h"

Stack *newStack () {
  int i;
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  for (i = 0; i < MAX_STACK; i ++) {
    stack->elems[i] = NULL;
  }
  stack->top = -1;
}

int isEmpty (Stack *s) {
  return s->top == -1;
}

int isFull (Stack *s) {
  return s->top >= MAX_STACK - 1;
}

status_t push (Stack *s, void *key) {
  if (isFull(s)) {
    return FAILURE;
  }
  s->elems[++s->top] = key;
  return SUCCESS;
}

void *pop (Stack *s) {
  if (isEmpty(s)) {
    return 0;
  }
  return s->elems[s->top--];
}

void *top (Stack *s) {
  if (isEmpty(s)) {
    return 0;
  }
  return s->elems[s->top];
}

void freeStack(Stack *s) {
  void *poped = pop(s);
  while (poped) {
    free(poped);
    poped = pop(s);
  }
  free(s);
}
    
