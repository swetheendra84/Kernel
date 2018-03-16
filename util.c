#include<sys/util.h>

long strtoint(const char *str)
{
int i=0;
int n='0';
long result=0;
while(str[i]!='\0')
{
	int temp=str[i]-n;
	result=result*10+temp;
	i++;
}
return(result);
}

int strncmp (const char * s1, const char * s2,int size)
{
    int i=0;
   while(i<size)
   {
        if(*s1==*s2)
        {
                i++;
                s1++;
                s2++;
                continue;
        }

        if( *(unsigned char *)s1 < *(unsigned char *)s2)
        {
                return(-1);
        }
        else
        {
                return(1);
        }

   }
return(0);
}


uint64_t pow(uint64_t a,int b)
{
if(b==0)
return(1);
else
return a*pow(a,b-1);
}

uint64_t octtodec(uint64_t num)
{
	uint64_t result=0;
	uint64_t temp=num;
	int i=0;
	while(temp>0)
	{
		int t=temp%10;
		result=result+pow(8,i)*t;
		i++;
		temp=temp/10;
	}
	return(result);

}

void mmemcpy(void *dest, void *src, size_t n)
{
   uint8_t *csrc = (uint8_t *)src;
   uint8_t *cdest = (uint8_t *)dest;

   for (int i=0; i<n; i++)
       cdest[i] = csrc[i];
}



char *strcpy(char *d, const char *s)
{
   char *saved = d;
   while (*s)
   {
       *d++ = *s++;
   }
   *d = 0;
   return saved;
}


void *mmemset(void *dest,int c,size_t n)
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


int mstrcmp (const char * s1, const char * s2)
{
    for(; *s1 == *s2; ++s1, ++s2)
        if(*s1 == 0)
            return 0;
    return *(unsigned char *)s1 < *(unsigned char *)s2 ? -1 : 1;
}

int mystrlen(char *str)
{
    int len = 0;
    int i;

    for (i=0; str[i] != 0; i++)
    {
        len++;
    }
    return(len);
}





