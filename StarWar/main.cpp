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
int             all_bullets[10];
float           bullet_speed[10];
int             v_enemy[5];
int             h_enemy[5];
int             ran_enemy;


void display();
void init();
void ship();
void writeText(char *a, float x, float y, void *font);
void writeIntText(int a, float x, float y, void *font);
void DrawBox(int x1, int x2, int y1, int y2);
void renderbitmap(float x, float y, void *font, char *Xtring);
void my_keyboard(unsigned char key, int x, int y);
void arrow_keyboard(int key, int x, int y);
void my_reshape(int w, int h);
void timer(int);
void drawCircle(float rx, float ry, int half, float xp, float yp);
void DrawEnemy(int x, int y);
void enemies();
void DrawBullet(int x, int y);
void bullets();


int main()
{
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (1000, 600);
    glutCreateWindow ("Star Wars");
    init();
    for(int i=0; i<10 ; i++) all_bullets[i]=-100;
    glutDisplayFunc(display);
    glutSpecialFunc(arrow_keyboard);
    glutKeyboardFunc(my_keyboard);
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
    bullets();
    glPopMatrix();

    //ship
    glPushMatrix();
    glTranslatef(tx,0,0);
    ship();
    glPopMatrix();
    //enemy
    glPushMatrix();
    enemies();
    glPopMatrix();
    /*
    	glPushMatrix();
        enemy(6,-6);
        glPopMatrix();
    */
    glFlush();
}

void ship()
{
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

void my_keyboard(unsigned char key, int x, int y)
{
	if(key=='a' || key=='A')
    {
        if(tx >= -28)
            tx -= .5;
        score+=3;
    }
    if(key=='d' || key=='D')
    {
        if(tx <= 16)
            tx += .5;
        score+=3;
    }
    if(key==' ')
    {
        static int bullet_no = 0;
        bullet_no++;
        if(bullet_no>=10) bullet_no=0;
        all_bullets[bullet_no] = tx;
        bullet_speed[bullet_no] = 0.0;
    }
}

void arrow_keyboard(int key, int x, int y)
{
    if(key==GLUT_KEY_LEFT)
    {
        if(tx >= -28)
            tx -= .5;
        score+=3;
    }
    if(key==GLUT_KEY_RIGHT)
    {
        if(tx <= 16)
            tx += .5;
        score+=3;
    }
}

void renderbitmap(float x, float y, void *font, char *Xtring)
{
    char *c;
    glRasterPos2f(x,y);
    for(c=Xtring; *c!='\0'; c++)
        glutBitmapCharacter(font,*c);
}

void writeText(char *a, float x, float y, void *font)
{
    glColor3f(0,0,0);
    char buf[100] = {0};
    sprintf(buf,a);
    renderbitmap(x,y,font,buf);
}

void writeIntText(int a, float x, float y, void *font)
{
    glColor3f(0,0,0);
    char buf[100] = {0};
    sprintf(buf, "%d",a);
    renderbitmap(x,y,font,buf);
}

void DrawBox(int x1, int x2, int y1, int y2)
{
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

void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(100, timer, 0);
}

void drawCircle(float rx, float ry, int half, float xp, float yp)
{
    glPushMatrix();
    //glPointSize(3);
    glBegin(GL_POLYGON);
    float i;
    float len = 360;
    if(half == 1)
        len = 180;
    for(i=0; i<len; i++)
    {
        float theta = (i*3.1416)/180.0;
        glVertex2f(rx*cos(theta)  + xp, ry*sin(theta) + yp);
    }
    glEnd();
    glPopMatrix();
}

void DrawEnemy(int x, int y)
{
    //glPushMatrix();
    //upper circle part
    drawCircle(1.25f,2.5f,1,-23.5+x,35+y);
    glColor3f(1,1,1);
    drawCircle(.25f,.75f,2,-24+x,36+y);
    drawCircle(.25f,.75f,2,-23+x,36+y);

    //rectangle
    glColor3f(1,1,1);
    DrawBox(-25+x,-22+x,35+y,33+y);

    //glPopMatrix();
   // glFlush();
}

void enemies ()
{
//    ran_enemy=rand()%2;
//    if(ran_enemy==0)
//    {
        //Horizontal
        static int move_x=0;
        glPushMatrix();
        glTranslatef(move_x,0,0);
        if(direction)
            move_x+=1;
        else
            move_x-=1;
        if(move_x>39)
            direction=0;
        if(move_x<-4)
            direction=1;
        DrawEnemy(0,0);
        glPopMatrix();
       // glFlush();

        //vertical
        static int move_y=0;
        glPushMatrix();
        glTranslatef(0,-move_y,0);
        move_y++;
        if(move_y>=80) move_y = 0;
        DrawEnemy(0,0);
        glPopMatrix();
        //glFlush();
//    }
//    if(ran_enemy==1)
//    {
//        glPushMatrix();
//        glTranslatef(MoveEnemy,0,0);
//        if(direction)
//            MoveEnemy+=1;
//        else
//            MoveEnemy-=1;
//        if(MoveEnemy>39)
//            direction=0;
//        if(MoveEnemy<-4)
//            direction=1;
//        DrawEnemy(1,1);
//        glPopMatrix();
//        glFlush();
//    }

}

void DrawBullet(int x, int y)
{
    glBegin(GL_POLYGON);
    glColor3f(1,1,0);
    DrawBox(x,x+1,y,4+y);
    glEnd();
}

void bullets()
{
    for(int i=0; i<10; i++)
    {
        if(all_bullets[i] != -100)
        {
            glPushMatrix();
            glTranslatef(0,bullet_speed[i],0);
            DrawBullet(all_bullets[i]-.5,-33);
            bullet_speed[i]+=6;
            ///if(all_bullets)
            if(bullet_speed[i]>80)
            {
                bullet_speed[i]=0.0;
                all_bullets[i] = -100;
            }
            glPopMatrix();
        }
    }
}

