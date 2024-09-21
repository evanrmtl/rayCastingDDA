#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

struct Coordonnee {
    double x;
    double y;
};

struct View {
    int x;
    int y;
    double cosA;
    double sinA;
};

struct Vecteur {
    int x;
    int y;
};

struct PlayerMapPosition {
    int x;
    int y;
};

void display();

void init();

void reshape(int, int);

void displayMap(int map[5][5]);

void displayGrid();

void displayPlayer();

void keyPressed(unsigned char, int, int);

void changePVision(unsigned char key, struct View *v);

void movePlayer(unsigned char key);

void timer(int);

void calculCosSin(struct View *v, struct Coordonnee *c);

double calculAlpha(struct View *v);

void playerInMap(int x, int y, struct PlayerMapPosition *p);

double firstGridX(struct Coordonnee *c);

double firstGridY(struct Coordonnee *c);

void findGridX();

void findGridY();

void playerToGrid();

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;
const double PLAYER_SPEED = 3;

int heightMap = 5;
int widthMap = 5;
int cellSize = 100;

int map[5][5] = {
    {1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1}
};

struct Coordonnee player = {150, 150};

struct Coordonnee gridPos = {0, 0};

struct PlayerMapPosition posMap = {1, 1};

struct View centralVision = {150, 0, 90, 0};

struct View lVision = {100, 0, 90, 0};

struct View rVision = {200, 0, 90, 0};

struct Vecteur vecteurDirectionnel = {0, 0};

int main(int argc, char *argv[]) {
    calculCosSin(&centralVision, &player);
    playerToGrid();
    playerInMap(player.x, player.y, &posMap);
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
            glVertex2d(y * cellSize, i * cellSize);
            glVertex2d(y * cellSize + cellSize, i * cellSize);
            glVertex2d(y * cellSize + cellSize, i * cellSize + cellSize);
            glVertex2d(y * cellSize, i * cellSize + cellSize);
            glEnd();
        }
    }
}

void displayGrid() {
    glColor3b(0, 0, 0);
    glBegin(GL_LINES);
    for (int i = 1; i < widthMap; i++) {
        glVertex2d(i * cellSize, 0);
        glVertex2d(i * cellSize, heightMap * cellSize);
    }
    for (int i = 1; i < heightMap; i++) {
        glVertex2d(0, i * cellSize);
        glVertex2d(widthMap * cellSize, i * cellSize);
    }
    glEnd();
}

void displayPlayer() {
    glPointSize(10);
    glColor3f(1, 1, 0);
    glBegin(GL_POINTS);
    glVertex2d(player.x, player.y);
    glEnd();
}

void displayVision() {
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2d(player.x, player.y);
    glVertex2d(centralVision.x, centralVision.y);
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
            firstGridX(&player);
            break;
        case 'q':
        case 'd':
            changePVision(key, &centralVision);
            changePVision(key, &rVision);
            changePVision(key, &lVision);
            firstGridX(&player);
            break;
        default:



    }
    playerInMap(player.x, player.y, &posMap);
    glutPostRedisplay();
}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}

//TODO
void movePlayer(unsigned char key) {
    if (key == 'z') {
        player.x += centralVision.cosA * PLAYER_SPEED;
        player.y += centralVision.sinA * PLAYER_SPEED;
    } else {
        player.x -= centralVision.cosA * PLAYER_SPEED;
        player.y -= centralVision.sinA * PLAYER_SPEED;
    }
    playerToGrid();
}

void changePVision(unsigned char key, struct View *v) {
    if (key == 'd') {
        if (v->y <= 0 && v->x < 500)
            v->x += 20;
        else if (v->y >= 500 && v->x > 0)
            v->x -= 20;
        else if (v->x >= 500 && v->y < 500)
            v->y += 20;
        else if (v->x <= 0 && v->y > 0)
            v->y -= 20;
    } else {
        if (v->y == 0 && v->x > 0)
            v->x -= 20;
        else if (v->y == 500 && v->x < 500)
            v->x += 20;
        else if (v->x == 500 && v->y > 0)
            v->y -= 20;
        else if (v->x == 0 && v->y < 500)
            v->y += 20;
    }
    if (v->x >= 500) {
        v->x = 500;
    }
    if (v->x <= 0) {
        v->x = 0;
    }
    if (v->y >= 500) {
        v->y = 500;
    }
    if (v->y <= 0) {
        v->y = 0;
    }
    calculCosSin(&centralVision, &player);
}

void calculCosSin(struct View *v, struct Coordonnee *c) {
    int vectorX = v->x - c->x;
    int vectorY = v->y - c->y;
    float hypoLenght = sqrt(vectorX * vectorX + vectorY * vectorY);
    if (hypoLenght == 0)
        hypoLenght = 1;
    v->cosA = vectorX / hypoLenght;
    v->sinA = vectorY / hypoLenght;
}

double calculAlpha(struct View *v) {
    return atan2(v->y, v->x);
}

void playerInMap(int x, int y, struct PlayerMapPosition *p) {
    p->x = (x - (x % 100) + 1) / 100;
    p->y = (y - (y % 100) + 1) / 100;
}

void playerToGrid() {
    gridPos.x = player.x - (posMap.x * cellSize);
    gridPos.y = player.y - (posMap.y * cellSize);
}

//TODO
double firstGridX(struct Coordonnee *p) {
    double yNext;
    calculCosSin(&centralVision, &player);
    double alpha = atan2(centralVision.sinA,centralVision.cosA);
    printf("first alpha X = %f\n", alpha);
    if (centralVision.cosA > 0)
        yNext = (((p->x / cellSize) * cellSize + (cellSize) - p->x) * tan(alpha)) + p->y;
    else
        yNext = ((p->x / cellSize) * cellSize - (cellSize) - p->x * tan(alpha)) + p->y;
    // printf("first Grid X = %f\n", yNext);
    return yNext;
}

//TODO
double firstGridY(struct Coordonnee *c) {
    return NAN;
}

//TODO
void findGridX() {
}

//TODO
void findGridY() {
}

//TODO for 3D
//gluLookAt(player.x,player.y,0,centralVision.x,centralVision.y,0,0,0,0);
