#ifndef _threadqueue_
#define _threadqueue_

#include "thread.h"

typedef struct
{
	unsigned int start;
	unsigned int end;
	thread* thread[5];
}threadQueue;

void threadQueueConstruct(threadQueue *q);
void threadQueueEnqueue(threadQueue *q, thread *t);
thread* threadQueuePop(threadQueue *q);

threadQueue tq;
#endif