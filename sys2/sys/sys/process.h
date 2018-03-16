#ifndef __PROCESS_H
#define __PROCESS_H
#include<sys/defs.h>

#define STACK_START 0xf0000000 
#define STACK_END 0xEFFFF000 

#define HEAP_END 0x10000000
#define HEAP_START 0x08000000

enum {OTHER,STACK,HEAP,BSS} section;


typedef struct {
    int inode_num;
    char filename[100];
    int perm;
    int size;
    char type;
    int sector_loc[10];
    int offset;
    uint64_t address;
    int is_fs;
} file;


struct mm_struct
{
struct vma *mmap,*current;
uint64_t start_code,end_code,start_data,end_data;
uint64_t start_brk, start_stack;
uint64_t arg_start,arg_end,env_start,env_end;
uint64_t rss,total_vm,locked_vm;
};

struct vma{
	struct mm_struct *mm;
	uint64_t start;
	uint64_t end;
	struct vma *next;
	uint64_t flags;
	uint64_t type;
	uint64_t prot;
	uint64_t phys;
};


typedef struct {
    uint64_t rax, rbx, rcx, rdx, rsi, rdi, rsp, rbp, rip, eflags;
} registers;

struct processqueue
{
struct task_struct *process;
struct processqueue *next;
} *current,*temp,*startq;


struct task_struct
{
	registers regs;
	uint64_t pid;
	uint64_t *stack;
	uint64_t kstack;
	uint64_t rsp;
	enum { RUNNING, SLEEPING, ZOMBIE } state;
	int exitstatus;
	uint64_t pmle;
	uint64_t cr3;
	struct mm_struct *mm; 
	char func[100]; 
	file *fd[50];
};

struct task_struct *readyq[100];

void createpagetable(struct task_struct *);
uint64_t *kmalloc_ap(struct task_struct *);
void mappage(uint64_t addr);
void initialize(struct task_struct *pcb);
uint64_t getadress(struct task_struct *);
void createprocess(struct task_struct *pcb);
void mappage(uint64_t virt);
void kmmap(uint64_t ,int ,int );
void maptables(uint64_t );
struct value_t
{
uint64_t physic;
uint64_t *virtue;
};

struct value_t kmalloc_p(struct task_struct *pcb);
uint64_t allocateUser(uint64_t first,uint64_t second,uint64_t third,uint64_t fourth,uint64_t fifth,uint64_t sixth);
struct task_struct *getCurrentProcess();
void deallocateUser(uint64_t first,uint64_t second);
void schedule();
void switchtoRing3(struct task_struct *pcb);

#endif

