__asm__(".code16gcc\n");
#include "e820.h"
void __attribute__((noinline)) detectMemory() {

	unsigned int contID = 0;
	int entries = 0, signature, bytes;

	int i = 0;
	for (i = 0; i < 8; i++)
	{
		__asm__ __volatile__ ("int  $0x15" 
					: "=a"(signature), "=c"(bytes), "=b"(contID)
					: "a"(0xE820), "b"(contID), "c"(24), "d"(0x534D4150), "D"(&s[i]));
		if (contID == 0)
		{
			break;
		}
	}
}