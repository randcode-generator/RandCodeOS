#include <threadQueue.h>

#define READY 1
#define RUNNING 2
#define DONE 3

unsigned int savedEIP = 0;
unsigned int savedESP = 0;
unsigned int savedCR3 = 0;

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
	savedCR3 = currentThread->process->cr3;
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

thread* getFreeThread()
{
	static unsigned int index = 0;
	if(index == 10)
		return 0;
	threads[index].id = index;
	return &threads[index++];
}