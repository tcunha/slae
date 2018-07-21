global _start

section .text
_start:
	jmp short read_eip

read_start:
	mov eax, 0x5
	; int open(const char *, int)
	pop ebx			; path from the jmp/call/pop technique
	xor ecx, ecx		; flags
	int 0x80

	mov ebx, eax		; fd (from the return code of open(2))
	mov eax, 0x3
	; ssize_t read(int, void *, size_t)
	mov edi, esp		; save contents to the stack
	mov ecx, edi		; buf
	mov edx, 0x1000		; count
	int 0x80

	mov edx, eax		; count (from the return code of read(2))
	mov eax, 0x4
	; ssize_t write(int, const void *, size_t)
	mov ebx, 0x4		; fd
	; ecx is already pointing to the correct location
	int 0x80

	mov eax, 0x1
	; void exit(int)
	mov ebx, 0x0		; status
	int 0x80

read_eip:
	call dword read_start
	path: db "/proc/version", 0x00
