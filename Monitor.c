#include "Monitor.h"
#include <stdio.h>



bool InitMutex(Mutex* _Mutex)
{
    return sem_init(&_Mutex->Semaphore, 0, 1) == 0;
}

void DestroyMutex(Mutex* _Mutex)
{
    sem_destroy(&_Mutex->Semaphore);
}

void LockMutex(Mutex* _Mutex)
{
    sem_wait(&_Mutex->Semaphore);
}

void UnlockMutex(Mutex* _Mutex)
{
    sem_post(&_Mutex->Semaphore);
}



bool InitCondVar(CondVar* _CondVar)
{
    _CondVar->Count = 0;
    return sem_init(&_CondVar->Semaphore, 0, 0) == 0;
}

void DestroyCondVar(CondVar* _CondVar)
{
    sem_destroy(&_CondVar->Semaphore);
}

void WaitCondVar(CondVar* _CondVar, Mutex* _Mutex)
{
    _CondVar->Count++;
    UnlockMutex(_Mutex);
    sem_wait(&_CondVar->Semaphore);
    LockMutex(_Mutex);
    _CondVar->Count--;
}

void PostCondVar(CondVar* _CondVar)
{
    sem_post(&_CondVar->Semaphore);
}