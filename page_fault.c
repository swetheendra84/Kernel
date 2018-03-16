#include<sys/kprintf.h>
#include<sys/util.h>
#include<sys/task.h>
#include<sys/util.h>
#include<sys/process.h>
#include<sys/page.h>
uint64_t base=0xffffffff80000000;
void page_fault(registers_t regs)
{
kprintf("page faultoocured \n");
struct task_struct *pcb=getCurrentProcess();
uint64_t faulting_address;
   __asm__ __volatile__("movq %%cr2, %0" : "=r" (faulting_address));

   
   int present   = !(regs.err_code & 0x1); 
   int rw = regs.err_code & 0x2;           
   int us = regs.err_code & 0x4;               
   int flag=0;
  
   kprintf("page fault address %x present %d r/w %d us/k %d",faulting_address,present,rw,us);
   kprintf("\n");

struct vma *vm=pcb->mm->mmap;


        if(us && present==0)
        {
	kmmap1(faulting_address,0x1000,7);		
        }
	
	else if(present)
   	{
	while(vm)
	{
	if(vm->type==0x1)
	{
		if(faulting_address>STACK_END && faulting_address<=STACK_START)
		{
			kprintf("Auto growing stack");
			kmmap(faulting_address-0x1000,0x1000,7,pcb->pmle);	
			pcb->mm->start_stack= pcb->mm->start_stack-0x1000;
			break;
		}
	
	
	}
	if(vm->type==0x2)
	{
		if((faulting_address >= HEAP_START) || (faulting_address <HEAP_END) )
		{	
			kprintf("HEAP");
			kmmap(faulting_address,0x1000,7,pcb->pmle);
			pcb->mm->startbrk=pcb->mm->startbrk+0x1000;
			break;
		}
		
	
		else
		{
			kprintf("VMA");
			if((faulting_address >= vm->start) && (faulting_address <vm->end) )
			{
			kmmap(faulting_address,(vm->end-vm->start),7,pcb->pmle);
				break;
			}
	
	
		} 	
	
	}

	vm=vm->next;
   }

if(flag)
{
kprintf("SEGMENTATION FAULT at %x",faulting_address);
//kill();
}

}}

