#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define C_RED "\033[31m"
#define C_RESET "\033[0m"
#define USAGE printf(C_RED"USAGE: ./sim Nmarcos tamanomarco [--verbose] traza.txt"C_RESET)

unsigned Nmarcos;
unsigned marcosize;
char verbose;

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
    unsigned PAGE_SIZE=1<<b;
    unsigned MASK=PAGE_SIZE-1;
    //offset = VA & MASK,
    //npv = VA ≫ b.

    //read entries
    int VA;
    fscanf(trace,"0x%x",&VA);
    printf("0x%x",VA);

}