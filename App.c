#include "Monitor.h"
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>



Mutex MyMutex;
CondVar MyCondVar;
int SubsNo = 0;

typedef struct
{
    int Balance;
} Account;
Account Acc = {0}; // un cont cu balanta initiala 0 de lei

typedef struct
{
    char Name[20];
    int Price;
    pthread_t Thread;
} Subscription;

#define MAX_SUBSCRIPTIONS 10
Subscription Subs[MAX_SUBSCRIPTIONS];

void* SubscriptionPayment(void* _Sub)
{
    LockMutex(&MyMutex);

    printf("Payment attempt\n");
    while(Acc.Balance < (*(Subscription*)_Sub).Price)
    {
        printf("Insufficient funds for %s payment\n", (*(Subscription*)_Sub).Name);
        WaitCondVar(&MyCondVar, &MyMutex);
        printf("Payment attempt\n");
    }
    Acc.Balance -= (*(Subscription*)_Sub).Price;
    PostCondVar(&MyCondVar);
    printf("Payment successful to %s. Remaining in your account: %d\n", (*(Subscription*)_Sub).Name, Acc.Balance);

    UnlockMutex(&MyMutex);
}

void DepositMoney(int _Amount)
{
    LockMutex(&MyMutex);

    Acc.Balance += _Amount;
    printf("Money deposited successfully. Current amount: %d\n", Acc.Balance);
    PostCondVar(&MyCondVar);

    UnlockMutex(&MyMutex);
}

void AskBalance()
{
    LockMutex(&MyMutex);

    printf("Your account balance is %d\n", Acc.Balance);

    UnlockMutex(&MyMutex);
}

void CreateNewSubscription()
{
    if (SubsNo < sizeof(Subs) / sizeof(Subs[0]))
    {
        printf("Enter subscription name: ");
        scanf("%s", Subs[SubsNo].Name);

        printf("Enter subscription price: ");
        scanf("%d", &Subs[SubsNo].Price);

        // Creare thread pentru gestionarea abonamentului
        pthread_create(&Subs[SubsNo].Thread, NULL, SubscriptionPayment, &Subs[SubsNo]);

        SubsNo++;
    }
    else
    {
        printf("Cannot create new subscription. Subscription limit reached.\n");
    }
}


int main()
{
    InitMutex(&MyMutex);
    InitCondVar(&MyCondVar);

    int _Choice;
    do 
    {
        printf("\nMenu:\n");
        printf("1. Deposit money\n");
        printf("2. Ask balance\n");
        printf("3. Create new subscription\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &_Choice);

        switch (_Choice) 
        {
            case 1:
                int _Amount;
                printf("Enter deposit amount: ");
                scanf("%d", &_Amount);
                printf("\n");
                DepositMoney(_Amount);
                break;
            case 2:
                AskBalance();
                break;
            case 3:
                CreateNewSubscription();
                break;
            case 0:
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }

        sleep(2);

    } while (_Choice != 0);

    for(int i = 0; i < SubsNo; i++)
    {
        pthread_join(Subs[i].Thread, NULL);
    }

    DestroyMutex(&MyMutex);
    DestroyCondVar(&MyCondVar);

    return 0;
}
