/*
Created by Jason King
January 2016
Purdue IEEE ROV
*/
#include "matrices.h"

int32_t dot6(vect6 a, vect6 b)
{
    //return a.L.x * b.L.x + a.L.y * b.L.y + a.L.z * b.L.z + a.R.x * b.R.x + a.R.y * b.R.y + a.R.z * b.R.z; 
    return dot(a.L,b.L) + dot(a.R,b.R);
}

int32_t dot(vect3 a, vect3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

int32_t dot2(vect2 a, vect2 b)
{
    return a.a * b.a + a.b * b.b;
}

vect3 cross(vect3 a, vect3 b)
{
    vect3 result;
    
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    
    return result;
}

vect3 add(vect3 a, vect3 b)
{
    vect3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

vect6 add6(vect6 a, vect6 b)
{
    vect6 result;
    result.L = add(a.L,b.L);
    result.R = add(a.R,b.R);
    return result;
}

vect3 sub(vect3 a, vect3 b)
{
    vect3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

vect6 sub6(vect6 a, vect6 b)
{
    vect6 result;
    result.L = sub(a.L,b.L);
    result.R = sub(a.R,b.R);
    return result;
}

vect3 mul(vect3 a, int32_t factor)
{
    vect3 result;
    result.x = a.x * factor;
    result.y = a.y * factor;
    result.z = a.z * factor;
    return result;
}

vect6 mul6(vect6 a, int32_t factor)
{
    vect6 result;
    result.L = mul(a.L, factor);
    result.R = mul(a.R, factor);
    return result;
}

vect3 div(vect3 a, int32_t factor)
{
    vect3 result;
    result.x = a.x / factor;
    result.y = a.y / factor;
    result.z = a.z / factor;
    return result;
}

vect6 div6(vect6 a, int32_t factor)
{
    vect6 result;
    result.L = div(a.L, factor);
    result.R = div(a.R, factor);
    return result;
}

int32_t max3(vect3 vect)
{
    return MAX(MAX(vect.x, vect.y), vect.z);
}
int32_t max6(vect6 vect)
{
    return MAX(MAX(MAX(MAX(MAX(vect.L.x, vect.L.y), vect.L.z), vect.R.x), vect.R.y), vect.R.z);
}
int32_t max8(vect8 vect)
{
    return MAX(MAX(MAX(MAX(MAX(MAX(MAX(vect.a, vect.b), vect.c), vect.d), vect.e), vect.f), vect.g), vect.h);
}

vect3 vect3Make(int32_t a, int32_t b, int32_t c)
{
    vect3 result;
    result.x = a;
    result.y = b;
    result.z = c;
    return result;
}

vect6 vect6Make(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f)
{
    vect6 result;
    result.L = vect3Make(a, b, c);
    result.R = vect3Make(d, e, f);
    return result;
}

vect2 vect2Make(int32_t a, int32_t b)
{
    vect2 result;
    result.a = a;
    result.b = b;
    return result;
}

vect8 matMul_86x61(matrix8_6 mat, vect6 v)
{
    vect8 result;
    result.a = dot6(mat.t1, v);
    result.b = dot6(mat.t2, v);
    result.c = dot6(mat.t3, v);
    result.d = dot6(mat.t4, v);
    result.e = dot6(mat.t5, v);
    result.f = dot6(mat.t6, v);
    result.g = dot6(mat.t7, v);
    result.h = dot6(mat.t8, v);
    return result;
}

vect3 matMul_33x31(matrix3_3 m,vect3 v)
{
    vect3 result;
    result.x = dot(m.a, v);
    result.y = dot(m.b, v);
    result.z = dot(m.c, v);
    return result;
}

matrix2_2 matMul_22x22(matrix2_2 a, matrix2_2 b)
{
    matrix2_2 result;
    result.one.a = dot2(a.one, vect2Make(b.one.a, b.two.a));
    result.one.b = dot2(a.one, vect2Make(b.one.b, b.two.b));
    result.two.a = dot2(a.two, vect2Make(b.one.a, b.two.a));
    result.two.b = dot2(a.two, vect2Make(b.one.b, b.two.b));
    return result;
}

matrix2_2 invert2_2(matrix2_2 m)
{
    matrix2_2 result;
    int32_t det = m.one.a * m.two.b - m.one.b * m.two.a;//ad-bc
    result.one.a = m.two.b*1024/det;
    result.two.b = m.one.a*1024/det;
    result.one.b = -m.one.b*1024/det;
    result.two.a = -m.two.a*1024/det;
    return result;
}