//------------------------------------------------------------------------------
// inrnd.c - единица компиляции, вбирающая функции генерации случайных данных
//------------------------------------------------------------------------------

#include <stdlib.h>

#include "extdata.h"

// rnd.c - содержит генератор случайных чисел в диапазоне от 1 до 20
int Random() {
    return rand() % 20 + 1;
}

// Случайный ввод параметров прямоугольника
void InRndRectangle(void *r) {
    int x = Random();
    *((int*)r) = x;
    int y = Random();
    *((int*)(r+intSize)) = y;
//     printf("    Rectangle %d %d\n", *((int*)r), *((int*)r+1));
}

// Случайный ввод параметров треугольника
void InRndTriangle(void *t) {
    int a, b, c;
    a = *((int*)t) = Random();
    b = *((int*)(t+intSize)) = Random();
    do {
        c = *((int*)(t+2*intSize)) = Random();
    } while((c >= a + b) || (a >= c + b) || (b >= c + a));
//     printf("    Triangle %d %d %d\n", *((int*)t), *((int*)t+1), *((int*)t+2));
}

// Случайный ввод обобщенной фигуры
int InRndShape(void *s) {
    int k = rand() % 2 + 1;
    switch(k) {
        case 1:
            *((int*)s) = RECTANGLE;
            InRndRectangle(s+intSize);
//              printf("Shape %d %d %d %d\n",
//                     *((int*)s), *((int*)s+1),
//                     *((int*)s+2), *((int*)s+3));
            return 1;
        case 2:
            *((int*)s) = TRIANGLE;
            InRndTriangle(s+intSize);
//              printf("Shape %d %d %d %d\n",
//                     *((int*)s), *((int*)s+1),
//                     *((int*)s+2), *((int*)s+3));
            return 1;
        default:
            return 0;
    }
}

// Случайный ввод содержимого контейнера
void InRndContainer(void *c, int *len, int size) {
    void *tmp = c;
    while(*len < size) {
        if(InRndShape(tmp)) {
            tmp = tmp + shapeSize;
            (*len)++;
        }
    }
}
