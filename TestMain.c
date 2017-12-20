#include <stdio.h>
#include <stdlib.h>

#include "TestCase1.h"
#include "TestCase2.h"
#include "TestCase3.h"

#include "Init.h"
#include "Scheduler.h"
#include "Thread.h"

// #include "Task.h"
// #include "init.h"
// #include "ObjMgr.h"
// #include "Scheduler.h"
int count; /* Definition here */

void main(int argc, char *argv[])
{
	int TcNum;
	thread_t tid1, tid2, tid3, tid4;
	//int count=0;
	if (argc != 2)
	{
		perror("Input TestCase Number!");
		exit(0);
	}

	Init();

	TcNum = atoi(argv[1]);

	switch (TcNum)
	{
	case 1:
		thread_create(&tid1, NULL, (void *)TestCase1, 0);
		break;
	case 2:
		thread_create(&tid2, NULL, (void *)TestCase2, 0);
		break;
	case 3:
		thread_create(&tid3, NULL, (void *)TestCase3, 0);
		break;
	}

	RunScheduler();
	while (1)
	{
	}
}
