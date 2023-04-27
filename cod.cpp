#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <GL/freeglut.h>

using namespace std;

bool isGameOver = false;
bool isGameRunning = true;
bool isGamePaused = false;

// Strada
const int TOTAL_LANES = 3;
const int LANES[TOTAL_LANES] = {0, 160, 320};

// Politist
double height_police = 0.0; // oy politist
int directie = 0;           // -1 jos, 1 sus, 0 pe loc

// Persoane
double height_people = LANES[rand() % 3]; // folosit ca si locatie oy pentru persoane
double location_people = 1200;

// Viteze - pe device-uri performante recomand police = 0.15, people = 0.05, raise_speed=0.05 ?
const double police_speed = 30.0;
double people_speed = 10.15;
const double raise_speed = 5;

int score = 0;
int prag_puncte = 1000;

void init(void)
{
    GLdouble left_m = -100.0;
    GLdouble right_m = 700.0;
    GLdouble bottom_m = -140.0;
    GLdouble top_m = 460.0;
    glClearColor(0.561, 0.58, 0.569, 0.0); // culoare sosea
    glMatrixMode(GL_PROJECTION);
    glOrtho(left_m, right_m, bottom_m, top_m, -1.0, 1.0);
}

void RenderString(float x, float y, void *font, const unsigned char *string)
{
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x, y);
    glutBitmapString(font, string);
}
void startgame(void)
{
    if (location_people < -45)
    {
        if (isGameOver == false) // Sa nu se randeze in consola de mai multe ori
        {
            cout << "Game Over!\n";
        }
        isGameOver = true;
    }
    else
    {
        location_people -= people_speed;
        // cout << location_people << '\n';

        if (height_people == height_police && (location_people < 45 && location_people > -15))
        {
            score += 100;
            height_people = LANES[rand() % 3];
            cout << "Score:  " << score << endl;
            location_people = 800;
        }
        if (score >= prag_puncte && prag_puncte <= 3000)
        {
            people_speed += raise_speed;
            prag_puncte += 500;
        }

        glutPostRedisplay();
    }
}

void drawScene(void)
{
    if (!isGamePaused)
    {
        // #8F9491 culoare strada
        // #2B4570 culoare politist

        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(0.42, 0.212, 0.137); // culoare cladiri

        // Iarba de jos
        glBegin(GL_POLYGON);
        glVertex2i(-100, -140); // Stanga jos
        glVertex2i(700, -140);  // Dreapta jos
        glVertex2i(700, -80);   // Dreapta sus
        glVertex2i(-100, -80);  // Stanga sus
        glEnd();

        // Iarba de sus
        glBegin(GL_POLYGON);
        glVertex2i(-100, 400); // Stanga jos
        glVertex2i(700, 400);  // Dreapta jos
        glVertex2i(700, 460);  // Dreapta sus
        glVertex2i(-100, 460); // Stanga sus
        glEnd();
        if (isGameOver)
            RenderString(250.0f, 425.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)"Ai pierdut!");
        else
            RenderString(150.0f, 425.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)"Nu lasa ciorile sa treaca!");

        RenderString(400.0f, 425.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)("Scor: " + to_string(score / 100)).c_str());

        // Delimitare sosea
        glLineWidth(3);
        glColor3f(1, 1, 1);

        // Delimitam soseaua de iarba partea de jos
        glBegin(GL_LINES);
        glVertex2i(-100, -80);
        glVertex2i(1500, -80);
        glEnd();

        // Delimitam soseaua de iarba partea de sus
        glBegin(GL_LINES);
        glVertex2i(-100, 400);
        glVertex2i(1500, 400);
        glEnd();

        glBegin(GL_LINES);
        glVertex2i(-100, 80);
        glVertex2i(1500, 80);
        glEnd();

        glBegin(GL_LINES);
        glVertex2i(-100, 240);
        glVertex2i(1500, 240);
        glEnd();

        // desenam politistul
        glPushMatrix();
        glTranslated(0.0, height_police, 0.0);
        glColor3f(0.169, 0.271, 0.439);
        glRecti(-15, -15, 15, 15);
        glPopMatrix();

        if (isGameOver == true)
        {
            RenderString(250.0f, 200.0f, GLUT_BITMAP_8_BY_13, (const unsigned char *)"GAME OVER");
        }

        // Movement politist
        if (directie == 1)
        {
            if (height_police < 160 && height_police + police_speed >= 160)
            {
                height_police = 160;
                directie = 0;
            }
            else if (height_police < 320 && height_police + police_speed >= 320)
            {
                height_police = 320;
                directie = 0;
            }
            else
                height_police += police_speed;
        }
        else if (directie == -1)
        {
            if (height_police > 160 && height_police - police_speed <= 160)
            {
                height_police = 160;
                directie = 0;
            }
            else if (height_police > 0 && height_police - police_speed <= 0)
            {
                height_police = 0;
                directie = 0;
            }
            else
                height_police -= police_speed;
        }
        else
        {
            directie = 0;
        }

        // desenam persoana adversa
        glPushMatrix();
        glTranslated(location_people, height_people, 0.0);

        glColor3f(0.1, 0.1, 0.1); // negru
        glRecti(-45, -15, -15, 15);

        glPopMatrix();

        startgame(); // logica coliziuni
        glutPostRedisplay();
        glutSwapBuffers();
        glFlush();
    }
    else
    {
        glClear(GL_COLOR_BUFFER_BIT);

        RenderString(250.0f, 200.0f, GLUT_BITMAP_8_BY_13, (const unsigned char *)"GAME PAUSED");
        glutPostRedisplay();
        glutSwapBuffers();
        glFlush();
    }
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-100.0, 700.0, -140.0, 460.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void miscasus(void)
{
    if (isGameOver != true && isGamePaused != true)
    {
        if (height_police < 320)
        {
            directie = 1;
        }

        glutPostRedisplay();
    }
}

void miscajos(void)
{
    if (isGameOver != true && isGamePaused != true)
    {
        if (height_police > 0)
        {
            directie = -1;
        }

        glutPostRedisplay();
    }
}

void keyboard(int key, int x, int y)
{

    switch (key)
    {
    case GLUT_KEY_UP:
        miscasus();
        break;
    case GLUT_KEY_DOWN:
        miscajos();
        break;
    }
}

void keyboard_callback(unsigned char key, int x, int y)
{
    switch (key)
    {
    case ' ': // Space key
        isGamePaused = !isGamePaused;
        break;
    case 'r':
        isGameOver = false;
        isGamePaused = false;
        height_police = 0.0;
        height_people = LANES[rand() % 3];
        location_people = 1200.0;
        people_speed = 10.15;
        score = 0;
        prag_puncte = 1000;

        break;

    default:
        break;
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Prinde ciorile - mini game");
    init();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard_callback);
    glutSpecialFunc(keyboard);

    glutMainLoop();
    return 0;
}