#include "barrier.h"
#include <stdio.h>
#include <stdlib.h>

reusableBarrier RB;

void *work(void* args){
    while(RB.etapa<=RB.maxE){
        int random = rand()%50;
        _sleep(random*10);
        printf("Hebra [%d] esperando en etapa %d \n", _threadid, RB.etapa);
        useRB(&RB);
        printf("Hebra [%d] paso barrera y esta ahora en etapa %d \n", _threadid, RB.etapa);
    }
}


int main(int argc, char* argv[]){
    int N = 5; //Numero de hebras
    int E = 4; //Cantidad de etapas
    if (argc>1){
        int auxN = atoi(argv[1]);
        if (auxN>0) N=auxN;
        if (argc>2){
            int auxE = atoi(argv[2]);
            if (auxE>0) E=auxE;
        }
    }
    initRB(&RB,N, E);

    pthread_t *procesos = malloc(N* sizeof *procesos);
    if (!procesos) { perror("malloc"); return 1; }

    for (int i = 0; i<N; i++){
        pthread_create(&procesos[i], NULL, work, NULL);
    }
    for (int i = 0; i<N; i++){
        pthread_join(procesos[i], NULL);
    }
    free(procesos);
    destroyRB(&RB);
    return 0;
}
