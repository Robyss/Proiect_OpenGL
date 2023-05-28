
#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <GL/freeglut.h>
#include <math.h>
using namespace std;
// angle of rotation for the camera direction
float angle = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, z = 5.0f;

// Strada
const int TOTAL_LANES = 3;
const int LANES[] = {-10, 0, 10};
int current_lane = 1;

int currentTime = 0;

enum direction
{
    STANGA = -1,
    DREAPTA = 1,
    LOC = 0
};
direction directie = LOC; // directia deplasarii politistului, initial sta pe loc

const float police_speed = 0.3f;

void changeSize(int w, int h)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if (h == 0)
        h = 1;
    float ratio = w * 1.0 / h;

    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);

    // Reset Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set the correct perspective.
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW);
}

void drawSnowMan()
{
    glColor3f(1.0f, 1.0f, 1.0f);

    // Draw Body
    glTranslatef(0.0f, 0.75f, 0.0f);
    glutSolidSphere(0.75f, 20, 20);

    // Draw Head
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.25f, 20, 20);

    // Draw Eyes
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glTranslatef(0.05f, 0.10f, 0.18f);
    glutSolidSphere(0.05f, 10, 10);
    glTranslatef(-0.1f, 0.0f, 0.0f);
    glutSolidSphere(0.05f, 10, 10);
    glPopMatrix();

    // Draw Nose
    glColor3f(1.0f, 0.5f, 0.5f);
    glutSolidCone(0.08f, 0.5f, 10, 2);
}

void renderScene(void)
{
    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();
    // Set the camera
    gluLookAt(x, 1.0f, z,
              x + lx, 1.0f, z + lz,
              0.0f, 1.0f, 0.0f);

    if (currentTime == 0)
    {
        glClearColor(0.52f, 0.87f, 0.92f, 0.0f);
    }
    else if (currentTime == 1)
    {
        glClearColor(0.0f, 0.05f, 0.29f, 0.1f);
    }

    // Draw ground
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glVertex3f(-100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, -100.0f);
    glEnd();

    // Draw road
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex3f(-7.5f, 0.01f, -100.0f);
    glVertex3f(-7.5f, 0.01f, 100.0f);
    glVertex3f(7.5f, 0.01f, 100.0f);
    glVertex3f(7.5f, 0.01f, -100.0f);
    glEnd();

    // Draw road border
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(-7.5f, 0.015f, -100.0f);
    glVertex3f(-7.5f, 0.015f, 100.0f);
    glVertex3f(7.5f, 0.015f, -100.0f);
    glVertex3f(7.5f, 0.015f, 100.0f);
    glEnd();

    // Enable line stippling
    glEnable(GL_LINE_STIPPLE);

    // Set line stipple pattern (0xAAAA pentru linie punctată)
    glLineStipple(1, 0xAAAA);

    // Set line width
    glLineWidth(2.0f);

    // Draw dashed line in the middle of the road
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    float y = 0.016f;
    float zStart = -100.0f;
    float zEnd = 100.0f;
    float zIncrement = 5.0f;

    for (float z = zStart; z <= zEnd; z += zIncrement)
    {
        glVertex3f(2.5f, y, z);
        glVertex3f(2.5f, y, z + 2.0f);
    }
    for (float z = zStart; z <= zEnd; z += zIncrement)
    {
        glVertex3f(-2.5f, y, z);
        glVertex3f(-2.5f, y, z + 2.0f);
    }

    glEnd();

    // Disable line stippling
    glDisable(GL_LINE_STIPPLE);

    // drawSnowMan();
    // Movement politist

    // Movement politist
    if (directie == STANGA)
    {

        if (x - police_speed < (current_lane - 1) * 5.0f)
        {
            directie = LOC;
            x = (current_lane - 1) * 5.0f;
        }
        else
        {
            x -= police_speed;
        }
    }
    else if (directie == DREAPTA)
    {
        if (x + police_speed > (current_lane - 1) * 5.0f)
        {
            directie = LOC;
            x = (current_lane - 1) * 5.0f;
        }
        else
        {
            x += police_speed;
        }
    }

    glutPostRedisplay();
    glutSwapBuffers();
    glFlush();
}

void processNormalKeys(unsigned char key, int x, int y)
{
    float fraction = 0.1f;
    switch (key)
    {
    case 'l':
        angle -= 0.01f;
        lx = sin(angle);
        lz = -cos(angle);
        break;

    case 'a': // stanga
        x -= police_speed;
        break;
    case 'd':
        x += police_speed;
        break;
    case 27: // escape key
        exit(0);
        break;
    }
}

void moveRight()
{
    directie = DREAPTA;
    current_lane = min(current_lane + 1, TOTAL_LANES - 1);

    glutPostRedisplay();
}

void moveLeft()
{
    directie = STANGA;
    current_lane = max(current_lane - 1, 0);

    glutPostRedisplay();
}

void processSpecialKeys(int key, int xx, int yy)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        angle -= 0.01f;
        lx = sin(angle);
        lz = -cos(angle);
        break;
    case GLUT_KEY_DOWN:
        angle += 0.01f;
        lx = sin(angle);
        lz = -cos(angle);
        break;
    case GLUT_KEY_LEFT:
        moveLeft();
        break;
    case GLUT_KEY_RIGHT:
        moveRight();
        break;
    }
    cout << "X: " << x << "  Z: " << z << "  Angle: " << angle << '\n';
    cout << current_lane << '\n';
}

void callback_Main(int key)
{
    if (key == 0)
    {
        exit(0);
    }
}
void callback_Time(int key)
{
    currentTime = key;
}

int main(int argc, char **argv)
{
    // init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Scena 3D cu oameni de zapada");

    // register callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(processSpecialKeys);

    // OpenGL init
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);

    // Menu
    int menuMain, menuTimeOfDay;

    menuTimeOfDay = glutCreateMenu(callback_Time);
    glutAddMenuEntry("Zi ", 0);
    glutAddMenuEntry("Noapte ", 1);

    menuMain = glutCreateMenu(callback_Main);

    glutAddSubMenu("Zi/Noapte ", menuTimeOfDay);
    glutAddMenuEntry("Iesi ", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    // enter GLUT event processing cycle
    glutMainLoop();

    return 0;
}