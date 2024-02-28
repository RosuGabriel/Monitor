#ifndef Monitor_h

#define Monitor_h



#include <semaphore.h>
#include <stdbool.h>



typedef struct
{
    sem_t Semaphore;
} Mutex;

bool InitMutex(Mutex* _Mutex);

void DestroyMutex(Mutex* _Mutex);

void LockMutex(Mutex* _Mutex);

void UnlockMutex(Mutex* _Mutex);



typedef struct
{
    sem_t Semaphore;
    int Count;
} CondVar;

bool InitCondVar(CondVar* _CondVar);

void DestroyCondVar(CondVar* _CondVar);

void WaitCondVar(CondVar* _CondVar, Mutex* _Mutex);

void PostCondVar(CondVar* _CondVar);



#endif