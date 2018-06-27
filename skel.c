#include <stdio.h>

#define QUOTE(s)	#s
#define STR(s)		QUOTE(s)

unsigned char shellcode[] = STR(SHELLCODE);

int
main(void)
{
	printf("Shellcode length: %zu\n", sizeof shellcode - 1);
	(*(void(*)(void)) shellcode)();
}
