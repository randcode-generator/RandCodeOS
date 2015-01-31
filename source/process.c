
#include <process.h>
#include <threadQueue.h>

void processConstruct(process *p, void(*func1)(), unsigned int cr3)
{
	setup_kernel_space(cr3);
	p->threadCount = 0;
	p->cr3 = cr3;
	p->state = PROCESS_RUNNING;
	unsigned int stack = 0x600800;
	allocate_memory(stack, cr3);

	thread *t = processGetFreeThread(p);
	threadConstruct(t, func1, stack);
	threadQueueEnqueue(&tq, t);
}

thread* processGetFreeThread(process *p)
{
	if(p->threadCount == 3)
		return 0;
	thread *t = &p->threads[p->threadCount++];
	t->process = p;
	return t;
}

process *getFreeProcess()
{
	process *p = 0;
	int maxProcesses = sizeof processes / sizeof processes[0];
	int i = 0;
	for (; i < maxProcesses; i++)
	{
		p = &processes[i];
		if(p->state == PROCESS_READY)
			break;
	}
	return p;
}