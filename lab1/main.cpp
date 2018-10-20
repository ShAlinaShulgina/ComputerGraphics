#include <iostream>
#include <GL/glut.h>
#include <cstdio>
#include <cmath>

using namespace std;

#define PI 3.14

// yellow
#define colorR 1.0
#define colorG 1.0
#define colorB 0.0

const int width = 700; //x
const int height = 600; //y

const float radius = 50.0;
// координаты центра окружности
float centerSunX = 650.0;
float centerSunY = 440.0;

bool isDay = 0; // для смены дня и ночи

void reshape(int w, int h)
{
        glViewport(0, 0, width, height); // задание области вывода (0,0) - левый нижний угол, (w, h) - ширина и высота

        glMatrixMode(GL_PROJECTION); // задание перспективной проекции;
                                     // матрица проекций задает как будут проецироваться трехмерные объекты
                                     // на плоскость экрана (в оконные координаты)
        glLoadIdentity(); // заменяет текущую матрицу на единичную
        gluOrtho2D(0, width, 0, height); // задание ортографической (параллельной) проекции

        glMatrixMode(GL_MODELVIEW); // далее будет проводиться только преобразование объектов сцены
        glLoadIdentity();
}

void drawGround()
{
    glBegin(GL_QUADS); // каждые четыре вершины образуют четырехугольник
        glColor3f(0.13, 0.55, 0.13);
        glVertex2i(0, 0);
        glVertex2i(0, 150);
        glVertex2i(700, 150);
        glVertex2i(700, 0);
    glEnd();
}

void drawSky(float red, float green, float blue)
{
    glBegin(GL_QUADS); // каждые четыре вершины образуют четырехугольник
        glColor3f(red, green, blue);
        glVertex2i(0, 150);
        glVertex2i(0, 600);
        glVertex2i(700, 600);
        glVertex2i(700, 150);
    glEnd();
}

void drawHome()
{
    glBegin(GL_QUADS); // каждые четыре вершины образуют четырехугольник
        glColor3f(0.36, 0.1, 0.2);
        glVertex2i(100, 50);
        glVertex2i(100, 250);
        glVertex2i(350, 250);
        glVertex2i(350, 50);
    glEnd();
    glBegin(GL_LINES);
        glColor3f(0.4, 0.12, 0.21);
        for (int j = 50; j < 250; j += 40)
        {
            glVertex2i(100, j);
            glVertex2i(350, j);
        }
    glEnd();
}

void drawRoof()
{
    glBegin(GL_TRIANGLES); // каждая отдельная тройка вершин задает треугольник
        glColor3f(0.36, 0.12, 0.2);
        glVertex2i(75, 250);
        glVertex2i(225, 425);
        glVertex2i(375, 250);
    glEnd();
}

void drawCircle(float center_x, float center_y, float red, float green, float blue)
{
    glBegin(GL_POLYGON);
        glColor3f(red, green, blue);
        for (int i = 0; i < 360; i+=10)
        {
            float rad = i * PI / 180;
            float dx = radius * cos(rad);
            float dy = radius * sin(rad);
            glVertex2f(dx + center_x, dy + center_y);
        }
    glEnd();
}

void drawWindow()
{
    glBegin(GL_LINE_LOOP);
        glColor3f(1.0, 1.0, 1.0);
        glVertex2i(175, 125);
        glVertex2i(175, 200);
        glVertex2i(275, 200);
        glVertex2i(275, 125);
    glEnd();
    glBegin(GL_LINES);
        glVertex2i(225, 125);
        glVertex2i(225, 200);
        glVertex2i(175, 162);
        glVertex2i(275, 162);
    glEnd();
}

void timer(int val)
{
    if (centerSunX >= -radius) //движение влево
    {
        centerSunX--;
        if (centerSunY <= (height - radius) && centerSunX >= width/ 2)
            centerSunY += 0.35;
        else
            centerSunY -= 0.45;
    }
    else if (centerSunX <= -radius)
    {
        centerSunX = 690;
        centerSunY = 440;
        drawCircle(centerSunX, centerSunY, colorR, colorG, 1.0);
        isDay = !isDay;
    }
    glutPostRedisplay(); //перерисовка экрана
    glutTimerFunc(10, timer, 1); //10 - время в миллисекундах, 1 - идентификатор таймера
}

void display()
{
        glClear(GL_COLOR_BUFFER_BIT);

        drawGround();
        if (!isDay)
        {
            drawSky(0.19, 0.6, 0.8);
            drawCircle(centerSunX, centerSunY, colorR, colorG, colorB);
        }
        else
        {
            drawSky(0.32, 0.32, 0.32);
            drawCircle(centerSunX, centerSunY, colorR, colorG, 1.0);
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
