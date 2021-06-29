#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
struct Node {
	int size;
	bool isFree;
	struct Node* prev;
};

struct Node* head = NULL;
int node_count = 0;
struct Node* tail = NULL;

void* add_memory (int memory_request) {
	struct Node* temp_node = tail;
	while (temp_node != head) {
		if (temp_node -> isFree && temp_node -> size >= memory_request) {
			temp_node -> isFree = false;
			return ((void*)temp_node+sizeof(struct Node));
		}
		temp_node = temp_node -> prev;
	}
	

	struct Node* new_node = sbrk(0);
	sbrk(memory_request + sizeof(struct Node));
	
	new_node -> size = memory_request;
	new_node -> isFree = false;
	new_node -> prev = NULL;
	
	if (head == NULL) {
		head = new_node;
		tail = head;
	} else {
		struct Node* temp_tail = tail;
		tail = new_node;
		tail -> prev = temp_tail;
		
		
	}
	
	return ((void*)new_node + sizeof(struct Node));
	
}

int free_memory (void* addr) {
	struct Node* address = (struct Node*)(addr - sizeof(struct Node));
	address -> isFree = true;
	sbrk(- address -> size);

	if (address == tail) {
		struct Node* temp_node = address;
	
		while (temp_node -> prev -> isFree == true) {
			temp_node = temp_node -> prev;
		}	

		if (temp_node -> prev != NULL) {
			tail  = temp_node -> prev;
		} else {
			tail = head;
		}
		brk(tail + sizeof(struct Node) + tail -> size);
	}
	return 1;
}

void show_list () {
	struct Node* temp_node = tail;
	while (temp_node != NULL) {
		printf("size %d, isfree %s;", temp_node -> size,  temp_node -> isFree ? "true" : "false");
		temp_node = temp_node -> prev;
	}
	printf("\n");
}

int main () {

	
	int* check = (int*) add_memory(sizeof(int));
	show_list();
	double* check1 = (double*) add_memory(sizeof(double));
	show_list();
	int* check2 = (int*) add_memory(sizeof(int));
	show_list();
	free_memory(check1);
	show_list();
	free_memory(check2);
	show_list();
}
