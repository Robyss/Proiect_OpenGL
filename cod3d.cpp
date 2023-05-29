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
const float LANES[] = {-5.0f, 0.0f, 5.0f};
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
const float people_start_position = -60.0f;
float people_speed = 0.2f;
float people_position = -80.0f;
float people_lane = LANES[rand() % TOTAL_LANES];

bool isGameOver = false;
bool isGamePaused = false;

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

void game_logic()
{
    if (isGameOver || isGamePaused)
        return;

    people_position += people_speed;
    if (people_position > 2.0f)
    {
        if (abs(x - people_lane) < 0.5f)
        {
            people_position = people_start_position;

            people_lane = LANES[rand() % TOTAL_LANES];
        }
        else if (people_position > 6.0f)
        {
            cout << "Ai pierdut!\n";
            isGameOver = true;
        }
    }

    /////////////////////
    // Movement politist
    /////////////////////
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
}

void drawPerson()
{
    glPushMatrix();
    glColor3f(0.435, 0.31, 0.114);
    glTranslatef(0.0f, 0.75f, 0.0f);
    glScalef(0.7f, 1.0f, 0.7f);
    glutSolidSphere(0.75f, 20, 20);
    glPopMatrix();

    // Draw Head
    glPushMatrix();
    glTranslatef(0.0f, 1.75f, 0.0f);
    glutSolidSphere(0.25f, 20, 20);

    // Draw Eyes
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 1.0f); // Set color to blue for eyes
    glTranslatef(0.05f, 0.10f, 0.18f);
    glutSolidSphere(0.05f, 10, 10);
    glTranslatef(-0.1f, 0.0f, 0.0f);
    glutSolidSphere(0.05f, 10, 10);
    glPopMatrix();

    // Draw Nose
    glColor3f(1.0f, 0.5f, 0.5f); // Set color to a light shade of red for nose
    glutSolidCone(0.08f, 0.5f, 10, 2);

    glPopMatrix();

    // Draw Arms/Hands
    glPushMatrix();
    glColor3f(0.435, 0.31, 0.114);       // Set color to match the body color
    glTranslatef(-0.75f, 0.25f, 0.0f);   // Position the first hand
    glRotatef(-45.0f, 0.0f, 0.0f, 1.0f); // Rotate the hand
    glScalef(0.2f, 0.6f, 0.2f);          // Scale the hand
    glutSolidCube(1.0f);                 // Draw the first hand

    // glTranslatef(3.75f, 0.0f, 0.0f);    // Position the second hand
    // glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // Rotate the hand
    // glScalef(0.2f, 0.6f, 0.2f);         // Scale the hand
    // glutSolidCube(1.0f);                // Draw the second hand

    glPopMatrix();
}

void drawScene()
{
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

    glDisable(GL_LIGHTING);

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
    glEnable(GL_LIGHTING);
}

void renderScene(void)
{
    // Clear Color and Depth Buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();
    // Set the camera
    gluLookAt(x, 1.5f, z,
              x + lx, 1.5f, z + lz,
              0.0f, 1.5f, 0.0f);

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    // Set light properties
    GLfloat light_position[] = {15.0f, 5.0f, 5.0f, 1.0f}; // Light position
    GLfloat light_color[] = {0.3f, 0.3f, 0.3f, 0.3f};     // Light color
    if (currentTime == 0)
    {
        light_color[0] = 1.0f;
        light_color[1] = 1.0f;
        light_color[2] = 1.0f;
        light_color[3] = 1.0f;

        light_position[0] = 15.0f;
        light_position[1] = 5.0f;
        light_position[2] = 5.0f;
        light_position[3] = 1.0f;
    }
    else if (currentTime == 1)
    {
        light_color[0] = 0.3f;
        light_color[1] = 0.3f;
        light_color[2] = 0.3f;
        light_color[3] = 0.3f;

        light_position[0] = -10.0f;
        light_position[1] = 5.0f;
        light_position[2] = 5.0f;
        light_position[3] = 1.0f;
    }

    glLightfv(GL_LIGHT0, GL_POSITION, light_position); // Set light position
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);     // Set light color
    glEnable(GL_LIGHT0);

    // Add light source sphere
    glPushMatrix();
    glTranslatef(light_position[0], light_position[1], light_position[2]);
    glColor3f(1.0f, 1.0f, 1.0f);
    glutSolidSphere(0.1f, 10, 10);
    glPopMatrix();

    // Draw scene
    drawScene();

    // Draw people
    glPushMatrix();
    glTranslatef(people_lane, 0.0f, people_position);
    drawPerson();
    glPopMatrix();

    game_logic();

    glutPostRedisplay();
    glutSwapBuffers();
    glFlush();
}

void initializeOpenGL()
{
    // Alte inițializări OpenGL...
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);

    // Activează efectul de ceață
    glEnable(GL_FOG);

    const float fogZStart = -90.0f;
    const float fogZEnd = 0.0f;
    // Setează parametrii ceții
    GLfloat fogColor[] = {0.3f, 0.3f, 0.3f, 1.0f}; // Culoarea ceții (în acest caz, un gri deschis)
    glFogfv(GL_FOG_COLOR, fogColor);               // Setează culoarea ceții

    // Setează parametrii pentru ceața pe axa Oz
    glFogf(GL_FOG_MODE, GL_EXP2);    // Modul de calcul al ceții (în acest caz, liniar)
    glFogf(GL_FOG_START, fogZStart); // Poziția de început a ceții pe axa Oz
    glFogf(GL_FOG_END, fogZEnd);     // Poziția de sfârșit a ceții pe axa Oz

    glFogf(GL_FOG_DENSITY, 0.03f);  // Densitatea ceții (poți ajusta acest parametru pentru a obține efectul dorit)
    glHint(GL_FOG_HINT, GL_NICEST); // Sugestie pentru calculul ceții (în acest caz, sugestia de cea mai bună calitate)
}

void processNormalKeys(unsigned char key, int x, int y)
{
    float fraction = 0.1f;
    switch (key)
    {
    case 'r':
        isGameOver = false;
        people_position = people_start_position;
        people_lane = LANES[rand() % TOTAL_LANES];
        break;
    case 'n':
        currentTime = (currentTime + 1) % 2;
        break;
    case 'c':
        glDisable(GL_FOG);
        break;
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
    case 32: // space key
        isGamePaused = !isGamePaused;
        break;
    }
}

void moveRight()
{
    if (isGameOver)
        return;

    directie = DREAPTA;
    current_lane = min(current_lane + 1, TOTAL_LANES - 1);

    glutPostRedisplay();
}

void moveLeft()
{
    if (isGameOver)
        return;

    directie = STANGA;
    current_lane = max(current_lane - 1, 0);

    glutPostRedisplay();
}

void processSpecialKeys(int key, int xx, int yy)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        angle -= 0.05f;
        lx = sin(angle);
        lz = -cos(angle);
        break;
    case GLUT_KEY_DOWN:
        angle += 0.05f;
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
    initializeOpenGL();

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