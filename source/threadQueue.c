#include <threadQueue.h>

void threadQueueConstruct(threadQueue *q)
{
	q->start = q->end = 0;
}

void threadQueueEnqueue(threadQueue *q, thread *t)
{
	q->thread[q->end] = t;
	q->end++;
	q->end = q->end % 10;
}

thread* threadQueuePop(threadQueue *q)
{
	if(q->start == q->end)
		return 0;

	thread *t = q->thread[q->start];
	q->thread[q->start] = 0;
	q->start++;
	q->start = q->start % 10;
	return t;
}
