#include <iostream>
#include <cmath>

using namespace std;

static float octX = 100.0;
static float octY = 150.0;
static float octZ = 100.0;

static float extendSides = 5.; // коэффицент раздвижения граней
// массивы точек граней октаэдра
static float point1[3] = {0., 0., 0.};
static float point2[3] = {0., 0., 0.};
static float point3[3] = {0., 0., 0.};

static GLuint tex[9];
//окрас граней
// 0 - цвета, 1 - одна текстура, 2 - 8 текстур
static int paint = 0;

void getNormal(float p1[3], float p2[3], float p3[3], float vNormal[3])
{
    float v1[3], v2[3]; // для промежуточных вычислений
    float l; // норма

    v1[0] = p2[0]- p1[0];
    v1[1] = p2[1]- p1[1];
    v1[2] = p2[2]- p1[2];

    v2[0] = p3[0]- p1[0];
    v2[1] = p3[1]- p1[1];
    v2[2] = p3[2]- p1[2];

    vNormal[0] = v1[1] * v2[2] - v1[2] * v2[1];
    vNormal[1] = v1[2] * v2[0] - v1[0] * v2[2];
    vNormal[2] = v1[0] * v2[1] - v1[1] * v2[0];

    l = sqrt(vNormal[0]*vNormal[0] + vNormal[1]*vNormal[1] + vNormal[2]*vNormal[2]);
    // нормирование
    vNormal[0] /= l;
    vNormal[1] /= l;
    vNormal[2] /= l;
}

void getNewCoord(int i)
{
    switch(i)
    {
    case 0: // 1 1 1
        point1[0] = octX + extendSides;      point1[1] = point1[2] =  extendSides;
        point2[0] = point2[2] = extendSides; point2[1] = octY + extendSides;
        point3[0] = point3[1] = extendSides; point3[2] = octZ + extendSides;
        if (!(paint%3)) glColor3f(1., 0., 0.);
        break;
    case 1: //-1 1 1
        point1[0] *= -1;
        point2[0] *= -1;
        point3[0] *= -1;
        if (!(paint%3)) glColor3f(0., 1., 0.);
        break;
    case 2: // -1 -1 1
        point1[1] *= -1;
        point2[1] *= -1;
        point3[1] *= -1;
        if (!(paint%3)) glColor3f(0., 0., 1.);
        break;
    case 3: //-1 -1 -1
        point1[2] *= -1;
        point2[2] *= -1;
        point3[2] *= -1;
        if (!(paint%3)) glColor3f(1., 1., 0.);
        break;
    case 4: // -1 1 -1
        point1[1] *= -1;
        point2[1] *= -1;
        point3[1] *= -1;
        if (!(paint%3)) glColor3f(1., 0., 1.);
        break;
    case 5: // 1 -1 -1
        point1[0] *= -1;
        point2[0] *= -1;
        point3[0] *= -1;
        point1[1] *= -1;
        point2[1] *= -1;
        point3[1] *= -1;
        if (!(paint%3)) glColor3f(0., 1., 1.);
        break;
    case 6: // 1 -1 1
        point1[2] *= -1;
        point2[2] *= -1;
        point3[2] *= -1;
        if (!(paint%3)) glColor3f(1., 1., 1.);
        break;
    case 7: // 1 1 -1
        point1[1] *= -1;
        point2[1] *= -1;
        point3[1] *= -1;
        point1[2] *= -1;
        point2[2] *= -1;
        point3[2] *= -1;
        break;
    }
}
