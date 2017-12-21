#include "Thread.h"
#include "MsgQueue.h"
#include <stdlib.h>
#include <stdio.h>
/* 반드시 구현할 필요는 없음. 만일 구현했다면, Init.c에 있는 Init()에 추가해야 함.*/
void _InitMsgQueue(void)
{
	for (int i = 0; i < MAX_QCB_SIZE; i++)
	{
		qcbTblEntry[i].key = -1;
	}
}

int mymsgget(int key, int msgflg)
{

	for (int i = 0; i < MAX_QCB_SIZE; i++)
	{
		if (key == qcbTblEntry[i].key)
			return i;
	}
	for (int i = 0; i < MAX_QCB_SIZE; i++)
	{
		if (qcbTblEntry[i].key == -1)
		{
			qcbTblEntry[i].key = key;

			Qcb *p = malloc(1 * sizeof *p);
			p->pMsgHead = NULL;
			p->pMsgTail = NULL;
			p->pThreadHead = NULL;
			p->pThreadTail = NULL;
			p->msgCount = 0;
			p->waitThreadCount = 0;
			qcbTblEntry[i].pQcb = p;
			return i;
		}
	}
	return 0;
}

int mymsgsnd(int msqid, const void *msgp, int msgsz, int msgflg)
{
	//int ret;
	Message *p = malloc(1 * sizeof *p);
	p->size = msgsz;
	p->type = ((Message *)msgp)->type;
	//strcpy(p->data, ((Message *)msgp)->data);
	for (int i = 0; i < msgsz; i++)
	{
		p->data[i] = ((Message *)msgp)->data[i];
	}

	for (int i = 0; i < MAX_QCB_SIZE; i++)
	{
		if (qcbTblEntry[i].key == msqid)
		{
			if (qcbTblEntry[i].pQcb->pMsgHead == NULL && qcbTblEntry[i].pQcb->pMsgTail == NULL)
			{
				p->pPrev = p->pNext = NULL;
				qcbTblEntry[i].pQcb->pMsgHead = qcbTblEntry[i].pQcb->pMsgTail = p;
				return 0;
				//qcbTblEntry[i].pQcb->pMsgHead=p;
			}
			else if (NULL == qcbTblEntry[i].pQcb->pMsgHead || NULL == qcbTblEntry[i].pQcb->pMsgTail)
			{
				return -1;
			}
			else
			{
				p->pPrev = p->pNext = NULL;
				qcbTblEntry[i].pQcb->pMsgTail->pNext = p;
				p->pPrev = qcbTblEntry[i].pQcb->pMsgTail;
				qcbTblEntry[i].pQcb->pMsgTail = p;
				return 0;
			}
			//break;
		}
	}
	return -1; //for문이 다 끝나면 에러가 나오는게 맞음
}

int mymsgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg)
{
				

	for (int i = 0; i < MAX_QCB_SIZE; i++)
	{
		if (qcbTblEntry[i].key == msqid)
		{
			Message *p = qcbTblEntry[i].pQcb->pMsgHead;
			//printf("ReadyQoutput\n");
			for (; p; p = p->pNext)
			{
				if (msgtyp == p->type)
				{
					for (int i = 0; i < msgsz; i++)
					{
						((Message *)msgp)->data[i] = p->data[i];
					}
					if (NULL == p->pNext && (NULL == qcbTblEntry[i].pQcb->pMsgHead->pNext && NULL == qcbTblEntry[i].pQcb->pMsgTail->pNext)) /* only one element in queue */
					{
						qcbTblEntry[i].pQcb->pMsgHead = qcbTblEntry[i].pQcb->pMsgTail = NULL;
					}
					else if ((NULL == p->pNext) && p->pPrev) /* removing pTail */
					{
						qcbTblEntry[i].pQcb->pMsgTail = p->pPrev;
						p->pPrev->pNext = NULL;
					}
					else if (p->pNext && (NULL == p->pPrev)) /* removing qcbTblEntry[i].pQcb->pMsgHead */
					{
						qcbTblEntry[i].pQcb->pMsgHead = p->pNext;
						qcbTblEntry[i].pQcb->pMsgHead->pPrev = NULL;
					}
					else /* removing from center or somewhere */
					{
						p->pPrev->pNext = p->pNext;
						p->pNext->pPrev = p->pPrev;
					}
					free(p);  //이거 해야되나?
					return 0; //이렇게 되면 정상적으로 삭제가 되네
				}
			}
		}
	}
			//for문 끝나고 이제 waitingqueue 구현
			printf("WaitingQ 내가 구현 안해서 오류야\n");
			//fflush(stdout);
			//이건 waittingqueue 삭제
			// if (NULL == p->pNext && (NULL == qcbTblEntry[i].pQcb->pThreadHead->pNext && NULL == qcbTblEntry[i].pQcb->pThreadTail->pNext)) /* only one element in queue */
			// 	{
			// 		qcbTblEntry[i].pQcb->pThreadHead = qcbTblEntry[i].pQcb->pThreadTail = NULL;
			// 	}
			// 	else if ((NULL == p->pNext) && p->pPrev) /* removing pTail */
			// 	{
			// 		qcbTblEntry[i].pQcb->pThreadTail = p->pPrev;
			// 		p->pPrev->pNext = NULL;
			// 	}
			// 	else if (p->pNext && (NULL == p->pPrev)) /* removing qcbTblEntry[i].pQcb->pThreadHead */
			// 	{
			// 		qcbTblEntry[i].pQcb->pThreadHead = p->pNext;
			// 		qcbTblEntry[i].pQcb->pThreadHead->pPrev = NULL;
			// 	}
			// 	else /* removing from center or somewhere */
			// 	{
			// 		p->pPrev->pNext = p->pNext;
			// 		p->pNext->pPrev = p->pPrev;
			// 	}

			//break;
		
	
	return -1; //이럼 실패 같은데?
}

int mymsgctl(int msqid, int cmd, void *buf)
{
	for (int i = 0; i < MAX_QCB_SIZE; i++)
	{
		if (msqid == qcbTblEntry[i].key)
			{
				qcbTblEntry[i].key=-1;
				//다른거 다 free 해주고 NULL 해주어야함
			}
	}
	return 0;
}