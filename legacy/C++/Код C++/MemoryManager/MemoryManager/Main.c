#include <stdlib.h>
struct Header
{
	unsigned short next; //in blocks of sizeof(Header)
	unsigned short size; //in blocks of sizeof(Header)
};

static struct Header* _dBuffer;
static struct Header* _dTail;

static  void dInit()
{
	_dBuffer = getMemBlock(64 * 1024);
	_dTail = _dBuffer;
}

static  void* dMalloc(unsigned short nbytes)
{
	if (_dTail == NULL) //empty free list
		return NULL;

	struct Header* prev = NULL;
	struct Header* current = _dTail;

	// below is equivalent to ceil((nbytes + sizeof(Header)) / sizeof(Header))
	int nblocks = (nbytes + sizeof(struct Header) - 1) / sizeof(struct Header) + 1;
	while (current->next != 0 && current->size < nblocks)
	{
		prev = current;
		current = _dBuffer + current->next;
	}

	if (current->size < nblocks)
		//we've reached the end of the free list without finding a chunk big enough 
		return NULL;

	if (current->size == nblocks)
	{
		//we've found an exact match, remove the chunk from the free list
		if (prev == NULL) //remove the first chunk
			_dTail = NULL;
		else
			prev->next = current->next;
		return current + 1;
	}

	//current->size > nblocks
	current->size -= nblocks;
	current += current->size;
	current->size = nblocks;
	return current + 1;
}

static bool coalesce(struct Header* block)
{
	Header* nextBlock = _dBuffer + block->next;
	if (block + block->size != nextBlock)
		return false;

	block->size += nextBlock->size;
	block->next = nextBlock->next;
	return true;
}

static public void dFree(void* p)
{
	Header* released = (Header*)p - 1;
	Header* before = NULL;
	Header* after = _dTail;

	if (_dTail == NULL) //the free list is empty
	{
		released->next = 0;
		_dTail = released;
		return;
	}

	//find where released is located in the free list
	while (after->next != 0 && after < released)
	{
		before = after;
		after = _dBuffer + after->next;
	}

	if (after < released) //released chunk after the end of the free list
	{
		released->next = 0;
		after->next = released - _dBuffer;
		coalesce(after);
		return;
	}

	if (before == NULL) //released chunk before the beginning of the free list
	{
		released->next = _dTail - _dBuffer;
		_dTail = released;
		coalesce(released);
		return;
	}

	//released chunk is somewhere in the middle of the list
	released->next = before->next;
	before->next = released - _dBuffer;
	if (coalesce(before)) //before was coalesced with released
		coalesce(before); //now try to coalesce it with after
	else
		coalesce(released); //otherwise, try coalescing released with after
}