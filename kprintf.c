#include <sys/kprintf.h>
#include<sys/stdarg.h>
#include<sys/defs.h>
extern char *video;
extern char *video1;
extern char *videomem;
int pid;
int rev(int a)
{
int sum=0;
int m=10;
while(a>0)
{
int n=a%10;
sum=sum*m+n;
a=a/10;
}
return(sum);
}
char getcharacter(int res)
{
return(res+'0');
}

void printglyph(uint8_t ascii,int shiftPressed , int controlPressed){
  char glyphwcontrol[7] ="    ^";
  char glyphwocontrol[7] ="     ";
  glyphwcontrol[5] = ascii;
  glyphwcontrol[6] = '\0';
  glyphwocontrol[5] = ascii;
  glyphwocontrol[6] ='\0';
  int len=sizeof(glyphwcontrol);
  char *s;
  if(controlPressed){
        s = glyphwcontrol;
  }else{
        s = glyphwocontrol;
  }
  char *glyphposition = videomem+0xA0*23 + (75-len)*2;
  while(*s != '\0'){
        *glyphposition =*s;
        glyphposition += 2;
        s++;

  }


}


void itoa(unsigned long value, char * str,int base){
        char *ptr =str;
        char charmap[] = "0123456789abcdef";
        char temp;
        if(base == 10 && value < 0){
           *ptr++ = '-';
            value = -1 * value;
        }else if(value == 0){
                str[0] ='0';
                str[1] = '\0';
        }else{
        char * low = ptr;
        while(value){
                *ptr++ = charmap[value%base];
                value =value  /base;
        }
        *ptr='\0';
        ptr --;
        while(low < ptr){
                temp = *low;
                *low = *ptr;
                *ptr = temp;
                low++;
                ptr--;
        }

        }

}


void printTimeSinceBoot(long value){
   char time[100];
   itoa(value,time,10);
   char *s  =time;
   int len=0;
   while(*s != '\0'){
     len++;
     s++;
   }
   char * timerposition =  videomem+0xA0*23 + (80 - len) *2;
   s = time;
  while(*s != '\0'){
     *timerposition = *s;
      timerposition +=2;
      s++;
  }

}


void fun(unsigned long res,char fed[20])
{
char he[16]="0123456789ABCDEF";
int c=0;
while(res>0)
{
int n=res%16;
fed[c]=he[n];
c++;
res=res/16;
}
int i,j;
for(i=0;fed[i]!='\0';i++)
{}
i--;
j=0;
while(j<i)
{
char t=fed[j];
fed[j]=fed[i];
fed[i]=t;
i--;
j++;
}

}


void kprintf(const char *fmt, ...)
{
//static char *video=(char *)0xffffffff802a2000;
//static char *video1=(char *)0xffffffff802a2000;;
int colour=0x1f;
va_list list;
void *ptr;
va_start(list,fmt);
int a,hexval;
char ch,cha,temp;
char *buf;
char *buff="0";
char buft[1];
char final[64];
char hexbuf[20];
char *buf1="%";
while(*fmt!='\0')
{

if(video==(char *)(videomem+0xA0*25))
{
video=(char *)videomem;
}
if(*fmt=='%')
{
ch=*(++fmt);
switch(ch)
{
case 'd':
a=va_arg(list,int);
if(a<0)
{
*video++='-';
*video++=colour;
a=a*-1;
}
if(a==0)
{
*video++=buff[0];
*video++=colour;
fmt++;
break;
}
int result=rev(a);
while(result>0)
{
int num=result%10;
temp=getcharacter(num);
buft[0]=temp;
*video++=buft[0];
*video++=colour;
result=result/10;
}
fmt++;
break;

case '%':
*video++=buf1[0];
*video++=colour;
fmt++;
break;

case 'c':
cha=(char)va_arg(list,int);
*video++=cha;
*video++=colour;
fmt++;
break;

case 's':
buf=va_arg(list,char *);
int i=0;
while(buf[i]!='\0')
{
*video++=buf[i];
*video++=colour;
i++;
}
fmt++;
break;

case 'x':
hexval=va_arg(list,int);
fun(hexval,hexbuf);
int t=0;
while(hexbuf[t]!='\0')
{
*video++=hexbuf[t];
*video++=colour;
t++;
}
fmt++;
break;

case 'p':
ptr=va_arg(list,void *);
if(ptr==NULL)
{
*video++=buff[0];
*video++=colour;
}
else
{
unsigned long lt=(unsigned long)ptr;
fun(lt,final);
int r=0;
while(final[r]!='\0')
{
*video++=final[r];
*video++=colour;
r++;
}
}
fmt++;
break;
}
}
else
{
if(*fmt=='\n')
{
video=video+0xA0-((video-video1)%0xA0);
fmt++;
}
else if(*fmt=='\r')
{
video=video-((video-video1)%0xA0);
fmt++;
}
else
{
*video++=*fmt;
*video++=colour;
fmt++;
}
}
}
}
