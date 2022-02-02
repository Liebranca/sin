// ---   *   ---   *   ---
// STACK
// Last In, First Out
//
// LIBRE SOFTWARE
// Licensed under GNU GPL3
// be a bro and inherit
//
// CONTRIBUTORS
// lyeb,
// ---   *   ---   *   ---

// deps
  #include "stack.h"

  #include <stdlib.h>
  #include <string.h>
  #include "ce/arstd.h"

// ---   *   ---   *   ---

typedef struct stk {
  size_t sz;
  size_t top;

  int buff[];

};

// ---   *   ---   *   ---

stk* stknt(size_t sz) {

  // allocate header+dynarray
  stk* s=malloc(
    sizeof(stk)
   +(sz*sizeof(int))

  );s->sz=sz;
    s->top=0;

  // zero out the array
  memset(s->buff,0,s->sz);
  return s;

};

// ---   *   ---   *   ---
// these two bounds check;
// top freezes at max or min

void stkpush(stk* s,int x) {

  s->top-=s->top>=s->sz;

  s->buff[s->top]=x;
  s->top+=s->top<s->sz;

};int stkpop(stk* s) {
  s->top-=s->top>0;
  return s->buff[s->top];

};

// ---   *   ---   *   ---
