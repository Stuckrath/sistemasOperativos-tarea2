#include <pthread.h>
typedef struct ReusableBarrier {
    int maxT;
    int waitingT;
    int etapa;
    int maxE;
    pthread_cond_t waitingCondition;
    pthread_mutex_t barrierLock;
} reusableBarrier;
void initRB(reusableBarrier *RB, int T, int E);
void useRB(reusableBarrier *RB);
void destroyRB(reusableBarrier *RB);