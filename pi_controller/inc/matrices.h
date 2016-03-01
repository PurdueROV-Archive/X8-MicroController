/*
Created by Jason King
January 2016
Purdue IEEE ROV
*/

#include "main.h"

#ifndef __MATRICES__H
#define __MATRICES__H
typedef struct{
    int32_t x;
    int32_t y;
    int32_t z;
}vect3;

typedef struct{
    vect3 L;
    vect3 R;
}vect6;

typedef struct{
    vect6 t1;
    vect6 t2;
    vect6 t3;
    vect6 t4;
    vect6 t5;
    vect6 t6;
    vect6 t7;
    vect6 t8;
}matrix8_6;

typedef struct{
    vect3 t1;
    vect3 t2;
    vect3 t3;
    vect3 t4;
    vect3 t5;
    vect3 t6;
    vect3 t7;
    vect3 t8;
}matrix8_3;

typedef struct{
    vect3 a;
    vect3 b;
    vect3 c;
}matrix3_3;

typedef struct{
    int32_t a;
    int32_t b;
    int32_t c;
    int32_t d;
    int32_t e;
    int32_t f;
    int32_t g;
    int32_t h;
}vect8;

typedef struct{
    int32_t a;
    int32_t b;
}vect2;

typedef struct{
    vect2 one;
    vect2 two;
}matrix2_2;

vect3 cross(vect3, vect3);
int32_t dot(vect3, vect3);
int32_t dot6(vect6, vect6);
int32_t dot2(vect2, vect2);

vect3 add(vect3, vect3);
vect6 add6(vect6, vect6);

vect3 sub(vect3, vect3);
vect6 sub6(vect6, vect6);

vect3 mul(vect3, int32_t);
vect6 mul6(vect6, int32_t);

vect3 div(vect3, int32_t);
vect6 div6(vect6, int32_t);

int32_t max3(vect3);
int32_t max6(vect6);
int32_t max8(vect8);

vect3 vect3Make(int32_t, int32_t, int32_t);
vect6 vect6Make(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);
vect2 vect2Make(int32_t, int32_t);

vect8 matMul_86x61(matrix8_6, vect6);
vect3 matMul_33x31(matrix3_3,vect3);
matrix2_2 matMul_22x22(matrix2_2, matrix2_2);

matrix2_2 invert2_2(matrix2_2);

#endif