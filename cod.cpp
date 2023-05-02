#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <GL/freeglut.h>
#include <string> // pentru windows

using namespace std;

bool isGameOver = false;
bool isGameRunning = true;
bool isGamePaused = false;
bool isGameWon = false;

// Strada
const int TOTAL_LANES = 3;
const int LANES[TOTAL_LANES] = {0, 160, 320};
int current_lane = 0;

// Politist
double height_police = 0.0; // oy politist
enum direction
{
    JOS = -1,
    SUS = 1,
    LOC = 0
};
direction directie = LOC; // directia deplasarii politistului, initial sta pe loc

// Persoane / Infractori
double height_people = LANES[rand() % 3]; // folosit ca si locatie oy pentru persoane
double location_people = 1200;

// Viteze - pe device-uri performante recomand police = 0.15, people = 0.05, raise_speed=0.05
const double police_speed = 30.0;
double people_speed = 10.15;
const double raise_speed = 4;

int score = 0;
int prag_puncte = 1000;
double progress = 0.0;
const double finish_progress = 560.0;

double reinforcement = -100.0;

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

void RenderString(float x, float y, void *font, const unsigned char *string, int color = 0)
{
    if (color == 1)
        glColor3f(1.0f, 1.0f, 1.0f);
    else
        glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x, y);
    glutBitmapString(font, string);
}

void startgame(void)
{
    if (isGameRunning && !isGameOver && progress < 560)
        progress += 0.2;

    if (progress >= 560)
    {
        isGameRunning = false;
    }

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
        if (!isGameRunning && location_people == 800)
        {
            isGameWon = true;
            return;
        }

        location_people -= people_speed;
        // cout << location_people << '\n';

        if (height_people == height_police && (location_people < 45 && location_people > -15))
        {
            score += 100;
            height_people = LANES[rand() % 3];
            cout << "Score:  " << score << endl;
            location_people = 800;
        }
        if (score >= prag_puncte && prag_puncte <= 3500)
        {
            people_speed += raise_speed;
            prag_puncte += 500;
        }

        glutPostRedisplay();
    }
}

void drawPoliceMan()
{
    // Figura 30x60
    /*
        (-15, 30)   (15, 30)

        (-15, -30)  (15, -30)
    */
    int h = 30;  // height
    int l = -30; // lower height

    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 1.0);
    glVertex2i(-15, h);
    glVertex2i(-15, h - 10);
    glVertex2i(15, h - 10);
    glVertex2i(15, h);
    glEnd();

    // Head
    glBegin(GL_POLYGON);
    glColor3f(0.91, 0.745, 0.675);
    glVertex2i(-15, h - 10);
    glVertex2i(-15, h - 10 - 20); // baza - modificare
    glVertex2i(15, h - 10 - 20);
    glVertex2i(15, h - 10);
    glEnd();

    // Neck
    glBegin(GL_POLYGON);
    glColor3f(0.81, 0.645, 0.575);
    glVertex2i(-8, h - 30);
    glVertex2i(-8, h - 30 - 2);
    glVertex2i(8, h - 30 - 2);
    glVertex2i(8, h - 30);
    glEnd();

    // Fata
    if (isGameOver)
    {
        glBegin(GL_LINES);
        glColor3f(0.0, 0.0, 0.0);
        glVertex2i(8, h - 15);
        glVertex2i(13, h - 15 - 5);
        glVertex2i(8, h - 15 - 5);
        glVertex2i(13, h - 15);

        glVertex2i(0, h - 15);
        glVertex2i(-5, h - 15 - 5);
        glVertex2i(0, h - 15 - 5);
        glVertex2i(-5, h - 15);
        glEnd();
    }
    else
    {
        // Ochi drept
        glBegin(GL_QUADS);
        glColor3f(0.0, 0.0, 0.0);
        glVertex2i(8, h - 15);
        glVertex2i(8, h - 15 - 5);
        glVertex2i(13, h - 15 - 5);
        glVertex2i(13, h - 15);

        // Ochi stang
        glVertex2i(0, h - 15);
        glVertex2i(0, h - 15 - 5);
        glVertex2i(-5, h - 15 - 5);
        glVertex2i(-5, h - 15);
        glEnd();
    }

    // Gura
    if (!isGameOver)
    {
        glBegin(GL_QUADS);
        glVertex2i(0, h - 25);
        glVertex2i(0, h - 25 - 2);
        glVertex2i(10, h - 25 - 2);
        glVertex2i(10, h - 25);
        glEnd();
    }

    // Brate
    glBegin(GL_QUADS);
    glColor3f(0.81, 0.645, 0.575);
    glVertex2i(-15, h - 32);
    glVertex2i(-15, h - 32 - 20);
    glVertex2i(-10, h - 32 - 20);
    glVertex2i(-10, h - 32);

    glVertex2i(10, h - 32);
    glVertex2i(10, h - 32 - 20);
    glVertex2i(15, h - 32 - 20);
    glVertex2i(15, h - 32);
    glEnd();

    // Corp
    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 0.7);
    glVertex2i(-10, h - 32);
    glVertex2i(-10, l + 3);
    glVertex2i(10, l + 3);
    glVertex2i(10, h - 32);
    glEnd();
    glRecti(-15, h - 32, 15, h - 40);

    // Picioare
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2i(-10, l + 3);
    glVertex2i(-10, l - 4);
    glVertex2i(-5, l - 4);
    glVertex2i(-5, l + 3);

    glVertex2i(5, l + 3);
    glVertex2i(5, l - 4);
    glVertex2i(10, l - 4);
    glVertex2i(10, l + 3);

    glEnd();
}

void drawInfractor()
{
    /* Figura

    (-45, 15)  (-15, 15)

    (-45, -15) (-15, -15)

    */

    int h = 30;  // height
    int l = -30; // lower height

    // Caciula
    glBegin(GL_POLYGON);
    glColor3f(0.1, 0.1, 0.1);
    glVertex2i(-15, h);
    glVertex2i(-15, h - 10);
    glVertex2i(15, h - 10);
    glVertex2i(15, h);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(0.561, 0.58, 0.569);
    glVertex2i(-15, h);
    glVertex2i(-15, h - 5);
    // glVertex2i(-10, h - 5);
    glVertex2i(-8, h);

    glVertex2i(8, h);
    // glVertex2i(10, h - 5);
    glVertex2i(15, h - 5);
    glVertex2i(15, h);

    glEnd();
    // Head
    glBegin(GL_POLYGON);
    glColor3f(0.435, 0.31, 0.114);
    glVertex2i(-15, h - 10);
    glVertex2i(-15, h - 10 - 20); // baza - modificare
    glVertex2i(15, h - 10 - 20);
    glVertex2i(15, h - 10);
    glEnd();

    // Neck
    glBegin(GL_POLYGON);
    glColor3f(0.335, 0.21, 0.014);
    glVertex2i(-8, h - 30);
    glVertex2i(-8, h - 30 - 2);
    glVertex2i(8, h - 30 - 2);
    glVertex2i(8, h - 30);
    glEnd();

    // Fata

    // Ochi drept
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2i(0, h - 15);
    glVertex2i(0, h - 15 - 5);
    glVertex2i(5, h - 15 - 5);
    glVertex2i(5, h - 15);

    // Ochi stang
    glVertex2i(-13, h - 15);
    glVertex2i(-13, h - 15 - 5);
    glVertex2i(-8, h - 15 - 5);
    glVertex2i(-8, h - 15);
    glEnd();

    // Gura

    glBegin(GL_QUADS);
    glVertex2i(-10, h - 25);
    glVertex2i(-10, h - 25 - 2);
    glVertex2i(0, h - 25 - 2);
    glVertex2i(0, h - 25);
    glEnd();

    // Brate
    if (((int)location_people / 100) % 2)
    {
        glBegin(GL_QUADS);
        glColor3f(0.335, 0.21, 0.014);
        glVertex2i(-15, h - 32);
        glVertex2i(-15, h - 32 - 17);
        glVertex2i(-10, h - 32 - 17);
        glVertex2i(-10, h - 32);

        glVertex2i(10, h - 32);
        glVertex2i(10, h - 32 - 17);
        glVertex2i(15, h - 32 - 17);
        glVertex2i(15, h - 32);
        glEnd();
    }
    else
    {
        glBegin(GL_QUADS);
        glColor3f(0.335, 0.21, 0.014);
        glVertex2i(-15, h - 32);
        glVertex2i(-15, h - 32 - 20);
        glVertex2i(-10, h - 32 - 20);
        glVertex2i(-10, h - 32);

        glVertex2i(10, h - 32);
        glVertex2i(10, h - 32 - 20);
        glVertex2i(15, h - 32 - 20);
        glVertex2i(15, h - 32);
        glEnd();
    }

    // Corp
    glBegin(GL_POLYGON);
    glColor3f(0.13, 0.13, 0.13);
    glVertex2i(-10, h - 32);
    glVertex2i(-10, l + 3);
    glVertex2i(10, l + 3);
    glVertex2i(10, h - 32);
    glEnd();

    // Picioare

    glBegin(GL_QUADS);
    glColor3f(0.335, 0.21, 0.014);
    if (((int)location_people / 100) % 2)
    {
        glVertex2i(15, l + 3);
        glVertex2i(15, l - 2);
        glVertex2i(5, l - 2);
        glVertex2i(5, l + 3);

        glVertex2i(-10, l + 3);
        glVertex2i(-17, l - 4);
        glVertex2i(-10, l - 4);
        glVertex2i(-3, l + 3);
    }
    else
    {
        glVertex2i(-10, l + 3);
        glVertex2i(-10, l - 4);
        glVertex2i(-5, l - 4);
        glVertex2i(-5, l + 3);

        glVertex2i(5, l + 3);
        glVertex2i(5, l - 4);
        glVertex2i(10, l - 4);
        glVertex2i(10, l + 3);
    }

    glEnd();
    glutPostRedisplay();
}

void drawScene(void)
{
    if (!isGamePaused)
    {
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
            RenderString(250.0f, 425.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)"Ai pierdut!", 1);
        else if (isGameWon)
            RenderString(200.0f, 425.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)"Felicitari! Ai invins!", 1);
        else
            RenderString(150.0f, 425.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)"Nu lasa infractorii sa treaca!", 1);

        RenderString(500.0f, 425.0f, GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char *)("Scor: " + to_string(score / 100)).c_str(), 1);

        // Delimitare sosea
        glLineWidth(3);
        glColor3f(1, 1, 1);

        // Delimitam soseaua de iarba partea de jos
        glBegin(GL_LINES);
        glVertex2i(-100, -80);
        glVertex2i(1500, -80);

        // Delimitam soseaua de iarba partea de sus
        glVertex2i(-100, 400);
        glVertex2i(1500, 400);

        // Linie discontinua 1
        for (int i = -125; i < 700; i += 100)
        {
            glVertex2i(i, 240);
            glVertex2i(i + 50, 240);
        }
        // Linie discontinua 2
        for (int i = -125; i < 700; i += 100)
        {
            glVertex2i(i, 80);
            glVertex2i(i + 50, 80);
        }
        glEnd();

        glLineWidth(2.0);
        glBegin(GL_LINES);
        glVertex2i(0, -125);
        glVertex2i(600, -125);
        glEnd();

        // desenam infractorii
        glPushMatrix();
        glTranslated(location_people - 30, height_people, 0.0);
        drawInfractor();
        glPopMatrix();

        // desenam politistul
        glPushMatrix();
        glTranslated(0.0, height_police, 0.0);
        drawPoliceMan();
        glPopMatrix();

        if (isGameOver)
            RenderString(250.0f, 200.0f, GLUT_BITMAP_8_BY_13, (const unsigned char *)"GAME OVER");
        else if (isGameWon)
            RenderString(250.0f, 200.0f, GLUT_BITMAP_8_BY_13, (const unsigned char *)"YOU WIN!");

        // Movement politist
        if (directie == SUS)
            height_police = min(height_police + police_speed, current_lane * 160.0);
        else if (directie == JOS)
            height_police = max(height_police - police_speed, current_lane * 160.0);

        // progresul politistului
        glPushMatrix();
        glTranslated(progress, -120.0, 0.0);
        glColor3f(0.0, 0.0, 1.0);
        glRecti(0, 0, 40, 20);
        glPopMatrix();

        if (isGameWon)
        {
            reinforcement += 10;
            if (current_lane != 0)
            {
                glPushMatrix();
                glTranslated(min(reinforcement, 0.0), LANES[0], 0.0);
                drawPoliceMan();
                glPopMatrix();
            }
            if (current_lane != 1)
            {
                glPushMatrix();
                glTranslated(min(reinforcement, 0.0), LANES[1], 0.0);
                drawPoliceMan();
                glPopMatrix();
            }
            if (current_lane != 2)
            {
                glPushMatrix();
                glTranslated(min(reinforcement, 0.0), LANES[2], 0.0);
                drawPoliceMan();
                glPopMatrix();
            }
        }

        startgame(); // logica coliziuni + event handler
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
    if (isGameOver || isGamePaused || isGameWon)
        return;
    directie = SUS;
    current_lane = min(current_lane + 1, TOTAL_LANES - 1);

    glutPostRedisplay();
}

void miscajos(void)
{
    if (isGameOver || isGamePaused || isGameWon)
        return;

    directie = JOS;
    current_lane = max(current_lane - 1, 0);

    glutPostRedisplay();
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
        if (isGameWon || isGameOver)
            break;
        isGamePaused = !isGamePaused;
        break;
    case 'r':
        isGameOver = false;
        isGamePaused = false;
        isGameRunning = true;
        isGameWon = false;

        height_police = 0.0;
        directie = LOC;
        current_lane = 0;
        height_people = LANES[rand() % 3];
        location_people = 1200.0;
        people_speed = 10.15;
        score = 0;
        prag_puncte = 1000;
        progress = 0.0;
        reinforcement = -100.0;

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
    glutCreateWindow("Prinde infractorii - mini game");
    init();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard_callback);
    glutSpecialFunc(keyboard);

    glutMainLoop();
    return 0;
}