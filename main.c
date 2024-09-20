#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

void display();

void init();

void reshape(int, int);

void displayMap(int map[5][5]);

void displayGrid();

void displayPlayer();

void keyPressed(unsigned char, int, int);

void changePVision(unsigned char key);

void movePlayer(unsigned char key);

void timer(int);

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;
const double PLAYER_SPEED = 3;

int map[5][5] = {
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1}
};
int heightMap = 5;
int widthMap = 5;
int unite = 100;

struct Coordonnee {
    double x;
    double y;
};

struct View {
    int x;
    int y;
};

struct Vecteur {
    int x;
    int y;
};

struct Coordonnee joueur = {150, 150};

struct View lVision = {150, 0};

struct Vecteur vecteurDirectionnel = {0, 0};

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutInitWindowPosition(2300, 100);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    glutCreateWindow("test");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyPressed);
    glutIdleFunc(display);
    glutTimerFunc(0, timer, 0);

    init();
    glutMainLoop();
}

void displayMap(int map[5][5]) {
    for (int i = 0; i < heightMap; i++) {
        for (int y = 0; y < widthMap; y++) {
            if (map[i][y] == 1) {
                glColor3f(1, 0, 0);
            } else {
                glColor3f(0, 0, 1);
            }
            glBegin(GL_QUADS);
            glVertex2d(y * unite, i * unite);
            glVertex2d(y * unite + unite, i * unite);
            glVertex2d(y * unite + unite, i * unite + unite);
            glVertex2d(y * unite, i * unite + unite);
            glEnd();
        }
    }
}

void displayGrid() {
    glColor3b(0, 0, 0);
    glBegin(GL_LINES);
    for (int i = 1; i < widthMap; i++) {
        glVertex2d(i * unite, 0);
        glVertex2d(i * unite, heightMap * unite);
    }
    for (int i = 1; i < heightMap; i++) {
        glVertex2d(0, i * unite);
        glVertex2d(widthMap * unite, i * unite);
    }
    glEnd();
}

void displayPlayer() {
    glPointSize(10);
    glColor3f(1, 1, 0);
    glBegin(GL_POINTS);
    glVertex2d(joueur.x, joueur.y);
    glEnd();
}

void displayVision() {
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2d(joueur.x, joueur.y);
    glVertex2d(lVision.x, lVision.y);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    displayMap(map);
    displayGrid();
    displayPlayer();
    displayVision();
    glutSwapBuffers();
    glFlush();
}

void init() {
    glClearColor((float) 0.0, (float) 0.0, (float) 0.0, (float) 1.0);
}

void reshape(int w, int h) {
    glViewport(0, 0, 500, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0, 500, 500, 0);
    glMatrixMode(GL_MODELVIEW);
}

void keyPressed(const unsigned char key, int x, int y) {
    switch (key) {
        case 'z':
        case 's':
            movePlayer(key);
        case 'q':
        case 'd':
            changePVision(key);
            break;
        default:



    }
    glutPostRedisplay();
}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}

//TODO
void movePlayer(unsigned char key) {
    vecteurDirectionnel.x = lVision.x - joueur.x;
    vecteurDirectionnel.y = lVision.y - joueur.y;
    const int alpha = atan2(vecteurDirectionnel.y, vecteurDirectionnel.x);
    if (key == 'z') {
        joueur.x = joueur.x + cos(alpha) * PLAYER_SPEED;
        joueur.y = joueur.y + sin(alpha) * PLAYER_SPEED;
    } else {
    }
}


void changePVision(const unsigned char key) {
    if (key == 'd') {
        if (lVision.y <= 0 && lVision.x < 500)
            lVision.x += 20;
        else if (lVision.y >= 500 && lVision.x > 0)
            lVision.x -= 20;
        else if (lVision.x >= 500 && lVision.y < 500)
            lVision.y += 20;
        else if (lVision.x <= 0 && lVision.y > 0)
            lVision.y -= 20;
    } else {
        if (lVision.y == 0 && lVision.x > 0)
            lVision.x -= 20;
        else if (lVision.y == 500 && lVision.x < 500)
            lVision.x += 20;
        else if (lVision.x == 500 && lVision.y > 0)
            lVision.y -= 20;
        else if (lVision.x == 0 && lVision.y < 500)
            lVision.y += 20;
    }
    if (lVision.x >= 500) {
        lVision.x = 500;
    }
    if (lVision.x <= 0) {
        lVision.x = 0;
    }
    if (lVision.y >= 500) {
        lVision.y = 500;
    }
    if (lVision.y <= 0) {
        lVision.y = 0;
    }
}

//TODO for 3D
//gluLookAt(joueur.x,joueur.y,0,lVision.x,lVision.y,0,0,0,0);
