.section .text
.global sample
sample:

             
        movq %rdi,%rax 
        movq %rsi,%rbx 
        movq %rax,%rsp 
        pushq %rbx 
        iretq 
# "a"(stk),"b"(ptr)


.global flush
flush:
   
        pushq $0x23 
        pushq %rax 
        pushfq 
        pushq $0x1B
        

/*
.global usermode
usermode:
      
movq   $len,%rdx              
//movq   $str,%rcx               
movq   $1,%rbx         
movq   $4,%rax         
int $0x80
*/
