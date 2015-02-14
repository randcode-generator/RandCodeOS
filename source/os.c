#include <threadQueue.h>
#include <thread.h>
#include <commandline.h>
#include <process.h>
const char *msg = "Welcome to RandcodeOS";

int main()
{
	clrScreen();
	printf(msg);
	setXY(0, 1);
	initialize_paging(0x9C000);
	enable_paging();

	memset(processes, 0, sizeof processes);
	process *p1 = getFreeProcess();
	threadQueueConstruct(&tq);
	processConstruct(p1, commandline, 0x200000);
	currentThread = &p1->threads[0];

	init_timer(100);

	__asm("sti");
	while(1);
}
