#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"
#include "hash.h"
#include "bitmap.h"
#define LIST 1
#define HASH 2
#define BITMAP 3
struct list List[15];
struct hash Hash[15];
struct bitmap* Bitmap[15];
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
    int Type = 0;
    while(1){
        fgets(input,100,stdin);
        parser(input,&argc,argv,", \f\n\r\t\v");
        
        if(!strcmp(argv[0],"quit"))
            break;
        if(!strcmp(argv[0],"create")){
            if(!strcmp(argv[1],"list")){
                Type = LIST;
                int idx = argv[2][4]-'0';
                list_init(&List[idx]);
            }
            else if(!strcmp(argv[1],"hashtable")){

            }
            else if(!strcmp(argv[1],"bitmap")){
            
            }
        }
        else if(!strcmp(argv[0],"delete")){
            if(Type == LIST){
                int idx = argv[1][4]-'0';
                while(!list_empty(&List[idx])){
                    free(list_pop_back(&List[idx]));
                }
            }
            else if(Type == HASH){

            }
            else if(Type == BITMAP){

            }
        }
        else if(!strcmp(argv[0],"dumpdata")){
            if(Type == LIST){
                int idx = argv[1][4]-'0';
                for(struct list_elem* i = list_begin(&List[idx]);i!=list_end(&List[idx]);i=list_next(i)){
                    struct list_node* node = list_entry(i,struct list_node,elem);
                    printf("%d ", node->data);
                }
                printf("\n");
            }
            else if(Type == HASH){

            }
            else if(Type == BITMAP){

            }
        }
        ////////////////////////////////////////////// LIST
        else if(!strcmp(argv[0],"list_push_back")){

        }
        else if(!strcmp(argv[0],"list_push_front")){

        }
        else if(!strcmp(argv[0],"list_pop_back")){

        }
        else if(!strcmp(argv[0],"list_pop_front")){

        }
        else if(!strcmp(argv[0],"list_size")){

        }
        else if(!strcmp(argv[0],"list_insert")){

        }
        else if(!strcmp(argv[0],"list_empty")){

        }
        else if(!strcmp(argv[0],"list__back")){

        }
        else if(!strcmp(argv[0],"list_front")){

        }
        else if(!strcmp(argv[0],"list_min")){

        }
        else if(!strcmp(argv[0],"list_max")){

        }
        else if(!strcmp(argv[0],"list_remove")){

        }
        else if(!strcmp(argv[0],"list_reverse")){

        }
        else if(!strcmp(argv[0],"list_sort")){

        }
        else if(!strcmp(argv[0],"list_splice")){

        }
        else if(!strcmp(argv[0],"list_swap")){

        }
        else if(!strcmp(argv[0],"list_shuffle")){

        }
        else if(!strcmp(argv[0],"list_unique")){

        }
        ///////////////////////////////////////////////
    }
    return 0;
}
