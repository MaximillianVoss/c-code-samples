#include "memallocator.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
	char* p = malloc(sizeof(char)*101);
	meminit(p, 100);
	char *a = memalloc(21);
	char *b = memalloc(22);
	char *c = memalloc(23);
	memfree(a);
	memfree(b);
	memfree(c);
	memdone();
	return 0;
}