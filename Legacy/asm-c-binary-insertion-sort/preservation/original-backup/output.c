//------------------------------------------------------------------------------
// output.c - единица компиляции, вбирающая в себя все функции вывода данных
//------------------------------------------------------------------------------

#include <stdio.h>

#include "extdata.h"

// Вычисление периметра прямоугольника
double PerimeterRectangle(void *r);
// Вывод параметров прямоугольника в файл
void OutRectangle(void *r, FILE *ofst) {
    fprintf(ofst, "It is Rectangle: x = %d, y = %d. Perimeter = %g\n",
            *((int*)r), *((int*)(r+intSize)), PerimeterRectangle(r));
}

// Вычисление периметра треугольника
double PerimeterTriangle(void *t);

// Вывод параметров треугольника в файл
void OutTriangle(void *t, FILE *ofst) {
    fprintf(ofst, "It is Triangle: a = %d, b = %d, c = %d. Perimeter = %g\n",
           *((int*)t), *((int*)(t+intSize)), *((int*)(t+2*intSize)),
            PerimeterTriangle(t));
}

// Вывод параметров текущей фигуры в поток
void OutShape(void *s, FILE *ofst) {
    int k = *((int*)s);
    if(k == RECTANGLE) {
        OutRectangle(s+intSize, ofst);
    }
    else if(k == TRIANGLE) {
        OutTriangle(s+intSize, ofst);
    }
    else {
        fprintf(ofst, "Incorrect figure!\n");
    }
}

// Вывод содержимого контейнера в файл
void OutContainer(void *c, int len, FILE *ofst) {
    void *tmp = c;
    fprintf(ofst, "Container contains %d elements.\n", len);
    for(int i = 0; i < len; i++) {
        fprintf(ofst, "%d: ", i);
        OutShape(tmp, ofst);
        tmp = tmp + shapeSize;
    }
}
