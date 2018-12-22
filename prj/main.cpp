#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

static GLuint tex[9];
//окрас граней
// 0 - цвета, 1 - одна текстура, 2 - 8 текстур
static int paint = 0;

//поворот
static float rotateOctX = 10.; // движение Ox up/down
static float rotateOctY = 10.; // движение в Oy <- ->
static bool autoRotateROctY = false;
static bool autoRotateLOctY = false;

static bool isLight = true; // вкл./выкл. освещение
static float rotateLight = 1.; // угол вращения источника света
static bool autoRotateLightR = false;
static bool autoRotateLightL = false;

static const int speed = 2; // скорость вращения

static bool isBlend = false;

static bool isListOct = false;
static GLuint list;

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

void side()
{
    float norm[3] = {0. , 0. , 0.}; // массив для нормали
    glBegin(GL_TRIANGLES);
        point1[0] = octX + extendSides;      point1[1] = point1[2] =  extendSides;
        point2[0] = point2[2] = extendSides; point2[1] = octY + extendSides;
        point3[0] = point3[1] = extendSides; point3[2] = octZ + extendSides;
        getNormal(point1, point2, point3, norm); // получение нормали
        // построение граней
        glNormal3fv(norm); //нормаль к поверхности останется неизменной
        if (paint%3) glTexCoord2f(0.0, 0.0);
        glVertex3fv(point1);
        if (paint%3) glTexCoord2f(1.0, 0.0);
        glVertex3fv(point2);
        if (paint%3) glTexCoord2f(0.0, 1.0);
        glVertex3fv(point3);
    glEnd();
}

void sideSlice()
{
    float norm[3] = {0. , 0. , 0.}; // массив для нормали
    point1[0] = octX + extendSides;      point1[1] = point1[2] =  extendSides;
    point2[0] = point2[2] = extendSides; point2[1] = octY + extendSides;
    point3[0] = point3[1] = extendSides; point3[2] = octZ + extendSides;
    getNormal(point1, point2, point3, norm); // получение нормали
    // построение граней
    glNormal3fv(norm); //нормаль к поверхности останется неизменной
    for(int k = 0; k < 100; k++)
    {
        glBegin(GL_POLYGON);
        glVertex3f(point1[0]-k, point1[1]+k*1.5,point1[2]);
        glVertex3f(point3[0], point3[1]+k*1.5,point3[2]-k);
        glVertex3f(point1[0]-(k-1), point1[1]+(k+1)*1.5,point1[2]);
        glVertex3f(point3[0], point3[1]+(k+1)*1.5,point3[2]-(k-1));
        k+=0.1;
        glEnd();
    }
}
//отрисовка окраэдра линиями для использования display list
void drawOctSlice()
{
    glPushMatrix();
        if (!(paint%3)) glColor3f(1.0, 0.0, 0.0);
        sideSlice();//рисует грань

        glPushMatrix();
            glTranslatef(1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            if (!(paint%3)) glColor3f(0.0, 1.0, 0.0);
            sideSlice();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(1.0, 0.0, 0.0);
            glRotatef(180 ,0.0, 1.0, 0.0);
            if (!(paint%3)) glColor3f(0.0, 0.0, 1.0);
            sideSlice();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(1.0, 0.0, 0.0);
            glRotatef(270, 0.0, 1.0, 0.0);
            if (!(paint%3)) glColor3f(1.0, 1.0, 0.0);
            sideSlice();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(1.0, 0.0, 0.0);
            glRotatef(180.0, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            if (!(paint%3)) glColor3f(1.0, 0.0, 1.0);
            sideSlice();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(1.0, 0.0, 0.0);
            glRotatef(180.0, 1.0, 0.0, 0.0);
            glRotatef(180 ,0.0, 1.0, 0.0);
            if (!(paint%3)) glColor3f(0.0, 1.0, 1.0);
            sideSlice();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(1.0, 0.0, 0.0);
            glRotatef(180.0, 1.0, 0.0, 0.0);
            glRotatef(270, 0.0, 1.0, 0.0);
            if (!(paint%3)) glColor3f(1.0, 1.0, 1.0);
            sideSlice();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(1.0, 0.0, 0.0);
            glRotatef(180.0, 1.0, 0.0, 0.0);
            glRotatef(360, 0.0, 1.0, 0.0);
            if (!(paint%3)) glColor3f(0.10, 0.08, 0.24);
            sideSlice();
        glPopMatrix();
    glPopMatrix();
}

void drawOct()
{
    int i = 0;
    if ((paint%3) == 2) glBindTexture(GL_TEXTURE_2D, tex[i]);
    if ((paint%3) == 1) glBindTexture(GL_TEXTURE_2D, tex[8]);
    glPushMatrix();
        if (!(paint%3)) glColor3f(1.0, 0.0, 0.0);
        side();//рисует грань
        i++;
        if ((paint%3) == 2) glBindTexture(GL_TEXTURE_2D, tex[i]);
        glPushMatrix();
            glTranslatef(1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            if (!(paint%3)) glColor3f(0.0, 1.0, 0.0);
            side();
        glPopMatrix();
        i++;
        if ((paint%3) == 2) glBindTexture(GL_TEXTURE_2D, tex[i]);
        glPushMatrix();
            glTranslatef(1.0, 0.0, 0.0);
            glRotatef(180 ,0.0, 1.0, 0.0);
            if (!(paint%3)) glColor3f(0.0, 0.0, 1.0);
            side();
        glPopMatrix();
        i++;
        if ((paint%3) == 2) glBindTexture(GL_TEXTURE_2D, tex[i]);
        glPushMatrix();
            glTranslatef(1.0, 0.0, 0.0);
            glRotatef(270, 0.0, 1.0, 0.0);
            if (!(paint%3)) glColor3f(1.0, 1.0, 0.0);
            side();
        glPopMatrix();
        i++;
        if ((paint%3) == 2) glBindTexture(GL_TEXTURE_2D, tex[i]);
        glPushMatrix();
            glTranslatef(1.0, 0.0, 0.0);
            glRotatef(180.0, 1.0, 0.0, 0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            if (!(paint%3)) glColor3f(1.0, 0.0, 1.0);
            side();
        glPopMatrix();
        i++;
        if ((paint%3) == 2) glBindTexture(GL_TEXTURE_2D, tex[i]);
        glPushMatrix();
            glTranslatef(1.0, 0.0, 0.0);
            glRotatef(180.0, 1.0, 0.0, 0.0);
            glRotatef(180 ,0.0, 1.0, 0.0);
            if (!(paint%3)) glColor3f(0.0, 1.0, 1.0);
            side();
        glPopMatrix();
        i++;
        if ((paint%3) == 2) glBindTexture(GL_TEXTURE_2D, tex[i]);
        glPushMatrix();
            glTranslatef(1.0, 0.0, 0.0);
            glRotatef(180.0, 1.0, 0.0, 0.0);
            glRotatef(270, 0.0, 1.0, 0.0);
            if (!(paint%3)) glColor3f(1.0, 1.0, 1.0);
            side();
        glPopMatrix();
        i++;
        if ((paint%3) == 2) glBindTexture(GL_TEXTURE_2D, tex[i]);
        glPushMatrix();
            glTranslatef(1.0, 0.0, 0.0);
            glRotatef(180.0, 1.0, 0.0, 0.0);
            glRotatef(360, 0.0, 1.0, 0.0);
            if (!(paint%3)) glColor3f(0.10, 0.08, 0.24);
            side();
        glPopMatrix();
    glPopMatrix();
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

    if (isBlend)
    {
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    }
    else
    {
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }

    if (paint%3)
    {
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
        glRotatef(rotateOctX, 1.0, 0.0, 0.0);
        glRotatef(rotateOctY, 0.0, 1.0, 0.0);
        if (!isListOct)
            drawOct();
        else
            glCallList(list);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
    else
    {
        glPushMatrix();
        glRotatef(rotateOctX, 1.0, 0.0, 0.0);
        glRotatef(rotateOctY, 0.0, 1.0, 0.0);
        if (!isListOct)
            drawOct();
        else
            glCallList(list);
        glPopMatrix();
    }
    glDisable(GL_LIGHTING);
    glRotatef(rotateLight, 0., 1., 0.); // вращение сферы относительно 'Oy' под углом 1
    drawSphere();
    float lightpos[] = {0., 0., 200., 0.};
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    glEnable(GL_LIGHTING);
    glutSwapBuffers();
}

void loadTexture()
{
    unsigned char* image[9];
    int w[9], h[9], bpp[9];

    image[0] = stbi_load("img/img1.png", &w[0], &h[0], &bpp[0], STBI_rgb_alpha);
    image[1] = stbi_load("img/img2.png", &w[1], &h[1], &bpp[1], STBI_rgb_alpha);
    image[2] = stbi_load("img/img3.png", &w[2], &h[2], &bpp[2], STBI_rgb_alpha);
    image[3] = stbi_load("img/img4.png", &w[3], &h[3], &bpp[3], STBI_rgb_alpha);
    image[4] = stbi_load("img/img5.png", &w[4], &h[4], &bpp[4], STBI_rgb_alpha);
    image[5] = stbi_load("img/img6.png", &w[5], &h[5], &bpp[5], STBI_rgb_alpha);
    image[6] = stbi_load("img/img7.png", &w[6], &h[6], &bpp[6], STBI_rgb_alpha);
    image[7] = stbi_load("img/img8.png", &w[7], &h[7], &bpp[7], STBI_rgb_alpha);
    image[8] = stbi_load("img/img10.png", &w[8], &h[8], &bpp[8], STBI_rgb_alpha);
    for(int i = 0; i < 9; i++)
    {
        //Текстурирование
        glGenTextures(1, &tex[i]); //создание и актуализация необходимой текстуры
        glBindTexture(GL_TEXTURE_2D, tex[i]);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w[i], h[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, image[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        stbi_image_free(image[i]);
    }
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

    list = glGenLists(1);
    glNewList(list, GL_COMPILE);
        drawOctSlice();
    glEndList();

    loadTexture();
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
            isLight = isLight ? false : true;
            break;
        case GLUT_KEY_F2:
            rotateLight = (rotateLight >= 0) ? rotateLight + 5 : 360;
            break;
        case GLUT_KEY_F3:
            rotateLight = (rotateLight <= 0) ? 360 : rotateLight - 5;
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
        case GLUT_KEY_F6:
            autoRotateROctY = autoRotateROctY ? false : true;
            if (autoRotateROctY) autoRotateLOctY = false;
            break;
        case GLUT_KEY_F7:
            autoRotateLOctY = autoRotateLOctY ? false : true;
            if (autoRotateLOctY) autoRotateROctY = false;
            break;
        case GLUT_KEY_F8:
            paint += 1;
            break;
        case GLUT_KEY_F9:
            isBlend = isBlend ? false : true;
            break;
        case GLUT_KEY_F10:
            autoRotateLightR = autoRotateLightR ? false : true;
            if (autoRotateLightR) autoRotateLightL = false;
            break;
        case GLUT_KEY_F11:
            autoRotateLightL = autoRotateLightL ? false : true;
            if (autoRotateLightL) autoRotateLightR = false;
            break;
        case GLUT_KEY_F12:
            isListOct = isListOct ? false : true;
            break;
    }
    glutPostRedisplay();
}

void timer(int val)
{
    if (autoRotateROctY) //Ox
    {
        rotateOctY += speed;
        if (rotateOctY > 360) rotateOctY = speed;
    }
    else if (autoRotateLOctY) //Oy
    {
        rotateOctY -= speed;
        if (rotateOctY < -360) rotateOctY = -speed;
    }
    if (autoRotateLightR) //Ox
    {
        rotateLight += speed;
        if (rotateLight > 360) rotateLight = speed;
    }
    else if (autoRotateLightL) //Oy
    {
        rotateLight -= speed;
        if (rotateLight < -360) rotateLight = -speed;
    }
    glutPostRedisplay(); //перерисовка экрана
    glutTimerFunc(10, timer, 1); //10 - время в миллисекундах, 1 - идентификатор таймера
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
         << "Up/Down: движение в плоскости Ox" << endl
         << "F6: автоматическое вращение в <-" << endl
         << "F7: автоматическое вращение в ->" << endl
         << "F8: Изменить окрас граней" << endl
         << "F9: Установить прозрачность граней" << endl
         << "F10: автоматическое вращение источника света <-" << endl
         << "F11: автоматическое вращение источника света ->" << endl
         << "F12: нарисовать октаэдр с помощью display list" << endl;
}

int main(int argc, char * argv[])
{
    printRule();
    glutInit(&argc, argv); // инициализация функций библиотеки GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(width, height);
    glutCreateWindow("Oct");

    glutDisplayFunc(display);
    glutTimerFunc(10, timer, 1); // время в миллисекундах, функция, идентиикатор таймера
    init();
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);

    glutMainLoop(); // контроль всех событий

    return 0;
}
