#include "barrier.h"

void initRB(reusableBarrier *RB, int T, int E){
    RB->waitingT = 0;
    RB->maxT = T;
    RB->etapa = 1;
    RB->maxE = E;
    pthread_mutex_init(&RB->barrierLock, NULL);
    pthread_cond_init(&RB->waitingCondition, NULL);
}

void useRB(reusableBarrier* RB){
    pthread_mutex_lock(&RB->barrierLock);
    int doubleCheck = RB->etapa;
    RB->waitingT++;
    if(RB->waitingT < RB->maxT){
        while (doubleCheck == RB->etapa){
            pthread_cond_wait(&RB->waitingCondition,&RB->barrierLock);
        }
    } else{
        RB->waitingT = 0;
        RB->etapa++;
        pthread_cond_broadcast(&RB->waitingCondition);
    }
    pthread_mutex_unlock(&RB->barrierLock);
}

void destroyRB(reusableBarrier* RB){
    pthread_mutex_destroy(&RB->barrierLock);
    pthread_cond_destroy(&RB->waitingCondition);
}
