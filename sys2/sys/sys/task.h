#ifndef _TASK_H
#define _TASK_H
#include<sys/defs.h>
#include<sys/process.h>
extern void initTasking();

struct tss_t {
  uint32_t reserved_0;
  void *rsp0;
  void *rsp1;
  void *rsp2;
  uint64_t reserved_1;
  void *ist1;
  void *ist2;
  void *ist3;
  void *ist4;
  void *ist5;
  void *ist6;
  void *ist7;
  uint64_t reserved_2;
  uint16_t reserved_3;
  uint16_t iomap_base;
}__attribute__((packed));

struct sys_segment_descriptor {
  uint64_t sd_lolimit :16;/* segment extent (lsb) */
  uint64_t sd_lobase :24; /* segment base address (lsb) */
  uint64_t sd_type :5; /* segment type */
  uint64_t sd_dpl :2; /* segment descriptor priority level */
  uint64_t sd_p :1; /* segment descriptor present */
  uint64_t sd_hilimit :4; /* segment extent (msb) */
  uint64_t sd_xx1 :3; /* avl, long and def32 (not used) */
  uint64_t sd_gran :1; /* limit granularity (byte/page) */
  uint64_t sd_hibase :40; /* segment base address (msb) */
  uint64_t sd_xx2 :8; /* reserved */
  uint64_t sd_zero :5; /* must be zero */
  uint64_t sd_xx3 :19; /* reserved */
}__attribute__((packed));

/*
static uint64_t gdt[MAX_GDT] = {
  0, 
  GDT_CS | P | DPL0 | L, 
  GDT_DS | P | W | DPL0, 
  GDT_CS | P | DPL3 | L, 
  GDT_DS | P | W | DPL3, 
  0, 0, 
};
*/

 
struct registers
{
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsi, rbp, rdx, rcx, rbx, rax, rdi;
    uint64_t int_no, err_code;
    uint64_t rip, cs, rflags, rsp, ss;
} __attribute__((packed));

typedef struct registers registers_t;

typedef struct {
    uint64_t rax, rbx, rcx, rdx, rsi, rdi, rsp, rbp, rip, eflags, cr3;
} Registers;
 
typedef struct Task {
    Registers regs;
    struct Task *next;
} Task;

//extern void switchTask1(Registers *,Registers *);
 
extern void initTasking();
extern void createTask(Task*, void (*)(), uint64_t, uint64_t*);
//void createprocess(struct task_struct *); 
extern void yield(); 
extern void switchTask(Registers *old, Registers *new);
extern void yield1();
void otherMain();
void funMain();
#endif
