.global isr_handler128
.global isr128

isr128 :
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
   callq isr_handler128
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
  
   

   sti
   iretq
   

