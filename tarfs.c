
#include<sys/tarfs.h>
#include<sys/kprintf.h>
#include<sys/util.h>
#include<sys/page.h>
#include<sys/process.h>


int writeto(int first,char *result,int third)
{
        int i=0;
        if(first==1)
        {
        for(i=0;i<third && result[i]!='\0';i++)
        kprintf("%c",result[i]);
        }
        return(i);

}

uint64_t exist(char *filename)
{
	struct posix_header_ustar *pelfhdr;
	uint64_t count=0;
	pelfhdr=(struct posix_header_ustar *)(&_binary_tarfs_start);
	while(pelfhdr!=(struct posix_header_ustar *)(&_binary_tarfs_end))
	{

		if(mystrlen(pelfhdr->name)==0)
		{
			return(0);
		}

		if(mstrcmp(pelfhdr->name,filename)>=0)
		{
			return(count+512);
		}
		uint64_t res=octtodec(strtoint(pelfhdr->size));
		if(res%512!=0)
		{
			res=res-(res%512)+512;
		}
		count=count+512+res;
		 pelfhdr=(struct posix_header_ustar *)(&_binary_tarfs_start+count);
	
	}
return(0);

}

void tarfsinit()
{
       
	struct task_struct *pcb=getCurrentProcess();
         struct posix_header_ustar *pelfhdr;
        uint64_t count=0;
        int i=3;
        pelfhdr=(struct posix_header_ustar *)(&_binary_tarfs_start);
        while(pelfhdr!=(struct posix_header_ustar *)(&_binary_tarfs_end))
        {

                if(mystrlen(pelfhdr->name)==0)
                {
                        break;
                }
	
				   file *fp=(file *)kmalloc(sizeof(file));
	                            strcpy(fp->filename,pelfhdr->name);
                   		    fp->offset=0;
               		            fp->inode_num=i;
                      		    fp->address=(uint64_t)pelfhdr;
				    fp->type=pelfhdr->typeflag[0];
				   pcb->fd[i]=fp;
                uint64_t res=octtodec(strtoint(pelfhdr->size));
                if(res%512!=0)
                {
                        res=res-(res%512)+512;
                }
                count=count+512+res;
                 pelfhdr=(struct posix_header_ustar *)(&_binary_tarfs_start+count);
                i++;
        }


}


void tarfsopen(char *name)
{

        struct task_struct *pcb=getCurrentProcess();
         struct posix_header_ustar *pelfhdr;
        uint64_t count=0;
	int i=3;
        pelfhdr=(struct posix_header_ustar *)(&_binary_tarfs_start);
        while(mystrlen(pelfhdr->name)!=0)
        {

                if(mstrcmp(pelfhdr->name,name)==0)
                {
                        break;
                }
		else
		{
	                 uint64_t res=octtodec(strtoint(pelfhdr->size));
                if(res%512!=0)
                {
                         res=res-(res%512)+512;
                }
        	         count=count+512+res;
                	 pelfhdr=(struct posix_header_ustar *)(&_binary_tarfs_start+count);
			 i++;
			 continue;
	
		}

        }
                      		    file *fp=(file *)kmalloc(sizeof(file));
                                    strcpy(fp->filename,pelfhdr->name);
                                    fp->offset=0;
                                    fp->inode_num=i;
                                    fp->address=(uint64_t)pelfhdr;
                                    fp->type=pelfhdr->typeflag[0];
                                    pcb->fd[i]=fp;
             
        


}





uint64_t fun1(char *file)
{
	struct posix_header_ustar *pelfhdr;
        uint64_t count=0;
        int i=3;
        pelfhdr=(struct posix_header_ustar *)(&_binary_tarfs_start);
        while(pelfhdr!=(struct posix_header_ustar *)(&_binary_tarfs_end))
        {

                if(mystrlen(pelfhdr->name)==0)
                {
                        break;
                } 
                if(mstrcmp(pelfhdr->name,file)==0)
		{
			return(uint64_t)(pelfhdr);
		}

                uint64_t res=octtodec(strtoint(pelfhdr->size));
                if(res%512!=0)
                {
                        res=res-(res%512)+512;
                }
                count=count+512+res;
                 pelfhdr=(struct posix_header_ustar *)(&_binary_tarfs_start+count);
                i++;
        }
return(-1);
}

int open(char *file1,int flags)
{

	struct task_struct *pcb=getCurrentProcess();

	tarfsopen(file1);	
	int i=3;
	file *p;
        while(i<50)
        {

         	p=pcb->fd[i];




		if(p==NULL)
		{
		i++;
		continue;
		}
		if(mstrcmp(p->filename,file1)==0 )
		{
			file *fp=(file *)kmalloc();
			strcpy(fp->filename,file1);
			fp->inode_num=i;
			fp->address=fun1(file1);
			if(fp->address==-1)
			return(-1);
			fp->type=0;
			//while(pcb->fd[i]!=NULL)
			// i++;
			pcb->fd[i]=fp;
			return(i);
		}
		
	 	if(p->filename==NULL)
		{
			return(-1);
		}		
		if(mstrcmp(p->filename,file1)==0)
			{
				if(p->type==48)
				{
				return(i);
				}
			}
		
       
	i++;
        }

      		
return(-1);
}


int close (int i)
{
	struct task_struct *pcb=getCurrentProcess();
	file *fp=pcb->fd[i];
	fp->offset=0;
	strcpy(fp->filename,"");
	fp->inode_num=0;
	fp->address=0;
	pcb->fd[i]=NULL;
	return(1);
}



int read(int filedis, char *buffer, int limit)
{
	struct task_struct *pcb=getCurrentProcess();
	 file *fp=pcb->fd[filedis];
         struct posix_header_ustar *pelfhdr;
        uint64_t count=0;
        int i=3;
	uint64_t count1;
	uint64_t result;
        pelfhdr=(struct posix_header_ustar *)(&_binary_tarfs_start);
        while(pelfhdr!=(struct posix_header_ustar *)(&_binary_tarfs_end))
        {

                if(fp==NULL||mystrlen(pelfhdr->name)==0)
                {
                        return(-1);
                }

                if(mstrcmp(pelfhdr->name,fp->filename)==0)
                {
			
                	count1=count+512;
			char *temp=(char *)(&_binary_tarfs_start+count1);
			uint64_t value=octtodec(strtoint(pelfhdr->size));
			if(value < limit)
			{
				limit=value;
			}	
			result=limit;
			while(limit)
			{
				*buffer=*temp;
				buffer++;
				temp++;
				limit--;
			}
			*buffer='\0';
			
			return(result);
		}
                                     
		uint64_t res=octtodec(strtoint(pelfhdr->size));
                if(res%512!=0)
                {
                        res=res-(res%512)+512;
                }
                count=count+512+res;

                    
        
                pelfhdr=(struct posix_header_ustar *)(&_binary_tarfs_start+count);
                i++;
        }
return(-1);

}


	

uint64_t opendir(char *dirname)
{

	struct task_struct *pcb=getCurrentProcess();
        int i=3;
	file *p;
	tarfsopen(dirname);
        while(i<50)
        {
                	p=pcb->fd[i];
			if(p==NULL)
			{
				i++;
				continue;
			}
 			if(mystrlen(p->filename)==0)
			{	
			        return(-1);
			}
                        if(mstrcmp(p->filename,dirname)==0)
                        {
                                if(p->type==53)
                                {
				uint64_t res=(uint64_t)p;
                                return(res);
                                }
                        }
                
        i++;
        }

return(-1);
}



uint64_t closedir(uint64_t address)
{

        struct task_struct *pcb=getCurrentProcess();
        int i=3;
        file *p;
        while(i<50)
        {
                        p=pcb->fd[i];
                        if(p==NULL)
                        {
                                i++;
                                continue;
                        }
                        if(mystrlen(p->filename)==0)
                        {
                                return(-1);
                        }
                        if(address==(uint64_t)p)
                        {
                                if(p->type==53)
                                {
                		pcb->fd[i]=NULL;                
                                return(0);
                                }
                        }

        i++;
        }



	return(-1);
}


int checkParent(char *source,char *destination)
{
	int i;
	int len=mystrlen(destination);
	i=len-2;
	while(i>=0)
	{
		if(destination[i]=='/')
		break;
	i--;
	}
	int j;
	for(j=0;j<i;j++)
	{
		if(source[j]!=destination[j])
		{
			return(0);
		}
	}
	return(1);
}

uint64_t readdir(uint64_t address)
{
	struct task_struct *pcb=getCurrentProcess();
        int i=3;
        file *fp;
	char *directory=NULL;
	int length=0;
	int flag=0;
        while(i<50)
        {
                	fp=pcb->fd[i];
			if(fp==NULL)
			{
				i++;
				continue;
			}
			if(mystrlen(fp->filename)==0)
                        {
                                return(-1);
                        }
			uint64_t lhs=(uint64_t)fp;
                        if(lhs==address)
                        {
				flag=1;
				i++;
				file *f=(file *)address;
				directory=f->filename;
				length=mystrlen(directory);
				continue;
                        }

			if((flag && strncmp((fp->filename), directory, length) == 0 )&& checkParent(directory,fp->filename))
		        {  
				 kprintf("%s \n ", fp->filename + length);   
        		}
        		i++;
        		}

return(0);

}































