#ifndef __KPRINTF_H
#define __KPRINTF_H
#include<sys/defs.h>
void kprintf(const char *fmt, ...);
void printTimeSinceBoot(long );
void itoa(unsigned long , char * ,int);
void printTimeSinceBoot(long );
void printglyph(uint8_t ascii,int shiftPressed , int controlPressed);
#endif
