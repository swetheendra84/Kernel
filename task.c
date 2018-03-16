#include <sys/task.h>
#include<sys/defs.h> 
#include<sys/kprintf.h>
#include<sys/page.h>
#include<sys/process.h>
static Task *runningTask;
static Task mainTask;
static Task otherTask;
void switchTask1(Registers *,Registers *);
void switchTask2(Registers *,Registers *);
int a=0;
void funMain() {

Task *last=runningTask->next;

while(1)
{
	kprintf("before running thread %d\n",a);
        switchTask1(&last->regs,&runningTask->regs);
        kprintf("after tunning thread \n");	
  	struct task_struct *pcb0=(struct task_struct *)kmalloc();
        createprocess(pcb0);

}

}

void otherMain() {
   createTask(&otherTask, funMain, mainTask.regs.eflags, (uint64_t *)mainTask.regs.cr3);
    mainTask.next = &otherTask;
    otherTask.next = &mainTask;
    runningTask= &mainTask;

   Task *last=runningTask->next;
   last->regs.rip=(uint64_t)funMain;
  
	while(1)
	{	
   
	kprintf("before running task %d\n",a);
	switchTask1(&runningTask->regs,&last->regs);	
	kprintf("after tunning task \n");
		 
	}
	

} 



void initTasking()
{
    __asm__ __volatile__("movq %%cr3, %%rax; movq %%rax, %0;":"=m"(mainTask.regs.cr3)::"%rax");
    __asm__ __volatile__("pushfq; movq (%%rsp), %%rax; movq %%rax, %0; popfq;":"=m"(mainTask.regs.eflags)::"%rax");
 
    createTask(&otherTask, otherMain, mainTask.regs.eflags, (uint64_t *)mainTask.regs.cr3);
    mainTask.next = &otherTask;
    otherTask.next = &mainTask;
 
    runningTask = &mainTask;
}
 
void createTask(Task *task, void (*main)(), uint64_t flags, uint64_t *pagedir) {
    task->regs.rax = 0;
    task->regs.rbx = 0;
    task->regs.rcx = 0;
    task->regs.rdx = 0;
    task->regs.rsi = 0;
    task->regs.rdi = 0;
    task->regs.eflags = flags;
    task->regs.rip =  (uint64_t)main;
    task->regs.cr3 = (uint64_t) pagedir;
    task->regs.rsp = (uint64_t) kmalloc()+0x1000;
    task->next = 0;
    kprintf("address of function %p",main);
}
 
void yield() {
	kprintf("inside yeild");
    initTasking();
    Task *last = runningTask;
    runningTask = runningTask->next;
     

    //kprintf("before switch task\n");
    switchTask1(&last->regs,&runningTask->regs);
    //kprintf("after switch task\n");
    }
	
