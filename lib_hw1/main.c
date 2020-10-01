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
struct hash_item{
    struct hash_elem elem;
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
bool list_comp1(const struct list_elem* a, const struct list_elem* b, void* aux){
    struct list_item* itemA = list_entry(a,struct list_item,elem);
    struct list_item* itemB = list_entry(b,struct list_item,elem);
    return itemA->data < itemB->data;
}
unsigned hash_func1 (const struct hash_elem *e, void *aux){
    struct hash_item* node = hash_entry(e,struct hash_item,elem);
    return hash_int(node->data);
}
unsigned hash_func2 (const struct hash_elem *e, void *aux){
    struct hash_item* node = hash_entry(e,struct hash_item,elem);
    return hash_int_2(node->data);
}
bool hash_comp1 (const struct hash_elem *a, const struct hash_elem *b, void *aux){
    struct hash_item* itemA = hash_entry(a,struct hash_item,elem);
    struct hash_item* itemB = hash_entry(b,struct hash_item,elem);
    return itemA->data < itemB->data;
}
void hash_destructor(struct hash_elem *e, void *aux){
    struct hash_item* node = hash_entry(e,struct hash_item,elem);
    free(node);
}
void hash_print_data(struct hash_elem *e, void *aux){
    struct hash_item* node = hash_entry(e,struct hash_item,elem);
    printf("%d ",node->data);
}
void hash_square(struct hash_elem *e, void *aux){
    struct hash_item* node = hash_entry(e,struct hash_item,elem);
    node->data = (node->data)*(node->data);
}
void hash_triple(struct hash_elem *e, void *aux){
    struct hash_item* node = hash_entry(e,struct hash_item,elem);
    node->data = (node->data)*(node->data)*(node->data);
}

list_less_func* LIST_LESS = list_comp1;
/////// Choose one :: hash_int or hash_int_2 function    
hash_hash_func* HASH_FUNC = hash_func1;
//hash_hash_func* HASH_FUNC = hash_func2;
///////
hash_less_func* HASH_LESS = hash_comp1;
hash_action_func* HASH_DESTRUCTOR = hash_destructor;
hash_action_func* HASH_PRINT_DATA = hash_print_data;
hash_action_func* HASH_SQUARE = hash_square;
hash_action_func* HASH_TRIPLE = hash_triple;


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
                Type = HASH;
                int idx = argv[2][4]-'0';
                hash_init(&Hash[idx],HASH_FUNC,HASH_LESS,NULL);
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
                int idx = argv[1][4]-'0';
                hash_destroy(&Hash[idx],HASH_DESTRUCTOR);
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
                int idx = argv[1][4]-'0';
                if(hash_empty(&Hash[idx]))
                    continue;
                hash_apply(&Hash[idx],HASH_PRINT_DATA);
                printf("\n");
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
            struct list_elem* node = list_min(&List[idx],LIST_LESS,NULL);
            struct list_item* item = list_entry(node,struct list_item,elem);
            printf("%d\n",item->data);
        }
        else if(!strcmp(argv[0],"list_max")){
            int idx = argv[1][4]-'0';
            struct list_elem* node = list_max(&List[idx],LIST_LESS,NULL);
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
            list_sort(&List[idx],LIST_LESS,NULL);
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
                list_unique(&List[idx],NULL,LIST_LESS,NULL);
            }
            else if(argc == 3){
                int idx1 = argv[1][4]-'0';
                int idx2 = argv[2][4]-'0';
                list_unique(&List[idx1],&List[idx2],LIST_LESS,NULL);
            }

        }
        else if(!strcmp(argv[0],"list_insert_ordered")){
            int idx = argv[1][4]-'0';
            int data;
            sscanf(argv[2],"%d",&data);
            struct list_item* newNode = (struct list_item*)malloc(sizeof(struct list_item));
            newNode->data = data;
            list_insert_ordered(&List[idx],&(newNode->elem),LIST_LESS,NULL);
        }
        ///////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////
        else if(!strcmp(argv[0],"hash_apply")){
            int idx = argv[1][4]-'0';
            if(!strcmp(argv[2],"square"))
                hash_apply(&Hash[idx],HASH_SQUARE);
            else if(!strcmp(argv[2],"triple"))
                hash_apply(&Hash[idx],HASH_TRIPLE);
        }
        else if(!strcmp(argv[0],"hash_find")){
            int idx = argv[1][4]-'0';
            int data;
            sscanf(argv[2],"%d",&data);
            struct hash_item* temp = (struct hash_item*)malloc(sizeof(struct hash_elem));
            temp->data = data;
            struct hash_elem* e = hash_find(&Hash[idx],&(temp->elem));
            free(temp);
            if(e!=NULL)
                printf("%d\n",data);
        }
        else if(!strcmp(argv[0],"hash_empty")){
            int idx = argv[1][4]-'0';
            if(hash_empty(&Hash[idx]))
                printf("true\n");
            else
                printf("false\n");
        }
        else if(!strcmp(argv[0],"hash_size")){
            int idx = argv[1][4]-'0';
            size_t siz = hash_size(&Hash[idx]);
            printf("%zu\n",siz);
        }
        else if(!strcmp(argv[0],"hash_clear")){
            int idx = argv[1][4]-'0';
            hash_clear(&Hash[idx],HASH_DESTRUCTOR);
        }
        else if(!strcmp(argv[0],"hash_insert")){
            int idx = argv[1][4]-'0';
            int data;
            sscanf(argv[2],"%d",&data);
            struct hash_item* newNode = (struct hash_item*)malloc(sizeof(struct hash_item));
            newNode->data = data;
            hash_insert(&Hash[idx],&(newNode->elem));
        }
        else if(!strcmp(argv[0],"hash_replace")){
            int idx = argv[1][4]-'0';
            int data;
            sscanf(argv[2],"%d",&data);
            struct hash_item* newNode = (struct hash_item*)malloc(sizeof(struct hash_item));
            newNode->data = data;
            struct hash_elem* ret = hash_replace(&Hash[idx],&(newNode->elem));
            HASH_DESTRUCTOR(ret,NULL);
        }
        else if(!strcmp(argv[0],"hash_delete")){
            int idx = argv[1][4]-'0';
            int data;
            sscanf(argv[2],"%d",&data);
            struct hash_item* temp = (struct hash_item*)malloc(sizeof(struct hash_item));
            temp->data = data;
            struct hash_elem* ret = hash_delete(&Hash[idx],&(temp->elem));
            free(temp);
            HASH_DESTRUCTOR(ret,NULL);
        }
        ///////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////
        else if(!strcmp(argv[0],"bitmap_all")){
            int idx = argv[1][2]-'0';
            size_t st,cnt;
            sscanf(argv[2],"%zu",&st);
            sscanf(argv[3],"%zu",&cnt);
            if(bitmap_all(Bitmap[idx],st,cnt))
                printf("true\n");
            else
                printf("false\n");
        }
        else if(!strcmp(argv[0],"bitmap_any")){
            int idx = argv[1][2]-'0';
            size_t st,cnt;
            sscanf(argv[2],"%zu",&st);
            sscanf(argv[3],"%zu",&cnt);
            if(bitmap_any(Bitmap[idx],st,cnt))
                printf("true\n");
            else
                printf("false\n");
        }
        else if(!strcmp(argv[0],"bitmap_contains")){
            int idx = argv[1][2]-'0';
            size_t st,cnt;
            sscanf(argv[2],"%zu",&st);
            sscanf(argv[3],"%zu",&cnt);
            bool ret = false;
            if(!strcmp(argv[4],"true"))
                ret = bitmap_contains(Bitmap[idx],st,cnt,true);
            else
                ret = bitmap_contains(Bitmap[idx],st,cnt,false);
            if(ret)
                printf("true\n");
            else
                printf("false\n");
        }
        else if(!strcmp(argv[0],"bitmap_count")){
            int idx = argv[1][2]-'0';
            size_t st,cnt;
            sscanf(argv[2],"%zu",&st);
            sscanf(argv[3],"%zu",&cnt);
            size_t ret = 0;
            if(!strcmp(argv[4],"true"))
                ret = bitmap_count(Bitmap[idx],st,cnt,true);
            else
                ret = bitmap_count(Bitmap[idx],st,cnt,false);
            printf("%zu\n",ret);
        }
        else if(!strcmp(argv[0],"bitmap_dump")){
            int idx = argv[1][2]-'0';
            bitmap_dump(Bitmap[idx]);
        }
        else if(!strcmp(argv[0],"bitmap_expand")){
            int idx = argv[1][2]-'0';
            int siz;
            sscanf(argv[2],"%d",&siz);
            Bitmap[idx] = bitmap_expand(Bitmap[idx],siz);
        }
        else if(!strcmp(argv[0],"bitmap_flip")){
            int idx = argv[1][2]-'0';
            size_t pos;
            sscanf(argv[2],"%zu",&pos);
            bitmap_flip(Bitmap[idx],pos);
        }
        else if(!strcmp(argv[0],"bitmap_mark")){
            int idx = argv[1][2]-'0';
            size_t pos;
            sscanf(argv[2],"%zu",&pos);
            bitmap_mark(Bitmap[idx],pos);
        }
        else if(!strcmp(argv[0],"bitmap_none")){
            int idx = argv[1][2]-'0';
            size_t st,cnt;
            sscanf(argv[2],"%zu",&st);
            sscanf(argv[3],"%zu",&cnt);
            size_t ret = BITMAP_ERROR;
            if(bitmap_none(Bitmap[idx],st,cnt))
                printf("true\n");
            else
                printf("false\n");
        }
        else if(!strcmp(argv[0],"bitmap_reset")){
            int idx = argv[1][2]-'0';
            size_t pos;
            sscanf(argv[2],"%zu",&pos);
            bitmap_reset(Bitmap[idx],pos);
        }
        else if(!strcmp(argv[0],"bitmap_scan")){
            int idx = argv[1][2]-'0';
            size_t st,cnt;
            sscanf(argv[2],"%zu",&st);
            sscanf(argv[3],"%zu",&cnt);
            size_t ret = BITMAP_ERROR;
            if(!strcmp(argv[4],"true"))
                ret = bitmap_scan(Bitmap[idx],st,cnt,true);
            else
                ret = bitmap_scan(Bitmap[idx],st,cnt,false);
            printf("%zu\n",ret);
        }
        else if(!strcmp(argv[0],"bitmap_scan_and_flip")){
            int idx = argv[1][2]-'0';
            size_t st,cnt;
            sscanf(argv[2],"%zu",&st);
            sscanf(argv[3],"%zu",&cnt);
            size_t ret = BITMAP_ERROR;
            if(!strcmp(argv[4],"true"))
                ret = bitmap_scan_and_flip(Bitmap[idx],st,cnt,true);
            else
                ret = bitmap_scan_and_flip(Bitmap[idx],st,cnt,false);
            printf("%zu\n",ret);
        }
        else if(!strcmp(argv[0],"bitmap_set")){
            int idx = argv[1][2]-'0';
            size_t pos;
            sscanf(argv[2],"%zu",&pos);
            if(!strcmp(argv[3],"true"))
                bitmap_set(Bitmap[idx],pos,true);
            else
                bitmap_set(Bitmap[idx],pos,false);
        }
        else if(!strcmp(argv[0],"bitmap_set_all")){
            int idx = argv[1][2]-'0';
            if(!strcmp(argv[2],"true"))
                bitmap_set_all(Bitmap[idx],true);
            else
                bitmap_set_all(Bitmap[idx],false);
        }
        else if(!strcmp(argv[0],"bitmap_set_multiple")){
            int idx = argv[1][2]-'0';
            size_t pos,len;
            sscanf(argv[2],"%zu",&pos);
            sscanf(argv[3],"%zu",&len);
            if(!strcmp(argv[4],"true"))
                bitmap_set_multiple(Bitmap[idx],pos,len,true);
            else
                bitmap_set_multiple(Bitmap[idx],pos,len,false);
        }
        else if(!strcmp(argv[0],"bitmap_size")){
            int idx = argv[1][2]-'0';
            size_t siz = bitmap_size(Bitmap[idx]);
            printf("%zu\n",siz);
        }
        else if(!strcmp(argv[0],"bitmap_test")){
            int idx = argv[1][2]-'0';
            size_t pos;
            sscanf(argv[2],"%zu",&pos);
            printf("%s\n",bitmap_test(Bitmap[idx],pos)?"true":"false");
        }
    }
    return 0;
}
