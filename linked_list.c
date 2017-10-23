#include <stdio.h>
#include <stdlib.h>

struct node {
	int data;
	struct node * next;
};
typedef struct node node;


node * node_init();

node * node_add(node *head, int data);
node * node_insert(node *head, int i,int data);
void node_del(node *head, int i);

void list_print(node *head);
int list_length(node *head);
void list_del(node *head);




int main(){
	node * head = node_init();
	node_add(head, 1);
	node_add(head, 2333);
	node_insert(head, 1, 200);
	node_insert(head, list_length(head) -1, 250);
	node_del(head, 5);

	list_print(head);
	printf("%d\n",list_length(head));

	return 0;
}


node * node_init(){
	node * head = (node *)malloc(sizeof(node));
	head -> next = NULL;
	head -> data = -1;
	return head;
}

node * node_add(node *head, int data){
	node * newnode = (node *)malloc(sizeof(node));
	if(!newnode)return NULL;
	node *p = head;
	while(p -> next != NULL )p = p -> next;
	p -> next = newnode;
	newnode -> next = NULL;
	newnode -> data = data;
	return newnode;
}

void list_print(node *head){
	node *p = head;
	int x = 0;
	while(p != NULL){
		printf("node(%d):%d\n", x, p -> data);
		p = p -> next;
		x++;
	}
}

node * node_insert(node *head, int i,int data){
	node * newnode = (node *)malloc(sizeof(node));
	if(!newnode)return NULL;
	newnode -> data = data;
	node * p = head;
	while(i > 0 && p -> next != NULL){
		p = p -> next;
		i--;
	}
	newnode -> next = p -> next;
	p -> next = newnode;
	return newnode;
}

void node_del(node *head, int i){
	node *p1 = head;
	node *p2;
	if(i == 0)i++;
	while(i > 0 && p1 -> next != NULL){
		i--;
		p2 = p1;
		p1 = p1 -> next;
	}
	p2 -> next = p1 -> next;
	free(p1);
}

int list_length(node *head){
	node *p = head;
	int count = 0;
	while(p != NULL){
		p = p -> next;
		count++;
	}
	return count;
}

void list_del(node *head){
	node * p1 = head;
	node * p2;
	while(p1 != NULL){
		p2 = p1;
		p1 = p1 -> next;
		free(p2);
	}
}