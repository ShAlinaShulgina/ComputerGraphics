#include <iostream>
#include <cmath>
#include <GL/gl.h>
#include <GL/glut.h>

using namespace std;

const static int width = 500;
const static int height = 500;

static float octX = 100.0;
static float octY = 150.0;
static float octZ = 100.0;

static float extendSides = 5.; // коэффицент раздвижения граней
// массивы точек граней октаэдра
static float point1[3] = {0., 0., 0.};
static float point2[3] = {0., 0., 0.};
static float point3[3] = {0., 0., 0.};
//поворот
static float rotateOctX = 10.; // движение Ox up/down
static float rotateOctY = 10.; // движение в Oy <- ->

static bool isLight = true; // вкл./выкл. освещение
static float rotateLight = 1.; // угол вращения источника света

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
        glColor3f(1., 0., 0.);
        break;
    case 1: //-1 1 1
        point1[0] *= -1;
        point2[0] *= -1;
        point3[0] *= -1;
        glColor3f(0., 1., 0.);
        break;
    case 2: // -1 -1 1
        point1[1] *= -1;
        point2[1] *= -1;
        point3[1] *= -1;
        glColor3f(0., 0., 1.);
        break;
    case 3: //-1 -1 -1
        point1[2] *= -1;
        point2[2] *= -1;
        point3[2] *= -1;
        glColor3f(1., 1., 0.);
        break;
    case 4: // -1 1 -1
        point1[1] *= -1;
        point2[1] *= -1;
        point3[1] *= -1;
        glColor3f(1., 0., 1.);
        break;
    case 5: // 1 -1 -1
        point1[0] *= -1;
        point2[0] *= -1;
        point3[0] *= -1;
        point1[1] *= -1;
        point2[1] *= -1;
        point3[1] *= -1;
        glColor3f(0., 1., 1.);
        break;
    case 6: // 1 -1 1
        point1[2] *= -1;
        point2[2] *= -1;
        point3[2] *= -1;
        glColor3f(1., 1., 1.);
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

void drawOct()
{
    float norm[3] = {0. , 0. , 0.}; // массив для нормали
    glBegin(GL_TRIANGLES);
        for(int i = 0; i < 8; i++)
        {
            getNewCoord(i); // получение новых координат
            getNormal(point1, point2, point3, norm); // получение нормали
            // построение граней
            glNormal3fv(norm);
            if (i == 7)
                glColor3f(1., 0., 0.);
            glVertex3fv(point1);
            if (i == 7)
                glColor3f(0., 1., 0.);
            glVertex3fv(point2);
            if (i == 7)
                glColor3f(0., 0., 1.);
            glVertex3fv(point3);
        }
    glEnd();
}
// построение сферы - светильника
void drawSphere()
{
    glColor3f(1., 1., 1.);
    glTranslatef(0., 0., 200.);
    glutWireSphere(10., 25, 25);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    if (isLight)
        glEnable(GL_LIGHT0);
    else
        glDisable(GL_LIGHT0);

    glPushMatrix();
    glRotatef(rotateOctX, 1.0, 0.0, 0.0);
    glRotatef(rotateOctY, 0.0, 1.0, 0.0);
    drawOct();
    glPopMatrix();

    glDisable(GL_LIGHTING);
    glRotatef(rotateLight, 0., 1., 0.); // вращение сферы относительно 'Oy' под углом 1
    drawSphere();
    float lightpos[] = {0., 0., 200., 0.};
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    glEnable(GL_LIGHTING);
    glutSwapBuffers();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING); // глобальное
    glEnable(GL_LIGHT0);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // двухстороннее освещение

    GLfloat light_col[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_col);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
}

void reshape(int w, int h)
{
        glViewport(0, 0, width, height); // задание области вывода (0,0) - левый нижний угол, (w, h) - ширина и высота
        glMatrixMode(GL_PROJECTION); // задание перспективной проекции;
                                     // матрица проекций задает как будут проецироваться трехмерные объекты
                                     // на плоскость экрана (в оконные координаты)
        glLoadIdentity(); // заменяет текущую матрицу на единичную
        glOrtho(-width/2, width/2, -height/2, height/2, -500, 500); // задание ортографической (параллельной) проекции
        glMatrixMode(GL_MODELVIEW); // далее будет проводиться только преобразование объектов сцены
        glLoadIdentity();
}

void keyboard(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_F1:
            isLight = (isLight) ? false : true;
            break;
        case GLUT_KEY_F2:
            rotateLight = (rotateLight >= 0) ? rotateLight + 5 : 350;
            break;
        case GLUT_KEY_F3:
            rotateLight = (rotateLight <= 0) ? 350 : rotateLight - 5;
            break;
        case GLUT_KEY_F4:
            extendSides = (abs(extendSides) < 200) ? abs(extendSides) + 5 : 95;
            break;
        case GLUT_KEY_F5:
            extendSides = (abs(extendSides) <= 5) ? 0 : abs(extendSides) - 5;
            break;
        case GLUT_KEY_RIGHT:
            rotateOctY += 5;
            break;
        case GLUT_KEY_LEFT:
            rotateOctY -= 5;
            break;
        case GLUT_KEY_UP:
            rotateOctX += 5;
            break;
        case GLUT_KEY_DOWN:
            rotateOctX -= 5;
            break;
    }
    glutPostRedisplay();
}

void printRule()
{
    cout << "Клавиши:" << endl
         << "F1: выкл./вкл. свет" << endl
         << "F2: движение источника света ->" << endl
         << "F3: движение источника света <-" << endl
         << "F4: раздвинуть грани" << endl
         << "F5: сдвинуть грани" << endl
         << "Left/Right: движение в полоскости Oy <-/-> " << endl
         << "Up/Down: движение в плоскости Ox" << endl;
}

int main(int argc, char * argv[])
{
    printRule();
    glutInit(&argc, argv); // инициализация функций библиотеки GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(width, height);
    glutCreateWindow("Oct");

    glutDisplayFunc(display);
    init();
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);

    glutMainLoop(); // контроль всех событий

    return 0;
}
