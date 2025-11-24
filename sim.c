#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define C_RED "\033[31m"
#define C_RESET "\033[0m"
#define USAGE printf(C_RED"USAGE: ./sim Nmarcos tamanomarco [--verbose] traza.txt"C_RESET)

typedef unsigned va; //virtual adress
typedef unsigned pa; //physical adress

unsigned Nmarcos;
unsigned marcosize;
char verbose;


pa translate(va npv){

}

int main(int argc, char *argv[]){
    ///EXTRACT ARGS
    if (argc<4){
        USAGE; return 1;
    }
    Nmarcos = atoi(argv[1]);
    marcosize = atoi(argv[2]);
    verbose=(argc==5);
    //checking --verbose
    if (verbose && strcmp(argv[3],"--verbose")!=0){
        USAGE; return 1;
    }
    //trace file
    FILE* trace;
    if (verbose){
        trace = fopen(argv[4],"r");
    }else{
        trace = fopen(argv[3],"r");
    }
    if (trace==NULL){
        printf(C_RED"FILE NOT FOUND\n"C_RESET);
        USAGE; return 1;
    }

    ///find page bits
    unsigned b=0;
    while (marcosize>1)
    {
        marcosize>>=1;
        b++;
    }
    va PAGE_SIZE=1<<b;
    va MASK=PAGE_SIZE-1;
    //offset = VA & MASK,
    //npv = VA ≫ b.

    //create page table
    pa valid_bit = 1<<(sizeof(va)*8-b);
    pa dirty_bit = valid_bit<<1;
    pa *PAGE_TABLE = (pa *)calloc(1<<(sizeof(va)*8-b),sizeof(pa));
    pa TABLE_MASK = (~0)>>b;

    //read adresses
    int hits=0;
    int fallos=0;
    while (!feof(trace)){
        va VA;
        fscanf(trace,"0x%x\n",&VA);
        //printf("0x%x\n",VA);

        va offset = VA & MASK;
        va npv = VA >> b;

        //getting page table entry
        pa pte = PAGE_TABLE[npv];
        pa marco;
        if (pte & valid_bit){
            //HIT
            marco = pte & TABLE_MASK;
            hits++;
        }else{
            //FALLO
            marco = 16;
            PAGE_TABLE[npv] = valid_bit | dirty_bit | marco;
            printf("FALLO DE PAGINA %x\n",PAGE_TABLE[npv]);
            fallos++;
        }

        pa PA = (marco << b) | (pa)offset;

        //printf("0x%x\n",PA);
    }

    printf("HIT %d / FALLOS %d",hits,fallos);

}