/*
 * stack.h
 */

typedef struct stack_elt_tag {
  int n;
  int d;
  struct stack_elt_tag *next;
} stack_elt;

typedef struct {
  stack_elt *first, *last;
} stack;

void init_stack (stack *s);
void push (stack *s, int nn, int dd);
void pop (stack *s);
void print_top (stack *s);
void delete_all (stack *s);

