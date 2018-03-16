#include<sys/kprintf.h>
#include<sys/isr.h>
#include<sys/process.h>
#include<sys/tarfs.h>
void isr_handler128() {

    uint64_t syscall_no = 0;
  uint64_t second;
	uint64_t four;
    uint64_t first,third;
	uint64_t fifth=2;
	uint64_t sixth=0;

    __asm __volatile__("movq %%rax, %0; \n"
            "movq %%rbx, %1; \n"
            "movq %%rcx, %2; \n"
            "movq %%rdx, %3;  \n"
	    "movq %%rdi, %4;  \n"
	    "movq %%r9, %5;  \n"
	    "movq %%r10, %6;  \n"

            : "=g"(syscall_no),"=g"(first), "=g"(second), "=g"(third), "=g"(four), "=g"(fifth), "=g"(sixth)
            :
            : "rax","rbx","rcx","rdx","rdi","r9","r10"
            );  
    
if(syscall_no == 1) {
	char *result=(char *)second;
	writeto(first,result,third);
	kprintf("\n");	
	
}

else if(syscall_no==2)
{

        uint64_t addr= (uint64_t)allocateUser(first,second,third,four,2,sixth);
        
	        __asm__ __volatile__(
                "movq %0, %%rax;"
                :
                :"a" ((uint64_t)addr)
                :"cc", "memory"
                );


}

else if(syscall_no == 3) {
    
	struct task_struct *pcb=getCurrentProcess();
	struct vma *temp= pcb->mm->mmap;
	while(temp->start!=first)
	{
		temp=temp->next;
	}
	deallocateUser(first,temp->end-temp->start);
}


else if(syscall_no==4)
{

struct task_struct *p=getCurrentProcess();

__asm__ __volatile__ ("movq %%rbx, %0;" : "=r"(p->regs.rbx));
__asm__ __volatile__ ("movq %%rcx, %0;" : "=r"(p->regs.rcx));
__asm__ __volatile__ ("movq %%rdx, %0;" : "=r"(p->regs.rdx));
__asm__ __volatile__ ("movq %%rbp, %0;" : "=r"(p->regs.rbp));
__asm__ __volatile__ ("movq %%r15, %0;" : "=r"(p->regs.r15));
__asm__ __volatile__ ("movq %%r14, %0;" : "=r"(p->regs.r14));
__asm__ __volatile__ ("movq %%r13, %0;" : "=r"(p->regs.r13));
__asm__ __volatile__ ("movq %%r12, %0;" : "=r"(p->regs.r12));
__asm__ __volatile__ ("movq %%r11, %0;" : "=r"(p->regs.r11));
__asm__ __volatile__ ("movq %%r10, %0;" : "=r"(p->regs.r10));
__asm__ __volatile__ ("movq %%r9, %0;" : "=r"(p->regs.r9));
__asm__ __volatile__ ("movq %%r8, %0;" : "=r"(p->regs.r8));

	fork();

		
	
}
else if(syscall_no==5)
{
	char *file=(char *)first;
	open(file,second);
}
else if(syscall_no==6)
{
	close (first);
}
else if(syscall_no==7)
{
	char *file=(char *)second;
	read(first,file,third);
}
else if(syscall_no==9)
{
	char *directory=(char *)first;
	opendir(directory);
}
else if(syscall_no==10)
{
	readdir(first);
}
else if(syscall_no==11)
{
	closedir(first);
}
else if(syscall_no==12)
{
        schedule();
}
else if(syscall_no==13)
{
	struct task_struct *pcb=getCurrentProcess();
        loadUser(pcb,(char *)first,(char **)second);
}
else if(syscall_no==14)
{
	char *file=(char *)first;
	char **argv=(char **)second;
	char **envp=(char **)third;
	execvpe(file,argv,envp);	
}
else if(syscall_no==15)
{
	//exit(0);	
}
else
{
	kprintf("other \n");
}

}
