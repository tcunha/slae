#include <stdio.h>

unsigned char shellcode[] = "%%SHELLCODE%%";

int
main(void)
{
	printf("Shellcode length: %zu\n", sizeof shellcode - 1);
	(*(void(*)(void)) shellcode)();
}
