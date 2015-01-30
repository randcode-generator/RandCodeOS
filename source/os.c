#include <threadQueue.h>
#include <thread.h>
#include <commandline.h>
#include <process.h>
#include <e820.h>
const char *msg = "Welcome to RandcodeOS\n";

int main()
{
	clrScreen();
	int i = 0;
	for (i = 0; i < 8; i++)
	{
		printf("%x %x %x %x %x\n", 
			s[i].BaseH, s[i].BaseL, 
			s[i].LengthH, s[i].LengthL, s[i].Type);
	}
	printf(msg);

	initialize_paging(0x9C000);
	enable_paging();

	memset(processes, 0, sizeof processes);
	process *p1 = getFreeProcess();
	threadQueueConstruct(&tq);
	processConstruct(p1, commandline, 0x200000);
	currentThread = &p1->threads[0];

	init_timer(100);
	while(1);
}
