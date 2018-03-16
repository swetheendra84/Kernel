#include<sys/page.h>
#include<sys/process.h>
#include<sys/kprintf.h>
#include<sys/util.h>
#include<sys/task.h>
#include<sys/gdt.h>
#include<unistd.h>
#include<sys/tarfs.h>
uint64_t kern= 0xffffffff80000000;
uint64_t ceil(uint64_t number)
{
  if(number %0x1000 !=0)
	{
		number=(number)+0x1000-(number%0x1000);
	}
return(number);
}

uint64_t floor(uint64_t number)
{
  if(number %0x1000 !=0)
        {
                number=(number)-(number%0x1000);
        }
return(number);
}

 void mapprocess(uint64_t virtual,uint64_t physical,uint64_t arg)
        {
          


                        int pt=(((virtual) >> 12) & 0x1ff);
                        int tid=((((uint64_t)virtual) >> 21) & 0x1ff);
                        int pdid=((((uint64_t)virtual) >> 30) & 0x1ff);
                        int pmid=((((uint64_t)virtual) >> 39) & 0x1ff);

                uint64_t *pml=(uint64_t *)arg;
                uint64_t *pdpt;
                uint64_t *pde;
                uint64_t *pte;

                        if(pml[pmid] )
                         {
                                pdpt = (uint64_t *)(floor(pml[pmid])+kern);
                        }
                        else
                        {
                        pdpt=kmalloc();
                         uint64_t phypdpte_p=virttophys(pdpt);
                        uint64_t p1=(uint64_t)phypdpte_p;
                        p1=p1|0x7;
                        *(pml+pmid)=p1;

                }


        if(pdpt[pdid])
        {
                pde=(uint64_t *)(floor(pdpt[pdid])+kern);
        }
        else
        {
        pde=kmalloc();
        uint64_t phypde_p=virttophys(pde);
        uint64_t p2=(uint64_t)phypde_p;
        p2=p2|0x7;
	 *(pdpt+pdid)=p2;
        }


        if(pde[tid])
        {
                pte=(uint64_t *)(floor(pde[tid])+kern);
            

        }
        else
        {
        pte=kmalloc();
        uint64_t phypte_p=virttophys(pte);
        uint64_t p3=(uint64_t)phypte_p;
        p3=p3|0x7;
        *(pde+tid)=p3;
        }

	if(pte[pt]==0)
        *(pte+pt)=physical;
     
        }
	

int i,j,k,l;
int usermode(int fd,void *buf,int size);
void flush();
void sample(uint8_t * , void (*)() );
void *addr=NULL;
int length=100;
int prot=3;
int flags=34;
int fd1=-1;
char *str="hello world";
void *ref;
uint64_t res,res1;
int val=3;
int id[1000]={0};
uint64_t *temp1,*temp2;
void processEntry()
{

uint64_t *ptr;


 __asm__ volatile ( "int %1"
                    :"=a"(res)
                    :"i"(0x80),"0"(2),"b"(NULL),"c"(6000),"d"(prot),"r"(flags)
                    :
);


ptr=(uint64_t *)res;
*ptr=10;
val=*ptr;

kprintf("pointer and value %p and %x \n",ptr,*ptr);


 


 __asm__ volatile ( "int %1"
                    :"=a"(res1)
                    :"i"(0x80),"0"(3),"b"(0),"c"(6000),"d"(prot)
                    :
);


*ptr=20;
kprintf("pointer and value %p and %x \n",ptr,*ptr);
                 

while(1)
{   
}

}


int assignpid()
{
	int i;
	for(i=0;i<1000;i++)
	{
	if(id[i]==0)
	{
	id[i]=1;
	return(i);
	}
	}
return(-1);
}

uint64_t virttophys(uint64_t * virt)
{
	return((uint64_t)virt-kern);
}

void memcpy(void *dest, void *src, size_t n)
{
   
   char *csrc = (char *)src;
   char *cdest = (char *)dest;
 
   for (int i=0; i<n; i++)
       cdest[i] = csrc[i];
}




struct task_struct *getCurrentProcess()
{
   	return (startq->process);
}

uint64_t *getpdptentry(uint64_t addr,int pmid)
{
	uint64_t *ad=(uint64_t *)addr;
	uint64_t *ret=(uint64_t *)(floor(ad[pmid]+kern)); 
	return(ret);
}

uint64_t *getpdeentry(uint64_t *addr,int pdid)
{
	 uint64_t *ret=(uint64_t *)(floor(addr[pdid]+kern));
	 return(ret);
}

uint64_t *getpteentry(uint64_t *addr,int tid)
{
       uint64_t * ret=(uint64_t *)(floor(addr[tid]+kern));
	return(ret);
}

void createchildtable(struct task_struct *parent,struct task_struct *child)
{

	temp1=kmalloc_ap(child);
	temp2=kmalloc_ap(child);
	uint64_t *pmle_c=kmalloc_ap(child);
	uint64_t *ppmle=(uint64_t *)parent->pmle;
	

	memset(pmle_c,0,0x1000);

	uint64_t page=0;
	for(i=0;i<511;i++)
	{
		uint64_t pmlentry=ppmle[i];
		if(pmlentry &0x1)
		{	
			if(pmle_c[i]==0)
			{
			temp1=kmalloc();
			pmle_c[i]=virttophys(temp1)|0x7;
			}
			uint64_t *pdpt_p=getpdptentry(parent->pmle,i);

			for(j=0;j<512;j++)
			{				
				if(pdpt_p[j] &0x1)	
				{
					temp1=getpdptentry((uint64_t)pmle_c,i);
					if(temp1[j]==0)
					{					
					temp2=kmalloc();
					temp1[j]=virttophys(temp2)|0x7;				
					}
					 uint64_t *pdeent=getpdeentry(pdpt_p,j);
					for(k=0;k<512;k++)
					{
						if(pdeent[k] &0x1)
						{
							temp2=getpdeentry(temp1,j);
							
							if(temp2[k]==0)
							{
							uint64_t *pte_c=kmalloc();
							temp2[k]=virttophys(pte_c)|0x7;
							}
							 
							uint64_t *pte_p=getpteentry(pdeent,k);

							for(l=0;l<512;l++)
							{
								if(pte_p[l] &0x1)
								{
									uint64_t *pte_c=getpteentry(temp2,k);
									uint64_t temp=~(0x1<<1);
									page=pte_p[l];
									if(page &0x1)
									{
									uint64_t clean=page&temp;
									pte_c[l]=clean|0x1|0x9|0x4;
									pte_p[l]=pte_p[l] & temp;
									pte_p[l]=pte_p[l] | 0x1|0x4|0x9;									
									}
								}
							}
				

						}
					}
				}
			}				
		
		}

	}

	
		
                pmle_c[511]=ppmle[511]|0x7;
	
                
		child->pmle= (uint64_t)pmle_c;
		child->cr3 = virttophys(pmle_c);
       
}

void createpagetable(struct task_struct *pcb)
{


	uint64_t *pmle_2=kmalloc_ap(pcb);
        uint64_t *pdpte_2=kmalloc_ap(pcb);
        uint64_t *pde_2=kmalloc_ap(pcb);
        uint64_t *pte_2=kmalloc_ap(pcb);

      
        memset(pmle_2,0,0x1000);
        memset(pdpte_2,0,0x1000);
        memset(pde_2,0,0x1000);
        memset(pte_2,0,0x1000);

        uint64_t phypmle_p=virttophys(pmle_2);
	
 
	
		pmle_2[511]=pmle_pp[511]|0x7;
		pdpte_2[510]=pdpte_pp[510]|0x7;
		pde_2[1]=pde_pp[1]|0x7;
	

	uint64_t virtual=phybas;
	uint64_t addr=0xffffffff80200000;
	
        while(virtual<phyfree)
        {
	
	uint64_t pt=(((addr) >> 12) & 0x1ff);
	uint64_t value=virtual;
        value=value|0x7;
        *(pte_2 +pt)=value;
        addr=addr+0x1000;
        virtual=virtual+0x1000;
	}	
	
	pcb->pmle=(uint64_t)pmle_2;
	pcb->cr3=phypmle_p;

}

uint64_t *kmalloc_ap(struct task_struct *pcb)
{
        uint64_t startad=firstframe();
        uint64_t saddr=(startad*0x1000);
        uint64_t physadd=saddr+free;
        setpageframe(saddr);
        saddr=saddr+kern+free;
        int pt=(((saddr) >> 12) & 0x1ff);
        uint64_t value=physadd;
        value=value|0x7;
        *(pte_pp +pt)=value;
        uint64_t *temp=(uint64_t *)(saddr);
        
        return(temp);
}





void clear(uint64_t first,uint64_t pml)
{
uint64_t frameaddr=(uint64_t)first-kern-free;
        clearframe(frameaddr);
                int pt=(((first) >> 12) & 0x1ff);
        int tid=((((uint64_t)first) >> 21) & 0x1ff);
        int pdid=((((uint64_t)first) >> 30) & 0x1ff);
        int pmid=((((uint64_t)first) >> 39) & 0x1ff);
        uint64_t *pmle=(uint64_t *)pml;
        uint64_t *pdpt=(uint64_t *)(floor(pmle[pmid]+kern));
        uint64_t *pde=(uint64_t *)(floor(pdpt[pdid]+kern));
        uint64_t *pte=(uint64_t *)(floor(pde[tid]+kern));
        pte[pt]=0;


}

                              
int execvpe(char *filename, char *argv[],char *envp[])
{
        //uint64_t str;
        if(is_file_exists(filename)==0)
        {
                return(-1);
        }

        struct task_struct *task=(struct task_struct *)kmalloc();
        struct task_struct *curr=getCurrentProcess();
        task->pid=curr->pid;
        task->ppid=curr->pid;
        mmemcpy(&(task->fd[0]),&(curr->fd[0]),(sizeof(curr->fd[0])* 50));

        uint64_t *pmle=kmalloc();
        uint64_t *pmle2=(uint64_t *)curr->pmle;
        pmle[511]=pmle2[511];
        task->pmle=(uint64_t)pmle;
        task->cr3=virttophys((uint64_t *)task->pmle);


        task->stack=kmalloc();
        task->kstack=(uint64_t)(task->stack)+0x1000-0x8;
        task->mm=(struct mm_struct *)kmalloc();
        loadUser(task,filename,argv);


        struct vma *tem=curr->mm->mmap;
        while(tem!=NULL)
        {
                clear((uint64_t)tem,curr->pmle);
                tem=tem->next;
        }
        clear((uint64_t)curr->mm,curr->pmle);

        removept(curr->pmle);



        struct processqueue *reference=startq;

        if(reference->process==startq->process)
        {
                reference->process=task;
                startq=reference;
        }
        else
        {
        while(reference->next->process!=startq->process)
        {
                reference=reference->next;
        }


        struct processqueue *temp=reference->next->next;
        reference->next=(struct processqueue *)kmalloc();
        reference->next->process=task;
        reference->next->next=temp;
        startq=reference->next;

        }


        task->state=RUNNING;
        task->waitpid=-1;
        switchtoRing3(task);
        return(-1);


}


void kmmap(uint64_t virtual,int length,int flags,uint64_t pmel)
{
        //struct task_struct *pcb=getCurrentProcess();
	uint64_t m=floor(virtual);
	uint64_t n=ceil(virtual+length);
	int no=(n-m)/0x1000;
	while(no)
	{
	virtual=m;
        uint64_t startad=firstframe();
        uint64_t saddr=(startad*0x1000);
        uint64_t physadd=saddr+free;
        setpageframe(saddr);
        int pt=(((virtual) >> 12) & 0x1ff);
	uint64_t value=physadd;
        value=value|0x7;
	
	uint64_t *pdpt=NULL;
	 uint64_t *pde=NULL;	
	uint64_t *pte=NULL;

	int tid=((((uint64_t)virtual) >> 21) & 0x1ff);
        int pdid=((((uint64_t)virtual) >> 30) & 0x1ff);
        int pmid=((((uint64_t)virtual) >> 39) & 0x1ff);

	uint64_t *pmle_2 =(uint64_t *)pmel;
	if(pmle_2[pmid] )
	{
		pdpt = (uint64_t *)(floor(pmle_2[pmid]+kern));
	}	
        else
        {
        pdpt=kmalloc();
         uint64_t phypdpte_p=virttophys(pdpt);
        uint64_t p1=(uint64_t)phypdpte_p;
        p1=p1|0x7;
        *(pmle_2+pmid)=p1;
     	}


	if(pdpt[pdid])
        {
                pde=(uint64_t *)(floor(pdpt[pdid]+kern));
        }
        else
        {
        pde=kmalloc();
        uint64_t phypde_p=virttophys(pde);
        uint64_t p2=(uint64_t)phypde_p;
        p2=p2|0x7;
        *(pdpt+pdid)=p2;
        }


	if(pde[tid])
        {
                pte=(uint64_t *)(floor(pde[tid]+kern));
        }
        else
        {
        pte=kmalloc();
        uint64_t phypte_p=virttophys(pte);
        uint64_t p3=(uint64_t)phypte_p;
        p3=p3|0x7;
        *(pde+tid)=p3;
        }

        *(pte+pt)=value;
	virtual=virtual+0x1000;	
	no--;
        }
	
}

 

void kmmap1(uint64_t virtual,int length,int flags)
{
        //struct task_struct *pcb=getCurrentProcess();
        uint64_t m=floor(virtual);
        uint64_t n=ceil(virtual+length);
        int no=(n-m)/0x1000;
        while(no)
        {
        virtual=m;
        uint64_t startad=firstframe();
        uint64_t saddr=(startad*0x1000);
        uint64_t physadd=saddr+free;
        setpageframe(saddr);
        int pt=(((virtual) >> 12) & 0x1ff);
        uint64_t value=physadd;
        value=value|0x7;

        uint64_t *pdpt=NULL;
         uint64_t *pde=NULL;
        uint64_t *pte=NULL;

        int tid=((((uint64_t)virtual) >> 21) & 0x1ff);
        int pdid=((((uint64_t)virtual) >> 30) & 0x1ff);
        int pmid=((((uint64_t)virtual) >> 39) & 0x1ff);

	struct task_struct *pcb=getCurrentProcess();
	uint64_t *pml=(uint64_t *)pcb->pmle;
        if(pml[pmid] )
        {
                pdpt = (uint64_t *)(floor(pml[pmid]+kern));
        }
        else
        {
        pdpt=kmalloc();
         uint64_t phypdpte_p=virttophys(pdpt);
        uint64_t p1=(uint64_t)phypdpte_p;
        p1=p1|0x7;
        *(pml+pmid)=p1;
        }


        if(pdpt[pdid])
        {
	                pde=(uint64_t *)(floor(pdpt[pdid]+kern));
        }
        else
        {
        pde=kmalloc();
        uint64_t phypde_p=virttophys(pde);
        uint64_t p2=(uint64_t)phypde_p;
        p2=p2|0x7;
        *(pdpt+pdid)=p2;
        }


        if(pde[tid])
        {
                pte=(uint64_t *)(floor(pde[tid]+kern));
        }
        else
        {
        pte=kmalloc();
        uint64_t phypte_p=virttophys(pte);
        uint64_t p3=(uint64_t)phypte_p;
        p3=p3|0x7;
        *(pde+tid)=p3;
        }

        *(pte+pt)=value;
        virtual=virtual+0x1000;
        no--;
        }

}



void initialize(struct task_struct *pcb)
{
 
	createpagetable(pcb);      
    	pcb->mm=(struct mm_struct *)kmalloc();
	pcb->pid=1;
	pcb->stack=kmalloc();
	pcb->kstack=(uint64_t)pcb->stack+0x1000-0x8;
	ref=(void *)(pcb->kstack);
	pcb->regs.rax = 0;
   	pcb->regs.rbx = 0;
    	pcb->regs.rcx = 0;
    	pcb->regs.rdx = 0;
    	pcb->regs.rsi = 0;
    	pcb->regs.rdi = 0;
    	__asm__ __volatile__("pushfq; movq (%%rsp), %%rax; movq %%rax, %0; popfq;":"=m"(pcb->regs.eflags)::"%rax");
	    	
}


struct vma *temp;

uint64_t allocateUser(uint64_t first,uint64_t second,uint64_t third,uint64_t fourth,uint64_t fifth,uint64_t sixth)
{

 struct task_struct *pcb=getCurrentProcess();
        
       
	temp=pcb->mm->mmap;

	struct vma  *temp1=NULL;
 
	if(first!=0)
	{
	if(temp==NULL)
	{
		pcb->mm->mmap=(struct vma *)kmalloc();
		pcb->mm->mmap->start=first;
		pcb->mm->mmap->end=first+second;
		pcb->mm->mmap->prot=third;
                pcb->mm->mmap->flags=fourth;
		pcb->mm->mmap->type=fifth;
		pcb->mm->mmap->next=NULL;
		pcb->mm->current=(struct vma *)kmalloc();
		pcb->mm->current=pcb->mm->mmap;
		return first;

	}
	
	if(temp->next==NULL)
	{
	        pcb->mm->mmap->next=(struct vma *)kmalloc();
                pcb->mm->mmap->next->start=first;
                pcb->mm->mmap->next->end=first+second;
                pcb->mm->mmap->next->prot=third;
                pcb->mm->mmap->next->flags=fourth;
                pcb->mm->mmap->next->type=fifth;
                pcb->mm->mmap->next->next=NULL;
                return first;

	}	
	
	while(temp!=NULL)
	{
		
			
		
		if(temp->start==first)
		{
			temp1=temp->next;
			uint64_t addr=temp->end;
			temp->end=first+second;
			temp->next=(struct vma *)kmalloc();
			temp->next->start=first+second;
			temp->next->end=addr;
			temp->next->next=temp1;
			temp->next->prot=third;
			temp->next->flags=fourth;
			temp->next->type=fifth;
			break;
		}

		 if(temp==pcb->mm->current && temp->end<=first)
                {
                        if(pcb->mm->current->next!=NULL)
                                pcb->mm->current=pcb->mm->current->next;
                        pcb->mm->current->next=(struct vma *)kmalloc();
                        pcb->mm->current=pcb->mm->current->next;
                        pcb->mm->current->start=first;
                        pcb->mm->current->end=first+second;
                        pcb->mm->current->prot=third;
                        pcb->mm->current->flags=fourth;
                        pcb->mm->current->type=fifth;
                        pcb->mm->current->next=NULL;
                        break;

                }


		else if((temp->start<first)&&(first<temp->end)&&((first+second)==temp->end))
		{
			uint64_t addr=temp->end;
			temp->end=first;
			temp1=temp->next;
			temp->next=(struct vma *)kmalloc();
			temp->next->start=first;
			temp->next->end=addr;
			temp->next->prot=third;
                        temp->next->flags=fourth;
			temp->next->type=fifth;
			temp->next->next=temp1;
			break;
		}

		else if((temp->start<first)&&(first<temp->end)&&((first+second)>temp->end))
                {
                        //uint64_t addr=temp->end;
                        temp->end=first;
                        temp1=temp->next;
                        temp->next=(struct vma *)kmalloc();
                        temp->next->start=first;
                        temp->next->end=(first+second);
			temp->next->prot=third;
                        temp->next->flags=fourth;
			temp->next->type=fifth;
                        temp->next->next=temp1;
                        break;
                }

		else if((temp->start<first)&&(first<temp->end)&& (temp->end >(first+second)))
		{
			if(temp->type==third && temp->flags==fourth)// permissions
			{
				temp1=temp->next;
				temp->next=(struct vma *)kmalloc();
				temp->next->start=first;
				temp->next->end=first+second;
			        temp->next->prot=third;
                        	temp->next->flags=fourth;
				temp->next->type=fifth;
				temp->next->next=temp1;
				break;
			}
			else
			{
				uint64_t addr=temp->end;
				temp->end=first;
				temp1=temp->next;
				temp->next=(struct vma *)kmalloc();
				temp->next->start=first;
				temp->next->prot=third;
                        	temp->next->flags=fourth;
				temp->next->type=fifth;
				temp->next->end=first+second;
				temp->next->next=(struct vma *)kmalloc();		
				temp->next->next->start=first+second;
				 temp->next->next->end=addr;
	                       temp->next->next->prot=third;
	                        temp->next->next->flags=fourth;
				temp->next->next->type=fifth;
				 temp->next->next->next=temp1;
				break;
			}
		

		}

		if(temp->next!=NULL)
		{	
		if((temp->end < first) && (first < temp->next->start) && ((temp->next->start)< (first+second)))
                {
	
                        temp1=(struct vma *)kmalloc();
                        temp1->start=first;
                        temp1->end=temp->next->start;
                        temp1->prot=third;
                        temp1->flags=fourth;
                        temp->type=fifth;
                        temp1->next=temp->next;
                        temp->next=temp1;
                        break;
                }

		}

		if(temp->next!=NULL)
		{
		if((temp->end < first) && (first < temp->next->start) && ((temp->next->start) > (first+second)))
		{	
			temp1=(struct vma *)kmalloc();
			temp1->start=first;
			temp1->end=first+second;
			temp1->prot=third;
                        temp1->flags=fourth;
			temp->type=fifth;
			temp1->next=temp->next;
                        temp->next=temp1;
			break;
		}

		}
		if(temp==pcb->mm->current && temp->end<=first)
                {
			if(pcb->mm->current->next!=NULL)
				pcb->mm->current=pcb->mm->current->next;
                        pcb->mm->current->next=(struct vma *)kmalloc();
                        pcb->mm->current=pcb->mm->current->next;
                        pcb->mm->current->start=first;
                        pcb->mm->current->end=first+second;
			pcb->mm->current->prot=third;
                        pcb->mm->current->flags=fourth;
			pcb->mm->current->type=fifth;
                        pcb->mm->current->next=NULL;
			break;

                }
		        
                        temp=temp->next;
	}

	
		
	while(1)
	{
	if(pcb->mm->current->next!=NULL)
	{
	 	pcb->mm->current=pcb->mm->current->next;
	}
	else
	{
		break;
	}
	}
	
	return(first);
	}
	else
	{
        if(pcb->mm->mmap==NULL)
        {
                pcb->mm->mmap=(struct vma *)kmalloc();
                pcb->mm->mmap->start=HEAP_START;
                pcb->mm->mmap->end=pcb->mm->mmap->start+second;
		pcb->mm->mmap->prot=third;
		pcb->mm->mmap->flags=fourth;
		pcb->mm->mmap->type=fifth;
                pcb->mm->current=pcb->mm->mmap;
		return(pcb->mm->current->start);
        }

	struct vma *mmap=pcb->mm->mmap;

        if(mmap->next==NULL)
        {
		
                mmap->next=(struct vma *)kmalloc();
                mmap->next->start=mmap->end;
                mmap->next->end=mmap->end+second;
		 mmap->next->prot=third;
		 mmap->next->flags=fourth;
		mmap->next->type=fifth;
                pcb->mm->current=mmap->next;
                pcb->mm->current->next=NULL;
		return(pcb->mm->current->start);
		
        }

	struct vma *temp= pcb->mm->mmap;
	
   	while(temp->next->start!=pcb->mm->startbrk)
	{
		temp=temp->next;
	}	
	struct vma *temp1=temp->next;

				
		if((pcb->mm->startbrk>pcb->mm->endbrk) && (pcb->mm->endbrk ==HEAP_END))
		{
		return(HEAP_END);		
		}
		struct vma *tem1=(struct vma *)kmalloc();
		tem1->start=pcb->mm->startbrk;
		tem1->end=tem1->start+second;
		tem1->prot=third;
                tem1->flags=fourth;
                tem1->type=fifth;
		temp1->start=tem1->end;
		tem1->next=temp1;
		temp->next=tem1;
		pcb->mm->startbrk=pcb->mm->startbrk+second;
		return(tem1->start);
		}
	
		
 	}	



void deleteVma(struct vma *base)
{
        uint64_t frameaddr=(uint64_t)base-kern-free;
        clearframe(frameaddr);
        uint64_t first=base->start;
        uint64_t second=base->end;
        int x=second-first;
        int result;
        if(x%0x1000!=0)
        {
        result=x/0x1000+1;
        }
        else
        {
        result=x/0x1000;
        }
        while(result!=0)
        {
        int pt=(((first) >> 12) & 0x1ff);
        int tid=((((uint64_t)first) >> 21) & 0x1ff);
        int pdid=((((uint64_t)first) >> 30) & 0x1ff);
        int pmid=((((uint64_t)first) >> 39) & 0x1ff);
        struct task_struct *pcb=getCurrentProcess();
        uint64_t *pmle=(uint64_t *)pcb->pmle;
        uint64_t *pdpt=(uint64_t *)(floor(pmle[pmid]+kern));
        uint64_t *pde=(uint64_t *)(floor(pdpt[pdid]+kern));
        uint64_t *pte=(uint64_t *)(floor(pde[tid]+kern));
        pte[pt]=0;
        result--;
        first=first+0x1000;
        }
}





void removept(uint64_t pmle)
{
uint64_t *ppmle=(uint64_t *)pmle;
 for(i=0;i<511;i++)
        {
                if(ppmle[i] &0x1)
                {
                        uint64_t *pdpte_p=getpdptentry(pmle,i);
                        for(j=0;j<512;j++)
                        {
                                if(pdpte_p[j] &0x1)
                                {

                                        uint64_t *pde_p=getpdeentry(pdpte_p,j);
                                        for(k=0;k<512;k++)
                                        {
                                                if(pde_p[k] &0x1)
                                                {
                                                         uint64_t *pte_p=getpteentry(pde_p,k);
                                                        for(l=0;l<512;l++)
                                                        {
                                                                if(pte_p[l] &0x1)
                                                                {

                                                                        uint64_t page=pte_p[l];
                                                                        uint64_t frameaddr=page-free;
                                                                        clearframe(frameaddr);
                                                                        pte_p[l]=0;
                                                                                                                                                }
                                                                }
                                                        }


                                                }
                                        }
                                }
                        }

		}


	}


struct task_struct *pcb;
void deallocateUser(uint64_t first,uint64_t second)
{
//one case is missing where memory is beyond vma size
	pcb=getCurrentProcess();
        //struct mm_struct *mm=pcb->mm;
	struct vma *temp,*temp1,*temp2;
	temp=pcb->mm->mmap;
	temp1=NULL;
	while(temp!=pcb->mm->current->next)
	{
		if(temp->start==first)
		{
			if((temp->end - temp->start==second)||(second==0x0))
			{
			if(temp1==NULL)
			{
				pcb->mm->mmap=temp->next;
			}
			else
			{
				temp1->next=temp->next;
			}
			deleteVma(temp);//Risk of callimg it when size is not page size
			break;
			}
			else if(temp->end - temp->start > second)
			{
				temp->start=temp->start+second;
				break;
			}
		
		}
		else if((temp->start<first) && (first < temp->end))
			{
			uint64_t addr=temp->end;
			temp->end=first;
			if(temp->end > second)
			{
				temp2=temp->next;
				temp->next=(struct vma *)kmalloc();
				temp->next->start=temp->start+second;
				temp->next->end=addr;
				temp->next->next=temp2;
				break;
			}
		}
		temp1=temp;
		temp=temp->next;
	}



}


void createprocess(struct task_struct *pcb)
{
	
	initialize(pcb);
	
	if(current==NULL)
	{
	startq=(struct processqueue *)kmalloc();
        startq->process=pcb;
        startq->next=startq;

	}
	else
	{
	reference=current->next;
	current->next=(struct processqueue *)kmalloc();
	current=current->next;
	current->process=pcb;
	current->next=reference;
	}
	
        int i;
        for(i=0;i<50;i++)
        {
               pcb->fd[i]=NULL;
        }

        tarfsinit();


	 char *entry="bin/cat";

	char *argv[]={"HELLO",NULL};

        load(pcb,entry,argv);
	
	
}
 

void load(struct task_struct *pcb,char *file,char *argv[] )
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


                mm->startbrk=heapstart;
		mm->endbrk=heapstart+0x1000;


                 size=0x1000;
                   uint64_t stackstart=0xff003000;


                mm->start_stack=stackstart-0x8;


                 __asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(pcb->cr3));


                allocateUser(heapstart,size,7,0,2,0);



                allocateUser(stackstart,size,7,0,1,0);


                kmmap(stackstart-0x1000,size,7,pcb->pmle);


		 pcb->regs.rip=elf->e_entry;

	
		switchtoRing3(pcb);

	}	

		
               
                

