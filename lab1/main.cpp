#include <iostream>
#include <GL/glut.h>
#include <cstdio>
#include <cmath>

using namespace std;

#define PI 3.14

// yellow
#define R 1.0
#define G 1.0
#define B 0.0

const int width = 700; //x
const int height = 600; //y

const float radius = 50.0;
// координаты центра окружности
float X = 650.0;
float Y = 440.0;

bool count = 0; // для смены дня и ночи

void reshape(int w, int h)
{
        glViewport(0, 0, w, h); // задание области вывода (0,0) - левый нижний угол, (w, h) - ширина и высота

        glMatrixMode(GL_PROJECTION); // задание перспективной проекции;
                                     // матрица проекций задает как будут проецироваться трехмерные объекты
                                     // на плоскость экрана (в оконные координаты)
        glLoadIdentity(); // заменяет текущую матрицу на единичную
        gluOrtho2D(0, w, 0, h); // задание ортографической (параллельной) проекции

        glMatrixMode(GL_MODELVIEW); // далее будет проводиться только преобразование объектов сцены
        glLoadIdentity();
}

void drawGround()
{
    glBegin(GL_QUADS); // каждые четыре вершины образуют четырехугольник
        glColor3f(0.13, 0.55, 0.13);
        glVertex2d(0, 0);
        glVertex2d(0, 150);
        glVertex2d(700, 150);
        glVertex2d(700, 0);
    glEnd();
}

void drawSky(float r, float g, float b)
{
    glBegin(GL_QUADS); // каждые четыре вершины образуют четырехугольник
        glColor3f(r, g, b);
        glVertex2d(0, 150);
        glVertex2d(0, 600);
        glVertex2d(700, 600);
        glVertex2d(700, 150);
    glEnd();
}

void drawHome()
{
    glBegin(GL_QUADS); // каждые четыре вершины образуют четырехугольник
        glColor3f(0.36, 0.1, 0.2);
        glVertex2d(100, 50);
        glVertex2d(100, 250);
        glVertex2d(350, 250);
        glVertex2d(350, 50);
    glEnd();
    glBegin(GL_LINES);
        glColor3f(0.4, 0.12, 0.21);
        for (int j = 50; j < 250; j += 40)
        {
            glVertex2d(100, j);
            glVertex2d(350, j);
        }
    glEnd();
}

void drawRoof()
{
    glBegin(GL_TRIANGLES); // каждая отдельная тройка вершин задает треугольник
        glColor3f(0.36, 0.12, 0.2);
        glVertex2d(75, 250);
        glVertex2d(225, 425);
        glVertex2d(375, 250);
    glEnd();
}

void drawCircle(float x, float y, float r, float g, float b)
{
    glBegin(GL_POLYGON);
        glColor3f(r, g, b);
        for (int i = 0; i < 360; i+=10)
        {
            float rad = i * PI / 180;
            float dx = radius * cos(rad);
            float dy = radius * sin(rad);
            glVertex2f(dx + x, dy + y);
        }
    glEnd();
}

void drawWindow()
{
    glBegin(GL_LINE_LOOP);
        glColor3f(1.0, 1.0, 1.0);
        glVertex2d(175, 125);
        glVertex2d(175, 200);
        glVertex2d(275, 200);
        glVertex2d(275, 125);
    glEnd();
    glBegin(GL_LINES);
        glVertex2d(225, 125);
        glVertex2d(225, 200);
        glVertex2d(175, 162);
        glVertex2d(275, 162);
    glEnd();
}

void timer(int val)
{
    if (X >= -radius) //движение влево
    {
        X--;
        if (Y <= (height - radius) && X >= width/ 2)
            Y += 0.35;
        else
            Y -= 0.45;
    }
    else if (X <= -radius)
    {
        X = 690;
        Y = 440;
        drawCircle(X, Y, R, G, 1.0);
        count = !count;
    }
    glutPostRedisplay(); //перерисовка экрана
    glutTimerFunc(10, timer, 1); //10 - время в миллисекундах, 1 - идентификатор таймера
}

void display()
{
        glClear(GL_COLOR_BUFFER_BIT);

        drawGround();
        if (!count)
        {
            drawSky(0.19, 0.6, 0.8);
            drawCircle(X, Y, R, G, B);
        }
        else
        {
            drawSky(0.32, 0.32, 0.32);
            drawCircle(X, Y, R, G, 1.0);
        }
        drawHome();
        drawWindow();
        drawRoof();

        glutSwapBuffers();
}

int main (int argc, char * argv[])
{
        glutInit(&argc, argv); // инициализация функций библиотеки GLUT
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // задание режима с двойной буферизацией, представление цвета в формате RGB

        glutInitWindowSize(width, height);
        glutCreateWindow("OpenGL lab1");

        glutReshapeFunc(reshape);
        glutDisplayFunc(display);
        glutTimerFunc(10, timer, 1); // время в миллисекундах, функция, идентиикатор таймера

        glutMainLoop(); // контроль всех событий

        return 0;
}
