.global page_fault
.global isr14
/*
isr14 :
   cli
   pushq %rbx
   pushq %rcx
   pushq %rdx
   pushq %rbp
   pushq %rsi
   pushq %rdi
   pushq %r8
   pushq %r9
   pushq %r10
   pushq %r11

   callq page_fault
   popq %r11
   popq %r10
   popq %r9
   popq %r8
   popq %rdi
   popq %rsi
   popq %rbp
   popq %rdx
   popq %rcx
   popq %rbx
   sub $0x8,%rsp
   sti
   iretq



isr14:
    cli
   pushq %rbx
   pushq %rcx
   pushq %rdx
   pushq %rbp
   pushq %rsi
   pushq %rdi
   pushq %r8
   pushq %r9
   pushq %r10
   pushq %r11


    movq %rsp, %rdi
    callq page_fault
    
   popq %r11
   popq %r10
   popq %r9
   popq %r8
   popq %rdi
   popq %rsi
   popq %rbp
   popq %rdx
   popq %rcx
   popq %rbx


    add $0x10, %rsp
    sti
    iretq 
*/

isr14:
 	cli
    pushq %rdi
    pushq %rsi
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rbp
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    pushq %rsp
    movq %rsp,%rdi
    callq page_fault
    popq %rsp
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rbp
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax
    popq %rsi
    popq %rdi
    add $0x08,%rsp
    sti
    iretq





