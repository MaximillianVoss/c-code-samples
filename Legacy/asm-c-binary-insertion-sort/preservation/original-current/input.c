//------------------------------------------------------------------------------
// input.c - единица компиляции, вбирающая в себя все функции ввода данных
//------------------------------------------------------------------------------

#include <stdio.h>

#include "extdata.h"

// Ввод параметров прямоугольника из файла
void InRectangle(void *r, FILE *ifst) {
    fscanf(ifst, "%d%d", (int*)r, (int*)(r+intSize));
    //printf("    Rectangle %d %d\n", *((int*)r), *((int*)r+1));
}

// Ввод параметров треугольника из файла
void InTriangle(void *t, FILE *ifst) {
    fscanf(ifst, "%d%d%d", (int*)t, (int*)(t+intSize), (int*)(t+2*intSize));
    //printf("    Triangle %d %d %d\n", *((int*)t), *((int*)t+1), *((int*)t+2));
}

// Ввод параметров обобщенной фигуры из файла
int InShape(void *s, FILE *ifst) {
    int k;
    fscanf(ifst, "%d", &k);
    switch(k) {
        case 1:
            *((int*)s) = RECTANGLE;
            InRectangle(s+intSize, ifst);
            return 1;
        case 2:
            *((int*)s) = TRIANGLE;
            InTriangle(s+intSize, ifst);
            return 1;
        default:
            return 0;
    }
}

// Ввод содержимого контейнера из указанного файла
void InContainer(void *c, int *len, FILE *ifst) {
    void *tmp = c;
    while(!feof(ifst)) {
        if(InShape(tmp, ifst)) {
            tmp = tmp + shapeSize;
            (*len)++;
        }
    }
}
