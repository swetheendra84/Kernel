/*
GLOBAL _jump_usermode ;you may need to remove this _ to work right..
EXTERN _test_user_function
_jump_usermode:
     mov ax,0x23
     mov ds,ax
     mov es,ax
     mov fs,ax
     mov gs,ax ;we don't need to worry about SS. it's handled by iret

     mov eax,esp

GLOBAL _jump_usermode ;you may need to remove this _ to work right..
EXTERN _test_user_function
_jump_usermode:
     mov ax,0x23
     mov ds,ax
     mov es,ax
     mov fs,ax
     mov gs,ax ;we don't need to worry about SS. it's handled by iret
      push 0x23 ;user data segment with bottom 2 bits set for ring 3
     push eax ;push our current stack just for the heck of it
     pushf
     push 0x1B; ;user code segment with bottom 2 bits set for ring 3
     push _test_user_function ;may need to remove the _ for this to work right
     iret


GLOBAL tss_flush   ; Allows our C code to call tss_flush().
tss_flush:
   mov ax, 0x2B      ; Load the index of our TSS structure - The index is
                     ; 0x28, as it is the 5th selector and each is 8 bytes
                     ; long, but we set the bottom two bits (making 0x2B)
                     ; so that it has an RPL of 3, not zero.
   ltr ax            ; Load 0x2B into the task state register.
   ret
*/
