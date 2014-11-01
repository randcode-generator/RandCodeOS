#include <threadQueue.h>
#include <thread.h>
#include <commandline.h>
const char *msg = "Welcome to RandcodeOS";

int main()
{
	clrScreen();
	printf(msg);
	setXY(0, 1);
	setup_paging();

	threadConstruct(&threads[0], commandline, 0x7100);

	threadQueueConstruct(&tq);
	threadQueueEnqueue(&tq, &threads[0]);

	currentThread = &threads[0];

	init_timer(100);
	while(1);
}
