#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdbool.h>

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

void keyPressed(const unsigned char key);

void changePVision(unsigned char key, struct View *v);

void movePlayer(unsigned char key);

void timer(int);

void calculCosSin(struct View *v, const struct Coordonnee *c);

void playerInMap(int x, int y, struct PlayerMapPosition *p);

double firstGridY(struct Coordonnee *c);

void findGridX();

void findGridY();

double firstGridX(struct Coordonnee *p);

double nearestGrid(double p);

struct PlayerMapPosition playerToGrid(int x, int y);

bool playerColision(int x, int y);

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

int main(int argc, char *argv[]) {
    calculCosSin(&centralVision, &player);
    playerInMap(player.x, player.y, &posMap); // NOLINT(*-narrowing-conversions)
    playerToGrid(player.x, player.y); // NOLINT(*-narrowing-conversions)
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

void keyPressed(const unsigned char key) {
    switch (key) {
        case 'z':
        case 's':
            movePlayer(key);
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
    playerInMap(player.x, player.y, &posMap); // NOLINT(*-narrowing-conversions)
    glutPostRedisplay();
}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}

void movePlayer(const unsigned char key) {
    double newX = player.x;
    double newY = player.y;
    if (key == 'z') {
        newX += centralVision.cosA * PLAYER_SPEED;
        newY += centralVision.sinA * PLAYER_SPEED;
    } else if (key == 's'){
        newX -= centralVision.cosA * PLAYER_SPEED;
        newY -= centralVision.sinA * PLAYER_SPEED;
    }
    if(!playerColision(newX,newY)) { // NOLINT(*-narrowing-conversions)
        player.x = newX;
        player.y = newY;
    }
    playerToGrid((int)player.x, (int)player.y);
    firstGridX(&player);
}

bool playerColision(const int x, const int y) {
    struct PlayerMapPosition pGrid = playerToGrid(x, y);
    if (pGrid.x < 0 || pGrid.x >= heightMap || pGrid.y < 0 || pGrid.y >= widthMap) {
        return false;
    }
    return map[pGrid.x][pGrid.y] == 1;
}

void changePVision(const unsigned char key, struct View *v) {
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

void calculCosSin(struct View *v, const struct Coordonnee *c) {
    const int vectorX = v->x - c->x; // NOLINT(*-narrowing-conversions)
    const int vectorY = v->y - c->y; // NOLINT(*-narrowing-conversions)
    float hypoLenght = sqrt(vectorX * vectorX + vectorY * vectorY); // NOLINT(*-narrowing-conversions)
    if (hypoLenght == 0)
        hypoLenght = 1;
    v->cosA = vectorX / hypoLenght; // NOLINT(*-narrowing-conversions)
    v->sinA = vectorY / hypoLenght; // NOLINT(*-narrowing-conversions)
}

void playerInMap(int x, int y, struct PlayerMapPosition *p) {
    p->x = (x - (x % 100) + 1) / 100;
    p->y = (y - (y % 100) + 1) / 100;
}

struct PlayerMapPosition playerToGrid(int x, int y) {
    struct PlayerMapPosition pos;
    pos.x = (int)(y / cellSize);
    pos.y = (int)(x / cellSize);
    return pos;
}

//TODO
double firstGridX(struct Coordonnee *p) {
    nearestGrid(p->x);
    printf("%f\n", nearestGrid(p->x)/tan(centralVision.cosA));
    return false;
}


//TODO
double nearestGrid(double p) {
    int pInt = (int)p;
    int nearestMultiple;
    if (pInt % 100 < 50) {
        nearestMultiple = pInt - (pInt % 100);
    } else {
        nearestMultiple = pInt + (100 - (pInt % 100));
    }
    return p - nearestMultiple;
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
