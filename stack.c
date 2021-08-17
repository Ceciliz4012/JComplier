
/*
 * stack.c
 */

#include "stack.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void init_stack (stack *s)
{
  s->first = NULL;
  s->last = NULL;
}

int gcd(int a, int b) { 
  if (a == 0) 
    return b; 
  return gcd(b%a, a); 
}

/*void print_top (stack *s) {
  if (s->first) {
    stack_elt *elt = s->first;
    printf ("%d/%d\n", elt->n, elt->d);
  }
}*/

void push (stack *s, int nn, int dd)
{
  stack_elt *elt;
  
  // Allocate a new list element with malloc
  elt = malloc (sizeof(*elt));

  // If malloc fails end the program
  if (elt == NULL) {
    //printf ("Couldn't allocate a new stack element\n");
    exit (1);
  }
  int factor = gcd(abs(nn), abs(dd));

  elt->n = nn/factor;
  elt->d = dd/factor;

  // Check if stack is empty
  if (s->first) {
    elt->next = s->first;
    s->first = elt;
  } else {
    s->first = elt;
    s->last = elt;
    elt->next = NULL;
  }
}

void pop (stack *s)
{
  stack_elt *elt;
  
  if (s->first) {
    elt = s->first;

    if (s->first == s->last) {
      //deleting the only element in the stack
      s->first = NULL;
      s->last = NULL;
    } else {
      s->first = elt->next;
    }

    // free the elt
    free(elt);
  }
}


void delete_all (stack *s)
{
  while (s->first) {
    pop(s);
  }
}

