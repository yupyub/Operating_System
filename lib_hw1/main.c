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
                Type = BITMAP;
                int idx = argv[2][2]-'0';
                size_t siz;
                sscanf(argv[3],"%zu",&siz);
                Bitmap[idx] = bitmap_create(siz);
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
                int idx = argv[1][2]-'0';
                bitmap_destroy(Bitmap[idx]);
                Bitmap[idx] = NULL;
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
                int idx = argv[1][2]-'0';
                if(Bitmap[idx] == NULL)
                    continue;
                size_t siz = bitmap_size(Bitmap[idx]);
                for(size_t i = 0;i<siz;i++)
                    printf("%d",bitmap_test(Bitmap[idx],i)?1:0);
                printf("\n");
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
            int idx1 = argv[1][4]-'0';
            int idx2 = argv[3][4]-'0';
            int pos1,pos2,pos3;
            sscanf(argv[2],"%d",&pos1);
            sscanf(argv[4],"%d",&pos2);
            sscanf(argv[5],"%d",&pos3);
            struct list_elem* node1 = list_findXth(&List[idx1],pos1);
            struct list_elem* node2 = list_findXth(&List[idx2],pos2);
            struct list_elem* node3 = list_findXth(&List[idx2],pos3);
            list_splice(node1,node2,node3);
        }
        else if(!strcmp(argv[0],"list_swap")){
            int idx = argv[1][4]-'0';
            int num1,num2;
            sscanf(argv[2],"%d",&num1);
            sscanf(argv[3],"%d",&num2);
            struct list_elem* node1 = list_findXth(&List[idx],num1);
            struct list_elem* node2 = list_findXth(&List[idx],num2);
            list_swap(node1,node2);
        }
        else if(!strcmp(argv[0],"list_shuffle")){
            int idx = argv[1][4]-'0';
            list_shuffle(&List[idx]);
        }
        else if(!strcmp(argv[0],"list_unique")){
            if(argc == 2){
                int idx = argv[1][4]-'0';
                list_unique(&List[idx],NULL,list_comp,NULL);
            }
            else if(argc == 3){
                int idx1 = argv[1][4]-'0';
                int idx2 = argv[2][4]-'0';
                list_unique(&List[idx1],&List[idx2],list_comp,NULL);
            }

        }
        else if(!strcmp(argv[0],"list_insert_ordered")){
            int idx = argv[1][4]-'0';
            int data;
            sscanf(argv[2],"%d",&data);
            struct list_item* newNode = (struct list_item*)malloc(sizeof(struct list_item));
            newNode->data = data;
            list_insert_ordered(&List[idx],&(newNode->elem),list_comp,NULL);
        }
        ///////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////
        else if(!strcmp(argv[0],"bitmap_all")){
        }
        else if(!strcmp(argv[0],"bitmap_any")){
        }
        else if(!strcmp(argv[0],"bitmap_contains")){
        }
        else if(!strcmp(argv[0],"bitmap_count")){
        }
        else if(!strcmp(argv[0],"bitmap_dump")){
        }
        else if(!strcmp(argv[0],"bitmap_expand")){
        }
        else if(!strcmp(argv[0],"bitmap_flip")){
        }
        else if(!strcmp(argv[0],"bitmap_mark")){

        }
        else if(!strcmp(argv[0],"bitmap_none")){
        }
        else if(!strcmp(argv[0],"bitmap_plain")){
        }
        else if(!strcmp(argv[0],"bitmap_reset")){
        }
        else if(!strcmp(argv[0],"bitmap_scan")){
        }
        else if(!strcmp(argv[0],"bitmap_scan_and_flip")){
        }
        else if(!strcmp(argv[0],"bitmap_set")){
        }
        else if(!strcmp(argv[0],"bitmap_set_all")){
        }
        else if(!strcmp(argv[0],"bitmap_set_multiple")){
        }
        else if(!strcmp(argv[0],"bitmap_size")){
        }
        else if(!strcmp(argv[0],"bitmap_test")){
        }
    }
    return 0;
}
