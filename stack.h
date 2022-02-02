#ifndef __STACK_H__
#define __STACK_H__

#ifdef __cplusplus
extern "C" {
#endif

// ---   *   ---   *   ---
// deps
  #include <stddef.h>

// ---   *   ---   *   ---
// types
typedef struct stk stk;

// ---   *   ---   *   ---
// methods

// initialize stack (requires free)
stk* stknt(size_t sz);

// push
void stkpush(stk* s,int x);

// pop
int stkpop(stk* s);

// ---   *   ---   *   ---

#ifdef __cplusplus
};
#endif

#endif // __STACK_H__
