#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"
#include "hash.h"
#include "bitmap.h"
struct list List[10];
struct hash Hash[10];
struct bitmap* Bitmap[10];
void parser(char str[], int* argc, char argv[100][100],char sep[]){
    char *token;
    *argc = 0;
    token = strtok(str,sep);
    while(token != NULL){
        strcpy(argv[(*argc)++],token);
        token = strtok(NULL,sep);
    }
}

int main(){
    char input[100];
    int argc;
    char argv[100][100];
    while(1){
        fgets(input,100,stdin);
        parser(input,&argc,argv,", \f\n\r\t\v");
        
        if(!strcmp(argv[0],"quit"))
            break;



    }
    return 0;
}
