  global main
  extern printf

  segment .bss
  i$0 resq 1
  sum$1 resq 1
  sum$2 resq 1
  a1$3 resq 1
  a2$4 resq 1
  a$5 resq 1

  section .text
main:
 mov rax,1
 push rax
 pop qword[i$0]
 mov rax,0
 push rax
 pop qword[sum$1]
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
 push qword[sum$2]
 mov rax,1
 push rax
 call add
 push rax
 pop qword[sum$2]
 jmp L2
L3:
 push qword[sum$2]
 pop rax
 ret
add:
 pop r15
 pop qword[a1$3]
 pop qword[a2$4]
 push qword[a1$3]
 push qword[a2$4]
 pop rbx
 pop rax
 add rax,rbx
 push rax
 pop qword[a$5]
 push qword[a$5]
 pop rax
 push r15
 ret

  section .data
