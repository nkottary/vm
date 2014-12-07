#define MAX_STACK 100

typedef enum {
  FAILURE,
  SUCCESS
} status_t;

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

