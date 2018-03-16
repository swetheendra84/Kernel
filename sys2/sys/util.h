#ifndef __UTIL_H
#define __UTIL_H
#include<sys/defs.h>
uint64_t free;
uint64_t *frame1;
uint64_t *pmle_pp;
uint64_t *pdpte_pp;
uint64_t *pde_pp;
uint64_t *pte_pp;
uint64_t *pmle_p;
uint64_t *pdpte_p;
uint64_t *pde_p;
uint64_t *pte_p;
uint64_t phybas;
uint64_t phyfree; 
char *strcpy(char *d, const char *s);
void *mmemset(void *dest,int c,size_t n);
int mstrcmp (const char * s1, const char * s2);
int mystrlen(char *str);
uint64_t virttophys(uint64_t *);
long strtoint(const char *str);
uint64_t octtodec(uint64_t num);
uint64_t pow(uint64_t a,int b);
void mmemcpy(void *dest, void *src, size_t n);
int strncmp (const char * s1, const char * s2,int size);
#endif

