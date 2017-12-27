  global main
  extern printf

  segment .bss
  i$0 resq 1
  sum$1 resq 1
  a1$2 resq 1
  a2$3 resq 1
  a$4 resq 1

  section .text
main:
 mov rax,1
 push rax
 pop qword[i$0]
 mov rax,0
 push rax
 pop qword[sum$1]
 mov rdi,fmt1
 mov rax,0
 push rbp
 call printf
 pop rbp
L2:
 push qword[i$0]
 mov rax,10
 push rax
 pop rbx
 pop rax
 cmp rax,rbx
 jl L4
 mov rax,0
 jmp L5
L4:
 mov rax, 1
L5:
 push rax
 pop rax
 cmp rax, 0
 je L3
 push qword[i$0]
 mov rax,1
 push rax
 call add
 push rax
 pop qword[i$0]
 push qword[sum$1]
 push qword[i$0]
 call add
 push rax
 pop qword[sum$1]
 push qword[i$0]
 push qword[sum$1]
 mov rdi,fmt2
 pop rdx
 pop rsi
 mov rax,0
 push rbp
 call printf
 pop rbp
 jmp L2
L3:
 mov rax,0
 push rax
 pop rax
 ret
add:
 pop r15
 pop qword[a1$2]
 pop qword[a2$3]
 push qword[a1$2]
 push qword[a2$3]
 pop rbx
 pop rax
 add rax,rbx
 push rax
 pop qword[a$4]
 push qword[a$4]
 pop rax
 push r15
 ret

  section .data
  fmt1: db `  i   sum\n`, 0
  fmt2: db `  %ld   %ld\n`, 0
