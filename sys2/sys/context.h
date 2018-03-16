#ifndef __PAGE_H
#define __PAGE_H
#include<sys/defs.h>

/*typedef struct _thread {
   process*  parent;
   void*     initialStack;
   void*     stackLimit;
   void*     kernelStack;
   int 	     priority;
   int       state;
   uint64_t rsp;
   uint64_t rbp;
   uint64_t rip;
   uint64_t rdi;
   uint64_t rsi;
   uint64_t rax;
   uint64_t rbx;
   uint64_t rcx;
   uint64_t rdx;
   uint64_t flags;

}thread;

*/

void contextswitch();
#endif

