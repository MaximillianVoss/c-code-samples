#include "memallocator.h"
#include <stdio.h>
#include <stdlib.h>
struct Item
{
	struct Item *next;
	int size;
};
struct Item*header;
struct Item*end;
int initsize;
int meminit(void *p, int size)
{
	((struct Item*)p)->next = NULL;//это голова списка,следующий элемент отсуствует
	((struct Item*)p)->size = size - memgetminimumsize();//указываем размер за вычетом дескриптора
	header = p;//сохраняем в глобальной перемнной
	initsize = size;//этот размер нужен чтобы потом проверть на утечки
	end = ((struct Item*)((char*)(header + 1) + size));//запоминаем где конец памяти
}

void memdone()
{
	//если текущий размер свободной памяти не равен той, что была при инициализации, то утечка памяти
	if (initsize-memgetminimumsize() != header->size)
		printf("memory leak!");
	else
		printf("no memory leak");
}

struct Item* getNewItem(struct Item*current, int size)
{
	//здесь определяем где будет находится следующий блок памяти	
	struct Item* result = ((struct Item*)((char*)(current + 1) + current->size));
	if (result <= end)//не выходит ли он за границы памяти
		return result;
	else
		return NULL;
}

void *memalloc(int size)
{
	struct Item*current = header;
	if (!header)return NULL;
	while (current->size < size && current->size>0)//ищем подходящий свободный блок, если размер меньше нуля - блок занят
		current = current->next;
	if (current->size > size)//проверка, потому что в current может остаться указатель на head, где может не хватать памяти
	{
		struct Item*prev = current->next, *_new = getNewItem(current, size);//находим предыдущий и создаем новый 
		if (_new != NULL)
		{
			current->size -= size + memgetminimumsize();//у свободного блока забираем память
			current->next = _new;//следующий у него вновь созданный блок
			_new->size = -size;//он занят, размер отрицательный
			_new->next = prev;//у нового блока предыдущий, тот что мы нашли ранеее
		}
	}
	else
		return NULL;
}

void memfree(void *p)
{
	struct Item*current = header, *prev = NULL;
	if (!header)return;
	while (current->next != (struct Item*)p)//находим блок который стоятит перед тем,что собираемся освободить
		current = current->next;
	if (current != NULL)
	{
		prev = current;//предыдущий блок
		current = (struct Item*)p;//блок для освобождения
		current->size = -current->size;//все теперь он свободная память
		current = header;//перешли на начало списка
		//объединяем свободные части
		//стратегия проста: идем сначала списка, елси видим два рядом стоящих участка - объединяем их
		//поле объедиения проверяем, можно ли объединить со следующим еще раз
		//если нет идем дальше
		//повторяем n раз
		while (current != NULL)
		{
			if (current->next != NULL && current->size > 0 && current->next->size > 0)
			{
				current->size += current->next->size + memgetminimumsize();//объединяем размеры
				current->next = current->next->next;//сливаем два в один
			}
			else
				current = current->next;
		}
	}
}

int memgetminimumsize()
{
	return sizeof(struct  Item);
}

int memgetblocksize()
{
	return sizeof(struct  Item);
}