/*Задача: Написать 3 функции: пользователь вводит число, если число < 0, то массив нужно
уменьшить на это количество элементов, если число > 0, то массив надо увеличить
на это количество элементов. Новые элементы заполняются нулями
*/

#include <stdio.h>
#include <stdlib.h>

int* create(int size);
void erase(int *m);
void init(int *m, int sz);
void print(int*, int sz);
int* change(int *m, int *sz);	//функция изменения рзмера массива, парметры: массив и его размер
int* decrease(int num, int *m, int *sz); //функция уменьшения размера массива, параметры: параметр, массив и его размер
int* increase(int num, int *m, int *sz);	//функция увеличения размера массива, параметры: параметр, массив и его размер

int main() {
	int sz;	//размер, больше нуля
	int* m;
	scanf_s("%i", &sz);
	printf("size=%d\n", sz);
	if (sz>0) {
		int *m = create(sz);	//адрес массива
		if (m) {	//выделена ли память
			init(m, sz);
			print(m, sz);
			m = change(m, &sz);
			print(m, sz);
			erase(m);
		}
	}
	return 0;
}

int* create(int size) {
	return (int*)malloc(size * sizeof(int));
}

void erase(int *m) {
	free(m);
}

void init(int *m, int sz) {
	int i;
	for (i = 0; i<sz; i++) m[i] = 1;
}

void print(int *m, int sz) {
	int i;
	for (i = 0; i<sz; i++) printf("%i", m[i]);
	printf("\n");
}

int* change(int *m, int *sz) {
	int num;
	scanf_s("%i", &num);	//запрашиваем число на которое будем меняться массив
	if (num != 0 && num >= -*sz) {	//проверяем, что мы будем уменьшать на число не большее, чем сам массив
		if (num<0) {
			m = decrease(num, m, sz);	//вызываем функцию уменьшения
		}
		else m = increase(num, m, sz);	//иначе вызываем функцию увеличения
	}
	else printf("Incorrect value of number");
	return m;
}

int* decrease(int num, int *m, int *sz) {
	*sz += num;	//получаем новый размер массива
	int *n = create(*sz); //создаем нове место в памяти, получаем адрес нового массива
	if (n) {	//выделена ли память
		int i;
		for (i = 0; i<*sz; i++) { //копируем элементы первого массива во второй массив
			n[i] = m[i];
		}
		erase(m); //освобождаем память из-под первого массива
	}
	return n;
}

int* increase(int num, int *m, int *sz) {
	int newsz = *sz + num;
	int *n = create(newsz);
	if (n) {
		int i, j;
		for (i = 0; i<*sz; i++) {
			n[i] = m[i];
		}
		for (j = *sz; j<newsz; j++)
			n[j] = 0;	//заполняем новые элементы массива нулями
		erase(m);
		*sz += num;
	}
	return n;
}
