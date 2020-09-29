#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"
#include "hash.h"
#include "bitmap.h"
#define LIST 1
#define HASH 2
#define BITMAP 3
struct list_item{
    struct list_elem elem;
    int data;
};
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
bool list_comp(const struct list_elem* a, const struct list_elem* b, void* aux){
    struct list_item* itemA = list_entry(a,struct list_item,elem);
    struct list_item* itemB = list_entry(b,struct list_item,elem);
    return itemA->data < itemB->data;
}
int main(){
    char input[100];
    int argc;
    char argv[100][100];
    int Type = 0;
    while(1){
        fgets(input,100,stdin);
        argv[0][0] = 0;
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
                    struct list_elem* back = list_pop_back(&List[idx]);
                    struct list_item* item = list_entry(back,struct list_item,elem);
                    free(item);
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
                    struct list_item* item = list_entry(i,struct list_item,elem);
                    printf("%d ", item->data);
                }
                if(!list_empty(&List[idx]))
                    printf("\n");
            }
            else if(Type == HASH){

            }
            else if(Type == BITMAP){

            }
        }
        ////////////////////////////////////////////// LIST
        else if(!strcmp(argv[0],"list_push_back")){
            int idx = argv[1][4] - '0';
            int data;
            sscanf(argv[2], "%d", &data);
            struct list_item* newNode = (struct list_item*)malloc(sizeof(struct list_item));
            newNode->data = data;
            list_push_back(&List[idx],&(newNode->elem));
        }
        else if(!strcmp(argv[0],"list_push_front")){
            int idx = argv[1][4] - '0';
            int data;
            sscanf(argv[2], "%d", &data);
            struct list_item* newNode = (struct list_item*)malloc(sizeof(struct list_item));
            newNode->data = data;
            list_push_front(&List[idx],&(newNode->elem));

        }
        else if(!strcmp(argv[0],"list_pop_back")){
            int idx = argv[1][4]-'0';
            struct list_elem* back = list_pop_back(&List[idx]);
            struct list_item* item = list_entry(back,struct list_item,elem);
            free(item);
        }
        else if(!strcmp(argv[0],"list_pop_front")){
            int idx = argv[1][4]-'0';
            struct list_elem* back = list_pop_front(&List[idx]);
            struct list_item* item = list_entry(back,struct list_item,elem);
            free(item);

        }
        else if(!strcmp(argv[0],"list_size")){
            int idx = argv[1][4]-'0';
            printf("%d\n",(int)list_size(&List[idx]));
        }
        else if(!strcmp(argv[0],"list_insert")){
            int idx = argv[1][4]-'0';
            int pos,data;
            sscanf(argv[2],"%d",&pos);
            sscanf(argv[3],"%d",&data);
            struct list_item* newNode = (struct list_item*)malloc(sizeof(struct list_item));
            newNode->data = data;
            if(pos > (int)list_size(&List[idx])-1){
                list_push_back(&List[idx],&(newNode->elem));
            }
            else if(pos == 0)
                list_push_front(&List[idx],&(newNode->elem));
            else{
                struct list_elem* bef = list_findXth(&(List[idx]),pos);
                list_insert(bef,&(newNode->elem));
            }
        }
        else if(!strcmp(argv[0],"list_empty")){
            int idx = argv[1][4]-'0';
            if(list_empty(&List[idx]))
                printf("true\n");
            else
                printf("false\n");
        }
        else if(!strcmp(argv[0],"list_back")){
            int idx = argv[1][4]-'0';
            struct list_elem* node = list_back(&List[idx]);
            struct list_item* item = list_entry(node,struct list_item,elem);
            printf("%d\n",item->data);
        }
        else if(!strcmp(argv[0],"list_front")){
            int idx = argv[1][4]-'0';
            struct list_elem* node = list_front(&List[idx]);
            struct list_item* item = list_entry(node,struct list_item,elem);
            printf("%d\n",item->data);
        }
        else if(!strcmp(argv[0],"list_min")){
            int idx = argv[1][4]-'0';
            struct list_elem* node = list_min(&List[idx],list_comp,NULL);
            struct list_item* item = list_entry(node,struct list_item,elem);
            printf("%d\n",item->data);
        }
        else if(!strcmp(argv[0],"list_max")){
            int idx = argv[1][4]-'0';
            struct list_elem* node = list_max(&List[idx],list_comp,NULL);
            struct list_item* item = list_entry(node,struct list_item,elem);
            printf("%d\n",item->data);
        }
        else if(!strcmp(argv[0],"list_remove")){
            int idx = argv[1][4]-'0';
            int pos;
            sscanf(argv[2],"%d",&pos);
            struct list_elem* node = list_findXth(&List[idx],pos);
            struct list_item* item = list_entry(node,struct list_item,elem);
            list_remove(node);
            free(item);
        }
        else if(!strcmp(argv[0],"list_reverse")){
            int idx = argv[1][4]-'0';
            list_reverse(&List[idx]);
        }
        else if(!strcmp(argv[0],"list_sort")){
            int idx = argv[1][4]-'0';
            list_sort(&List[idx],list_comp,NULL);
        }
        else if(!strcmp(argv[0],"list_splice")){

        }
        else if(!strcmp(argv[0],"list_swap")){
            int idx = argv[1][4]-'0';
            int num1,num2;
            sscanf(argv[2],"%d",&num1);
            sscanf(argv[3],"%d",&num2);
            list_swap(list_findXth(&List[idx],num1),list_findXth(&List[idx],num2));
        }
        else if(!strcmp(argv[0],"list_shuffle")){
            int idx = argv[1][4]-'0';
            list_shuffle(&List[idx]);
        }
        else if(!strcmp(argv[0],"list_unique")){

        }
        ///////////////////////////////////////////////
    }
    return 0;
}
