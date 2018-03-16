.section .text
.global switchtoRing3
.global contextswitch2
contextswitch2:
    popq 64(%rdi)
    movq %rax,(%rdi)
    movq %rbx,8(%rdi)
    movq %rcx,16(%rdi)
    movq %rdx,24(%rdi)
    movq %rbp,56(%rdi)
	movq %r8,88(%rdi)
	movq %r9,96(%rdi)
	movq %r10,104(%rdi)
	movq %r11,112(%rdi)
	movq %r12,120(%rdi)
	movq %r13,128(%rdi)
	movq %r14,136(%rdi)
	movq %r15,144(%rdi)

    movq %rsp,48(%rdi)
    movq 48(%rsi),%rsp
  
	movq 144(%rsi),%r15
	movq 136(%rsi),%r14
	movq 128(%rsi),%r13
	movq 120(%rsi),%r12
	movq 112(%rsi),%r11
	movq 104(%rsi),%r10
	movq 96(%rsi),%r9
	movq 88(%rsi),%r8
	movq 56(%rsi),%rbp

	movq 24(%rsi),%rdx
	movq 16(%rsi),%rcx
	movq 8(%rsi),%rbx
	movq (%rsi),%rax

	pushq 64(%rsi)	
        retq
	


.global switchTask1
switchTask1:
    movq %rax,(%rdi)
    movq %rbx,8(%rdi)
    movq %rcx,16(%rdi)
    movq %rdx,24(%rdi)
    movq %rsi,32(%rdi)
    movq %rsp,48(%rdi)
    movq %rbp,56(%rdi)
    popq %rax
    movq %rax,64(%rdi)
    movq %rsp,48(%rdi)
    movq 48(%rsi),%rsp
    movq (%rsi),%rax
    movq 8(%rsi),%rbx
    movq 16(%rsi),%rcx
    movq 24(%rsi),%rdx
    movq 40(%rsi),%rdi
    movq 48(%rsi),%rsp
    movq 56(%rsi),%rbp

    movq 64(%rsi),%rax
    pushq %rax
    ret
    
  










