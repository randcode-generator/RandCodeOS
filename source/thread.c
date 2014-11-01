#include <threadQueue.h>

#define READY 1
#define RUNNING 2
#define DONE 3

unsigned int savedEIP = 0;
unsigned int savedESP = 0;

void entry()
{
	currentThread->state = RUNNING;
	currentThread->func();
	currentThread->state = DONE;

	while(1);
}

void threadConstruct(thread *t, void(*func1)(), unsigned int esp)
{
	t->state = READY;
	t->func = func1;
	t->esp = esp;
	t->eip = (int)entry;
}

unsigned int getState()
{
	return currentThread->state;
}

void loadCurrentThread()
{
	savedESP = currentThread->esp;
	savedEIP = currentThread->eip;
}

void saveCurrentThread()
{
	currentThread->esp = savedESP;
	currentThread->eip = savedEIP;
}

void switchTask()
{
	if(currentThread->state == RUNNING || 
		currentThread->state == READY)
		threadQueueEnqueue(&tq, currentThread);
	currentThread = threadQueuePop(&tq);
}

void threadJoin(thread *t)
{
	while(t->state != DONE);
}