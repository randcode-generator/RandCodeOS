#include <threadQueue.h>

void task1()
{
	unsigned int t1 = 0;
	unsigned int i = 0;
	for(; i <= 5000000; i++)
		t1 = 0;
	i = 0;
	for(; i <= 99; i++)
	{
		t1 += i;
	}

	printf("summation of 99 is %u\n", t1);
}

void task2()
{
	unsigned int t2 = 0;
	unsigned int i = 0;
	for(; i <= 5000000; i++)
		t2 = 0;
	i = 0;
	for(; i <= 100; i++)
	{
		t2 += i;
	}
	
	printf("summation of 100 is %u\n", t2);
}

void initTasks()
{
	threadConstruct(&threads[1], task1, 0x7200);
	threadConstruct(&threads[2], task2, 0x7300);

	threadQueueEnqueue(&tq, &threads[1]);
	threadQueueEnqueue(&tq, &threads[2]);
}

void waitForTaskDone()
{
	threadJoin(&threads[1]);
	threadJoin(&threads[2]);
}
