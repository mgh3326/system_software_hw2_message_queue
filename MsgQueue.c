#include "Thread.h"
#include "MsgQueue.h"
#include "my.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
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
	sign = 1;
	
		pthread_mutex_lock(&run_lock);
	for (int i = 0; i < MAX_QCB_SIZE; i++)
	{
		if (key == qcbTblEntry[i].key)
		{sign = 0;
		pthread_cond_signal(&run_wait);
	
		pthread_mutex_unlock(&run_lock);
			return i;
		}
	}
	
	for (int i = 0; i < MAX_QCB_SIZE; i++)
	{

		if (qcbTblEntry[i].key == -1)
		{
			qcbTblEntry[i].key = key;;
			Qcb *p = malloc(1 * sizeof *p);
			p->pMsgHead = NULL;
			p->pMsgTail = NULL;
			p->pThreadHead = NULL;
			p->pThreadTail = NULL;
			p->msgCount = 0;
			p->waitThreadCount = 0;
			qcbTblEntry[i].pQcb = p;
			sign = 0;
			pthread_cond_signal(&run_wait);
		
			pthread_mutex_unlock(&run_lock);
			return i;
		}
	}
	sign = 0;
	pthread_cond_signal(&run_wait);

	pthread_mutex_unlock(&run_lock);
	return 0;
}

int mymsgsnd(int msqid, const void *msgp, int msgsz, int msgflg)
{
	sign = 1;
	
		pthread_mutex_lock(&run_lock);


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
	
		if (i == msqid)
		{

			if (qcbTblEntry[i].pQcb->pMsgHead == NULL && qcbTblEntry[i].pQcb->pMsgTail == NULL)
			{

				p->pPrev = p->pNext = NULL;
				qcbTblEntry[i].pQcb->pMsgHead = qcbTblEntry[i].pQcb->pMsgTail = p;
				
				//return 0;
				//qcbTblEntry[i].pQcb->pMsgHead=p;
			}
			else if (NULL == qcbTblEntry[i].pQcb->pMsgHead || NULL == qcbTblEntry[i].pQcb->pMsgTail)
			{
				sign = 0;
				pthread_cond_signal(&run_wait);
			
				pthread_mutex_unlock(&run_lock);
				return -1;
			}
			else
			{
				p->pPrev = p->pNext = NULL;
				qcbTblEntry[i].pQcb->pMsgTail->pNext = p;
				p->pPrev = qcbTblEntry[i].pQcb->pMsgTail;
				qcbTblEntry[i].pQcb->pMsgTail = p;
				
				//return 0;
			}

			if(qcbTblEntry[i].pQcb!=NULL)
			{
				Thread* q = qcbTblEntry[i].pQcb->pThreadHead;
				i=0;
				while(q)
			  {
				if(q->type==p->type)
				{//레디큐로 다시 올려줘야되
					
					if (NULL == ReadyQHead && NULL == ReadyQTail)
					{
						//Wait_remove_element(getThread_wait(tid));

						if (NULL == q->pNext && (NULL == qcbTblEntry[i].pQcb->pThreadHead->pNext && NULL == qcbTblEntry[i].pQcb->pThreadTail->pNext)) /* only one element in queue */
						{
							qcbTblEntry[i].pQcb->pThreadHead = qcbTblEntry[i].pQcb->pThreadTail = NULL;
						}
						else if ((NULL == q->pNext) && q->pPrev) /* removing pTail */
						{
							qcbTblEntry[i].pQcb->pThreadTail = q->pPrev;
							q->pPrev->pNext = NULL;
						}
						else if (q->pNext && (NULL == q->pPrev)) /* removing qcbTblEntry[i].pQcb->pThreadHead */
						{
							qcbTblEntry[i].pQcb->pThreadHead = q->pNext;
							qcbTblEntry[i].pQcb->pThreadHead->pPrev = NULL;
						}
						else /* removing from center or somewhere */
						{
							q->pPrev->pNext = q->pNext;
							q->pNext->pPrev = q->pPrev;
						}


						ReadyQHead = ReadyQTail = q;
						q->pNext = q->pPrev = NULL;
				
						ReadyQHead->status = THREAD_STATUS_READY;
						ReadyQHead->bRunnable = 0;
					}
					else if (NULL == ReadyQHead || NULL == ReadyQTail)
					{
						fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
						fprintf(stderr, "List one of the list's qcbTblEntry[i].pQcb->pThreadHead/pTail is null while other is not\n");
						return -1;
					}
					else
				
					{
						//Wait_remove_element(getThread_wait(tid));

						if (NULL == q->pNext && (NULL == qcbTblEntry[i].pQcb->pThreadHead->pNext && NULL == qcbTblEntry[i].pQcb->pThreadTail->pNext)) /* only one element in queue */
						{
							qcbTblEntry[i].pQcb->pThreadHead = qcbTblEntry[i].pQcb->pThreadTail = NULL;
						}
						else if ((NULL == q->pNext) && q->pPrev) /* removing pTail */
						{
							qcbTblEntry[i].pQcb->pThreadTail = q->pPrev;
							q->pPrev->pNext = NULL;
						}
						else if (q->pNext && (NULL == q->pPrev)) /* removing qcbTblEntry[i].pQcb->pThreadHead */
						{
							qcbTblEntry[i].pQcb->pThreadHead = q->pNext;
							qcbTblEntry[i].pQcb->pThreadHead->pPrev = NULL;
						}
						else /* removing from center or somewhere */
						{
							q->pPrev->pNext = q->pNext;
							q->pNext->pPrev = q->pPrev;
						}
				



						ReadyQTail->pNext = q;
						q->pPrev = ReadyQTail;
						ReadyQTail = q;
						ReadyQTail->pNext = NULL;
				
						ReadyQTail->status = THREAD_STATUS_READY;
						ReadyQTail->bRunnable = 0;
						
					}



					break;
				}
				q = q->pNext;
			  }
			}
			//break;
		}
	}
	sign = 0;
	pthread_cond_signal(&run_wait);

	pthread_mutex_unlock(&run_lock);
	return 0; //for문이 다 끝나면 에러가 나오는게 맞음
}

int mymsgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg)
{

	sign = 1;
	
		pthread_mutex_lock(&run_lock);
	for (int i = 0; i < MAX_QCB_SIZE; i++)
	{
		if (i == msqid)//key 받는거에서 이거로 바꿈
		{
			
			Message *p = qcbTblEntry[i].pQcb->pMsgHead;
			for (; p; p = p->pNext)
			{
				
				if (msgtyp == p->type)
				{
					for (int i = 0; i < msgsz; i++)
					{
						((Message *)msgp)->data[i] = p->data[i];	
					}
					((Message *)msgp)->type = p->type;	
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
					sign = 0;
					pthread_cond_signal(&run_wait);
				
					pthread_mutex_unlock(&run_lock);
					return strlen(((Message *)msgp)->data); //이렇게 되면 정상적으로 삭제가 되네
				}

			}

			//for문 끝나고 이제 waitingqueue 구현 index 때문에 여기서 부터 하는게 맞겠다.
			Thread *thread_p = getThread(thread_self());
			thread_p->type=msgtyp;
			
			if (NULL == qcbTblEntry[i].pQcb->pThreadHead && NULL == qcbTblEntry[i].pQcb->pThreadTail)
			{
				
				Ready_remove_element(getThread(thread_self()));
				
				qcbTblEntry[i].pQcb->pThreadHead = qcbTblEntry[i].pQcb->pThreadTail = thread_p;
				
				thread_p->pNext = thread_p->pPrev = NULL;
				qcbTblEntry[i].pQcb->pThreadHead->status = THREAD_STATUS_BLOCKED;
				qcbTblEntry[i].pQcb->pThreadHead->bRunnable = 0;
				
				
				sign = 0;
				
				pthread_cond_signal(&run_wait);
			
				pthread_mutex_unlock(&run_lock);
				
				// return 0;
			}
			else if (NULL == qcbTblEntry[i].pQcb->pThreadHead || NULL == qcbTblEntry[i].pQcb->pThreadTail)
			{
				fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
				fprintf(stderr, "List one of the list's ReadyQHead/pTail is null while other is not\n");
				sign = 0;
				pthread_cond_signal(&run_wait);
			
				pthread_mutex_unlock(&run_lock);
				return -1;
			}
			else{

				Ready_remove_element(getThread(thread_self()));

				qcbTblEntry[i].pQcb->pThreadTail->pNext = thread_p;
				thread_p->pPrev = qcbTblEntry[i].pQcb->pThreadTail;
				qcbTblEntry[i].pQcb->pThreadTail = thread_p;
				qcbTblEntry[i].pQcb->pThreadTail->pNext = NULL;
				qcbTblEntry[i].pQcb->pThreadTail->status = THREAD_STATUS_BLOCKED;
				qcbTblEntry[i].pQcb->pThreadTail->bRunnable = 0;
				sign = 0;
				pthread_cond_signal(&run_wait);
			
				pthread_mutex_unlock(&run_lock);
			
				// return 0;
			}
			// fflush(stdout);
			//__thread_wait_handler(0);	 //다시 재움
			//Thread *pTh;
			thread_p->bRunnable == FALSE;
			// __getThread()는 tid로 linked list의 TCB를 찾아서 반환한다.
			//thread_p = getThread(pthread_self()); // child에서 TCB가 초기화 안되었는데, 이 함수가 호출되어도 되나 ?
											 // printf(" wait tid %u\n",pTh->tid);
			
											 pthread_mutex_lock(&(thread_p->readyMutex));
			//         printf("__thread_wait_handler start %u\n",pTh->tid);
		
			while (thread_p->bRunnable == FALSE)
				pthread_cond_wait(&(thread_p->readyCond), &(thread_p->readyMutex));
			pthread_mutex_unlock(&(thread_p->readyMutex));
			i--;
			continue;
		}
	}
			
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