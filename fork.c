#include<sys/util.h>
#include<sys/process.h>
#include<sys/tarfs.h>
#include<sys/page.h>
#include<sys/gdt.h>
#include<sys/kprintf.h>
uint64_t ceil1(uint64_t number)
{
  if(number %0x1000 !=0)
        {
                number=(number)+0x1000-(number%0x1000);
        }
return(number);
}

struct task_struct *clone(struct task_struct *parent)
	{
	
	struct task_struct *child=NULL;
	child=(struct task_struct *)kmalloc_ap(child);
	child->pid=2;
	createchildtable(parent,child);

	__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(child->cr3));

	child->mm=(struct mm_struct *)kmalloc_ap(child);

	mmemcpy(&(child->fd[0]),&(parent->fd[0]),(sizeof(parent->fd[0])*50));	

	mmemcpy(child->mm,parent->mm,sizeof(struct mm_struct));

	struct vma *pvm = parent->mm->mmap;
	
	struct vma *cvm=NULL;	


	while(pvm)

	{
	cvm=(struct vma *)kmalloc_ap(child);
        cvm->mm=pvm->mm;
	cvm->start=pvm->start;
	cvm->end=pvm->end;
	cvm->next=pvm->next;
	cvm->flags=pvm->flags;
	cvm->type=pvm->type;
	cvm->prot=pvm->prot;
	cvm->phys=pvm->phys;
 	pvm=pvm->next;	
	}
	                
		
	return(child);

}


int fork()
{

	struct task_struct *parent=getCurrentProcess();

	struct task_struct *child=clone(parent);
	
	
	
	struct processqueue *temp;

	temp=startq->next;
	
	startq->next=(struct processqueue *)kmalloc_ap(child);

	startq->process=parent;
	
	startq->next->process=child;
	
	startq->next->next=temp;

	volatile uint64_t tos;

	child->stack=kmalloc_ap(child);	

	child->kstack=(uint64_t)(child->stack)+0x1000-8;
	 mmemcpy((uint64_t *)(child->kstack-0x1000+8),(uint64_t *)(parent->kstack-0x1000+8),0x1000-0x8);
	              
	child->regs.cr3=child->cr3;

	child->regs.rbx=parent->regs.rbx;
	child->regs.rcx=parent->regs.rcx;
	child->regs.rdx=parent->regs.rdx;
	child->regs.rbp=parent->regs.rdx;
	child->regs.r15=parent->regs.r15;
	child->regs.r14=parent->regs.r14;
	child->regs.r13=parent->regs.r13;
	child->regs.r12=parent->regs.r12;	
	child->regs.r11=parent->regs.r11;
	child->regs.r10=parent->regs.r10;
	child->regs.r9=parent->regs.r9;
	child->regs.r8=parent->regs.r8;
 	

	__asm__ __volatile__("movq %0, %%cr3"
                                     :
                                     : "r" (parent->cr3)
                                     : "memory","cc"
                                     );


	
         __asm__ __volatile__ ("movq %%rsp , %0;"
                                        : "=r"(tos)
                                        :
                                        :
        );



	            __asm__ __volatile__(
                "movq $2f, %0;"
                "2:\t"
                :"=g"(child->regs.rip)
        );


      

	struct task_struct *curr=getCurrentProcess();
	if(curr==parent)
	{
		child->kstack=child->kstack-(parent->kstack-tos);

		child->regs.rsp=child->kstack;

		return(child->pid);
	}
	
	else
	{
		return(0);

	}


}

















