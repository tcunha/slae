#include <stdio.h>

#define QUOTE(s) 	#s
#define STR(s)		QUOTE(s)

unsigned char egghunter[] = STR(EGGHUNTER);
unsigned char shellcode[] = "\xbe\xba\xfe\xca" STR(SHELLCODE);

int
main(void)
{
	printf("Egghunter length: %zu\n", sizeof egghunter - 1);
	printf("Shellcode length: %zu\n", sizeof shellcode - 1);
	(*(void(*)(void)) egghunter)();
}
