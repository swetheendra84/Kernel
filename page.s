.global write_cr3
write_cr3:
  movq 32(%rdi),%rax
  movq 32(%rsi),%rdx
  # Save old callee-save registers
  pushq %rbp
  pushq %rbx
  pushq %rsi
  pushq %rdi

  # Switch stacks
  movq %rsp, %rax
  movq %rdx, %rsp

  # Load new callee-save registers
  popq %rdi
  popq %rsi
  popq %rbx
  popq %rbp
  ret 


.global write_cr0
write_cr0:

  pushq %rbp
  movq %rsp,%rbp
  movq %cr0,%rax
  or $80000000,%rax
  movq %rax,%cr0
  movq %rbp,%rsp
  popq %rbp
  retq

.global switch
switch:

  movq 4(%rsp), %rax
  movq 8(%rsp), %rdx

  # Save old callee-save registers
  pushq %rbp
  pushq %rbx
  pushq %rsi
  pushq %rdi

  # Switch stacks
  movq %rsp, (%rax)
  movq %rdx, %rsp

  # Load new callee-save registers
  popq %rdi
  popq %rsi
  popq %rbx
  popq %rbp
  iretq












      
