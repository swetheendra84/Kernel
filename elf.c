#include<sys/tarfs.h>
#include<sys/process.h>
#include<sys/util.h>
#include<sys/gdt.h>

void switchTask1(registers ,registers );
void contextswitch2(registers *,registers *);


static char args[10][100];
           
int test(Elf_hdr *hdr)
{
	if(hdr==NULL)
	{
		return(0);
	}
	if(hdr->e_ident[0] == 'E' && hdr->e_ident[0] == 'L' && hdr->e_ident[0] == 'F')
	{
		return(1);
	}
return(0);
}

uint64_t is_file_exists(char* filename)
{
    
    struct posix_header_ustar* test_tarfs = (struct posix_header_ustar *)&_binary_tarfs_start;
    int i = 1, temp = 512;
    uint64_t size;
    
    while(mystrlen(test_tarfs->name) != 0)
    {
        test_tarfs = (struct posix_header_ustar *)(&_binary_tarfs_start + temp);
        size = octtodec(strtoint(test_tarfs->size));
        
        if(mystrlen(test_tarfs->name) == 0)
            return 999;
        if(mstrcmp(test_tarfs->name, filename) == 0)
            return temp+512 ;
        if(size == 0)
            temp = temp + 512;
        else
            temp +=  (size%512==0) ? size + 512 : size + (512 - size%512) + 512;
       
        i += 1;
    }
    return 0;
}



void copyargs(struct task_struct *pcb,int argc)
{

	uint64_t *argv[10];
	uint64_t *temp=(uint64_t *)pcb->mm->start_stack;
	int i;
	int index;
	for(i=argc-1;i>=0;i--)
	{
		index=mystrlen(args[i])+1;
		temp = (uint64_t*)((void*)temp - index);
		mmemcpy((char *)temp,args[i],index);
		argv[i]=temp;

	}

	for(i=argc-1;i>=0;i--)
	{
		temp--;
		*temp=(uint64_t)argv[i];
	}

	temp--;
	*temp=(uint64_t)argc;
	
	pcb->mm->start_stack=(uint64_t)temp;

}


void loadUser(struct task_struct *pcb,char *file,char *argv[] )
{
	 uint64_t seg=is_file_exists(file);
	 if(seg==0||seg==999)
		seg=0;
	 Elf_hdr *elf=(Elf_hdr *)(&_binary_tarfs_start + seg);
	Elf64_Phdr *phdr,*limit;
	struct mm_struct *mm=pcb->mm;
	uint64_t start,end,size;
	uint64_t max=0;
	
	
	uint64_t str;
	__asm__ __volatile__ ("movq %%cr3, %0;" : "=r"(str));

	phdr=( Elf64_Phdr *)((uint8_t *)elf + elf->e_phoff);
	limit=phdr+elf->e_phnum;
	for(;phdr<limit;phdr++)
	{
		if((int)(phdr->p_type)==1)
		{
			start=phdr->p_vaddr;
			size=phdr->p_memsz;
			end=start+size;					

		
		
		if(max<end)
		max=end;

		__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(pcb->cr3));

		allocateUser(start,size,phdr->p_type,0,0,0);

		kmmap(start,size,7,pcb->pmle);


		mmemcpy((char *)phdr->p_vaddr,(void *)elf+phdr->p_offset,phdr->p_filesz);

		mmemset((void *)start+phdr->p_filesz,0,size-phdr->p_filesz);
	
		__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(str));

	}
	phdr=phdr+1;

}	

		size=0x1000;

		uint64_t heapstart=0x07000000;
		
			
		pcb->mm->startbrk=heapstart;
		pcb->mm->endbrk=heapstart+0x1000;
		

		 size=0x1000;
		   uint64_t stackstart=0xff003000;
		
		 
		mm->start_stack=stackstart-0x8;
							

		 __asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(pcb->cr3));

		 
		allocateUser(heapstart,size,7,0,2,0);

		

		allocateUser(stackstart,size,7,0,1,0);


		kmmap(stackstart-0x1000,size,7,pcb->pmle);			

		
		int argc=0;

		strcpy(args[argc],file);

		argc++;

		
		if(argv)
		{
			while(argv[argc-1])
			{
				strcpy(args[argc],argv[argc-1]);
				argc++;
			}
			
		}

		copyargs(pcb,argc);
		
		pcb->regs.rip=elf->e_entry;
		
		

}
	

void loadChild(struct task_struct *pcb,char *file,char *argv[] )
{
         uint64_t seg=is_file_exists(file);
         if(seg==0||seg==999)
                seg=0;
         Elf_hdr *elf=(Elf_hdr *)(&_binary_tarfs_start + seg);
        //struct mm_struct *mm=pcb->mm;
        Elf64_Phdr *phdr,*limit;
        uint64_t start,end,size;
        uint64_t max=0;


        uint64_t str;
        __asm__ __volatile__ ("movq %%cr3, %0;" : "=r"(str));

        phdr=( Elf64_Phdr *)((uint8_t *)elf + elf->e_phoff);
        limit=phdr+elf->e_phnum;
        for(;phdr<limit;phdr++)
        {
                if((int)(phdr->p_type)==1)
                {
                        start=phdr->p_vaddr;
                        size=phdr->p_memsz;
                        end=start+size;



                if(max<end)
                max=end;

                __asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(pcb->cr3));

                allocateUser(start,size,phdr->p_type,7,0,0);

                kmmap(start,size,7,pcb->pmle);


                mmemcpy((char *)phdr->p_vaddr,(void *)elf+phdr->p_offset,phdr->p_filesz);

                mmemset((void *)start+phdr->p_filesz,0,size-phdr->p_filesz);

                __asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(str));

        }
        phdr=phdr+1;

}

pcb->regs.rip=elf->e_entry;

}

void schedule()
{
struct task_struct *previous=startq->process;
startq=startq->next;
struct task_struct *current=startq->process;
__asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(current->cr3));
set_tss_rsp((uint64_t *)current->kstack);

if(previous!=current)
{
contextswitch2(&previous->regs,&current->regs);
}

}
          

void switchtoRing3(struct task_struct *pcb)
{	
		uint64_t tos=pcb->mm->start_stack;

		
						
	      __asm__ __volatile__(

     "mov $0x23,%ax ;"
     "mov %ax,%ds ;"
     "mov %ax,%es ;"
     "mov %ax,%fs ;"
     "mov %ax,%gs ;"
);


	__asm__ __volatile__ ("pushq $0x23 ;");

      __asm__ __volatile__ ("movq %0, %%rax;" :: "r"(tos));



	__asm__ __volatile__(
     "pushq %rax;"
     "pushfq ;"
     "push $0x2B"
);

	       
		 __asm__ __volatile__ ("movq %0, %%rax;" :: "r"(pcb->regs.rip));



                __asm__ __volatile__ ("pushq %rax;" );
              

        __asm__  __volatile__ ("sti");

      set_tss_rsp((uint64_t *)pcb->kstack);


        __asm__ __volatile__("iretq");

			
		
}




