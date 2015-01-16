#include <threadQueue.h>
#include <process.h>

void entry()
{
	currentThread->process->state = PROCESS_RUNNING;
	currentThread->state = THREAD_RUNNING;
	currentThread->func();
	currentThread->state = THREAD_DONE;
	currentThread->process->threadCount--;
	if(currentThread->process->threadCount == 0) {
		currentThread->process->state = PROCESS_READY;
	}
	while(1);
}

void threadConstruct(thread *t, void(*func1)(), unsigned int esp)
{
	t->state = THREAD_READY;
	t->func = func1;
	t->esp = esp;
	t->eip = (int)entry;
}

void threadJoin(thread *t)
{
	while(t->state != THREAD_DONE);
}
