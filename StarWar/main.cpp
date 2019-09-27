#include<stdio.h>
#include<windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

static float	tx	=  0.0;
static float    MoveEnemy = 0.0;
static int      direction = 1;
long long int   score=0;
long long int   HighScore=0;
int             health=100;
int             bullet_on = 0;

void display();
void init();
void ship();
void writeText(char *a, float x, float y, void *font);
void writeIntText(int a, float x, float y, void *font);
void DrawBox(int x1, int x2, int y1, int y2);
void renderbitmap(float x, float y, void *font, char *Xtring);
void my_keyboard(int key, int x, int y);
void my_reshape(int w, int h);
void timer(int);
void drawCircle(float rx, float ry, int half, float xp, float yp);
void enemy(int x, int y);
void bullet(int x, int y);


int main()
{
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (1000, 600);
	glutCreateWindow ("Star Wars");
	init();
	glutDisplayFunc(display);
	glutSpecialFunc(my_keyboard);
	glutReshapeFunc(my_reshape);
	glutTimerFunc(0, timer,0);
	glutMainLoop();
	return 0;
}

void init()
{
	glClearColor (0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	gluOrtho2D(-30, 30, -40, 40);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    glColor3f(.85,.85,.85);
    DrawBox(18,30,40,-40);
    writeText("STAR WARS",20,30,GLUT_BITMAP_TIMES_ROMAN_24);
    writeText("Health: ",20,25,GLUT_BITMAP_9_BY_15);
    writeIntText(health,25,25,GLUT_BITMAP_9_BY_15);
    writeText("SCORE         : ",20,0,GLUT_BITMAP_HELVETICA_12);
    writeIntText(score, 26, 0, GLUT_BITMAP_HELVETICA_12);
    writeText("HIGH SCORE : ",20,-5,GLUT_BITMAP_HELVETICA_12);
    writeIntText(HighScore,26,-5,GLUT_BITMAP_HELVETICA_12);

    //bullet

    glPushMatrix();
    if(bullet_on)
    {
        static int tb=tx;
        bullet(tb-.5,-33);
        if(bullet_on==0) tb=tx;
    }
    glPopMatrix();

    //ship
    glPushMatrix();
    glTranslatef(tx,0,0);
    ship();
    glPopMatrix();
    //enemy
    glPushMatrix();
    enemy(0,0);
    glPopMatrix();
/*
	glPushMatrix();
    enemy(6,-6);
    glPopMatrix();
*/
	glFlush();
}

void ship(){
    //triangle
    glBegin(GL_POLYGON);
    glColor3f(0,255,255);
    glVertex2f(0,-25);
    glVertex2f(-1,-30);
    glVertex2f(1,-30);
    glEnd();
    //right side triangle
    glBegin(GL_POLYGON);
    glColor3f(255,255,0);
    glVertex2f(1.5,-40);
    glVertex2f(.5,-35);
    glVertex2f(.7,-32);
    glEnd();
    //left side triangle
    glBegin(GL_POLYGON);
    glColor3f(255,255,0);
    glVertex2f(-1.5,-40);
    glVertex2f(-.5,-35);
    glVertex2f(-.7,-32);
    glEnd();
    //rectangular
    glBegin(GL_POLYGON);
    glColor3f(255,0,0);
    glVertex2f(-1,-30);
    glVertex2f(1,-30);
    glVertex2f(-.5,-35);
    glVertex2f(.5,-35);
    glVertex2f(1,-30);
    glEnd();
    //bullet(0,-33);
}

void my_keyboard(int key, int x, int y)
{
	if(key==GLUT_KEY_LEFT){
        if(tx >= -28)
            tx -= .5;
        score+=3;
	}
	if(key==GLUT_KEY_RIGHT){
        if(tx <= 16)
            tx += .5;
        score+=3;
	}
	if(key==GLUT_KEY_UP){
        bullet_on=1;
    }
}

void renderbitmap(float x, float y, void *font, char *Xtring){
    char *c;
    glRasterPos2f(x,y);
    for(c=Xtring; *c!='\0'; c++)    glutBitmapCharacter(font,*c);
}

void writeText(char *a, float x, float y, void *font){
    glColor3f(0,0,0);
    char buf[100] = {0};
    sprintf(buf,a);
    renderbitmap(x,y,font,buf);
}

void writeIntText(int a, float x, float y, void *font){
    glColor3f(0,0,0);
    char buf[100] = {0};
    sprintf(buf, "%d" ,a);
    renderbitmap(x,y,font,buf);
}

void DrawBox(int x1, int x2, int y1, int y2){
    glBegin(GL_POLYGON);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glVertex2f(x1, y1);
    glEnd();
}

void my_reshape(int w, int h)
{
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(-30, 30, -40, 40);
    glMatrixMode(GL_MODELVIEW);

}

void timer(int){
    glutPostRedisplay();
    glutTimerFunc(100, timer, 0);
}

void drawCircle(float rx, float ry, int half, float xp, float yp){
    glPushMatrix();
    //glPointSize(3);
    glBegin(GL_POLYGON);
    float i;
    float len = 360;
    if(half == 1)   len = 180;
    for(i=0; i<len; i++){
        float theta = (i*3.1416)/180.0;
        glVertex2f(rx*cos(theta)  + xp, ry*sin(theta) + yp);
    }
    glEnd();
    glPopMatrix();
}

void enemy(int x, int y){
    glPushMatrix();
    glTranslatef(MoveEnemy,0,0);
    if(direction)MoveEnemy+=1;
    else MoveEnemy-=1;
    if(MoveEnemy>39) direction=0;
    if(MoveEnemy<-4) direction=1;

    //upper circle part
    drawCircle(1.25f,2.5f,1,-23.5+x,35+y);
    glColor3f(1,1,1);
    drawCircle(.25f,.75f,2,-24+x,36+y);
    drawCircle(.25f,.75f,2,-23+x,36+y);

    //rectangle
    glColor3f(1,1,1);
    DrawBox(-25+x,-22+x,35+y,33+y);

    glPopMatrix();
	glFlush();
}

void bullet(int x, int y){
    static float ty	= 0.0;
    glPushMatrix();
    if(bullet_on) glTranslatef(0,ty,0);
    glBegin(GL_POLYGON);
    glColor3f(1,1,0);
    DrawBox(x,x+1,y,4+y);
    glEnd();
    ty+=3;
    if(ty>80){
        ty=0.0;
        bullet_on=0;
    }
    glPopMatrix();
}

