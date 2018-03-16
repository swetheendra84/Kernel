#include<sys/page.h>
#include<sys/kprintf.h>
#include<sys/task.h>
#include<sys/process.h>
#include<sys/util.h>
#include<sys/tarfs.h>
#define index(a) (a/(8*8))
#define offset(a) (a%(8*8))
uint64_t kernstart;

uint64_t kern1= 0xffffffff80000000;
void *memset(void *dest,int c,size_t n)
{
	if(n)
	{
		do
		{	
			*(char *)dest++=c;
			n--;
		}
		while(n);
	}	
	return(dest);
}


uint64_t placement;
uint64_t *frame;
uint64_t nframes;
uint64_t pid;
int frames;
uint64_t *extra;
uint64_t addr;
uint64_t addr1;
char *video=(char *)0x0;
char *video1=(char *)0x0;
char *videomem;

uint64_t *getextraallocated()
{
	uint64_t *base;
	base=extra;
	uint64_t check=(uint64_t)extra;
	if(check>=placement)
		return NULL;
	check=check+0x1000;
	extra=(uint64_t *)check;
	return(base);
}

uint64_t firstframe()
{
   uint64_t i, j;
   for(i=0;i<frames; i++)
   {

           for (j = 0; j < 64; j++)
           {
               uint64_t toTest = 0x1 << j;
               if ( !(frame1[i]&toTest) )
               {
                   return i*8*8+j;
               }
           }
   }
kprintf("No FREE FRAMES FOUND");
return(0);
}

void setpageframe(uint64_t frameaddr)
{
   uint64_t fra = frameaddr/0x1000;
   uint64_t idx = index(fra);
   uint64_t off = offset(fra);
   frame1[idx] |= (0x1 << off);
}


void clearframe(uint64_t frameaddr)
{
   uint64_t fra = frameaddr/0x1000;
   uint64_t idx = index(fra);
   uint64_t off = offset(fra);
   frame1[idx] |= ~(0x1 << off);
}


uint64_t *kmalloc()
{
	uint64_t startad=firstframe();
	uint64_t saddr=(startad*0x1000);
	uint64_t physadd=saddr+free;
	setpageframe(saddr);
	saddr=saddr+kern1+free;
	int pt=(((saddr) >> 12) & 0x1ff);
	uint64_t value=physadd;
	value=value|0x3;
	*(pte_pp +pt)=value;
	uint64_t *temp=(uint64_t *)(saddr);
	placement=placement+0x1000;
	return(temp);
} 

//returns first free page frame
uint64_t *alloc(uint64_t size)
{
	uint64_t startad=freeframe();
	uint64_t saddr=(startad*0x1000);
	setframe(saddr);
	saddr=saddr+free;
	uint64_t *temp=(uint64_t *)saddr;
	return(temp);
}


void setframe(uint64_t frameaddr)
{
   uint64_t fra = frameaddr/0x1000;
   uint64_t idx = index(fra);
   uint64_t off = offset(fra);
   frame[idx] |= (0x1 << off);
}




uint64_t freeframe()
{
   uint64_t i, j;
   for(i=0;i<frames; i++)
   {
        
           for (j = 0; j < 64; j++)
           {
               uint64_t toTest = 0x1 << j;
               if ( !(frame[i]&toTest) )
               {
                   return i*8*8+j;
               }
           }
   }
kprintf("No FREE FRAMES FOUND");
return(0);
}

void map(uint64_t virtual)
{
	int pt=(((virtual) >> 12) & 0x1ff);
	uint64_t value=addr;
	value=value|0x3;
	*(pte_p +pt)=value;
	addr=addr+0x1000;
}


void map1(uint64_t virt)
{
	int pt=((virt >>12)& 0x1ff);
	uint64_t value=0xb8001;
	value=value|0x3;
	*(pte_p+pt)=value;
}

uint64_t *allocateadditional(uint64_t max)
{
	int size=0x1000;
	if(max%size==0)
	max=max/0x1000;
	else
	max=(max/0x1000)+0x1;
	uint64_t *temp=(uint64_t *)placement;
	while(max>0)
	{
		int pt=(((placement) >> 12) & 0x1ff);
		uint64_t value=(uint64_t)alloc(0x1000);
		value=value|0x3;
		*(pte_p+pt)=value;
		placement+=0x1000;
		max=max-0x1;
	}
	extra=temp;
	return(extra);
}


void mapvideo()
{
	map1(placement);
	video=(char *)placement;
	video1=(char *)placement;
	videomem=video;
	placement=placement+0x1000;
	free=free+0x1000;
}


void getpage(uint64_t *start,uint64_t *end )
{
	int tid=((((uint64_t)start) >> 21) & 0x1ff);
	int pdid=((((uint64_t)start) >> 30) & 0x1ff);
	int pmid=((((uint64_t)start) >> 39) & 0x1ff);

	uint64_t p1=(uint64_t)pdpte_p;
	p1=p1|0x3;
	*(pmle_p+pmid)=p1;
	uint64_t p2=(uint64_t)pde_p;
	p2=p2|0x3;
	*(pdpte_p+pdid)=p2;
	uint64_t p3=(uint64_t)pte_p;
	p3=p3|0x3;
	*(pde_p+tid)=p3;

	uint64_t i =(uint64_t)start;
	uint64_t j=(uint64_t)end;
	j=j+nframes+0x4000;
	placement=j;
	phyfree=phyfree+nframes+0x4000;
	while(i<j)
	{
	map(i);
	i=i+0x1000;
	}
}


void enablepaging(uint64_t direc)
{
__asm__ __volatile__("movq %0, %%cr3":
				     : "r" (direc)
				     : "memory","cc"
				     );

uint64_t cr0;
__asm__ __volatile__("movq %%cr0,%0":"=r"(cr0));
cr0|=0x80000000;
__asm__ __volatile__("movq %0, %%cr0"::"r"(cr0));
}



void page(uint64_t *start,uint64_t *end,uint64_t physbase,uint64_t *physfree,uint64_t endaddr,uint8_t *istack )
{
	placement=(uint64_t)end;
	free=(uint64_t)physfree;
	addr=physbase;
	addr1=(uint64_t)physfree;
	frame=physfree;
	frames=(endaddr-free)/0x1000;
	frames=index(frames);
	phybas=physbase;
	phyfree=(uint64_t)physfree;
	uint64_t size1=0x1000;
	if ((uint64_t)frames%size1 == 0)
       	        nframes = (uint64_t )(((uint64_t)frames/size1) * size1);
        else
                nframes = (uint64_t )((((uint64_t)frames/size1) * size1)+size1);

	kernstart=placement+nframes;
	free=free+(uint64_t)nframes;
	addr1=addr1+(uint64_t)nframes;
	memset(frame,0,index(nframes));
	pmle_p=alloc(0x1000);
	pdpte_p=alloc(0x1000);
	pde_p=alloc(0x1000);
	pte_p=alloc(0x1000);
	getpage(start,end);
	mapvideo();
	uint64_t addr=(uint64_t)pmle_p;
	enablepaging(addr);
//	__asm__  __volatile__ ("sti");
	uint64_t temp;
	temp=(uint64_t)kernstart;
	pmle_pp=(uint64_t *)temp;
	temp=0x1000+(uint64_t)pmle_pp;
	pdpte_pp=(uint64_t *)temp;
	temp=0x1000+(uint64_t)pdpte_pp;
	pde_pp=(uint64_t *)temp;
	temp=0x1000+(uint64_t)pde_pp;
	pte_pp=(uint64_t *)temp;
	frame1=end;
	uint64_t *p=(uint64_t *)kmalloc();
	kprintf("after paging %p \n",p);
	uint64_t *pp=(uint64_t *)kmalloc();
	kprintf("after 2nd %p",pp);
	yield();
	 
}
