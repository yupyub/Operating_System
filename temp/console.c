#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "bitmap.h"
#include "hash.h"
#include "console.h"

char** tokenize_str(char* str, int* token_cnt);
struct list_elem* n_th_elem(int index, int node_index);

struct list List[10];
struct bitmap* Bitmap[10];
struct hash Hash[10];
struct hash_iterator hash_itr;

struct hash_node{
	struct hash_elem elem;
	int data;
};

struct list_node{
	struct list_elem elem;
	int data;
};

static bool list_less(const struct list_elem *a, const struct list_elem *b, void *aux){
	struct list_node* anode =list_entry(a,struct list_node,elem);
	struct list_node* bnode =list_entry(b,struct list_node,elem);
	return anode->data < bnode ->data;
}

unsigned hash_hash(const struct hash_elem *a,void *aux){
	struct hash_node* node = hash_entry(a,struct hash_node,elem);
	return hash_int_2(node->data);
}

bool hash_less(const struct hash_elem *a, const struct hash_elem *b,void *aux){
	struct hash_node *anode = hash_entry(a,struct hash_node,elem);
	struct hash_node *bnode = hash_entry(b,struct hash_node,elem);
	return anode->data < bnode->data;
}

void destructor(struct hash_elem *a,void *aux){
	struct hash_node *node=hash_entry(a,struct hash_node,elem);
	node=NULL;
}
void square( struct hash_elem *a, void *aux){
	struct hash_node *node=hash_entry(a,struct hash_node,elem);
	node->data = (node->data)*(node->data);
}

void triple( struct hash_elem *a, void *aux){
	struct hash_node *node=hash_entry(a,struct hash_node,elem);
	node->data = (node->data)*(node->data)*(node->data);
}
void hash_print_all(struct hash_elem* a){
	struct hash_node* node = hash_entry(a, struct hash_node, elem);
	printf("%d ", node->data);
}
int main(){
	int type;
	int token_cnt, obj_cnt=0;
	int i;
	char command_str[256];	//사용자로부터 입력받는 명령어
	char** command;	//command_str을 공백단위로 쪼갠 문자열 배열

	while(1){	

		fgets(command_str,sizeof(command_str),stdin);	
		command=tokenize_str(command_str,&token_cnt);

		//QUIT
		if(!strcmp(command[0],"quit")){
			break;
		}

		//CREATE
		if( !strcmp(command[0], "create")){
			//LIST
			if( !strcmp(command[1], "list")){
				if(obj_cnt<=10){
					type=LIST;
					list_init(&List[obj_cnt++]);	
				}
			}

			//BITMAP
			else if( !strcmp( command[1], "bitmap") && token_cnt==4){

				if(obj_cnt<=10){
					type=BITMAP;
					unsigned len;
					sscanf(command[3], "%u", &len);
					Bitmap[obj_cnt++] = bitmap_create((size_t)len);
				}
			}

			else if(!strcmp( command[1], "hashtable") && token_cnt==3){
				if(obj_cnt<=10){
					type=HASH;
					hash_init(&Hash[obj_cnt++], hash_hash, hash_less, NULL);
				}
			}
		}

		//DELETE
		else if( !strcmp(command[0], "delete") && token_cnt==2){
			//LIST
			if( type==LIST){
				int index = command[1][4]-'0';

				while (!list_empty (&List[index]))
				{
					struct list_elem *e = list_pop_front (&List[index]);
					free(e);
				}
			}

			//BITMAP
			else if(type==BITMAP){
				int index= command[1][2]-'0';
				bitmap_destroy(Bitmap[index]);
			}

			else{
				int index=command[1][4]-'0';
				hash_destroy(&Hash[index], destructor);
			}
		}

		//DUMPDATA
		else if( !strcmp(command[0], "dumpdata")&& token_cnt==2){
			int index;

			//LIST
			if(type==LIST){
				index=command[1][4]-'0';

				if(!list_empty(&List[index])){
					struct list_elem* e;

					for(e= list_begin(&List[index]); e!=list_end(&List[index]); e = list_next(e)){
						struct list_node* node = list_entry(e,struct list_node, elem);
						printf("%d ", node->data);
					}
					printf("\n");
				}
			}

			//BITMAP
			else if(type==BITMAP){
				index= command[1][2]-'0';

				if( Bitmap[index]!=NULL){
					size_t cnt = bitmap_size(Bitmap[index]);
					size_t i;

					for(i=0; i<cnt; i++){
						if(bitmap_test(Bitmap[index], i))
							printf("1");
						else
							printf("0");
					}
					printf("\n");
				}
			}

			//HASHTABLE
			else if(type==HASH){
				index = command[1][4]-'0';
				if(hash_empty(&Hash[index]))
					continue;
				hash_apply(&Hash[index], hash_print_all);
				printf("\n");
			}
		}

		//LIST_EMPTY
		else if( !strcmp(command[0], "list_empty") && token_cnt==2){
			int index = command[1][4]-'0';
			if( list_empty(&List[index]))
				printf("true\n");
			else
				printf("false\n");
		}

		//LIST_SIZE
		else if( !strcmp(command[0], "list_size") && token_cnt==2){
			int index = command[1][4]-'0';
			printf("%d\n", (int)list_size(&List[index]));
		}

		//LIST_INSERT
		else if( !strcmp(command[0], "list_insert") && token_cnt==4){
			int index= command[1][4]-'0';
			int node_index, node_data;
			sscanf(command[2], "%d", &node_index);
			sscanf(command[3], "%d", &node_data);


			//Create new list item
			struct list_elem* new_elem = (struct list_elem*)malloc(sizeof(struct list_elem));
			struct list_node* node = list_entry(new_elem, struct list_node, elem);
			node->data = node_data;

			//Push back
			if((unsigned)list_size(&List[index])-1<node_index || list_empty(&List[index])){
				list_push_back(&List[index],new_elem);
				continue;
			}

			//Push front
			if(node_index==0){
				list_push_front(&List[index],new_elem);
				continue;
			}

			int cur= 0;
			struct list_elem* e;
			for(e = list_begin(&List[index]); e!=list_end(&List[index]); e = list_next(e)){
				if(cur==node_index){
					list_insert(e, new_elem);			
					break;
				}
				cur++;
			}

		}

		//LIST_INSERT_ORDERD
		else if( !strcmp(command[0], "list_insert_ordered") && token_cnt==3){
			int index = command[1][4]-'0';
			int node_data;
			sscanf(command[2], "%d", &node_data);

			//Create new list item
			struct list_elem* new_elem = (struct list_elem*)malloc(sizeof(struct list_elem));
			struct list_node* node = list_entry(new_elem, struct list_node, elem);
			node->data = node_data;

			list_insert_ordered(&List[index],new_elem, list_less, NULL); 

		}

		//LIST_PUSH_BACK
		else if( !strcmp(command[0], "list_push_back") && token_cnt==3){
			int index = command[1][4]-'0';
			int node_data;
			sscanf(command[2], "%d", &node_data);

			//create new list item
			struct list_elem* back = (struct list_elem*)malloc(sizeof(struct list_elem));;
			struct list_node* node = list_entry(back, struct list_node, elem);
			node->data = node_data;

			list_push_back(&List[index],back);
		}

		//LIST_PUSH_FRONT
		else if( !strcmp(command[0], "list_push_front") && token_cnt==3){
			int index = command[1][4]-'0';
			int node_data;
			sscanf(command[2], "%d", &node_data);

			//create new list item
			struct list_elem* front = (struct list_elem*)malloc(sizeof(struct list_elem));
			struct list_node* node = list_entry(front, struct list_node, elem);
			node->data = node_data;

			list_push_front(&List[index], front);
		}

		//LIST_POP_BACK
		else if( !strcmp(command[0], "list_pop_back") && token_cnt==2){
			int index = command[1][4]-'0';
			list_pop_back(&List[index]);
		}

		//LIST_POP_FRONT
		else if( !strcmp(command[0], "list_pop_front") && token_cnt==2){
			int index = command[1][4]-'0';
			list_pop_front(&List[index]);
		}

		//LIST_FRONT
		else if( !strcmp(command[0], "list_front") && token_cnt==2){
			int index = command[1][4]-'0';

			struct list_elem* e = list_front(&List[index]);
			struct list_node* node = list_entry(e,struct list_node, elem);

			if( node!=NULL) 
				printf("%d\n", node->data);
		}

		//LIST_BACK
		else if( !strcmp(command[0], "list_back") && token_cnt==2){
			int index = command[1][4]-'0';
			struct list_elem* e = list_back(&List[index]);
			struct list_node* node = list_entry(e, struct list_node, elem);

			if( node!=NULL) 
				printf("%d\n", node->data);
		}


		//LIST_MIN
		else if( !strcmp(command[0], "list_min") && token_cnt ==2){
			int index = command[1][4]-'0';
			struct list_elem* e = list_min(&List[index], list_less, NULL);
			struct list_node* node = list_entry(e, struct list_node, elem);
			printf("%d\n", node->data);
		}

		//LIST_MAX
		else if( !strcmp(command[0], "list_max") && token_cnt ==2){
			int index = command[1][4]-'0';
			struct list_elem* e = list_max(&List[index], list_less, NULL);
			struct list_node* node = list_entry(e, struct list_node, elem);
			printf("%d\n", node->data);

		}

		//LIST_REMOVE
		else if( !strcmp(command[0], "list_remove") && token_cnt==3){
			int index = command[1][4]-'0';
			int node_index, cur=0;
			sscanf(command[2], "%d", &node_index);

			list_remove(n_th_elem(index, node_index));
		}

		//LIST_REVERSE
		else if( !strcmp(command[0], "list_reverse") && token_cnt==2){
			int index = command[1][4]-'0';
			list_reverse(&List[index]);
		}

		//LIST_SORT
		else if( !strcmp(command[0], "list_sort") && token_cnt==2){
			int index = command[1][4]-'0';
			list_sort(&List[index], list_less, NULL);
		}

		//LIST_SPLICE
		else if( !strcmp( command[0], "list_splice") && token_cnt==6){
			int index1, index2;
			int before, first , last;
			int cur=0;

			index1=command[1][4]-'0';
			index2=command[3][4]-'0';
			sscanf(command[2], "%d", &before);
			sscanf(command[4], "%d", &first);
			sscanf(command[5], "%d", &last);

			struct list_elem* e, *e_before=NULL;
			struct list_elem *e_first=NULL, *e_last=NULL;

			e_before = n_th_elem(index1, before);
			e_first = n_th_elem(index2, first);
			e_last = n_th_elem(index2, last);
			list_splice(e_before, e_first, e_last);
		}

		//LIST_SWAP
		else if( !strcmp(command[0], "list_swap") && token_cnt==4){
			int index= command[1][4]-'0';
			int node1, node2;
			sscanf(command[2], "%d", &node1);
			sscanf(command[3], "%d", &node2);
			list_swap(n_th_elem(index, node1), n_th_elem(index, node2));
		}

		//LIST_SHUFFLE
		else if( !strcmp( command[0], "list_shuffle") && token_cnt==2){
			int index = command[1][4]-'0';
			list_shuffle(&List[index]);
		}

		//LIST_UNIQUE
		else if( !strcmp(command[0], "list_unique")&& ( token_cnt==3 || token_cnt==2)){
			int index1=command[1][4]-'0';
			int index2;

			if( token_cnt==2)
				list_unique(&List[index1], NULL, list_less, NULL);

			else{
				index2=command[2][4]-'0';
				list_unique(&List[index1], &List[index2], list_less, NULL);
			}

		}

		//BITMAP_MARK
		else if( !strcmp(command[0], "bitmap_mark") && token_cnt==3){
			int index = command[1][2]-'0';
			unsigned bit;
			sscanf(command[2], "%u", &bit);
			bitmap_mark(Bitmap[index], (size_t)bit);
		}

		//BITMAP_ALL
		else if( !strcmp(command[0], "bitmap_all") && token_cnt==4){
			int index = command[1][2]-'0';
			unsigned start, cnt;
			sscanf(command[2], "%u", &start);
			sscanf(command[3], "%u", &cnt);

			if(bitmap_all(Bitmap[index], (size_t)start, (size_t)cnt))
				printf("true\n");
			else
				printf("false\n");
		}

		//BITMAP_ANY
		else if( !strcmp( command[0], "bitmap_any") && token_cnt==4){
			int index = command[1][2]-'0';
			unsigned start, cnt;
			sscanf(command[2], "%u", &start);
			sscanf(command[3], "%u", &cnt);

			if(bitmap_any(Bitmap[index], (size_t)start, (size_t)cnt))
				printf("true\n");
			else
				printf("false\n");

		}

		//BITMAP_CONTAINS
		else if( !strcmp(command[0], "bitmap_contains") && token_cnt==5){
			int index = command[1][2]-'0';
			unsigned start, cnt;
			bool value=false;
			sscanf(command[2], "%u", &start);
			sscanf(command[3], "%u", &cnt);
			if( !strcmp(command[4],"true") )
				value=true;

			if(bitmap_contains(Bitmap[index], (size_t)start, (size_t)cnt, value) )
				printf("true\n");
			else
				printf("false\n");
		}

		//BITMAP_COUNT
		else if( !strcmp(command[0], "bitmap_count") && token_cnt ==5){
			int index = command[1][2]-'0';
			unsigned start, cnt;
			bool value=false;

			sscanf(command[2], "%u", &start);
			sscanf(command[3], "%u", &cnt);
			if( !strcmp(command[4],"true") )
				value=true;

			size_t count = bitmap_count(Bitmap[index], start, cnt, value);
			printf("%d\n", (int)count);
		}

		//BITMAP_DUMP
		else if( !strcmp( command[0], "bitmap_dump") && token_cnt==2){
			int index = command[1][2]-'0';
			bitmap_dump(Bitmap[index]);
		}

		//BITMAP_EXPAND
		else if( !strcmp( command[0],"bitmap_expand") && token_cnt==3){
			int index = command[1][2]-'0';
			int cnt;
			sscanf(command[2], "%d", &cnt);
			Bitmap[index]=	bitmap_expand(Bitmap[index], cnt);
		}

		//BITMAP_FLIP
		else if(!strcmp( command[0], "bitmap_flip") && token_cnt==3){
			int index = command[1][2]-'0';
			unsigned cnt;
			sscanf(command[2], "%u", &cnt);
			bitmap_flip(Bitmap[index], cnt);
		}

		//BITMAP_NONE
		else if( !strcmp( command[0], "bitmap_none") && token_cnt ==4){
			int index = command[1][2]-'0';
			unsigned start, cnt;

			sscanf(command[2], "%u", &start);
			sscanf(command[3], "%u", &cnt);

			if(bitmap_none(Bitmap[index], (size_t)start, (size_t)cnt) )
				printf("true\n");
			else
				printf("false\n");

		}

		//BITMAP_RESET
		else if( !strcmp( command[0], "bitmap_reset") && token_cnt==3){
			int index = command[1][2]-'0';
			unsigned bit_idx;

			sscanf(command[2], "%u", &bit_idx);
			bitmap_reset(Bitmap[index], bit_idx);
		}

		//BITMAP_SCAN_AND_FLIP
		else if( !strcmp( command[0], "bitmap_scan_and_flip") && token_cnt==5){
			int index = command[1][2]-'0';
			unsigned start, cnt;
			bool value=false;

			sscanf(command[2], "%u", &start);
			sscanf(command[3], "%u", &cnt);
			if( !strcmp(command[4],"true") )
				value=true;

			size_t idx =  bitmap_scan_and_flip(Bitmap[index], start, cnt, value);	
			printf("%u\n", (unsigned)idx);
		}

		//BITMAP_SCAN
		else if( !strcmp( command[0], "bitmap_scan") &&  token_cnt==5){
			int index = command[1][2]-'0';
			unsigned start, cnt;
			bool value=false;

			sscanf(command[2], "%u", &start);
			sscanf(command[3], "%u", &cnt);
			if( !strcmp(command[4],"true") )
				value=true;

			size_t idx= bitmap_scan(Bitmap[index], start, cnt, value);	
			printf("%u\n", (unsigned)idx);
		}

		//BITMAP_SET_ALL
		else if( !strcmp( command[0], "bitmap_set_all") && token_cnt== 3){
			int index = command[1][2]-'0';
			bool value=false;

			if( !strcmp(command[2],"true") )
				value=true;

			bitmap_set_all(Bitmap[index], value);
		}

		//BITMAP_SET_MULTIPLE
		else if( !strcmp( command[0], "bitmap_set_multiple") && token_cnt==5){
			int index = command[1][2]-'0';
			unsigned start, cnt;
			bool value=false;

			sscanf(command[2], "%u", &start);
			sscanf(command[3], "%u", &cnt);
			if( !strcmp(command[4],"true") )
				value=true;
			bitmap_set_multiple(Bitmap[index], start, cnt, value);
		}

		//BITMAP_SET
		else if( !strcmp( command[0], "bitmap_set") && token_cnt==4){
			int index = command[1][2]-'0';
			unsigned bit_idx;
			bool value =false;

			sscanf(command[2], "%u", &bit_idx);
			if( !strcmp(command[3],"true") )
				value=true;
			bitmap_set(Bitmap[index],bit_idx, value);
		}

		//BITMAP_SIZE
		else if( !strcmp( command[0], "bitmap_size") && token_cnt==2){
			int index = command[1][2]-'0';
			size_t size = bitmap_size(Bitmap[index]);	
			printf("%u\n", (unsigned)size);
		}

		//BITMAP_TEST
		else if( !strcmp( command[0], "bitmap_test") && token_cnt==3){
			int index = command[1][2]-'0';
			unsigned idx;

			sscanf(command[2], "%u", &idx);
			if(bitmap_test(Bitmap[index], idx))
				printf("true\n");
			else
				printf("false\n");

		}

		//HASH_INSERT
		else if( !strcmp( command[0], "hash_insert") && token_cnt==3){
			int index= command[1][4]-'0';
			int data;
			sscanf(command[2], "%d", &data);
			//Create new list item
			struct hash_elem* new_elem = (struct hash_elem*)malloc(sizeof(struct hash_elem));
			struct hash_node* node = hash_entry(new_elem, struct hash_node, elem);
			node->data = data;

			new_elem = hash_insert(&Hash[index], new_elem);
		}
		
		//HASH_APPLY
		else if( !strcmp( command[0], "hash_apply") && token_cnt==3){
			int index = command[1][4]-'0';
			
			if( !strcmp( command[2], "triple"))
				hash_apply(&Hash[index], triple);

			else
				hash_apply(&Hash[index], square);
		}

		//HAHS_EMPTY
		else if( !strcmp( command[0], "hash_empty") && token_cnt==2){
			int index = command[1][4]-'0';
			if( hash_empty(&Hash[index]))
				printf("true\n");
			else
				printf("false\n");
		}

		//HASH_SIZE
		else if( !strcmp(command[0], "hash_size") && token_cnt==2){
			int index = command[1][4] -'0';
			size_t size = hash_size(&Hash[index]);

			printf("%u\n", (unsigned)size);	
		}

		//HASH_CLEAR
		else if( !strcmp( command[0], "hash_clear") && token_cnt==2){
			int index = command[1][4]-'0';
			hash_clear(&Hash[index], destructor);
		}
		
		//HASH_FIND
		else if( !strcmp(command[0], "hash_find") && token_cnt==3){
			int index = command[1][4]-'0';
			int value;
			sscanf(command[2], "%d", &value);
			
			struct hash_elem* tmp = (struct hash_elem*)malloc(sizeof(struct hash_elem));
			struct hash_node* tmp_node = hash_entry(tmp, struct hash_node, elem);
			tmp_node->data = value;

			struct hash_elem* e = hash_find( &Hash[index],tmp_node);
			
			if( e!=NULL){
				printf("%d\n", value);
			}
		}
	
		else if( !strcmp(command[0], "hash_delete") && token_cnt==3){
			int index = command[1][4]-'0';
			int value;
			sscanf(command[2], "%d", &value);
			
			struct hash_elem* tmp = (struct hash_elem*)malloc(sizeof(struct hash_elem));
			struct hash_node* tmp_node = hash_entry(tmp, struct hash_node, elem);
			tmp_node->data = value;

			struct hash_elem* e = hash_delete( &Hash[index],tmp_node);
		}


		//HASH_REPLACE
		else if( !strcmp( command[0], "hash_replace") && token_cnt==3){
			int index = command[1][4]-'0';
			int value;
			sscanf(command[2], "%d", &value);
			
			struct hash_elem* tmp = (struct hash_elem*)malloc(sizeof(struct hash_elem));
			struct hash_node* tmp_node = hash_entry(tmp, struct hash_node, elem);
			tmp_node->data = value;
			
			hash_replace(&Hash[index], tmp_node);

		}
	
	}

	return 0;

}

char** tokenize_str(char* str, int* token_cnt){
	int i=0,cnt=0;
	char copy[100];
	char* tmp=NULL;
	char** tokens=NULL;

	if(!str)
		return NULL;

	//token 개수 구하기
	strcpy(copy, str);
	tmp = strtok(copy, " \n\r\t");

	while(1){	
		cnt++;
		tmp = strtok(NULL, " \n\t\r");
		if(!tmp)
			break;
	}

	//token 개수 만큼 문자열 배열 할당
	*token_cnt=cnt;
	tokens = (char**)malloc(cnt*sizeof(char*));	
	for(i=0; i<cnt; i++)
		tokens[i] = (char*)malloc(100*sizeof(char));

	//배열 채우기
	strcpy(copy, str);
	tmp = strtok(copy, " \n\r\t");
	strcpy(tokens[0], tmp);

	for(i=1; i<cnt; i++){
		tmp = strtok(NULL, " \n\r\t");
		strcpy(tokens[i], tmp);
	}
	return tokens;
}

struct list_elem* n_th_elem(int index, int node_index){
	int cur=0;
	struct list_elem* e;
	for( e= list_begin(&List[index]); e!= list_end(&List[index]); e= list_next(e)){
		if(cur==node_index){
			break;
		}
		cur++;
	}
	return e;
}
