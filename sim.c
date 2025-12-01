#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(a,b) (a>b)?a:b

#define C_RED "\033[31m"
#define C_RESET "\033[0m"
#define USAGE printf(C_RED"USAGE: ./sim Nmarcos tamanomarco [--verbose] traza.txt"C_RESET)

typedef unsigned va; //virtual adress
typedef unsigned pa; //physical adress

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
    unsigned temp = marcosize-1;
    while (temp>0)
    {
        temp>>=1;
        b++;
    }
    va PAGE_SIZE=1<<b;
    va MASK=PAGE_SIZE-1;
    //offset = VA & MASK,
    //npv = VA ≫ b.

    //create page table
    pa valid_bit = 1<<(sizeof(pa)*8-1);
    pa used_bit = 1<<(sizeof(pa)*8-2);
    pa *PAGE_TABLE = (pa *)calloc(1<<(sizeof(va)*8-(max(b,4))),sizeof(pa)); //el max es porque el programa no funciona si la tabla es demasiado grando, y no usamos todos los bits de va
    pa TABLE_MASK = (1<<(sizeof(pa)*8-2))-1;

    //marcos disponible (para algoritmo de reloj)
    va *FRAMES = (va *)calloc(Nmarcos,sizeof(va));
    va clock_valid_bit = 1<<(sizeof(va)*8-1);
    va clock_used_bit = 1<<(sizeof(va)*8-2);
    va CLOCK_MASK = (1<<(sizeof(va)*8-2))-1;
    pa clock=0;

    //read adresses
    int hits=0;
    int fallos=0;
    while (!feof(trace)){
        va VA;
        fscanf(trace,"0x%x\n",&VA);

        va offset = VA & MASK;
        va npv = VA >> b;

        //getting page table entry
        pa pte = PAGE_TABLE[npv];
        pa marco;

        char ishit=0;
        if (pte & valid_bit){
            //HIT
            marco = pte & TABLE_MASK;
            //marcamos como usado
            PAGE_TABLE[npv] = valid_bit | used_bit | marco;
            FRAMES[marco] = clock_valid_bit | clock_used_bit | npv;

            hits++;
            ishit=1;
        }else{
            //FALLO
            while (1)
            {
                va clockentry = FRAMES[clock];

                //revisa si es valido
                if (clockentry & clock_valid_bit){
                    //revisa used bit
                    if (clockentry & clock_used_bit){
                        //marcamos como no usado
                        PAGE_TABLE[clockentry & CLOCK_MASK] &= ~used_bit;
                        FRAMES[clock] &= ~clock_used_bit;
                    }else{
                        //quitamos marco de pagina
                        PAGE_TABLE[clockentry & CLOCK_MASK] &= ~valid_bit;
                        //lo damos a npv
                        FRAMES[clock] = clock_valid_bit | clock_used_bit | npv;
                        PAGE_TABLE[npv] = valid_bit | used_bit | clock;
                        break;
                    }
                }else{
                    //toma el marco
                    PAGE_TABLE[npv] = valid_bit | used_bit | clock;
                    FRAMES[clock] = clock_valid_bit | clock_used_bit | npv;
                    break;
                }

                clock++;
                clock%=Nmarcos;
            }
            
            marco = clock;

            clock++;
            clock%=Nmarcos;

            //printf("FALLO DE PAGINA page: %x, marco:0x%x\n",npv,marco);
            fallos++;
        }

        pa PA = (marco << b) | (pa)offset;

        if (verbose) printf("0x%6x | %3d | 0x%x | %s | %3d | 0x%x\n",VA,npv,offset,(ishit)?" HIT ":"FALLO",marco,PA);
    }
    //explica el significado de las salidas de verbose
    if (verbose) printf("Virtual Addres | npv | offset | HIT/FALLO | marco | Physical Addres\n");

    printf("Refs: %d | PageFaults: %d | FaultRate: %d%%",hits+fallos,fallos,(100*fallos)/(hits+fallos));

}