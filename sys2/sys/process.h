#ifndef __PROCESS_H
#define __PROCESS_H
#include<sys/defs.h>

#define STACK_START 0xff003000
#define STACK_END 0xff001000

#define HEAP_END 0x09000000
#define  HEAP_START 0x07000000


enum {OTHER,STACK,HEAP,BSS} section;

uint64_t floor(uint64_t number);
uint64_t ceil(uint64_t number);
uint64_t *childrip;

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
uint64_t startbrk, start_stack,endbrk;
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


uint64_t irbx,ircx,irdx,irbp,ir8,ir9,ir10,ir11,ir12,ir13,ir14,ir15,irip;

typedef struct {
    uint64_t rax, rbx, rcx, rdx, rsi, rdi, rsp, rbp, rip, eflags,cr3,r8,r9,r10,r11,r12,r13,r14,r15;
} registers;


struct processqueue
{
	struct task_struct *process;
	struct processqueue *next;
}*startq,*reference,*current;

/*


struct task_struct *processq,*startq;
struct task_struct *waitqueue=NULL;
struct task_struct *zombieq;
struct task_struct *current;
*/

struct task_struct
{
	registers regs;
	uint64_t pid;
	uint64_t ppid;
	int chno;
	uint64_t waitpid;
	uint64_t *stack;
	uint64_t kstack;
	uint64_t rsp;
	enum { RUNNING, SLEEPING, ZOMBIE,WAITING } state;
	int exitstatus;
	uint64_t pmle;
	uint64_t cr3;
	uint64_t *userstack;
	char *dir;
	struct mm_struct *mm; 
	char func[100]; 
	file *fd[50];
	struct task_struct *next;
};

struct task_struct *readyq[100];

void createpagetable(struct task_struct *);
uint64_t *kmalloc_ap(struct task_struct *);
void mappage(uint64_t addr);
void initialize(struct task_struct *pcb);
uint64_t getadress(struct task_struct *);
void createprocess(struct task_struct *pcb);
void mappage(uint64_t virt);
void kmmap(uint64_t ,int ,int ,uint64_t);
//void kmmap(uint64_t ,int ,int );
void copytostack(struct task_struct *pcb,int argc);
void kmmap1(uint64_t ,int ,int );
int exit(int status);
void maptables(uint64_t );
struct value_t
{
uint64_t physic;
uint64_t *virtue;
};
int execvpe(char *filename, char *argv[],char *envp[]);
void createchildtable(struct task_struct *parent,struct task_struct *child);
struct value_t kmalloc_p(struct task_struct *pcb);
uint64_t allocateUser(uint64_t first,uint64_t second,uint64_t third,uint64_t fourth,uint64_t fifth,uint64_t sixth);
struct task_struct *getCurrentProcess();
void deallocateUser(uint64_t first,uint64_t second);
void schedule();
void switchtoRing3(struct task_struct *pcb);
int fork();
int execvpe(char *filename, char *argv[],char *envp[]);
void kfree(uint64_t *base);
int assignpid();
struct task_struct *clone(struct task_struct *parent);
uint64_t *getpdeentry(uint64_t *addr,int);
uint64_t *getpteentry(uint64_t *addr,int);
uint64_t *getpdptentry(uint64_t addr,int);
void mapprocess(uint64_t virtual,uint64_t physical,uint64_t);
uint64_t is_file_exists(char* filename);
void removept(uint64_t pmle);
void load(struct task_struct *pcb,char *file,char *argv[] );
#endif

