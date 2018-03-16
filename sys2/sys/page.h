#ifndef __PAGE_H
#define __PAGE_H
#include<sys/defs.h>

/*
typedef struct pt
{
page_t PTE[512];
}pte;

typedef struct pd
{
page_t PDE[512];
}pde;

typedef struct pdpt
{
page_t PDPTE[512];
}pdpte;

typedef struct pml
{
page_t PMLE[512];
}pmle;
*/

uint64_t *kmalloc();
uint64_t firstframe();
void set(uint64_t );
void map(uint64_t );
void map1(uint64_t );
void getpage(uint64_t *,uint64_t * );
void enablepaging(uint64_t );
void page(uint64_t *,uint64_t *,uint64_t ,uint64_t *,uint64_t,uint8_t*);
void *memset(void *dest,int c,size_t n);
void setframe(uint64_t );
void clearframe(uint64_t);
void setpageframe(uint64_t);
uint64_t freeframe();
uint64_t *allocateadditional(uint64_t);
uint64_t *getextraallocated();
uint64_t *alloc(uint64_t size);
#endif

