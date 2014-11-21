#include <process.h>
#include <threadQueue.h>

void processConstruct(process *p, void(*func1)(), unsigned int cr3)
{
	setup_kernel_space(cr3);
	p->threadCount = 0;
	p->cr3 = cr3;
	unsigned int stack = 0x600000;
	allocate_memory(stack, cr3);

	thread *t = getFreeThread();
	threadConstruct(t, func1, stack);
	processAddThread(p, t);
	threadQueueEnqueue(&tq, t);
}

void processAddThread(process *p, thread *t)
{
	if(p->threadCount == 3)
		return;
	p->threads[p->threadCount++] = t;
	t->process = p;
}

process *getFreeProcess()
{
	static unsigned int index = 0;
	if(index == 10)
		return 0;
	processes[index].id = index;
	return &processes[index++];
}