#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "stdafx.h"
#include <Windows.h>

#define HEAP_MEM_SIZE 150
#define MEMORY_AVAILABLE 1 
#define MEMORY_USED 0 

struct chunk_header {
	struct chunk_header *next; // next pointer on free list
	size_t size;               // the size of this chunk
	bool is_available;         // indicates if this chunk is MEMORY_AVAILABLE or MEMORY_USED
};

typedef struct chunk_header chunk_header;
chunk_header *chunk_header_begin;
static char buffer[HEAP_MEM_SIZE];
unsigned int heap_size;

void init() {
	heap_size = HEAP_MEM_SIZE;
	chunk_header_begin = (chunk_header*)&buffer;
	chunk_header_begin->next = NULL;
	chunk_header_begin->size = heap_size;
	chunk_header_begin->is_available = MEMORY_AVAILABLE;
}

void init_next_chunk(chunk_header *curr, unsigned int bytes) {
	heap_size -= bytes;
	curr->next = NULL;
	curr->size = heap_size;
	curr->is_available = MEMORY_AVAILABLE;
}

void *my_malloc(unsigned int nbytes) {
	static bool init_flag = false;

	if (!init_flag) {
		init();
		init_flag = true;
	}

	int alloc_size = nbytes + sizeof(chunk_header);
	chunk_header *curr = chunk_header_begin;

	while (curr) {
		if (curr->is_available && curr->size >= alloc_size) {
			curr->is_available = MEMORY_USED;
			curr->size = alloc_size;
			curr->next = curr + alloc_size;

			init_next_chunk(curr->next, alloc_size);

			// return memory region
			curr = curr + sizeof(chunk_header);
			return curr;
		}

		curr = curr->next;
	}

	return NULL;
}

void my_free(void *firstbyte) {
	chunk_header *mem = (chunk_header*)firstbyte - sizeof(chunk_header);//???
	chunk_header *curr = chunk_header_begin;

	while (curr) {
		if (curr == mem) {
			heap_size += curr->size;

			// Mark the block as being available
			curr->is_available = MEMORY_AVAILABLE;
			break;
		}

		curr = curr->next;
	}

	firstbyte = NULL;
}

void print_heap_allocations() {
	chunk_header *curr = chunk_header_begin;

	printf("\n\tSize\tAvailable\tMemory-Ptr");

	while (curr) {
		void *mem_ptr = curr + sizeof(chunk_header);
		printf("\n\t%d\t%d\t\t%x", curr->size, curr->is_available, mem_ptr);
		curr = curr->next;
	}

	printf("\n");
}

int main() {
	char *mem1 = (char*)my_malloc(20);
	if (mem1 == NULL) {
		goto err;
	}
	memset(mem1, 'x', 19);
	mem1[19] = '\0';

	print_heap_allocations();
	//my_free(mem1);
	//print_heap_allocations();

	char *mem2 = (char*)my_malloc(20);
	if (mem2 == NULL) {
		goto err;
	}
	memset(mem2, 'y', 19);
	mem2[19] = '\0';

	print_heap_allocations();

	char *mem3 = (char*)my_malloc(20);
	if (mem3 == NULL) {
		goto err;
	}
	memset(mem3, 'z', 19);
	mem3[19] = '\0';

	print_heap_allocations();

	//my_free(mem2);

	//print_heap_allocations();

	char *mem4 = (char*)my_malloc(20);
	if (mem4 == NULL) {
		goto err;
	}
	memset(mem4, 'a', 20);
	mem4[19] = '\0';

	print_heap_allocations();

	printf("should be 19 x sein: %s\n", mem1);
	printf("should be 19 a sein: %s\n", mem4);
	printf("should be 19 z sein: %s\n", mem3);
	system("pause");
	return 0;

err:
	printf("could not allocate mem\n");
	system("pause");
	return 0;
}