#include "lightthread.h"
#include <pthread.h>
#include <unistd.h>
#include <sys/errno.h>
#include <stdio.h>
#include <time.h>
#include <string.h>



int CLightThread::CreateThread(void*( *proc )( void * ), void *pargs)
{
   pthread_t ntid;
   return pthread_create(&ntid, NULL, proc, pargs);
}
void CLightThread::EndThread()
{
   pthread_exit(NULL);
}

unsigned int CLightThread::GetCurrentThreadId()
{
   return pthread_self();
}
void CLightThread::Sleep(unsigned int milliseconds)
{
    if(milliseconds>=1000)
	{
       unsigned int s = milliseconds/1000;
       unsigned int us = milliseconds  - s*1000;
	   sleep(s);
       if(us>0)  usleep(us*1000);
	}
	else
	{
	   usleep(milliseconds*1000);
	}
}
//=====================================================================================
CLightThreadMutex::CLightThreadMutex()
{
	pthread_mutex_init(&m_hMutex, NULL);
}
CLightThreadMutex::~CLightThreadMutex()
{
	pthread_mutex_destroy(&m_hMutex);
}
int CLightThreadMutex::Lock()
{
	return pthread_mutex_lock(&m_hMutex) == 0 ?0:-1;
}
int CLightThreadMutex::TryLock(unsigned int dwMilliseconds)
{
	// The function pthread_mutex_trylock() returns zero if a lock on the mutex object referenced by mutex is acquired. Otherwise, an error number is returned to indicate the error.
	//unsigned int us= dwMilliseconds*1000;
	int rt = pthread_mutex_trylock(&m_hMutex);
	if( rt == EBUSY)
	{
		//CMyTimeSpan start;
		while(rt == EBUSY)
		{
			//if( start.GetSpaninMilliseconds()>dwMilliseconds)
			//{
			//	rt = -1;
			//}
			//else
			//{
			    usleep(dwMilliseconds);
				//usleep(20000);         //sleep  20ms
				rt = pthread_mutex_trylock(&m_hMutex);
			//}
		}
	}
	return rt;
}

void CLightThreadMutex::Unlock()
{
	//if(m_hMutex)
	pthread_mutex_unlock(&m_hMutex);
}


//=====================================================================================================
CThreadError::CThreadError()
{
	m_pStart = NULL;
}
CThreadError::~CThreadError()
{
	internal_thread_error_t *temp;
	while(m_pStart)
	{
		temp = m_pStart;
		m_pStart = (internal_thread_error_t*)m_pStart->next;
		delete temp;
	}
}
void CThreadError::operator=(int errorno)
{
	unsigned int tid = CLightThread::GetCurrentThreadId();
	internal_thread_error_t *temp = search(tid);
	if(!temp)
	{
		temp = allocMemory(tid);
	}
	temp->threaderror.errorno = errorno;
	temp->threaderror.errormsg[0] = '\0';
}
void CThreadError::operator=(const char * msg)
{
	unsigned int tid = CLightThread::GetCurrentThreadId();
	internal_thread_error_t *temp = search(tid);
	if(!temp)
	{
		temp = allocMemory(tid);
	}
	temp->threaderror.errorno = 0;
	strncpy(temp->threaderror.errormsg, msg, 510);
}
void CThreadError::operator=(thread_error_t & st)
{
	unsigned int tid = CLightThread::GetCurrentThreadId();
	internal_thread_error_t *temp = search(tid);
	if(!temp)
	{
		temp = allocMemory(tid);
	}
	memcpy(&temp->threaderror, &st, sizeof(thread_error_t));
}
unsigned int CThreadError::GetLastErrorNo()
{
	unsigned int tid = CLightThread::GetCurrentThreadId();
	internal_thread_error_t *temp = search(tid);
	return temp?temp->threaderror.errorno:0;
}
const char *CThreadError::GetLastErrorMsg()
{
	unsigned int tid = CLightThread::GetCurrentThreadId();
	internal_thread_error_t *temp = search(tid);
	return temp?(const char*)temp->threaderror.errormsg:NULL;
}
const thread_error_t *CThreadError::GetLastErrorStruct()
{
	unsigned int tid = CLightThread::GetCurrentThreadId();
	internal_thread_error_t *temp = search(tid);
	return temp?(const thread_error_t *)(&(temp->threaderror)):NULL;
}

CThreadError::internal_thread_error_t* CThreadError::allocMemory(unsigned int tid)
{
	internal_thread_error_t *temp = new internal_thread_error_t;
	temp->threadid = tid;
	temp->next = m_pStart;
	m_pStart = temp;
	return temp;
}
CThreadError::internal_thread_error_t * CThreadError::search(unsigned int tid)
{
	internal_thread_error_t *temp = m_pStart;
	while(temp)
	{
		if(temp->threadid == tid) break;
		temp->next = (void *)temp;
	}
	return temp;
}
