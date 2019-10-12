#include <stdio.h>
#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <MMsystem.h>
#include <bits/stdc++.h>

static float	tx	=  0.0;
static int      direction = 1;
long long int   score=0;
long long int   HighScore=0;
int             health=100;
int             choice=0;
int             all_bullets[10];
float           bullet_speed[10];
int             v_enemy[5]={0};
int             h_enemy[5]={0};
int             nxt_venemy=0;
int             nxt_henemy=0;
int             xmove_en[5]={0};
int             ymove_en[5]={0};
int             xpos_en = rand() % 35;
int             ypos_en = rand() % 5;
//int             ypos_en = 4;
int             bul_x1[10];
int             bul_x2[10];
int             bul_y[10];
int             ven_x1[5]={0};
int             ven_x2[5];
int             ven_y[5];
int             ven_pos[] = {0,0};
double          hen_y[5]={0.0};
int             hen_pos[]={0,0};
bool            v_crash = false;
bool            h_crash = false;


void display();
void OptionManue();
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
void delay(unsigned int mseconds);

using namespace std;

int main()
{
    OptionManue();
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (1000, 600);
    glutCreateWindow ("Star Wars");
    init();
    for(int i=0; i<10 ; i++)
        all_bullets[i]=-100;
    for(int i=0; i<10 ; i++)
        bul_x2[i]=3;
    for(int i=0; i<10 ; i++)
        bul_y[i]=12;
    for(int i=0; i<5 ; i++)
        ven_x2[i]=48;
    glutDisplayFunc(display);
    glutSpecialFunc(arrow_keyboard);
    glutKeyboardFunc(my_keyboard);
    glutReshapeFunc(my_reshape);
    glutTimerFunc(0, timer,0);
    if(choice == 1)
    {
        glutMainLoop();
    }
    else if(choice == 2)
        return 0;
}


void OptionManue()
{
    static int flag = 1;
    if(flag)
    {
        cout<<"\t:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n";
        cout<<"\t:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n";
        cout<<"\t::'######::'########::::'###::::'########:::::'##:::::'##::::'###::::'########:::'######:::\n";
        cout<<"\t:'##... ##:... ##..::::'## ##::: ##.... ##:::: ##:'##: ##:::'## ##::: ##.... ##:'##... ##::\n";
        cout<<"\t: ##:::..::::: ##:::::'##:. ##:: ##:::: ##:::: ##: ##: ##::'##:. ##:: ##:::: ##: ##:::..:::\n";
        cout<<"\t:. ######::::: ##::::'##:::. ##: ########::::: ##: ##: ##:'##:::. ##: ########::. ######:::\n";
        cout<<"\t::..... ##:::: ##:::: #########: ##.. ##:::::: ##: ##: ##: #########: ##.. ##::::..... ##::\n";
        cout<<"\t:'##::: ##:::: ##:::: ##.... ##: ##::. ##::::: ##: ##: ##: ##.... ##: ##::. ##::'##::: ##::\n";
        cout<<"\t:. ######::::: ##:::: ##:::: ##: ##:::. ##::::. ###. ###:: ##:::: ##: ##:::. ##:. ######:::\n";
        cout<<"\t::......::::::..:::::..:::::..::..:::::..::::::...::...:::..:::::..::..:::::..:::......::::\n";
        cout<<"\t:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n";
        cout<<"\n\n\n\t1. Play Game\n";
        cout<<"\t2. Exit";
        cout<<"\n\n\tEnter your choice >> ";
        flag = 0;
    }
    cin>>choice;
    if(choice == 1 || choice == 2)
    {
        return ;
    }
    else
    {
        cout<<"\n\t\tInvalid Choice!";
        cout<<"\n\n\tEnter your choice >> ";
        OptionManue();
    }
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
    static int bgsong= 0;
    if(bgsong==0)
        PlaySound("bgmusic.wav", NULL, SND_FILENAME| SND_ASYNC);
    bgsong ++ ;
//    printf("%d\n",bgsong);
    if(bgsong == 985)
        bgsong = 0;
    glShadeModel(GL_SMOOTH);

    glPushMatrix();
    enemies();
    glPopMatrix();

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
        score+=1;
    }
    if(key=='d' || key=='D')
    {
        if(tx <= 16)
            tx += .5;
        score+=1;
    }
    if(key==' ')
    {
        static int bullet_no = 0;
        bullet_no++;
        if(bullet_no>=10)
            bullet_no=0;
        all_bullets[bullet_no] = tx;
        bul_x1[bullet_no] = 28+tx;
        bul_x2[bullet_no] = tx + 3;
        bullet_speed[bullet_no] = 0.0;
        bul_y[bullet_no]=12;
    }
}

void arrow_keyboard(int key, int x, int y)
{
    if(key==GLUT_KEY_LEFT)
    {
        if(tx >= -28)
        {
            tx -= .5;
        }
        score+=1;
    }
    if(key==GLUT_KEY_RIGHT)
    {
        if(tx <= 16)
            tx += .5;
        score+=1;
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

void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
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
    glColor3f(1,0,0);
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
    v_enemy[nxt_venemy] = 1;
    h_enemy[nxt_henemy] = 1;
    for(int i=0; i<5; i++)
    {

        if(v_enemy[i])
        {
            glPushMatrix();
            ven_y[i]= ypos_en;
            if(nxt_venemy % 2 == 0)
            {
                direction = 1;
                glTranslatef(xmove_en[i],0,0);
                ven_x1[i]++;
                ven_pos[0]=ven_x1[i];
                ven_pos[1]=ypos_en;
            }
            else
            {
                direction = 0;
                glTranslatef(-xmove_en[i],0,0);
                ven_x2[i]--;
                ven_pos[0]=ven_x2[i];
                ven_pos[1]=ypos_en;
            }

            if(nxt_venemy % 2 == 0)
                DrawEnemy(-10,-ypos_en*6);
            else
                DrawEnemy(48,-ypos_en*6);
//            printf("En: %d X: %d Y: %d\n",i,ven_x2[i],ypos_en);
            xmove_en[i]+=1;
            if (v_crash == true)
            {
                v_enemy[i]=0;
                xmove_en[i] = 0;
                static int wait = 0;
                wait++;
                if(wait==10)
                {
                    nxt_venemy = rand() % 4 ;
                    ypos_en = rand() % 5;
                    ven_x1[i] = 0;
                    ven_x2[i] = 48;
                    v_crash = false;
                    wait = 0;
                }
            }
            else if(xmove_en[i]>50)
            {
                v_enemy[i]=0;
                xmove_en[i] = 0;
                nxt_venemy = rand() % 4 ;
                ypos_en = rand() % 5;
                ven_x1[i] = 0;
                ven_x2[i] = 48;
            }
            glPopMatrix();
        }

        if (h_enemy[i])
        {
            glPushMatrix();
            hen_y[i]=(ymove_en[i])/5;
//            printf("En: %d X: %d Y: %g\n",i,xpos_en,hen_y[i]);
            glTranslatef(0,-ymove_en[i],0);
            hen_pos[0]= xpos_en+4;
            hen_pos[1]=hen_y[i];
            ymove_en[i]++;
            if (h_crash == true)
            {
                h_enemy[i]=0;
                ymove_en[i] = 0 ;
                static int wait = 0;
                wait++;
                if(wait==10)
                {
                    nxt_henemy = rand() % 4 ;
                    xpos_en = rand() % 35;
                    hen_y[i]=0;
                    h_crash = false;
                    wait=0;
                }
            }
            else if(ymove_en[i]>80)
            {
                h_enemy[i]=0;
                ymove_en[i] = 0 ;
                nxt_henemy = rand() % 4 ;
                xpos_en = rand() % 35;
                hen_y[i]=0;
            }
            DrawEnemy(xpos_en,+10);
            glPopMatrix();
        }
    }
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
            bul_y[i]--;
            DrawBullet(all_bullets[i]-.5,-33);
            bullet_speed[i]+=6;
//            printf("BUL: %d X: %d Y: %d\n",i,bul_x1[i],bul_y[i]);

            ///Vertical Enemy-Bullet Collusion
            if((ven_pos[0]-5>=bul_x1[i] && ven_pos[0]-5<=bul_x1[i]+2) && ven_pos[1]==bul_y[i] && direction == 1) //direction 1
            {
                bullet_speed[i]=0;
                all_bullets[i] = -100;
                bul_x1[i]=tx+28;
                bul_x2[i]=3;
                bul_y[i]=12;
                score+=100;
                v_crash = true;
            }
            else if((ven_pos[0]+6>=bul_x1[i]+1 && ven_pos[0]+6<=bul_x1[i]+4) && ven_pos[1]==bul_y[i] && direction == 0) //direction 0
            {
                bullet_speed[i]=0;
                all_bullets[i] = -100;
                bul_x1[i]=tx+28;
                bul_x2[i]=3;
                bul_y[i]=12;
                score+=100;
                v_crash = true;
            }


            ///Horizontal Enemy-Bullet Collusion
            if(((hen_pos[0]>=bul_x1[i]-2 && hen_pos[0]<=bul_x1[i])||(hen_pos[0]>=bul_x1[i] && hen_pos[0]<=bul_x1[i]+2)) && hen_pos[1]==bul_y[i]+2)
            {
                bullet_speed[i]=0;
                all_bullets[i] = -100;
                bul_x1[i]=tx+28;
                bul_x2[i]=3;
                bul_y[i]=12;
                score+=100;
                h_crash = true;
            }

            if(bullet_speed[i]>80)
            {
                bullet_speed[i]=0.0;
                all_bullets[i] = -100;
                bul_x1[i]=tx+28;
                bul_x2[i]=3;
                bul_y[i]=12;
            }
            glPopMatrix();
        }
    }
}
