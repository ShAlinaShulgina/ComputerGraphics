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

//массив координат для солнца
GLfloat arrCoord[36 * 2];

void getArrayDxDy()
{
    for (int i = 0, j = 0; i < 360; i+=10, j+=2)
    {
        float rad = i * PI / 180;
        float dx = radius * cos(rad);
        float dy = radius * sin(rad);
        arrCoord[j] = dx;
        arrCoord[j + 1] = dy;
        //cout << j << " : " << arrCoord[j] << " : " << arrCoord[j + 1] << endl;
    }
}

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

// рисование дома, окна и крыши
void drawHouse()
{
    GLint arrHouse[] = { //house
                         100, 50, 100, 250,
                         350, 250, 350, 50,
                         // window
                         175, 125, 175, 200,
                         275, 200, 275, 125,
                         225, 125, 225, 200,
                         175, 162, 275, 162,
                         // roof
                         75, 250, 225, 425, 375, 250 };
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_INT, 0, arrHouse);
    glColor3f(0.36, 0.1, 0.2);
    glDrawArrays(GL_QUADS, 0, 4);

    glColor3f(1.0, 1.0, 1.0);
    glDrawArrays(GL_LINE_LOOP, 4, 4);
    glDrawArrays(GL_LINES, 8, 4);

    glColor3f(0.36, 0.12, 0.2);
    glDrawArrays(GL_TRIANGLES, 12, 3);
    glDisableClientState(GL_VERTEX_ARRAY);
}

// рисование травы и неба
void drawWorld()
{
    GLint arrWorld[] = { // ground
                         0, 0, 0, 150, width, 150, width, 0,
                         // sky
                          0, 150, 0, height, width, height, width, 150 };
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_INT, 0, arrWorld);
    glColor3f(0.13, 0.55, 0.13);
    glDrawArrays(GL_QUADS, 0, 4);

    if (!isDay)
    {
        glColor3f(0.19, 0.6, 0.8);
        glDrawArrays(GL_QUADS, 4, 4);
    }
    else
    {
        glColor3f(0.32, 0.32, 0.32);
        glDrawArrays(GL_QUADS, 8, 4);
    }
    glDisableClientState(GL_VERTEX_ARRAY);
}

void drawCircle(float center_x, float center_y)
{
    GLfloat arrSun[2 * 36];
    for(int i = 0; i < 36; i += 2)
    {
        arrSun[i] = arrCoord[i] + center_x;
        arrSun[i + 1] = arrCoord[i + 1] + center_y;
    }

    float colorSun[] = { 1.0, 1.0, 0.0, //day
                         1.0, 1.0, 1.0 }; //night
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(2 * 36, GL_FLOAT, 0, arrSun);
    if (isDay)
        glColorPointer(3, GL_FLOAT, 0, colorSun);    
    else
        glColorPointer(3, GL_FLOAT, 3, colorSun); 
    glDrawArrays(GL_POLYGON, 0, 2 * 36);
    glDisableClientState(GL_VERTEX_ARRAY);
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
        isDay = !isDay;
        centerSunX = 690;
        centerSunY = 440;
        drawCircle(centerSunX, centerSunY);
    }
    glutPostRedisplay(); //перерисовка экрана
    glutTimerFunc(10, timer, 1); //10 - время в миллисекундах, 1 - идентификатор таймера
}

void display()
{
        glClear(GL_COLOR_BUFFER_BIT);

        drawWorld();
        drawCircle(centerSunX, centerSunY);
        drawHouse();

        glutSwapBuffers();
}

int main (int argc, char * argv[])
{
        getArrayDxDy();
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
