#include <stdio.h>

#define QUOTE(s) 	#s
#define STR(s)		QUOTE(s)

unsigned char shellcode[] = STR(SHELLCODE);
unsigned char stage2[] = "\xbe\xba\xfe\xca" STR(STAGE2);

int
main(void)
{
	printf("Egghunter length: %zu\n", sizeof shellcode - 1);
	printf("Second stage length: %zu\n", sizeof stage2 - 5);
	(*(void(*)(void)) shellcode)();
}
