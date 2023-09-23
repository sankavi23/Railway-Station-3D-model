#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <windows.h>
#include<math.h>
#include<mmsystem.h>

double Txval = 0, Tyval = 0, Tzval = 0;
GLUquadric* qobj;

double windowHeight = 2000, windowWidth = 900;

GLfloat alpha = 0.0, theta = 0.0, axis_x = 0.0, axis_y = 0.0;

GLboolean bRotate = false, uRotate = false, welRotate = true, night = false;

GLfloat fTheta = 0.0; //door and window rotation in building
GLfloat faTheta = 0.0; //fan rotation
GLfloat ftTheta = 0.0; //train movement

GLboolean fanFlag = false, spotLight = false, Light1 = false, Light2 = false, ambL = true, diffL = true, specL = true;
GLfloat dx, dy, dz, dxyz;
double roll_value = 0.2, pi = acos(-1), cs_angle = cos(pi / 180), sn_angle = sin(pi / 180);
unsigned int ID[1000];
int k = 0;
float p = 0;

int wel[7] = { 28,29,30,31,32,33,34 };
int sound = 0; //not in use

GLboolean ambL1 = false, ambL2 = false, ambL3 = false, diffL1 = false, diffL2 = false, diffL3 = false, specL1 = false, specL2 = false, specL3 = false;

GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 }, light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_ambient1[] = { 0.2, 0.2, 0.2, 1.0 }, light_diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };  //white
GLfloat light_ambient2[] = { 0.0, 0.0, 0.5, 1.0 }, light_diffuse2[] = { 0.0, 0.0, 1.0, 1.0 };  //blue
GLfloat light_ambient3[] = { 0.5, 0.5, 0.5, 1.0 }, light_diffuse3[] = { 1.0, 1.0, 1.0, 1.0 };  //white
GLfloat light_ambient4[] = { 0.5, 0.5, 0.5, 1.0 }, light_diffuse4[] = { 1.0, 1.0, 1.0, 1.0 };  //white
static int slices = 16;
static int stacks = 16;

static GLfloat v_pyramid[65][3] =
{
    {0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 1.0},
    {1.0, 1.0, 0.0},
    {1.0, 1.0, 1.0},
    {1.57, 5.0, 1.0},
    {1.57, -5.0, 1.0},
    {1.0, 0.0, 0.0},//7
    {1.0, 0.0, 1.0},
    {0.0, 0.0, 1.0},//9
    {0.0, 10.0, 0.0}, //10
    {0.0, 0.0, 14.0},  //11
    {0.0, 10.0, 14.0},//12
    {-0.3, 0.7, 0.0},
    {-0.3, 0.7, 1.0},
    {1.3, 0.7, 0},
    {1.3, 0.7, 1.0},//16
    {-1.73, 1.0, 1.0},
    {-1.73, 1.0, 0.0},//18
    {-1.73, 3.0, 1.0},
    {-1.73, 3.0, 0.0},//20
    {0.0, 4.0, 1.0},
    {0.0, 4.0, 0.0},//22
    {1.73, 3.0, 0.0},//23
    {1.73, 3.0, 1.0},//24
    {1.73, 1.0, 1.0},//25
    {1.73, 1.0, 0.0},//26
    {1.0, 2.0, 0.0},//27 
    {3.0, 2.0, 0.0},//28
    {5, 0.0, 0.0},//29
    {0.0, 0.0, 2.0},//30
    {1.0, 2.0, 2.0},//31
    {3.0, 2.0, 2.0},//32
    {5.0, 0.0, 2.0},//33
    {0.0, 0.0, 0.0},//34
    {0.0, 1.0, 0.0},//35
    {0.0, 1.0, 1.0},//36
    {0.0, 0.0, 1.0},//37
    {0.0, 0.0, 0.0},//38 
    {0.0, 1.0, 0.2},//39
    {0.0, 1.0, 2.2},//40
    {0.0, 0.0, 2.4},//41
    {2.0, 0.0, 0.0},//42 //
    {2.0, 1.0, 0.2},//43
    {2.0, 1.0, 2.2},//44
    {2.0, 0.0, 2.4},//45
    {-2.0, 0.0, 1.5},//46
    {-0.75, 0.0, 1.25},//47
    {-1.75, 0.0, 2.5},//48
    {-0.5, 0.0, 2},//49
    {0.0, 0.0, 4.0},//50
    { 0.5, 0.0, 2},//51
    {1.75, 0.0, 2.5},//52
    {0.75, 0.0, 1.25},//53
    {2.0, 0.0, 1.5},//54
    {-2.0, -0.25, 1.5},//55
    {-0.75, -0.25, 1.25},//56
    {-1.75, -0.25, 2.5},//57
    {-0.5, -0.25, 2},//58
    {0.0, -0.25, 4.0},//59
    { 0.5, -0.25, 2.0},//60
    {1.75, -0.25, 2.5},//61
    {0.75, -0.25, 1.25},//62
    {2.0, -0.25, 1.5},//63
    {0.0, -0.25, 0.0}//64
};

static GLubyte p_Indices[10][3] =
{
    {0, 1, 5},
    {1, 2, 5},
    {2,3, 5},
    {3,4, 5},
    {4,0, 5},
    {6, 1, 0},
    {6, 2, 1},
    {6,2, 3},
    {6,4, 3},
    {6,0, 4}

};
static GLubyte quadIndices[32][4] =
{
    {0, 7, 8, 9},
    {0, 9, 2, 1},
    {0, 1, 3, 7},
    {3, 7, 8, 4},
    {8, 4, 2, 9},//4
    {3, 7, 8, 4},
    {1, 2, 4, 3},
    {0, 11, 12, 10},
    {0, 7, 8, 9},//basket start 8
    {0, 7, 8, 9},
    {0, 7, 8, 9},
    {9, 14, 13, 0},
    {7, 15, 16, 8},
    {0, 13, 15, 7},
    {8, 16 ,14 ,9},
    {0, 9, 17, 18}, //hexa 15
    {18, 17, 19, 20},
    {20, 19, 21, 22},
    {22, 21, 24, 23},
    {23, 24, 25, 26},//19
    {26, 25, 9, 0},//20
    {0, 27, 28, 29},//21 chimni
    {30, 33, 32, 31},//22
    {28, 32, 33, 29},//23
    {27, 0, 30, 31},//24
    {27, 31, 32, 28},//25
    {38, 39, 40, 41},//26 rail uchu
    {38, 39, 43, 42},//27
    {42, 43, 44, 45},//28
    {39, 40, 44, 43},//29
    {41, 45, 44, 40},//30
    {38, 41, 45, 42}//31
};
static GLubyte leafqIndices[10][4] =
{
    {0, 64, 55, 46},
    {56, 47, 46, 55},
    {47, 56, 57, 48},
    {58, 49, 48, 57},
    {59, 50, 49, 58},
    {60, 51, 50, 59},
    {61, 52, 51, 60},
    {62, 53, 52, 61},
    {63, 54, 53, 62},
    {63, 64, 0, 54}
};
static GLubyte hexaIndices[2][6] =
{
    {0, 18, 20, 22, 23, 26},
    {9, 25, 24, 21, 19, 17}
};
static GLubyte hexa2Indices[2][10] =
{
    {0, 46, 47, 48, 49, 50, 51, 52, 53, 54},
    {64, 55, 56, 57, 58, 59, 60, 61, 62, 63}
};
static GLfloat colors[18][3] =
{
    {0.0, 0.0, 1.0},
    {0.5, 0.0, 1.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 1.0},
    {0.8, 0.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.5, 1.0, 1.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 1.0},
    {0.8, 0.0, 0.0},
    {1.0, 0.3, 0.2},
    {0.0, 0.3, 0.2},
    {0.2, 1.0, 0.2},
    {0.4, 0.4, 1.0},
    {0.8, 0.8, 1.0},
    {0.8, 0.4, 0.2},
    {0.6, 0.6, 0.2},
    {0.2, 0.4, 0.2}
};

void cubes(float r, float g, float b, int a, float p)
{
    if (a == 1) {
        glBegin(GL_QUADS);
        for (GLint i = 0; i < 7; i++)
        {
            GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
            GLfloat mat_ambient[] = { r * 0.6, g * 0.6,b * 0.6, 1.0 };
            GLfloat mat_diffuse[] = { r, g, b, 1.0 };
            GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
            GLfloat mat_shininess[] = { 30 };

            glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
            

            glVertex3fv(&v_pyramid[quadIndices[i][0]][0]); /*glTexCoord2f(p, p);*/
            glVertex3fv(&v_pyramid[quadIndices[i][1]][0]); /*glTexCoord2f(0, p);*/
            glVertex3fv(&v_pyramid[quadIndices[i][2]][0]); /*glTexCoord2f(0, 0);*/
            glVertex3fv(&v_pyramid[quadIndices[i][3]][0]); /*glTexCoord2f(p, 0);*/
            
        }
        glEnd();
    }
    else {

        glBegin(GL_QUADS);
        for (GLint i = 0; i < 7; i++)
        {
            GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
            GLfloat mat_ambient[] = { colors[a + i][0] * 0.5, colors[a + i][1] * 0.5,colors[a + i][2] * 0.5, 1.0 };
            GLfloat mat_diffuse[] = { colors[a + i][0],colors[a + i][1],colors[a + i][2], 1.0 };
            GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
            GLfloat mat_shininess[] = { 30 };

            glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

            glVertex3fv(&v_pyramid[quadIndices[i][0]][0]); /*glTexCoord2f(1, 1);*/
            glVertex3fv(&v_pyramid[quadIndices[i][1]][0]); /*glTexCoord2f(1, 0);*/
            glVertex3fv(&v_pyramid[quadIndices[i][2]][0]); /*glTexCoord2f(0, 0);*/
            glVertex3fv(&v_pyramid[quadIndices[i][3]][0]); /*glTexCoord2f(0, 1);*/
        }
        glEnd();
    }


}

void leaf(float r, float g, float b, float r2, float g2, float b2)
{
    glBegin(GL_POLYGON);
    for (GLint i = 0; i < 1; i++)
    {
        

        GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
        GLfloat mat_ambient[] = { r2 * 0.3, g2 * 0.3,b2 * 0.3, 1.0 };
        GLfloat mat_diffuse[] = { r2 , g2 ,b2 ,1.0 };
        GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat mat_shininess[] = { 60 };

        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

        glVertex3fv(&v_pyramid[hexa2Indices[i][0]][0]); /*glTexCoord2f(0.5, 1);*/
        glVertex3fv(&v_pyramid[hexa2Indices[i][1]][0]); /*glTexCoord2f(0, 0.625);*/
        glVertex3fv(&v_pyramid[hexa2Indices[i][2]][0]); /*glTexCoord2f(0.2, 0.7);*/
        glVertex3fv(&v_pyramid[hexa2Indices[i][3]][0]); /*glTexCoord2f(0.1, 0.3);*/
        glVertex3fv(&v_pyramid[hexa2Indices[i][4]][0]);/* glTexCoord2f(0.25, 0.4);*/
        glVertex3fv(&v_pyramid[hexa2Indices[i][5]][0]); /*glTexCoord2f(0.5, 0);*/
        glVertex3fv(&v_pyramid[hexa2Indices[i][6]][0]); /*glTexCoord2f(0.75, 0.3);*/
        glVertex3fv(&v_pyramid[hexa2Indices[i][7]][0]); /*glTexCoord2f(0.9, 0.3);*/
        glVertex3fv(&v_pyramid[hexa2Indices[i][8]][0]); /*glTexCoord2f(0.7, 0.6);*/
        glVertex3fv(&v_pyramid[hexa2Indices[i][9]][0]); /*glTexCoord2f(1, 0.625);;*/
    }
    glEnd();

    glBegin(GL_QUADS);
    for (GLint i = 0; i < 10; i++)
    {
       
        GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
        GLfloat mat_ambient[] = { r * 0.3, g * 0.3, b * 0.3, 1.0 };
        GLfloat mat_diffuse[] = { r , g ,  b , 1.0 }; 
        GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat mat_shininess[] = { 60 };

        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

        glVertex3fv(&v_pyramid[leafqIndices[i][0]][0]);
        glVertex3fv(&v_pyramid[leafqIndices[i][1]][0]);
        glVertex3fv(&v_pyramid[leafqIndices[i][2]][0]);
        glVertex3fv(&v_pyramid[leafqIndices[i][3]][0]);
    }
    glEnd();
}

//green color Bin
void Basket(float r, float g, float b, float p)
{
    glBegin(GL_QUADS);
    for (GLint i = 8; i < 15; i++)
    {
        GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
        GLfloat mat_ambient[] = { r * 0.5, g * 0.5,b * 0.5, 1.0 };
        GLfloat mat_diffuse[] = { r, g, b, 1.0 };
        GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat mat_shininess[] = { 60 };

        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
        glVertex3fv(&v_pyramid[quadIndices[i][0]][0]); glTexCoord2f(p, p);
        glVertex3fv(&v_pyramid[quadIndices[i][1]][0]); glTexCoord2f(0, p);
        glVertex3fv(&v_pyramid[quadIndices[i][2]][0]); glTexCoord2f(0, 0);
        glVertex3fv(&v_pyramid[quadIndices[i][3]][0]); glTexCoord2f(p, 0);
    }
    glEnd();
}

void floorr(int r, int g, int b)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { r * 0.2, g * 0.2, b * 0.3, 1.0 };
    GLfloat mat_diffuse[] = { r, g, b, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 60 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glBegin(GL_QUADS);
    for (GLint i = 0; i < 1; i++)
    {
        glVertex3fv(&v_pyramid[quadIndices[i][0]][0]);
        glVertex3fv(&v_pyramid[quadIndices[i][1]][0]);
        glVertex3fv(&v_pyramid[quadIndices[i][2]][0]);
        glVertex3fv(&v_pyramid[quadIndices[i][3]][0]);
    }
    glEnd();
}


void floorr2(int r, int g, int b)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { r * 0.2, g * 0.2, b * 0.3, 1.0 };
    GLfloat mat_diffuse[] = { r, g, b, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 60 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glBegin(GL_QUADS);

    for (GLint i = 0; i < 1; i++)
    {
        glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
        glVertex3fv(&v_pyramid[quadIndices[i][0]][0]);
        glVertex3fv(&v_pyramid[quadIndices[i][1]][0]);
        glVertex3fv(&v_pyramid[quadIndices[i][2]][0]);
        glVertex3fv(&v_pyramid[quadIndices[i][3]][0]);
    }
    glEnd();
}


void sphere(float r, float g, float b)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { r * 0.2, g * 0.2, b * 0.3, 1.0 };
    GLfloat mat_diffuse[] = { r, g, b, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 60 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glutSolidSphere(1, 50, 50);
}


void init() 
{
    qobj = gluNewQuadric();
    gluQuadricNormals(qobj, GLU_SMOOTH);
}

void cylinder(float r, float g, float b)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.2 * r, 0.2 * g, 0.3 * b, 1.0 };
    GLfloat mat_diffuse[] = { r, g, b, 1.0 };
    GLfloat mat_specular[] = { 0.7 ,0.7, 0.7, 1.0 };
    GLfloat mat_shininess[] = { 60 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    const double PI = 3.14159;

    
    double i, resolution = 0.1;
    double height = 1;
    double radius = 0.5;

    glPushMatrix();
    glTranslatef(0, -0.5, 0);

    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5, 0.5);
    glVertex3f(0, height, 0); 
    for (i = 2 * PI; i >= 0; i -= resolution)

    {
        GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
        GLfloat mat_ambient[] = { 0.2 * r, 0.2 * g, 0.3 * b, 1.0 };
        GLfloat mat_diffuse[] = { r, g, b, 1.0 };
        GLfloat mat_specular[] = { 0.7 ,0.7, 0.7, 1.0 };
        GLfloat mat_shininess[] = { 60 };

        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        glTexCoord2f(0.5f * cos(i) + 0.5f, 0.5f * sin(i) + 0.5f);
        glVertex3f(radius * cos(i), height, radius * sin(i));
    }
    /* close the loop back to 0 degrees */
    glTexCoord2f(0.5, 0.5);
    glVertex3f(radius, height, 0);
    glEnd();

    /* bottom triangle: note: for is in reverse order */
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2f(0.5, 0.5);
    glVertex3f(0, 0, 0);  /* center */
    for (i = 0; i <= 2 * PI; i += resolution)
    {
        GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
        GLfloat mat_ambient[] = { 0.2 * r, 0.2 * g, 0.3 * b, 1.0 };
        GLfloat mat_diffuse[] = { r, g, b, 1.0 };
        GLfloat mat_specular[] = { 0.7 ,0.7, 0.7, 1.0 };
        GLfloat mat_shininess[] = { 60 };

        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        glTexCoord2f(0.5f * cos(i) + 0.5f, 0.5f * sin(i) + 0.5f);
        glVertex3f(radius * cos(i), 0, radius * sin(i));
    }
    glEnd();

    /* middle tube */
    glBegin(GL_QUAD_STRIP);

    for (i = 0; i <= 2 * PI; i += resolution)
    {
        GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
        GLfloat mat_ambient[] = { 0.2 * r, 0.2 * g, 0.3 * b, 1.0 };
        GLfloat mat_diffuse[] = { r, g, b, 1.0 };
        GLfloat mat_specular[] = { 0.7 ,0.7, 0.7, 1.0 };
        GLfloat mat_shininess[] = { 60 };

        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
        const float tc = (i / (float)(2 * PI));
        glTexCoord2f(tc, 0.0);
        glVertex3f(radius * cos(i), 0, radius * sin(i));
        glTexCoord2f(tc, 1.0);
        glVertex3f(radius * cos(i), height, radius * sin(i));
    }
    /* close the loop back to zero degrees */
    glTexCoord2f(0.0, 0.0);
    glVertex3f(radius, 0, 0);
    glTexCoord2f(0.0, 10.0);
    glVertex3f(radius, height, 0);
    glEnd();

    glPopMatrix();
}


void cyclider(float r, float g, float b)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.2 * r, 0.2 * g, 0.3 * b, 1.0 };
    GLfloat mat_diffuse[] = { r, g, b, 1.0 };
    GLfloat mat_specular[] = { 0.7 ,0.7, 0.7, 1.0 };
    GLfloat mat_shininess[] = { 60 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    GLUquadricObj* quadratic;
    quadratic = gluNewQuadric();
    gluCylinder(quadratic, 0.01f, 0.01f, 0.3f, 32, 32);
    
}




void pillar(float r, float g, float b, int a, int p, int h)
{
    glPushMatrix();
    glScalef(1, h, 1);
    cubes(r, g, b, a, p);
    glPopMatrix();
    
}

//front path of building
void buildingPath()
{
    glPushMatrix();
    glTranslatef(10, 0, 0);
    glScalef(20, 3, 20);
    cubes(0.9, 0.9, 0.9, 1, 5);
    glPopMatrix();
}

void building(float r, float g, float b)
{
    glPushMatrix();
    glTranslatef(10, 3, 0);
    pillar(1, 1, 1, 1, 1, 10);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(20, 3, 0);
    pillar(1, 1, 1, 1, 1, 10);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(29, 3, 0);
    pillar(1, 1, 1, 1, 1, 10);
    glPopMatrix();

    glPushMatrix();  
    glTranslatef(11, 3, 0);
    glScalef(4, 10, 0.4);
    cubes(r, g, b, 1, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(16, 3, 0);
    glScalef(4, 10, 0.4);
    cubes(r, g, b, 1, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(15, 9, 0);
    glScalef(1, 4, 0.4);
    cubes(r, g, b, 1, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(15, 3, 0);
    glScalef(1, 2.5, 0.4);
    cubes(r, g, b, 1, 1);
    glPopMatrix();
    glPushMatrix();  //Back wall
    glTranslatef(21, 3, 0);
    glScalef(4, 10, 0.4);
    cubes(r, g, b, 1, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(26, 3, 0);
    glScalef(3, 10, 0.4);
    cubes(r, g, b, 1, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(25, 9, 0);
    glScalef(1, 4, 0.4);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(25, 3, 0);
    glScalef(1, 2.5, 0.4);
    cubes(r, g, b, 1, 1);
    glPopMatrix();
    glPushMatrix();// front pillar
    glTranslatef(10, 3, 16); 
    pillar(1, 1, 1, 1, 1, 10);
    glTranslatef(7, 0, 0);
    pillar(1, 1, 1, 1, 1, 10);
    glTranslatef(4, 0, 0);
    pillar(1, 1, 1, 1, 1, 10);
    glTranslatef(8, 0, 0);
    pillar(1, 1, 1, 1, 1, 10);
    glPopMatrix();
   
    glPushMatrix(); //front_left_door
    glTranslatef(18, 3, 16); 
    glRotatef(-fTheta, 0, 1, 0);
    glScalef(1.5, 10, 0.08);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();
    
    glPushMatrix(); 
    glTranslatef(19.5, 3, 16); 
 
    glTranslatef(1.5, 1.5, 0);
    glRotatef(fTheta, 0, 1, 0);
    glTranslatef(-1.5, -1.5, 0);
    glScalef(1.5, 10, 0.08);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();
   
    glPushMatrix();  //front wall
    glTranslatef(11, 3, 16);
    glScalef(2, 10, 0.3);
    cubes(r, g, b, 1, 1);
    glTranslatef(2, 0, 0);
    cubes(r, g, b, 1, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(13, 3, 16); 
    glScalef(2, 3, 0.3);
    cubes(0.7, 0, 0, 1, 1);
    glTranslatef(0, 2, 0);
    cubes(0.7, 0, 0, 1, 1);
    glPopMatrix();
    glPushMatrix(); 
    glTranslatef(13, 6, 16); 
    glScalef(2, 0.3, 0.4);
    cubes(0, 0, 0, 1, 1);
    glTranslatef(0, 10, 0);
    cubes(0, 0, 0, 1, 1);
    glPopMatrix();
   
    glPushMatrix(); 
    glTranslatef(13, 6.3, 16.2);
   
    glRotatef(fTheta, 0, 1, 0);
    glScalef(1, 2.6, 0.08);
    cubes(0, 0.1, 0.8, 10, 1);
    glPopMatrix();
    
    glPushMatrix(); 
    glTranslatef(14, 6.3, 16.2);
  
    glTranslatef(1, 1.5, 0);
    glRotatef(-fTheta, 0, 1, 0);
    glTranslatef(-1, -1.5, 0);
    glScalef(1, 2.6, 0.08);
    cubes(0, 0.1, 0.8, 10, 1);
    glPopMatrix();
    glPushMatrix(); 
    glTranslatef(22, 3, 16);
    glScalef(2.5, 10, 0.3);
    cubes(r, g, b, 1, 1);
    glTranslatef(2, 0, 0);
    cubes(r, g, b, 1, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(24.5, 3, 16);
    glScalef(2.5, 3, 0.3);
    cubes(0.7, 0, 0, 1, 1);
    glTranslatef(0, 2, 0);
    cubes(0.7, 0, 0, 1, 1);
    glPopMatrix();
   
    glPushMatrix(); 
    glTranslatef(24.5, 6.3, 16.2);
   
    glRotatef(fTheta, 0, 1, 0);
    glScalef(1.25, 2.6, 0.08);
    cubes(0, 0.1, 0.8, 10, 1);
    glPopMatrix();
    glPushMatrix(); 
    glTranslatef(25.75, 6.3, 16.2); 
  
    glTranslatef(1.25, 1.5, 0);
    glRotatef(-fTheta, 0, 1, 0);
    glTranslatef(-1.25, -1.5, 0);
    glScalef(1.25, 2.6, 0.08);
    cubes(0, 0.1, 0.8, 10, 1);
    glPopMatrix();
   
    glPushMatrix(); 
    glTranslatef(24.5, 6, 16); 
    glScalef(2.5, 0.3, 0.4);
    cubes(0, 0, 0, 1, 1);
    glTranslatef(0, 10, 0);
    cubes(0, 0, 0, 1, 1);
    glPopMatrix();

    glPushMatrix();  //left wall
    glTranslatef(10, 3, 1);
    glScalef(0.3, 10, 15);
    cubes(r, g, b, 1, 1);
    glPopMatrix();
    glPushMatrix();  
    glTranslatef(29.6, 3, 1);
    glScalef(0.3, 10, 15); 
    cubes(r, g, b, 1, 1);
    glPopMatrix();
    glPushMatrix(); 
    glTranslatef(9, 13, 0);
    glScalef(22, 1, 20); 
    cubes(0.8, 1, 1, 1, 1);
    glPopMatrix();



};

void half_engine() {
    glPushMatrix(); 
    glTranslatef(15.2, 3.5, 28);
    glScalef(6, 2, 3);
    cubes(0.2, 0.2, 1, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(15.2, 5.5, 28);
    glScalef(6, 1.5, 3);
    cubes(1, 1, 0.2, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(15.2, 7, 28);
    glScalef(6, 2, 3);
    cubes(0.2, 0.2, 1, 1, 1);
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(15, 3.5, 27.5);
    glScalef(10, 0.2, 4);
    cubes(1, 1, 0, 1, 1);
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(21.2, 3.5, 31);
    glScalef(3, 2, 0.05);
    cubes(0.2, 0.2, 1, 1, 1);
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(21.2, 3.5, 28);
    glScalef(3, 2, 0.05);
    cubes(0.2, 0.2, 1, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(21.2, 5.5, 31);
    glScalef(1.5, 2.2, 0.05);
    cubes(1, 1, 0.2, 1, 1);
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(21.2, 5.5, 28);
    glScalef(1.5, 2.2, 0.05);
    cubes(1, 1, 0.2, 1, 1);
    glPopMatrix();


    glPushMatrix(); 
    glTranslatef(21.2, 7.7, 31);
    glScalef(1.5, 0.2, 0.05);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(21.2, 7.7, 28);
    glScalef(1.5, 0.2, 0.05);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();


    glPushMatrix(); 
    glTranslatef(21.2, 8.8, 31);
    glScalef(1.5, 0.2, 0.05);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(21.2, 8.8, 28);
    glScalef(1.5, 0.2, 0.05);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(21.2, 7.7, 31);
    glScalef(0.2, 1.4, 0.05);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(21.2, 7.7, 28);
    glScalef(0.2, 1.4, 0.05);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();


    glPushMatrix(); 
    glTranslatef(22.5, 7.7, 31);
    glScalef(0.2, 1.4, 0.05);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(22.5, 7.7, 28);
    glScalef(0.2, 1.4, 0.05);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(22.7, 3.5, 31);
    glScalef(1, 5.5, 0.05);
    cubes(0.2, 0.2, 1, 1, 1);
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(22.7, 3.5, 28);
    glScalef(1, 5.5, 0.05);
    cubes(0.2, 0.2, 1, 1, 1);
    glPopMatrix();
    
    glPushMatrix(); 
    glTranslatef(24, 3.5, 28);
    glScalef(1, 4, 3);
    cubes(0.2, 0.2, 1, 1, 1);
    glPopMatrix();
    

    glPushMatrix(); 
    glTranslatef(24, 8.5, 28);
    glScalef(1, 0.5, 3);
    cubes(1, 0.1, 0.1, 1, 1);
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(15, 9, 28);
    glScalef(9, 0.2, 3);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();


    glPushMatrix(); 
    glTranslatef(1, 32.5, 28);
    glTranslatef(0, 0, -0.5);
    glRotatef(90, 1, 0, 0);
    glTranslatef(0, 0, 0.5);
    glTranslatef(15.2, 2, 29.5);
    glScalef(2, 3.5, 2);
    cylinder(0, 0, 0);
    glTranslatef(2, 0, 0);
    cylinder(0, 0, 0);
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(17.5, 2, 30.5);
    glRotatef(90, 0, 1, 0);
    glScalef(35, 45, 5);
    cyclider(0.99, 0.99, 0.99);
    glPopMatrix();
    glPushMatrix(); 
    glTranslatef(17.5, 2.7, 30.5);
    glRotatef(90, 0, 1, 0);
    glScalef(30, 40, 3);
    cyclider(0.99, 0.99, 0.99);
    glPopMatrix();


    glPushMatrix(); 
    glTranslatef(15, 5.5, 31.4);
    glScalef(5, 0.1, 0.1);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(15, 5.5, 27.5);
    glScalef(5, 0.1, 0.1);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(15, 3.7, 31.4);
    glScalef(0.07, 1.8, 0.1);
    cubes(1, 1, 1, 1, 1);
    for (int i = 0; i < 20; i++) {
        glTranslatef(3.5, 0, 0);
        cubes(1, 1, 1, 1, 1);
    }
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(15, 3.7, 27.5);
    glScalef(0.07, 1.8, 0.1);
    cubes(1, 1, 1, 1, 1);
    for (int i = 0; i < 20; i++) {
        glTranslatef(3.5, 0, 0);
        cubes(1, 1, 1, 1, 1);
    }
    glPopMatrix();
}

void engine()
{
    glTranslatef(32, 0, 0);
    half_engine();
}


// seating arrangements inside train
void train_seat()
{
    glPushMatrix();  //back row
    glTranslatef(16, 5, 17.1); 
    glScalef(1, 0.2, 1);
    cubes(1, 0, 0, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(15.8, 5, 17.1);
    glScalef(0.2, 2, 1);
    cubes(0.1, 0.1, 1, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(15.8, 5.3, 17.1);
    glScalef(1, 0.1, 0.1);
    cubes(0, 0, 0, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(15.8, 5.3, 18);
    glScalef(1, 0.1, 0.1);
    cubes(0, 0, 0, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(15.8, 3.28, 17.5); 
    glScalef(0.2, 1.7, 0.2);
    cubes(0, 0, 0, 1, 1);
    glPopMatrix();

    glPushMatrix(); //front row
    glTranslatef(16, 5, 19); 
    glScalef(1, 0.2, 1);
    cubes(1, 0, 0, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(15.8, 5, 19);
    glScalef(0.2, 2, 1);
    cubes(0.1, 0.1, 1, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(15.8, 5.3, 19);
    glScalef(1, 0.1, 0.1);
    cubes(0, 0, 0, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(15.8, 5.3, 19.9);
    glScalef(1, 0.1, 0.1);
    cubes(0, 0, 0, 1, 1);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(15.8, 3.28, 19.4); 
    glScalef(0.2, 1.7, 0.2);
    cubes(0, 0, 0, 1, 1);
    glPopMatrix();

}

//railway track
void rail_line()
{
    
    glPushMatrix();
    glTranslatef(-20, 0, 28); 
    glScalef(150, 1, 3);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-20, 1, 28.5);
    glScalef(150, 0.2, 0.2);
    cubes(0, 0, 0, 1, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-20, 1, 30.5); 
    glScalef(150, 0.2, 0.2);
    cubes(0, 0, 0, 1, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-20, 1, 28); 
    glScalef(0.2, 0.2, 3);
    cubes(0, 0, 0, 1, 1);
    for (int j = 0; j < 400; j++)
    {
        glTranslatef(3, 0, 0);
        cubes(0, 0, 0, 1, 1);
    }
    glPopMatrix();
}



void bodyOneSide()
{

    glPushMatrix(); //left  start
    glTranslatef(12.5, 3, 20);
    glScalef(0.5, 8, 0.1);
    cubes(0.8, 0.8, 0.1, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(13, 3, 20);
    glScalef(2, 0.3, 0.1);
    cubes(0.8, 0.8, 0.1, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(13, 10.7, 20);
    glScalef(2, 0.3, 0.1);
    cubes(0.8, 0.8, 0.1, 1, 1);
    glPopMatrix();  //left finish


    glPushMatrix(); //right start
    glTranslatef(35, 3, 20);
    glScalef(0.5, 8, 0.1);
    cubes(0.8, 0.8, 0.1, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(33, 3, 20);
    glScalef(2, 0.3, 0.1);
    cubes(0.8, 0.8, 0.1, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(33, 10.7, 20);
    glScalef(2, 0.3, 0.1);
    cubes(0.8, 0.8, 0.1, 1, 1);
    glPopMatrix();  //right finish


    glPushMatrix();
    glTranslatef(15, 3, 20);
    glScalef(18, 3, 0.1);
    cubes(0, 0.7, 0.9, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(15, 8, 20);
    glScalef(18, 3, 0.1);
    cubes(0, 0.7, 0.9, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(15, 6, 20);
    glScalef(2, 2, 0.1);
    cubes(0.8, 0.8, 0.1, 1, 1);
    for (int j = 0; j < 4; j++)
    {
        glTranslatef(2, 0, 0);
        cubes(0.8, 0.8, 0.1, 1, 1);
    }
    glPopMatrix();
}


//train body box
void box()
{
    glPushMatrix();
    bodyOneSide();
    glTranslatef(0, 0, -3);
    bodyOneSide();
    glPopMatrix();

    glPushMatrix(); //left wheel
    glTranslatef(15, 2, 17);
    glScalef(1, 1.2, 3);
    cubes(0, 0, 0, 1, 1);
    glTranslatef(1.5, 0, 0);
    cubes(0, 0, 0, 1, 1);
    glPopMatrix();
    glPushMatrix(); //right wheel
    glTranslatef(31, 2, 17);
    glScalef(1, 1.2, 3);
    cubes(0, 0, 0, 1, 1);
    glTranslatef(1.5, 0, 0);
    cubes(0, 0, 0, 1, 1);
    glPopMatrix();

    glPushMatrix(); //floor
    glTranslatef(12.5, 3, 17);
    glScalef(23, 0.3, 3);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();
    glPushMatrix(); //ceiling
    glTranslatef(12.5, 11, 17);
    glScalef(23, 0.3, 3);
    cubes(0, 0.7, 0.9, 1, 1);
    glPopMatrix();

    glPushMatrix(); //left wall
    glTranslatef(12.5, 3, 17);
    glScalef(0.5, 8, 3);
    cubes(0, 0.7, 0.9, 1, 1);
    glPopMatrix();

    glPushMatrix(); //right wall
    glTranslatef(35, 3, 17);
    glScalef(0.5, 8, 3);
    cubes(0, 0.7, 0.9, 1, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(35.5, 4, 17.8);
    glScalef(1.5, 1, 0.4);
    cubes(0, 0, 0, 1, 1);
    glPopMatrix();
    glPushMatrix();
    train_seat();
    for (int i = 0; i < 8; i++) {
        glTranslatef(2, 0, 0);
        train_seat();
    }
    glPopMatrix();
}
void train()
{
    glPushMatrix();
    glTranslatef(-10, 0, 0);
    box();
    glTranslatef(24, 0, 0);
    box();
    glTranslatef(-48, 0, 0);
    box();
    for (int i = 0; i < 10; i++) {
        glTranslatef(-24, 0, 0);
        box();
    }
    glPopMatrix();
}

void fence()
{
    glPushMatrix();
    glTranslatef(-30, 0, -3);
    for (int j = 0; j < 50; j++) {
        pillar(0.3, 0, 0, 1, 1, 5);
        glTranslatef(5, 0, 0);
    }
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-30, 4, -2.4);
    glScalef(5, 0.5, 0.1);
    cubes(0.3, 0, 0, 1, 1);
    for (int j = 0; j < 48; j++) {
        glTranslatef(1, 0, 0);
        cubes(0.3, 0, 0, 1, 1);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-30, 2.5, -2.4);
    glScalef(5, 0.5, 0.1);
    cubes(0.3, 0, 0, 1, 1);
    for (int j = 0; j < 40; j++) {
        glTranslatef(1, 0, 0);
        cubes(0.3, 0, 0, 1, 1);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-30, 1, -2.4);
    glScalef(5, 0.5, 0.1);
    cubes(0.3, 0, 0, 1, 1);
    for (int j = 0; j < 40; j++) {
        glTranslatef(1, 0, 0);
        cubes(0.3, 0, 0, 1, 1);
    }
    glPopMatrix();
}

//entire platfrom
void platform()
{
    //path
    glPushMatrix();
    glTranslatef(-20, 0, 20); //z-10
    glScalef(200, 2, 7);
    cubes(0.8, 0.8, 0.8, 1, 20);
    glPopMatrix();

    glPushMatrix();
    for (int j = 0; j < 12; j++)
    {
        glPushMatrix();
       
        glPushMatrix();
        glTranslatef(8, 2, 22); 
        glScalef(2, 1, 2);
        cubes(0.5, 0.5, 1, 1, 1);
        glPopMatrix();
        glTranslatef(8.5, 2, 22.5); 
        pillar(1, 1, 0, 1, 1, 15);
        glPopMatrix();

        glTranslatef(15, 0, 0);
    }
    glPopMatrix();

    //roof
    glPushMatrix();
    glTranslatef(5, 17, 20);
    glScalef(200, 0.2, 6);
    cubes(0.8f, 0, 0, 1, 20);
    glPopMatrix();
    
}

void fan() {
    glPushMatrix();  
    glPushMatrix();
    glTranslatef(15, 13, 23); 
    glTranslatef(1.7, 0.005, 0.2);
    glRotatef(faTheta, 0, 1, 0);
    glTranslatef(-1.7, -0.005, -0.2);
    glScalef(1.5, 0.01, 0.4);
    cubes(0, 0, 0, 12, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(16.9, 13, 23); 
    glTranslatef(-0.2, 0.005, 0.2);
    glRotatef(faTheta, 0, 1, 0);
    glTranslatef(0.2, -0.005, -0.2);
    glScalef(1.5, 0.01, 0.4);
    cubes(0, 0, 0, 12, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(16.5, 13, 21.5);
    glTranslatef(0.2, 0.005, 1.7);
    glRotatef(faTheta, 0, 1, 0);
    glTranslatef(-0.2, -0.005, -1.7);
    glScalef(0.4, 0.01, 1.5);
    cubes(0, 0, 0, 12, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(16.5, 13, 23.4); 
    glTranslatef(0.2, 0.005, -0.2);
    glRotatef(faTheta, 0, 1, 0);
    glTranslatef(-0.2, -0.005, 0.2);
    glScalef(0.4, 0.01, 1.5);
    cubes(0, 0, 0, 12, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(16.5, 13, 23); //Middle fan
    glTranslatef(0.2, 0.005, 0.2);
    glRotatef(faTheta, 0, 1, 0);
    glTranslatef(-0.2, -0.005, -0.2);
    glScalef(0.4, 0.01, 0.4);
    cubes(1, 1, 1, 15, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(16.7, 13, 23.2); //fan_stick
    glScalef(0.1, 4, 0.1);
    cubes(0, 0, 0, 12, 1);
    glPopMatrix();
    glPopMatrix();

}

void tree()
{

    glPushMatrix();
    glTranslatef(20, 7, 25);
    glScalef(0.25, 1, 0.25);
    glRotatef(-90, 1, 0, 0);
    leaf(1, 1, 1, 0.1, 1, 0.1);
    glPopMatrix();

}

//flower pot
void pot() {

    glPushMatrix();
    glTranslatef(21.5, 2, 17.09);
    glScalef(0.5, 4, 1);
    Basket(0.5, 0.2, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.7, 0.2, -7);
    glScalef(1, 0.7, 1);
    tree();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(17, 2, 17.09);
    glScalef(0.5, 4, 1);
    Basket(0.5, 0.2, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2.7, 0.2, -7);
    glScalef(1, 0.7, 1);
    tree();
    glPopMatrix();

}

void sphere2(float r, float g, float b, int a) {
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.8 * r, 0.8 * g, 0.8 * b, 1.0 };
    GLfloat mat_diffuse[] = { r, g, b, 1.0 };
    GLfloat mat_specular[] = { 0.7 ,0.7, 0.7, 1.0 };
    GLfloat mat_shininess[] = { 80 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, a);

    GLUquadric* qobj = gluNewQuadric();
    gluQuadricTexture(qobj, GL_TRUE);
    gluSphere(qobj, 1, 20, 20);
    gluDeleteQuadric(qobj);
}


void steps()
{
    for (int i = 0; i < 12; i++) { //back left steps
        glPushMatrix();
        glTranslatef(-15 + (0.75 * i), 2 + i, 22);//-10
        glScalef(0.75, 0.25, 2);
        cubes(1, .1, .1, 1, 1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-14.25 + (0.75 * i), 2 + i, 22);//-9.25
        glScalef(0.25, 1, 2);
        cubes(0.2, 0.2, 0.2, 1, 1);
        glPopMatrix();
    }

    for (int i = 0; i < 13; i++) {
        glPushMatrix(); //SIDE 
        glTranslatef(-15 + (0.75 * i), 2 + i, 22);//-10
        glScalef(0.04, 2, 0.04);
        cubes(1, .1, .1, 1, 1);
        glPopMatrix();

        glPushMatrix(); //SIDE 
        glTranslatef(-15 + (0.75 * i), 2 + i, 24);//-10
        glScalef(0.04, 2, 0.04);
        cubes(1, .1, .1, 1, 1);
        glPopMatrix();
    }
    glPushMatrix(); 
    glTranslatef(-15, 4, 22);
    glTranslatef(0, 0, 0.02);
    glRotatef(53.13, 0, 0, 1);
    glTranslatef(0, 0, 0.02);
    glScalef(15, 0.1, 0.04);
    cubes(1, .1, .1, 1, 1);
    glPopMatrix();

    glPushMatrix(); 
    glTranslatef(-15, 4, 24);
    glTranslatef(0, 0, 0.02);
    glRotatef(53.13, 0, 0, 1);
    glTranslatef(0, 0, 0.02);
    glScalef(15, 0.1, 0.04);
    cubes(1, .1, .1, 1, 1);
    glPopMatrix();

    for (int i = 0; i < 12; i++) { 
        glPushMatrix();
        glTranslatef(-2 + (0.75 * i), 13 - i, 22); 
        glScalef(0.75, 0.25, 2);
        cubes(1, .1, .1, 1, 1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-2.25 + (0.75 * i), 13 - i, 22); 
        glScalef(0.25, 1, 2);
        cubes(0.2, 0.2, 0.2, 1, 1);
        glPopMatrix();
    }
    for (int i = 0; i < 13; i++) {
        glPushMatrix(); 
        glTranslatef(-2.04 + (0.75 * i), 14 - i, 22); 
        glScalef(0.04, 2, 0.04);
        cubes(1, .1, .1, 1, 1);
        glPopMatrix();

        glPushMatrix(); 
        glTranslatef(-2.04 + (0.75 * i), 14 - i, 24); 
        glScalef(0.04, 2, 0.04);
        cubes(1, .1, .1, 1, 1);
        glPopMatrix();
    }
    glPushMatrix(); 
    glTranslatef(-2, 16, 22);
    glTranslatef(0, 0, 0.02);
    glRotatef(-53.13, 0, 0, 1);
    glTranslatef(0, 0, 0.02);
    glScalef(15.64, 0.1, 0.04);
    cubes(1, .1, .1, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2, 16, 24);
    glTranslatef(0, 0, 0.02);
    glRotatef(-53.13, 0, 0, 1);
    glTranslatef(0, 0, 0.02);
    glScalef(15.64, 0.1, 0.04);
    cubes(1, .1, .1, 1, 1);
    glPopMatrix();
    for (int i = 0; i < 7; i++) { 
        glPushMatrix();
        glTranslatef(-6, 14, 22 + (2 * i));
        glScalef(4, 0.25, 1);
        cubes(1, .1, .1, 1, 1);
        glPopMatrix();

        glPushMatrix(); 
        glTranslatef(-6, 14, 22 + (2 * i));
        glScalef(0.04, 2, 0.04);
        cubes(1, .1, .1, 1, 1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-6, 14, 23 + (2 * i));
        glScalef(4, 0.25, 1);
        cubes(0.2, 0.2, 0.2, 1, 1);
        glPopMatrix();

        glPushMatrix(); 
        glTranslatef(-6, 14, 25 + (2 * i));
        glScalef(0.04, 2, 0.04);
        cubes(0.2, 0.2, 0.2, 1, 1);
        glPopMatrix();
    }
    glPushMatrix(); // middle railing 2 back and front
    glTranslatef(-6, 16, 22);
    glScalef(4, 0.1, 0.04);
    cubes(1, .1, .1, 1, 1);
    glPopMatrix();

    glPushMatrix(); // middle walkway side railing
    glTranslatef(-6, 16, 24);
    glScalef(0.04, 0.1, 5);
    cubes(1, .1, .1, 1, 1);
    glPopMatrix();

    glPushMatrix(); // middle walkway side railing
    glTranslatef(-2, 16, 24);
    glScalef(0.04, 0.1, 10);
    cubes(1, .1, .1, 1, 1);
    glPopMatrix();

    glPushMatrix(); // middle beem
    glTranslatef(-4, 2, 24);
    glScalef(20, -40.5, 20);
    glRotatef(90, 1, 0, 0);
    cyclider(0.5, 0.5, 0.5);
    glPopMatrix();

}

// bridge over track
void overbridge()
{
    glPushMatrix();
    steps();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-8, 0, 63);
    glRotatef(180, 0, 1, 0);
    steps();
    glPopMatrix();
}

void tree1(int a)
{
 
    glPushMatrix();
    glTranslatef(15, 6, 25);
    glScalef(1, 15, 1);
    cylinder(0.5, 0.5, 0.5);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(16.5, 11, 25);
  
    glRotatef(-45, 0, 0, 1);
    
    glScalef(0.5, 5, 0.2);
    cylinder(1, 1, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(14, 11, 25);
   
    glRotatef(30, 0, 0, 1);
   
    glScalef(0.5, 5, 0.2);
    cylinder(1, 1, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(15, 16, 25);
    glScalef(2, 5, 3);
    sphere2(0.1, 1.0, 0.1, a);
    glPopMatrix();
    glPushMatrix(); // right leave
    glTranslatef(17, 15, 25);
    glScalef(2, 4, 2);
    sphere2(0.1, 1.0, 0.1, a);
    glPopMatrix();
    glPushMatrix(); // left leave
    glTranslatef(13.5, 15, 25);
    glScalef(2, 4, 2);
    sphere2(0.1, 1.0, 0.1, a);
    glPopMatrix();
   
}

//sky
void sky(int a) {
    glPushMatrix(); // sky
    glScalef(200, 200, 200);
    sphere2(0.8, 0.8, 1, a);
    glPopMatrix();

}
void bench()
{
    glPushMatrix();
    glTranslatef(17, 5, 24); 
    glScalef(3, 0.1, 1);
    cubes(1, 0.1, 0.1, 1, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(17, 5, 25);
    glScalef(3, 0.1, 0.9);
    cubes(1, 1, 0, 1, 1);
    glPopMatrix();
    glPushMatrix(); 
    glTranslatef(17, 5, 24);
    glScalef(3, 2, 0.1);
    cubes(0.4, 0.4, .1, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(17, 6, 24); //left hand
    glScalef(0.1, 0.1, 2);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(19.9, 6, 24); //right hand
    glScalef(0.1, 0.1, 2);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(17, 2, 24); //left back 
    glScalef(0.1, 3, 0.1);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(17, 2, 25.9); //left front 
    glScalef(0.1, 4, 0.1);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(19.9, 2, 24); //right back 
    glScalef(0.1, 3, 0.1);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(19.9, 2, 25.9); //left front 
    glScalef(0.1, 4, 0.1);
    cubes(1, 1, 1, 1, 1);
    glPopMatrix();
}

void moon() {
    glPushMatrix();
    glTranslatef(50, 150, -110);
    glScalef(5, 5, 5);
    sphere2(1, 1, 1, 47);
    glPopMatrix();
}

//LIGHT0
void Light_One()
{
    glPushMatrix();
    GLfloat light_position[] = { 15, 40.0, 20.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glPopMatrix();
}

//LIGHT1
void Light_Two()
{
    glPushMatrix();
    GLfloat light_position[] = { 5.0, 7.0, 10.0, 1.0 };
    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
    glPopMatrix();
}

//LIGHT3 Spot Light
void Light_Three()
{
    glPushMatrix();
    GLfloat light_position[] = { 50,140,-103, 1.0 }; 
    glLightfv(GL_LIGHT2, GL_POSITION, light_position);
    GLfloat spot_direction[] = { 0.0, 1.0, -1.0 };
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0);
    glPopMatrix();
}

double eye[3] = { 20,9,47 };
double center[3] = { 20,9,0 };
double up[3] = { 0,1,0 };
double zoom[3] = { 1,1,1 };
double wind[2] = { 3,8 };
double nzoom[1] = { 4 };

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-wind[0], wind[1], -wind[0], wind[1], nzoom[0], 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

    glViewport(0, 0, windowHeight, windowWidth);

    glEnable(GL_LIGHTING);
    Light_One();
    Light_Two();
    Light_Three();

    glRotatef(theta, axis_x, axis_y, 0.0);

    glScalef(zoom[0], zoom[1], zoom[2]); //zoom the scene

    rail_line(); //track

    //Big building
    buildingPath();
    building(1, 1, 1); //ground floor
    glPushMatrix();
    glTranslatef(0, 11, 0);
    building(1, 1, 1); //first floor
    glPopMatrix();
    
    fence();
    glPushMatrix();
    glTranslatef(0, 0, -22);
    fence();
    glPopMatrix();

    //2nd building
    glPushMatrix();
    glTranslatef(65, 0, 0);
    buildingPath();
    building(0.9, 0.9, 0.1);
    glPopMatrix();

    //train
    glPushMatrix();
    glTranslatef(-180, -.5, 11); 
    glTranslatef(-ftTheta, 0, 0);
    train();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-177, 0, 0);
    glTranslatef(-ftTheta, 0, 0);
    engine();
    glPopMatrix();

    //back platform
    glPushMatrix();
    platform();
    glPopMatrix();

    //front platform
    glPushMatrix();
    glTranslatef(0, 0, 16);
    platform();
    glPopMatrix();

    //bin
    glPushMatrix();
    glTranslatef(10, 2, 23);
    glScalef(1, 6, 1);
    Basket(0, 0.4, 0, 1);
    for (int j = 0; j < 10; j++) {
        glTranslatef(15, 0, 0);
        Basket(0, 0.4, 0, 1);
    }
    glPopMatrix();

    //fan
    glPushMatrix();
    fan();
    for (int j = 0; j < 15; j++)
    {
        glTranslatef(15, 0, 0);
        fan();
    }
    glPopMatrix();

    pot();
 
    overbridge();

    glPushMatrix(); 
    glScalef(1.5, 1.5, 1.5);
    glTranslatef(-30, 0, -45);
    tree1(18);
    for (int i = 0; i < 50; i++) {
        glTranslatef(7, 0, 0);
        tree1(18);
    }
    glPopMatrix();

    //line of trees
    glPushMatrix();
    glScalef(1.5, 1.5, 1.5);
    glTranslatef(-15, 0, -17);
    tree1(45);
    glPopMatrix();

    glPushMatrix(); 
    glScalef(1.5, 2.2, 1.5);
    glTranslatef(7, 0, -17);
    tree1(45);
    glPopMatrix();

    glPushMatrix(); 
    glScalef(1.5, 2.2, 1.5);
    glTranslatef(23, 0, -17);
    tree1(45);
    glPopMatrix();

    //sky
    glPushMatrix(); 
    if (night == false)
    {
        sky(36);
    }
    else {
        sky(46);
        glPopMatrix();
        moon();
        glPushMatrix();
    }
    sky(36);
    
    //big tree
    glScalef(3, 5, 3);
    glTranslatef(-0.5, -7, -20);
    tree();
    glPopMatrix();

    //line of grasses in left side
    glPushMatrix();
    glScalef(1.5, 1.5, 1.5);
    glTranslatef(-15, -7, -15);
    tree();
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        for (int j = 0; j < 6; j++)
        {
            glTranslatef(-(1.3 * j), 0, 0);
            tree();
        }
        glPopMatrix();
        glTranslatef(0, 0, -(2 * i));
    }
    glPopMatrix();


    glPushMatrix();
    glScalef(2, 2, 2);
    sphere2(1, 1, 1, 35);
    glPopMatrix();


    //seats
    glPushMatrix();
    glTranslatef(30, 0, -4);
    bench();
    glTranslatef(15, 0, 0);
    bench();
    glTranslatef(15, 0, 0);
    bench();

    glTranslatef(45, 0, 0);
    bench();
    glTranslatef(15, 0, 0);
    bench();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(60, 0, -21);
    bench();
    glTranslatef(3.2, 0, 0);
    bench();
    glTranslatef(3.2, 0, 0);
    bench();
    glTranslatef(3.2, 0, 0);
    bench();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.5, 1, 1);
    glTranslatef(-2.5, 0, 22);
    glRotatef(90, 0, 1, 0);
    bench();
    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}


void Pitch_y1() {
    GLfloat ty, tz;

    ty = center[1] - eye[1];//ty = look_y - eye_y;
    tz = center[2] - eye[2];//tz = look_z - eye_z;

    dy = ty * cs_angle - tz * sn_angle;
    dz = ty * sn_angle + tz * cs_angle;

    center[1] = dy + eye[1];//look_y = dy + eye_y;
    center[2] = dz + eye[2];//look_z = dz + eye_z;

}
void Pitch_y2() {
    GLfloat ty, tz;

    ty = center[1] - eye[1];// ty = look_y - eye_y;
    tz = center[2] - eye[2];// tz = look_z - eye_z;

    dy = ty * cs_angle + tz * sn_angle;// dy = ty*cs_angle+tz*sn_angle;
    dz = ty * sn_angle + tz * cs_angle;// dz = -ty*sn_angle+tz*cs_angle;

    center[1] = dy + eye[1]; //look_y = dy + eye_y;
    center[2] = dz + eye[2];////look_z = dz + eye_z;

}
void Yaw_y1() {
    GLfloat tx, tz;

    tx = center[0] - eye[0];//tx = look_x - eye_x;
    tz = center[2] - eye[2];//tz = look_z - eye_z;

    dx = tx * cs_angle + tz * sn_angle;//dx = tx*cs_angle+tz*sn_angle;
    dz = -tx * sn_angle + tz * cs_angle;//dz = -tx*sn_angle+tz*cs_angle;

    center[0] = dx + eye[0];//look_x = dx + eye_x;
    center[2] = dz + eye[2];//look_z = dz + eye_z;

}

void Yaw_y2() {
    GLfloat tx, tz;

    tx = center[0] - eye[0];//tx = look_x - eye_x;
    tz = center[2] - eye[2];//tz = look_z - eye_z;

    dx = tx * cs_angle - tz * sn_angle;//dx = tx*cs_angle-tz*sn_angle;
    dz = tx * sn_angle + tz * cs_angle;//dz = tx*sn_angle+tz*cs_angle;

    center[0] = dx + eye[0];//look_x = dx + eye_x;
    center[2] = dz + eye[2];//look_z = dz + eye_z;

}


void myKeyboardFunc(unsigned char key, int x, int y)
{
    
    float x1, z1, r, theta1, dx, dz, dx_norm, dz_norm, r1 = 1;
    x1 = center[0] - eye[0];
    z1 = center[2] - eye[2];
    r = sqrt(x1 * x1 + z1 * z1);

    if (x1 == 0)
    {
        if (z1 > 0)
        {
            theta1 = 90;
        }
        else if (z1 < 0)
        {
            theta1 = -90;
        }
    }
    else
        theta1 = atan(z1 / x1) * 180 / 3.1416;

    if ((z1 > 0 && theta1 < 0) || (z1 < 0 && theta1>0))
        theta1 += 180;
    else if (z1 < 0 && theta1 < 0)
    {
        theta1 += 360;
    }

    dx = r1 * cos(theta1 * 3.1416 / 180);
    dz = r1 * sin(theta1 * 3.1416 / 180);

    dx_norm = r1 * cos((theta1 - 90) * 3.1416 / 180);
    dz_norm = r1 * sin((theta1 - 90) * 3.1416 / 180);

    switch (key)
    {
    
    case 'z':
        zoom[2] += 0.1;
        glScalef(zoom[0], zoom[1], zoom[2]);
        break;
    case 'Z':
        zoom[2] -= 0.1;
        glScalef(zoom[0], zoom[1], zoom[2]);
        break;
    case 'n':
        nzoom[0] -= 1;
        break;
    case 'N':
        nzoom[0] += 1;
        break;
    case 'j':
        wind[0] -= 1;
        wind[1] -= 1;
        break;
    case 'J':
        wind[0] += 1;
        wind[1] += 1;
        break;
    case 'f':
        eye[0] -= 1;
        gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
        break;
    case 'F':
        eye[0] += 1;
        gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
        break;
    case 'g':
        eye[1] -= 1;
        gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
        break;
    case 'G':
        eye[1] += 1;
        gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
        break;
    case 'h':
        eye[2] -= 1;
        gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
        break;
    case 'H':
        eye[2] += 1;
        gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
        break;

    case 'i':        
        eye[0] = 20;
        eye[1] = 9;
        eye[2] = 47;
        center[0] = 20;
        center[1] = 9;
        center[2] = -50;
        up[0] = 0;
        up[1] = 1;
        up[2] = 0;
        zoom[0] = 1;
        zoom[1] = 1;
        zoom[2] = 1;
        wind[0] = 3;
        wind[1] = 6;
        break;
  

    
    //open the door and windows
    case '-':
        fTheta -= 2;
        break;

    case '*':
        fTheta += 2;
        break;

    case 'l':   //new add
        eye[0] += dx_norm * 5;
        eye[2] += dz_norm * 5;


        center[0] += dx_norm * 5;
        center[2] += dz_norm * 5;
        break;
    case 'U':
        eye[0] += dx * 5;
        eye[2] += dz * 5;
        center[0] += dx * 5;
        center[2] += dz * 5;
        break;
    case 'K':
        eye[0] -= dx * 5;
        eye[2] -= dz * 5;

        center[0] -= dx * 5;
        center[2] -= dz * 5;
        break;
    case 'r':
        eye[0] -= dx_norm * 5;
        eye[2] -= dz_norm * 5;

        center[0] -= dx_norm * 5;
        center[2] -= dz_norm * 5;

    case 't':  //Train forward
        ftTheta -= 2;
        break;

    case 'T': //train backward
        ftTheta += 2;
        break;

    case '1':
        if (Light1 == false)
        {
            Light1 = true; ambL1 = true; diffL1 = true; specL1 = true;
            glEnable(GL_LIGHT0);   //Turn on light 1
        }
        else if (Light1 == true)
        {
            Light1 = false; ambL1 = false; diffL1 = false; specL1 = false;
            glDisable(GL_LIGHT0);   //Turn off light 2
        }
        break;
    case '2':
        if (Light2 == false)
        {
            Light2 = true; ambL2 = true; diffL2 = true; specL2 = true;
            glEnable(GL_LIGHT1);    //Turn on light 2
        }
        else if (Light2 == true)
        {
            Light2 = false; ambL2 = false; diffL2 = false; specL2 = false;
            glDisable(GL_LIGHT1);  //Turn off light 2
        }
        break;
    case '3':
        if (spotLight == false)
        {
            spotLight = true; ambL3 = true; diffL3 = true; specL3 = true;
            glEnable(GL_LIGHT2);   //Turn on spot light
        }
        else if (spotLight == true)
        {
            spotLight = false; ambL3 = false; diffL3 = false; specL3 = false;
            glDisable(GL_LIGHT2);  //Turn off spot light
        }
        break;

    //for enttire scene
    case '4':
        Pitch_y1();
        break;
    case '5':
        Pitch_y2();
        break;
    case '6':
        Yaw_y1();
        break;
    case '7':
        Yaw_y2();
        break;

    case 27:	// Escape key
        exit(1);
    }
}



void animate()
{
   
    if (welRotate == true)
    {
        p += 0.02;
        k = p;
        faTheta += 2;
        if (k >= 7)
        {
            k = 0;
            p = 0;
        }
        //Sleep(1000);
    }

    //if (bRotate == true)
    //{
    //    fTheta += 0.2;
    //    if (theta > 360.0)
    //        theta -= 360.0 * floor(theta / 360.0);
    //}


    //Light1
    if (ambL1 == true)
    {
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient1);
    }
    else if (ambL1 == false) {
        glLightfv(GL_LIGHT0, GL_AMBIENT, no_light);
    }
    if (diffL1 == true) {
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse1);
    }
    else if (diffL1 == false) {
        glLightfv(GL_LIGHT0, GL_DIFFUSE, no_light);
    }
    if (specL1 == true) {
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    }
    else if (specL1 == false) {
        glLightfv(GL_LIGHT0, GL_SPECULAR, no_light);
    }

    //Light2
    if (ambL2 == true) {
        glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient2);
    }
    else if (ambL2 == false) {
        glLightfv(GL_LIGHT1, GL_AMBIENT, no_light);
    }
    if (diffL2 == true) {
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
    }
    else if (diffL2 == false) {
        glLightfv(GL_LIGHT1, GL_DIFFUSE, no_light);
    }
    if (specL2 == true) {
        glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    }
    else if (specL2 == false) {
        glLightfv(GL_LIGHT1, GL_SPECULAR, no_light);
    }

    //Light3
    if (ambL3 == true) {
        glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient3);
    }
    else if (ambL3 == false) {
        glLightfv(GL_LIGHT2, GL_AMBIENT, no_light);
    }
    if (diffL3 == true) {
        glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse3);
    }
    else if (diffL3 == false) {
        glLightfv(GL_LIGHT2, GL_DIFFUSE, no_light);
    }
    if (specL3 == true) {
        glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular);
    }
    else if (specL3 == false) {
        glLightfv(GL_LIGHT2, GL_SPECULAR, no_light);
    }
    //light4
    if (ambL3 == true) {
        glLightfv(GL_LIGHT3, GL_AMBIENT, light_ambient4);
    }
    else if (ambL3 == false) {
        glLightfv(GL_LIGHT3, GL_AMBIENT, no_light);
    }
    if (diffL3 == true) {
        glLightfv(GL_LIGHT3, GL_DIFFUSE, light_diffuse4);
    }
    else if (diffL3 == false) {
        glLightfv(GL_LIGHT3, GL_DIFFUSE, no_light);
    }
    if (specL3 == true) {
        glLightfv(GL_LIGHT3, GL_SPECULAR, light_specular);
    }
    else if (specL3 == false) {
        glLightfv(GL_LIGHT3, GL_SPECULAR, no_light);
    }
    glutPostRedisplay();

}

static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };


int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(100, 100);
    glutInitWindowSize(windowHeight, windowWidth);
    glutCreateWindow("Railway Station");

    glutKeyboardFunc(myKeyboardFunc);
    glutDisplayFunc(display);
    glutIdleFunc(animate);

    printf("1=enable light 1\n");
    printf("1=click 1 again to disable light 1\n");
    printf("2=enable light 2\n");
    printf("3=enable light on moon\n");
    printf("l= go left \n");
    printf("U= ZOOM in \n");
    printf("K= ZOOM out \n");
    printf("r= go right \n");
    printf("t= click t many times to run the train \n");
    printf("-= click - to open the door\n");
    printf("i= click i to go initial position\n");
    printf("b= click b for top view\n");
    printf("f/F= to move camera left or right\n");
    printf("g/G= to move camera up or down\n");
    printf("h/H= to move camera near or far\n");

    glClearColor(0,0,0,1);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    glutMainLoop();
    return 0;
}
