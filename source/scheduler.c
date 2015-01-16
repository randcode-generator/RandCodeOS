#include <thread.h>
#include <threadQueue.h>

unsigned int savedEIP = 0;
unsigned int savedESP = 0;
unsigned int savedCR3 = 0;

unsigned int getCurrentThreadState()
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

void switchThread()
{
	if(currentThread->state == THREAD_RUNNING || 
		currentThread->state == THREAD_READY)
		threadQueueEnqueue(&tq, currentThread);
	currentThread = threadQueuePop(&tq);
}