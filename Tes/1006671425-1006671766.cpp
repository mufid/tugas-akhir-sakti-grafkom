
#define GLUT_DISABLE_ATEXIT_HACK
#undef WIN32_LEAN_AND_MEAN 
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string>
#include <cmath>
#include <ctime>
#include <iostream>
#include "GLee\GLee.h"
#include <GL/glut.h>
#include "Maths\Maths.h"
#include "glm\glm.h"
#include "model/zudomon.h"
#include "init.h"

using namespace std;

// Variables

// Perspective Constant
#define FOVY (double) 80.0
#define ZNEAR (double) 0.6
#define ZFAR (double) 98.0
#define DRAGON_SCALE (double) 0.6
#define SNOW_SCALE (double) 0.6
#define FPS 33

// Object Dimension Constant
#define HIP_UPPER 0.8
#define HIP_LOWER 0.8
#define HIP_HEIGHT 2.9
#define ULEG_UPPER 0.1
#define ULEG_LOWER 1.1
#define ULEG_HEIGHT 1.6
#define LLEG_UPPER 0.1
#define LLEG_LOWER 0.6
#define LLEG_HEIGHT 2.2
#define LEG_UPPER 0.6
#define LEG_LOWER 1.3
#define LEG_HEIGHT 4.2
#define FEET_LENGTH 2.0
#define FEET_WIDTH 1.4
#define FEET_HEIGHT 0.5
#define CALF_UPPER 0.8
#define CALF_LOWER 0.6
#define CALF_HEIGHT 4.2

#define MAX_WING 0.6

#define CHEST_UPPER 1.7
#define CHEST_LOWER 2.2
#define CHEST_HEIGHT 3.2
#define STOMACH_UPPER 1.7
#define STOMACH_LOWER 1.2

#define STOMACH_HEIGHT 3.6

#define SHOULDER_UPPER 0.1
#define SHOULDER_LOWER 1.1
#define SHOULDER_HEIGHT 3.6
#define BICEPT_UPPER 0.8
#define BICEPT_LOWER 0.4
#define BICEPT_HEIGHT 4.5

#define ELBOW_UPPER 0.1
#define ELBOW_LOWER 0.4
#define ELBOW_HEIGHT 2.1
#define ARM_UPPER 0.4
#define ARM_LOWER 0.4
#define ARM_HEIGHT 2.1

#define NECK_UPPER 0.8
#define NECK_LOWER 1.2
#define NECK_HEIGHT 1.1

// Quadric Object
GLUquadricObj *head, *neck, *body, *rChest, *lChest, *rShoulder, *lShoulder, *rArm, *lArm, *rLeg, *lLeg, *rFeet, *lFeet,
			  *pine, *snow;

// Light Color
GLfloat lightD[3][4] = {{0.0, 0.0, 0.8, 1.0}, 
					    {0.8, 0.0, 0.0, 1.0},
                        {1.0, 1.0, 1.0, 1.0}};

// Horizontal Light
GLfloat horizonA[] = {0.3, 0.3, 0.3, 1.0},
		horizonD[] = {0.8, 0.0, 0.0, 1.0}, 
		horizonS[] = {0.6, 0.6, 0.6, 1.0},
		horizonP[] = {10.0, 3.0, 0.0, 1.0};

// Vertical Lightx
GLfloat verticA[] = {0.0, 0.0, 0.0, 1.0},
		verticD[] = {0.8, 0.0, 0.0, 1.0}, 
		verticS[] = {0.6, 0.6, 0.6, 1.0},
		verticP[] = {10.0, 3.0, -30.0, 1.0};

// Materials
// silver, gold, ruby, emerald

GLfloat whiteA[] = { 0.3, 0.3, 0.3},
		whiteD[] = { 0.3, 0.3, 0.3},
		whiteS[] = { 0.3, 0.3, 0.3},
		whiteShine = 0.4 ;

GLfloat goldA[] = { 0.24725, 0.1995, 0.0745 },
		goldD[] = { 0.75164, 0.60648, 0.22648 },
		goldS[] = { 0.628281, 0.555802, 0.366065 },
		goldShine = 0.4 ;

GLfloat emeraldA[] = { 0.0215, 0.1745, 0.0215},
		emeraldD[] = { 0.07568, 0.61424, 0.07568 },
		emeraldS[] = { 0.633, 0.727811, 0.633},
		emeraldShine = 0.6;

GLfloat rubyA[] = { 0.1745, 0.01175, 0.01175 },
		rubyD[] = { 0.61424, 0.04136, 0.04136 },
		rubyS[] = { 0.727811, 0.626959, 0.626959 },
		rubyShine = 0.6;

GLfloat silverA[] = { 0.19225, 0.19225, 0.19225 },
		silverD[] = { 0.50754, 0.50754, 0.50754 },
		silverS[] = { 0.508273, 0.508273, 0.508273 },
		silverShine = 0.4 ;

int currentMaterial = 1;
MATRIX4X4 cameraProjectionMatrix;
MATRIX4X4 cameraViewMatrix;

//Output a cylinder with both lids.
//Erwan Martin <public@fzwte.net>
#define SOLID_CLOSED_CYLINDER(QUAD, BASE, TOP, HEIGHT, SLICES, STACKS) \
gluCylinder(QUAD, BASE, TOP, HEIGHT, SLICES, STACKS); \
glRotatef(180, 1,0,0); \
gluDisk(QUAD, 0.0f, BASE, SLICES, 1); \
glRotatef(180, 1,0,0); \
glTranslatef(0.0f, 0.0f, HEIGHT); \
gluDisk(QUAD, 0.0f, TOP, SLICES, 1); \
glTranslatef(0.0f, 0.0f, -HEIGHT); 

bool isPlay = false; // Apakah animasi bermain?
bool isHorizon = true, isVertic = false; // Apakah lampu menyala?

double lightDegree = 0, lightInc = 0.05; // Derajat Lampu

int lockedPart = 0; // Index joint yang sedang aktif
double jointDegree[9] = {20, 0, 0, 0, 0, 0, 0, 0, 0}; // Derajat tiap joint
double jointInc = 10;

GLfloat rot = 0.0, rotInc = 0.12; // Derajat perputaran model
bool isSpin = true; // Apakah model perlu berputar?

// Animation Variables
bool isAnimate = true; // Apakah model beranimasi?
double dragV = 0, dragVInc = -0.005;
int dragStat = 0;

// Texture Variables
bool showTexture = false;
GLuint shadowMapTexture;
GLuint dragonTex;
const int shadowMapSize = 512;
static GLfloat coef[] = {1.0, 1.0, 1.0, 0.0};

/* memuat dokumen .raw untuk dijadikan texture */
GLuint LoadTextureRAW(const char * filename, int wrap) {
    GLuint texture;
    int width, height;
    BYTE * data;
    FILE * file;

    // open texture data
    file = fopen(filename, "rb");
    if (file == NULL) return 0;

    // allocate buffer
    width = 256;
    height = 256;
    data = (unsigned char *) malloc(width * height * 3);

    // read texture data
    fread(data, width * height * 3, 1, file);
    fclose(file);

    // allocate a texture name
    glGenTextures(1, &texture);

    // select our current texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // select modulate to mix texture with color for shading
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // when texture area is small, bilinear filter the closest MIP map
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    // when texture area is large, bilinear filter the first MIP map
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);

    // build our texture MIP maps
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

    // free buffer
    free(data);

    return texture;
}

void FreeTexture(GLuint texture) {
    glDeleteTextures(1, &texture);
}


// Init Function
void init() {

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClearColor(1.0, 1.0, 1.0, 1.0);

	head = gluNewQuadric();
	gluQuadricDrawStyle(head, GLU_FILL);
	neck = gluNewQuadric();
	gluQuadricDrawStyle(neck, GLU_FILL);
	body = gluNewQuadric();
	gluQuadricDrawStyle(body, GLU_FILL);
	rChest = gluNewQuadric();
	gluQuadricDrawStyle(rChest, GLU_FILL);
	rShoulder = gluNewQuadric();
	gluQuadricDrawStyle(rShoulder, GLU_FILL);
	rArm = gluNewQuadric();
	gluQuadricDrawStyle(rArm, GLU_FILL);
	rLeg = gluNewQuadric();
	gluQuadricDrawStyle(rLeg, GLU_FILL);
	rFeet = gluNewQuadric();
	gluQuadricDrawStyle(rFeet, GLU_FILL);
	lChest = gluNewQuadric();
	gluQuadricDrawStyle(lChest, GLU_FILL);
	lShoulder = gluNewQuadric();
	gluQuadricDrawStyle(lShoulder, GLU_FILL);
	lArm = gluNewQuadric();
	gluQuadricDrawStyle(lArm, GLU_FILL);
	lLeg = gluNewQuadric();
	gluQuadricDrawStyle(lLeg, GLU_FILL);
	lFeet = gluNewQuadric();
	gluQuadricDrawStyle(lFeet, GLU_FILL);
	pine = gluNewQuadric();
	gluQuadricDrawStyle(pine, GLU_FILL);
	snow = gluNewQuadric();
	gluQuadricDrawStyle(snow, GLU_FILL);

    glLightfv(GL_LIGHT1, GL_POSITION, horizonP);
    glLightfv(GL_LIGHT1, GL_AMBIENT, horizonA);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, horizonD);
    glLightfv(GL_LIGHT1, GL_SPECULAR, horizonS);

    glLightfv(GL_LIGHT2, GL_POSITION, verticP);
    glLightfv(GL_LIGHT2, GL_AMBIENT, verticA);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, verticD);
    glLightfv(GL_LIGHT2, GL_SPECULAR, verticS);

	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
	
    dragonTex = LoadTextureRAW("orange.raw", TRUE);
    if (!dragonTex) printf("Texture File not found !\n");

}

// Buat kepala dari ini semua
void DrawKepala()
{
    glPushMatrix();
	glTranslatef(0, -3.7, 1);
	glTranslatef(0, 3.5, 3);
	glRotatef(-30, 1.0, 0.0, 0.0);
	glRotatef(-30, 0.0, 0.0, -1.0);
	glRotatef(-130, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, 0.4, MAX_WING, 7.0, 10, 10);
	glRotatef(130, 1.0, 0.0, 0.0);
	glRotatef(30, 0.0, 0.0, -1.0);   
	glRotatef(-30, 0.0, 0.0, 1.0);
	glRotatef(-130, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, 0.4, MAX_WING, 7.0, 10, 10);
	glRotatef(130, 1.0, 0.0, 0.0);
	glRotatef(30, 0.0, 0.0, 1.0);
	glRotatef(30, 1.0, 0.0, 0.0);
	glPushMatrix();
	for(int ii = 0; ii < 3; ++ii) {
		glRotatef(-10, 1.0, 0.0, 0.0);
		glTranslatef(0, 0.5, -1);
		glRotatef(-30, 1.0, 0.0, 0.0);
		glRotatef(-30, 0.0, 0.0, -1.0);
		glRotatef(-130, 1.0, 0.0, 0.0);
		SOLID_CLOSED_CYLINDER(neck, 0.4, MAX_WING, 5.0, 10, 10);
		glRotatef(130, 1.0, 0.0, 0.0);
		glRotatef(30, 0.0, 0.0, -1.0);   
		glRotatef(-30, 0.0, 0.0, 1.0);
		glRotatef(-130, 1.0, 0.0, 0.0);
		SOLID_CLOSED_CYLINDER(neck, 0.4, MAX_WING, 5.0, 10, 10);
		glRotatef(130, 1.0, 0.0, 0.0);
		glRotatef(30, 0.0, 0.0, 1.0);
		glRotatef(30, 1.0, 0.0, 0.0);
	}
	glTranslatef(0, -2, 4);
	glPopMatrix();
	glTranslatef(0, -2.5, -2);
	glTranslatef(0, -dragV*2/2, -dragV*1/2);
	glRotatef(dragV*40/2, 1.0, 0.0, 0.0);
	//low
	glRotatef(-30, 0.0, 0.0, -1.0);
	glRotatef(-130, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, 0.3, 0.5, 5.0, 10, 10);
	glRotatef(130, 1.0, 0.0, 0.0);
	glRotatef(30, 0.0, 0.0, -1.0);   
	glRotatef(-30, 0.0, 0.0, 1.0);
	glRotatef(-130, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, 0.3, 0.5, 5.0, 10, 10);
	glRotatef(130, 1.0, 0.0, 0.0);
	glRotatef(30, 0.0, 0.0, 1.0);
	glPushMatrix();
	for(int ii = 0; ii < 3; ++ii) {
		glRotatef(-5, 1.0, 0.0, 0.0);
		glTranslatef(0, 0.5, -0.5);
		glRotatef(-30, 0.0, 0.0, -1.0);
		glRotatef(-130, 1.0, 0.0, 0.0);
		SOLID_CLOSED_CYLINDER(neck, 0.3, 0.5, 4.0, 10, 10);
		glRotatef(130, 1.0, 0.0, 0.0);
		glRotatef(30, 0.0, 0.0, -1.0);   
		glRotatef(-30, 0.0, 0.0, 1.0);
		glRotatef(-130, 1.0, 0.0, 0.0);
		SOLID_CLOSED_CYLINDER(neck, 0.3, 0.5, 4.0, 10, 10);
		glRotatef(130, 1.0, 0.0, 0.0);
		glRotatef(30, 0.0, 0.0, 1.0);
	}
	glPopMatrix();
    glPopMatrix(); 
}

GLuint windowWidth;
GLuint windowHeight;

// Reshape Function
void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h;
	const float aspect = (float) w / (float) h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(FOVY, aspect, ZNEAR, ZFAR);
    glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void DrawLArm() {
	glPushMatrix();
	glTranslatef(-0.2, 0.7 - BICEPT_HEIGHT, BICEPT_HEIGHT - 2.7);
	glRotatef(jointDegree[4]+dragV*60/2+30, 1.0, 0.0, 0.0);
	glRotatef(0, 0.0, 1.0, 0.0);
	SOLID_CLOSED_CYLINDER(lArm, ARM_LOWER, ARM_UPPER, ARM_HEIGHT, 10, 10);
	glRotatef(0, 0.0, 1.0, 0.0);
	glTranslatef(0.5, 0, 5);
	glRotatef(-50, 1.0, 0.0, 0.0);
	for(int ii = 0; ii < 3; ++ii) {
		glRotatef(-130, 1.0, 0.0, 0.0);
		SOLID_CLOSED_CYLINDER(neck, 0.01, MAX_WING, 3.0, 10, 10);
		glRotatef(130, 1.0, 0.0, 0.0);
		glTranslatef(-0.5, 0, .0);
	}
	glPopMatrix();
}

void DrawLShoulder() {
	glPushMatrix();
	glTranslatef(CHEST_HEIGHT, 0.5, 0.0);
	glRotatef(jointDegree[2], 1.0, 0.0, 0.0);
	glRotatef(-dragV*30/2-30, 0.0, 0.0, -1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(-30, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(lShoulder, SHOULDER_LOWER, SHOULDER_UPPER, SHOULDER_HEIGHT, 10, 10);
	glRotatef(30, 1.0, 0.0, 0.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	
	glTranslatef(SHOULDER_HEIGHT/7, 0.0, 0.0);
	glRotatef(60, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(lShoulder, BICEPT_UPPER, BICEPT_LOWER, BICEPT_HEIGHT, 10, 10);
	glRotatef(-60, 1.0, 0.0, 0.0);
	DrawLArm();	
	glPopMatrix();
}

void DrawRArm() {
	glPushMatrix();
	glTranslatef(0.2, 0.7 - BICEPT_HEIGHT, BICEPT_HEIGHT - 2.7);
	glRotatef(jointDegree[3]+dragV*60/2+30, 1.0, 0.0, 0.0);
	glRotatef(0, 0.0, 1.0, 0.0);
	SOLID_CLOSED_CYLINDER(rArm, ARM_LOWER, ARM_UPPER, ARM_HEIGHT, 10, 10);
	glRotatef(0, 0.0, 1.0, 0.0);
	glTranslatef(0.5, 0, 5);
	glRotatef(-50, 1.0, 0.0, 0.0);
	for(int ii = 0; ii < 3; ++ii) {
		glRotatef(-130, 1.0, 0.0, 0.0);
		SOLID_CLOSED_CYLINDER(neck, 0.01, MAX_WING, 3.0, 10, 10);
		glRotatef(130, 1.0, 0.0, 0.0);
		glTranslatef(-0.5, 0, .0);
	}
	glPopMatrix();
}

void DrawRShoulder() {
	glPushMatrix();
	glTranslatef(-CHEST_HEIGHT, 0.5, 0.0);
	glRotatef(jointDegree[1], 1.0, 0.0, 0.0);
	glRotatef(-dragV*30/2-30, 0.0, 0.0, 1.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(-30, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(rShoulder, SHOULDER_LOWER, SHOULDER_UPPER, SHOULDER_HEIGHT, 10, 10);
	glRotatef(30, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	
	glTranslatef(-SHOULDER_HEIGHT/7, 0.0, 0.0);
	glRotatef(60, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(rShoulder, BICEPT_UPPER, BICEPT_LOWER, BICEPT_HEIGHT, 10, 10);
	glRotatef(-60, 1.0, 0.0, 0.0);
	DrawRArm();	
	glPopMatrix();
}

void DrawNeck() {
	glPushMatrix();
	glTranslatef(0.0, 0.6, 0.8);
	glRotatef(-25, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, NECK_LOWER, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(25, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.4, 0.9);
	glRotatef(-10, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, NECK_LOWER, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(10, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.2, 0.9);
	glRotatef(0, 1.0, 0.0, 0.0);
	glRotatef(0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 0.9);
	glRotatef(0, 1.0, 0.0, 0.0);
	glRotatef(0, 1.0, 0.0, 0.0);
    DrawKepala();
	glPopMatrix();
}

void DrawLFeet() {
	glTranslatef(2.8, -1.2, 2.3);	
	glRotatef(jointDegree[8], 1.0, 0.0, 0.0);
	glPushMatrix();
	glRotatef(140.0, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(lLeg, CALF_LOWER, CALF_UPPER, CALF_HEIGHT, 10, 10);
	glRotatef(-140.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -3.3, -2.7);
	glRotatef(30.0, 1.0, 0.0, 0.0);
	glScalef(FEET_WIDTH, FEET_HEIGHT, FEET_LENGTH);
	glScalef(1.0/FEET_WIDTH, 1.0/FEET_HEIGHT, 1.0/FEET_LENGTH);
	glRotatef(30.0, -1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 1.0);
	glTranslatef(0.5, -1.85, 0.8);
	for(int ii = 0; ii < 3; ++ii) {
		glRotatef(-130, 1.0, 0.0, 0.0);
		SOLID_CLOSED_CYLINDER(neck, 0.01, MAX_WING, 3.0, 10, 10);
		glRotatef(130, 1.0, 0.0, 0.0);
		glTranslatef(-0.5, 0, .0);
	}
	glPopMatrix();
}

void DrawRFeet() {
	glTranslatef(-2.8, -1.2, 2.3);	
	glRotatef(jointDegree[7], 1.0, 0.0, 0.0);
	glPushMatrix();
	glRotatef(140.0, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(lLeg, CALF_LOWER, CALF_UPPER, CALF_HEIGHT, 10, 10);
	glRotatef(-140.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -3.3, -2.7);
	glRotatef(30.0, 1.0, 0.0, 0.0);
	glScalef(FEET_WIDTH, FEET_HEIGHT, FEET_LENGTH);
	glScalef(1.0/FEET_WIDTH, 1.0/FEET_HEIGHT, 1.0/FEET_LENGTH);
	glRotatef(30.0, -1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 1.0);
	glTranslatef(0.5, -1.85, 0.8);
	for(int ii = 0; ii < 3; ++ii) {
		glRotatef(-130, 1.0, 0.0, 0.0);
		SOLID_CLOSED_CYLINDER(neck, 0.01, MAX_WING, 3.0, 10, 10);
		glRotatef(130, 1.0, 0.0, 0.0);
		glTranslatef(-0.5, 0, .0);
	}	
	glPopMatrix();
}

void DrawLLeg() {
	glPushMatrix();
	glTranslatef(-0.5, -0.1, 0.0);
	glRotatef(-40, 1.0, 1.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(30, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(lLeg, LEG_LOWER, LEG_UPPER, LEG_HEIGHT, 10, 10);
	glRotatef(-30, 1.0, 0.0, 0.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(40, 1.0, 1.0, 0.0);
	DrawLFeet();
	glPopMatrix();
}

void DrawRLeg() {
	glPushMatrix();
	glTranslatef(0.5, -0.1, 0.0);
	glRotatef(-40, 1.0, -1.0, 0.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(30, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(rLeg, LEG_LOWER, LEG_UPPER, LEG_HEIGHT, 10, 10);
	glRotatef(-30, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(40, 1.0, -1.0, 0.0);
	DrawRFeet();
	glPopMatrix();
}

void DrawTail() {
	//tail
	glPushMatrix();
	glTranslatef(0, 2, 1.5);
	glRotatef(200, 1.0, 0.0, 0.0);
	glTranslatef(0.0, STOMACH_HEIGHT - 1.4, 0);
	glRotatef(-70, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, NECK_LOWER, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(70, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.5, 0.5);
	glRotatef(-55, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, NECK_LOWER-0.1, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(55, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.6, 0.6);
	glRotatef(-40, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, NECK_LOWER-0.2, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(40, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.6, 0.8);
	glRotatef(-25, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, NECK_LOWER-0.3, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(25, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.4, 0.9);
	glRotatef(-10, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, NECK_LOWER-0.4, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(10, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.2, 0.9);
	glRotatef(0, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, NECK_LOWER-0.5, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 0.9);	
	glRotatef(10, 1.0, 0.0, 0.0);
	glRotatef(0, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, NECK_LOWER-0.6, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 0.9);
	SOLID_CLOSED_CYLINDER(neck, NECK_UPPER, 0.01, NECK_HEIGHT*4, 10, 10);
	glPopMatrix();
}

void DrawLeftWing() {
	glPushMatrix();
	glRotatef(dragV*40/2+30, 0.0, -1.0, 0.0);
	glTranslatef(1.0, STOMACH_HEIGHT, 0);
	glRotatef(-30, 0.0, 0.0, 1.0);
	glRotatef(-130, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, 0.4, MAX_WING, 5.0, 10, 10);
	glRotatef(130, 1.0, 0.0, 0.0);
	glRotatef(30, 0.0, 0.0, 1.0);
	glTranslatef(1.7, 3, -2.7);
	glRotatef(40, 0.0, 0.0, 1.0);
	glRotatef(110, 0.0, 1.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, 0.4, MAX_WING, 4.0, 10, 10);
	glRotatef(-110, 0.0, 1.0, 0.0);
	glRotatef(-40, 0.0, 0.0, 1.0);
	glTranslatef(2.7, 2.2, -1.2);
	//1
	glRotatef(-60, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, 0.3, MAX_WING, 4.0, 10, 10);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(60, 0.0, 0.0, 1.0);
	glTranslatef(1.9, -2.8, -0.2);
	glRotatef(80, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, MAX_WING, 0.01, 4.0, 10, 10);
	glRotatef(-80, 1.0, 0.0, 0.0);
	glTranslatef(-1.9, 2.8, 0.2);
	//end
	glRotatef(50, 0.0, 0.0, 1.0);
	glRotatef(60, 0.0, 1.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, 0.4, MAX_WING, 4.0, 10, 10);
	glRotatef(-60, 0.0, 1.0, 0.0);
	glTranslatef(3.2, 0.5, 1.7);
	//2
	glRotatef(-30, 0.0, 0.0, 1.0);
	glRotatef(-60, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, 0.3, MAX_WING, 7.0, 10, 10);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(60, 0.0, 0.0, 1.0);
	glTranslatef(3.5, -5, -0.2);
	glRotatef(-20, 0.0, 0.0, 1.0);
	glRotatef(80, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, MAX_WING, 0.01, 8.0, 10, 10);
	glRotatef(-80, 1.0, 0.0, 0.0);
	glRotatef(20, 0.0, 0.0, 1.0);
	glTranslatef(-3.5, 5, 0.2);
	glRotatef(30, 0.0, 0.0, 1.0);
	glRotatef(-50, 0.0, 0.0, 1.0);
	//end
	glTranslatef(0.5, -0.5, 0);
	glRotatef(60, 0.0, 0.0, 1.0);
	glTranslatef(0.5, 0, 0);
	glRotatef(30, 0.0, 1.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, 0.4, MAX_WING, 5.0, 10, 10);
	glRotatef(-30, 0.0, 1.0, 0.0);
	glTranslatef(2.4, 0.5, 4.3);
	//3
	glRotatef(-60, 0.0, 0.0, 1.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, 0.4, 0.8, 11.0, 10, 10);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(60, 0.0, 0.0, 1.0);
	glTranslatef(5.5, -9, -0.2);
	glRotatef(-50, 0.0, 0.0, 1.0);
	glRotatef(80, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, 0.8, 0.01, 13.0, 10, 10);
	glRotatef(-80, 1.0, 0.0, 0.0);
	glRotatef(50, 0.0, 0.0, 1.0);
	glTranslatef(-3.5, 5, 0.2);
	glRotatef(60, 0.0, 0.0, 1.0);
	//end
	glPopMatrix();
}

void DrawRightWing() {
	glPushMatrix();
	glRotatef(dragV*40/2+30, 0.0, 1.0, 0.0);
	glTranslatef(-1.0, STOMACH_HEIGHT, 0);
	glRotatef(-30, 0.0, 0.0, -1.0);
	glRotatef(-130, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, 0.4, MAX_WING, 5.0, 10, 10);
	glRotatef(130, 1.0, 0.0, 0.0);
	glRotatef(30, 0.0, 0.0, -1.0);
	glTranslatef(-1.7, 3, -2.7);
	glRotatef(40, 0.0, 0.0, -1.0);
	glRotatef(110, 0.0, -1.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, 0.4, MAX_WING, 4.0, 10, 10);
	glRotatef(-110, 0.0, -1.0, 0.0);
	glRotatef(-40, 0.0, 0.0, -1.0);
	glTranslatef(-2.7, 2.2, -1.2);
	//1
	glRotatef(-60, 0.0, 0.0, -1.0);
	glRotatef(90, 0.0, -1.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, 0.3, MAX_WING, 4.0, 10, 10);
	glRotatef(-90, 0.0, -1.0, 0.0);
	glRotatef(60, 0.0, 0.0, -1.0);
	glTranslatef(-1.9, -2.8, -0.2);
	glRotatef(80, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, MAX_WING, 0.01, 4.0, 10, 10);
	glRotatef(-80, 1.0, 0.0, 0.0);
	glTranslatef(1.9, 2.8, 0.2);
	//end
	glRotatef(50, 0.0, 0.0, -1.0);
	glRotatef(60, 0.0, -1.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, 0.4, MAX_WING, 4.0, 10, 10);
	glRotatef(-60, 0.0, -1.0, 0.0);
	glTranslatef(-3.2, 0.5, 1.7);
	//2
	glRotatef(-30, 0.0, 0.0, -1.0);
	glRotatef(-60, 0.0, 0.0, -1.0);
	glRotatef(90, 0.0, -1.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, 0.3, MAX_WING, 7.0, 10, 10);
	glRotatef(-90, 0.0, -1.0, 0.0);
	glRotatef(60, 0.0, 0.0, -1.0);
	glTranslatef(-3.5, -5, -0.2);
	glRotatef(-20, 0.0, 0.0, -1.0);
	glRotatef(80, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, MAX_WING, 0.01, 8.0, 10, 10);
	glRotatef(-80, 1.0, 0.0, 0.0);
	glRotatef(20, 0.0, 0.0, -1.0);
	glTranslatef(3.5, 5, 0.2);
	glRotatef(30, 0.0, 0.0, -1.0);
	glRotatef(-50, 0.0, 0.0, -1.0);
	//end
	glTranslatef(-0.5, -0.5, 0);
	glRotatef(60, 0.0, 0.0, -1.0);
	glTranslatef(-0.5, 0, 0);
	glRotatef(30, 0.0, -1.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, 0.4, MAX_WING, 5.0, 10, 10);
	glRotatef(-30, 0.0, -1.0, 0.0);
	glTranslatef(-2.4, 0.5, 4.3);
	//3
	glRotatef(-60, 0.0, 0.0, -1.0);
	glRotatef(90, 0.0, -1.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, 0.4, 0.8, 11.0, 10, 10);
	glRotatef(-90, 0.0, -1.0, 0.0);
	glRotatef(60, 0.0, 0.0, -1.0);
	glTranslatef(-5.5, -9, -0.2);
	glRotatef(-50, 0.0, 0.0, -1.0);
	glRotatef(80, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, 0.8, 0.01, 13.0, 10, 10);
	glRotatef(-80, 1.0, 0.0, 0.0);
	glRotatef(50, 0.0, 0.0, -1.0);
	glTranslatef(3.5, 5, 0.2);
	glRotatef(60, 0.0, 0.0, -1.0);
	//end
	glPopMatrix();
}

void DrawBody() {
	glPushMatrix();

	glTranslatef(0.0, -5.8 -(4), -15.0);
	
	//glRotatef(rot, 0.0, 1.0, 0.0);
	glRotatef(65, 0.0, 1.0, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(body, STOMACH_LOWER, STOMACH_UPPER, STOMACH_HEIGHT, 10, 10);	
	glRotatef(90, 1.0, 0.0, 0.0);

	glTranslatef(0.0, STOMACH_HEIGHT, 0);

	glRotatef(90, 0.0, 1.0, 0.0);
	SOLID_CLOSED_CYLINDER(lChest, CHEST_LOWER, CHEST_UPPER, CHEST_HEIGHT, 10, 10);
	glRotatef(-90, 0.0, 1.0, 0.0);
	DrawLShoulder();

	glRotatef(-90, 0.0, 1.0, 0.0);
	SOLID_CLOSED_CYLINDER(rChest, CHEST_LOWER, CHEST_UPPER, CHEST_HEIGHT, 10, 10);
	glRotatef(90, 0.0, 1.0, 0.0);	
	DrawRShoulder();

	glPushMatrix();
	glRotatef(jointDegree[0]+dragV*40/2, 1.0, 0.0, 0.0);
    // Ubah ini untuk mengubah normalnya leher
	glTranslatef(0.0, STOMACH_HEIGHT - 2.4, 0.0); 
	glRotatef(-70, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, NECK_LOWER, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(70, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.5, 0.5);
	glRotatef(-55, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, NECK_LOWER, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(55, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.6, 0.6);
	glRotatef(-40, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(neck, NECK_LOWER, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(40, 1.0, 0.0, 0.0);
	DrawNeck();
	glPopMatrix();
	glTranslatef(0.0, -STOMACH_HEIGHT-0.5, 0.5);
	glPushMatrix();
	glRotatef(-90, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(body, STOMACH_UPPER, STOMACH_LOWER, STOMACH_HEIGHT/4, 10, 10);
	glRotatef(90, 1.0, 0.0, 0.0);
	glPopMatrix();
	
	DrawTail();
	DrawLeftWing();
	DrawRightWing();

	glTranslatef(0.0, 0.0, 0.0);
	
	glRotatef(90, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(body, ULEG_LOWER, ULEG_UPPER, ULEG_HEIGHT, 10, 10);
	glRotatef(-90, 1.0, 0.0, 0.0);
	
	glPushMatrix();
	glRotatef(jointDegree[6]-dragV*80/2-40, 1.0, 0.0, 0.0);
	glTranslatef(1.2, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(-30, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(body, LLEG_LOWER, LLEG_UPPER, LLEG_HEIGHT, 10, 10);
	glRotatef(30, 1.0, 0.0, 0.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	DrawLLeg();
	glPopMatrix();
	
	glPushMatrix();
	glRotatef(jointDegree[5]-dragV*80/2-40, 1.0, 0.0, 0.0);
	glTranslatef(-1.2, 0.0, 0.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(-30, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(body, LLEG_LOWER, LLEG_UPPER, LLEG_HEIGHT, 10, 10);
	glRotatef(30, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	DrawRLeg();
	glPopMatrix();

	
	glPopMatrix();
}

void displayDragonObject(GLfloat initAngle, GLfloat initX, GLfloat initY, GLfloat initZ) {
	glPushMatrix();
    glTranslatef(initX, initY, initZ);
    switch (currentMaterial)
    {
    case 1:
    	glMaterialfv(GL_FRONT, GL_SPECULAR, silverS);
    	glMaterialfv(GL_FRONT, GL_AMBIENT, silverA);
	    glMaterialfv(GL_FRONT, GL_DIFFUSE, silverD);
	    glMaterialf(GL_FRONT, GL_SHININESS, silverShine);
        break;
    case 2:
    	glMaterialfv(GL_FRONT, GL_SPECULAR, rubyS);
    	glMaterialfv(GL_FRONT, GL_AMBIENT, rubyA);
	    glMaterialfv(GL_FRONT, GL_DIFFUSE, rubyD);
	    glMaterialf(GL_FRONT, GL_SHININESS, rubyShine);
        break;
    case 3:
    	glMaterialfv(GL_FRONT, GL_SPECULAR, goldS);
    	glMaterialfv(GL_FRONT, GL_AMBIENT, goldA);
	    glMaterialfv(GL_FRONT, GL_DIFFUSE, goldD);
	    glMaterialf(GL_FRONT, GL_SHININESS, goldShine);
        break;
    }
	
    if (showTexture) {
    	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteS);
    	glMaterialfv(GL_FRONT, GL_AMBIENT, whiteA);
	    glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteD);
	    glMaterialf(GL_FRONT, GL_SHININESS, whiteShine);
		glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_GEN_S); // Enable Texture Coord Generation For S ( NEW )
        glEnable(GL_TEXTURE_GEN_T); // Enable Texture Coord Generation For T ( NEW )
		glBindTexture(GL_TEXTURE_2D, dragonTex); //binding texture
    }

	glScalef(DRAGON_SCALE, DRAGON_SCALE+0.05, DRAGON_SCALE);
	glRotatef(initAngle, 0.0, 0.0, 1.0);
	DrawBody();
	glRotatef(initAngle, .0, 0.0, -1.0);
	glScalef(1/DRAGON_SCALE, 1/DRAGON_SCALE, 1/DRAGON_SCALE);
	
    if (showTexture) {
        glDisable(GL_TEXTURE_GEN_S); // Disable Texture Coord Generation For S ( NEW )
        glDisable(GL_TEXTURE_GEN_T); // Disable Texture Coord Generation For T ( NEW )
		glDisable(GL_TEXTURE_2D);
    }
	glPopMatrix();
}

void displayPineObject(GLfloat initAngle, GLfloat initX, GLfloat initY, GLfloat initZ) {
	glPushMatrix();
    glTranslatef(initX, initY, initZ);
    switch (currentMaterial)
    {
    case 1:
    	glMaterialfv(GL_FRONT, GL_SPECULAR, silverS);
    	glMaterialfv(GL_FRONT, GL_AMBIENT, silverA);
	    glMaterialfv(GL_FRONT, GL_DIFFUSE, silverD);
	    glMaterialf(GL_FRONT, GL_SHININESS, silverShine);
        break;
    case 2:
    	glMaterialfv(GL_FRONT, GL_SPECULAR, rubyS);
    	glMaterialfv(GL_FRONT, GL_AMBIENT, rubyA);
	    glMaterialfv(GL_FRONT, GL_DIFFUSE, rubyD);
	    glMaterialf(GL_FRONT, GL_SHININESS, rubyShine);
        break;
    case 3:
    	glMaterialfv(GL_FRONT, GL_SPECULAR, goldS);
    	glMaterialfv(GL_FRONT, GL_AMBIENT, goldA);
	    glMaterialfv(GL_FRONT, GL_DIFFUSE, goldD);
	    glMaterialf(GL_FRONT, GL_SHININESS, goldShine);
        break;
    }
	glTranslatef(0.0, -5.8 -(4), -15.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(pine, 2, 2, 7, 10, 10);	
	glRotatef(90, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 4, 0.0);	
	glRotatef(-90, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(pine, 8, 2, 4, 10, 10);	
	glRotatef(90, 1.0, 0.0, 0.0);
	for(int ii = 0; ii < 4; ++ii) {
		glTranslatef(0.0, 3, 0.0);	
		glRotatef(-90, 1.0, 0.0, 0.0);
		SOLID_CLOSED_CYLINDER(pine, 8 - ii, 2, 4, 10, 10);	
		glRotatef(90, 1.0, 0.0, 0.0);
	}
	glTranslatef(0.0, 3, 0.0);	
	glRotatef(-90, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(pine, 4, 0, 4, 10, 10);	
	glRotatef(90, 1.0, 0.0, 0.0);
	glPopMatrix();
}

void displaySnowObject(GLfloat initAngle, GLfloat initX, GLfloat initY, GLfloat initZ) {
	glPushMatrix();
    glTranslatef(initX, initY, initZ);
    switch (currentMaterial)
    {
    case 1:
    	glMaterialfv(GL_FRONT, GL_SPECULAR, silverS);
    	glMaterialfv(GL_FRONT, GL_AMBIENT, silverA);
	    glMaterialfv(GL_FRONT, GL_DIFFUSE, silverD);
	    glMaterialf(GL_FRONT, GL_SHININESS, silverShine);
        break;
    case 2:
    	glMaterialfv(GL_FRONT, GL_SPECULAR, rubyS);
    	glMaterialfv(GL_FRONT, GL_AMBIENT, rubyA);
	    glMaterialfv(GL_FRONT, GL_DIFFUSE, rubyD);
	    glMaterialf(GL_FRONT, GL_SHININESS, rubyShine);
        break;
    case 3:
    	glMaterialfv(GL_FRONT, GL_SPECULAR, goldS);
    	glMaterialfv(GL_FRONT, GL_AMBIENT, goldA);
	    glMaterialfv(GL_FRONT, GL_DIFFUSE, goldD);
	    glMaterialf(GL_FRONT, GL_SHININESS, goldShine);
        break;
    }
    	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteS);
    	glMaterialfv(GL_FRONT, GL_AMBIENT, whiteA);
	    glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteD);
	    glMaterialf(GL_FRONT, GL_SHININESS, whiteShine);

	glTranslatef(0.0, -5.8 -(4), -15.0);
	glScalef(SNOW_SCALE, SNOW_SCALE, SNOW_SCALE);
	
	glRotatef(-30, 0.0, 1.0, .0);
	gluSphere(snow, 5, 10, 10);	
	glTranslatef(1.0, 5, .0);
	gluSphere(snow, 4, 10, 10);	
	//kanan
	glPushMatrix();
	glTranslatef(2.0, 0, .0);
	glRotatef(100, 0.0, 0.0, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(snow, 0.5, 0.5, 7, 10, 10);	
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(-100, 0.0, 0.0, 1.0);
	glTranslatef(4.7, 0.4, .0);
	glRotatef(160, 0.0, 0.0, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(snow, 0.3, 0.3, 3, 10, 10);	
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(-160, 0.0, 0.0, 1.0);
	glPopMatrix();
	//kiri
	glPushMatrix();
	glTranslatef(-2.0, 0, .0);
	glRotatef(130, 0.0, 0.0, -1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(snow, 0.5, 0.5, 6, 10, 10);	
	glRotatef(-90, 1.0, 0.0, 0.0);
	glRotatef(-130, 0.0, 0.0, -1.0);
	glPopMatrix();
	//
	glTranslatef(-.2, 3, 0);
	gluSphere(snow, 3, 10, 10);	
	glTranslatef(-0.2, 2.7, -.5);
	glRotatef(10, 0.0, 0.0, 1.0);
	glRotatef(80, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(snow, 5, 5, 0.2, 10, 10);	
	glRotatef(-80, 1.0, 0.0, 0.0);
	glRotatef(-10, 0.0, 0.0, 1.0);
	glTranslatef(-.6, 3.8, -.5);
	glRotatef(10, 0.0, 0.0, 1.0);
	glRotatef(80, 1.0, 0.0, 0.0);
	SOLID_CLOSED_CYLINDER(snow, 3, 3, 4, 10, 10);	
	glRotatef(-80, 1.0, 0.0, 0.0);
	glRotatef(-10, 0.0, 0.0, 1.0);
	glTranslatef(0, -6, 2);
	SOLID_CLOSED_CYLINDER(snow, 1, 0, 5, 10, 10);	
	glScalef(1/SNOW_SCALE, 1/SNOW_SCALE, 1/SNOW_SCALE);
	glPopMatrix();
}

void DrawScene(void) {
    // Menggambar objek utama
    glEnable(GL_LIGHTING);
	    glPushMatrix();
	    displayDragonObject(-10+dragV*20/2, -8.0-dragV*5/2, 8.0+dragV, -2.0);
		displayPineObject(0.0, 15.0, 1.5, -11.0);
		displaySnowObject(0.0, 5, 5.5, -4.0);
	    glPopMatrix();

        // Gambar dinding ~
        glPushMatrix();
        glTranslatef(0.f, -7.f, -10.f);
        glColor3f(.7f, .2f, .4f);
	    glScalef(1.0f, 0.05f, 1.0f);
	    glutSolidCube(30.f);
        glPopMatrix(); 

        // Gambar kaktus yey
        glEnable(GL_TEXTURE_2D);
        glPushMatrix();
            glTranslatef(-10.f, -5.f, -10.f);
            glScalef(2.f,2.f,2.f);
            glCallList(kaktus_dp);

            glLoadIdentity();
            glTranslatef(10.f, -5.f, -10.f);
            glScalef(2.f,2.f,2.f);
            glCallList(kaktus_dp);

            glLoadIdentity();
            glTranslatef(0.f, -5.f, -10.f);
            glScalef(2.f,2.f,2.f);
            glCallList(kaktus_dp);

            glLoadIdentity();
            gambarNemo(0.f);

        glPopMatrix();

        // Zudomon!
        glPushMatrix();
            glLoadIdentity();
            glTranslatef(5.f, 0.f, 0);
            drawZudomon();
        glPopMatrix();
	glDisable(GL_LIGHTING);

}

/* menginisialisi kemunculan texture */
void showTextures() {
    // to handle the textures generation and coordinates
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR); // Set The Texture Generation Mode
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR); // Set The Texture Generation Mode
    glTexGenfv(GL_S, GL_OBJECT_PLANE, coef);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, coef);

    glEnable(GL_TEXTURE_2D); //enable texture
}

// Display Function
void display(void) {
    MATRIX4X4 lightProjectionMatrix1, lightViewMatrix1;
    MATRIX4X4 lightProjectionMatrix2, lightViewMatrix2;
    VECTOR3D lightPosition;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
	
	// Lampu horizontal
    glPushMatrix(); 
    if (isHorizon) {
        glEnable(GL_LIGHT1);
        glColor3f(0.2, 0.2, 0.85);
    } else {
        glDisable(GL_LIGHT1);
        glColor3f(0.0, 0.0, 0.0);
    }

    glColor3f(0.85, 0.2, 0.2);
    glTranslatef(0.0, -1.0, -15.0);
    glRotatef(lightDegree, 0.0, 1.0, 0.0);     
	    
    glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix1);
    glTranslatef(10.0, 4.0, -15.0);
        if (isHorizon) glLightfv(GL_LIGHT1, GL_POSITION, horizonP);
    glutSolidSphere(0.3, 10, 10);
    glPopMatrix();

	// Lampu vertikal
    glPushMatrix(); 
    if (isVertic) {
        glEnable(GL_LIGHT2);
        glColor3f(0.2, 0.2, 0.85);
    } else {
        glDisable(GL_LIGHT2);
        glColor3f(0.0, 0.0, 0.0);
    }

    {
        glColor3f(0.85, 0.2, 0.2);
        glTranslatef(0.0, -1.0, -15.0);
        glRotatef(lightDegree, 0.0, 1.0, 0.0);     
	    if (isVertic) glLightfv(GL_LIGHT1, GL_POSITION, horizonP);
        glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix1);
        glTranslatef(10.0, 4.0, -15.0);
        glutSolidSphere(0.3, 10, 10);
    }
    glPopMatrix();

    DrawScene();
    
    // pengaturan texture
    if (showTexture) {
        showTextures();
    } else {
        glDisable(GL_TEXTURE_2D);
    }
	glFlush();
	glutSwapBuffers();
}

// Fungsi animasi yang berjalan saat idle
void animation() {
	if(isSpin)rot += rotInc;
	if (isPlay) {

	}
	
	lightDegree += lightInc;
    if (lightDegree >= 360) lightDegree -= 360;
    if (rot >= 360) rot = 0;

	glutPostRedisplay();
}

// Fungsi menu
void menu(int id) {

}

// Fungsi menu pengaturan animasi
void animateMenu(int id) {
	lockedPart = id;
}

// Fungsi menu pengaturan lampu horizontal
void horizonMenu(int id) {
    isHorizon = id == 0;
}

// Fungsi pengaturan material
void materialMenu(int id) 
{
	if(id == -1) {
		showTexture = true;
	} else {
		currentMaterial = id + 1;
		showTexture = false;
	}
}

// Fungsi menu pengaturan lampu vertical
void verticMenu(int id) {
    isVertic = id == 0;
}

// Fungsi menu pengaturan perputaran model
void spinMenu(int id) {
    isSpin = id == 0;
}

// Fungsi yang mengatur gerak joint
void mouseClick(int btn, int state, int x, int y) {
	if (btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
		if(jointDegree[lockedPart] + jointInc >= 360) {
			jointDegree[lockedPart] = 0;
		} else {
			jointDegree[lockedPart] += jointInc;
		}
	} else if (btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN) {
		if(jointDegree[lockedPart] - jointInc <= 0) {
			jointDegree[lockedPart] = 360;
		} else {
			jointDegree[lockedPart] -= jointInc;
		}
	}
}



void frameAnimation(int value) {
	if(isAnimate) {
		dragV += dragVInc;
		if(dragVInc < 0 && dragV < -1.5) {
			dragVInc = 0.1;
		} else if(dragVInc == 0.1 && dragV > -0.75) {
			dragVInc = 0.05;
		} else if(dragVInc > 0 && dragV > 0) {
			dragVInc = -0.005;
		} 
	}
	glutTimerFunc(FPS, frameAnimation, 0); 	
}

// Main Function
int main(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 600);
    glutCreateWindow("WS2_Fajar Setyo Nugroho_1006671425_Muhammad Mufid Afif_1006671766");

	init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
	glutMouseFunc(mouseClick);
	glutIdleFunc(animation);
	
    initDisplayList();

    int animateInt, horizonInt, verticInt, materialInt, spinInt;
	animateInt = glutCreateMenu(animateMenu);
    glutAddMenuEntry("neck", 0);
	glutAddMenuEntry("right shoulder", 1);
	glutAddMenuEntry("left shoulder", 2);
	glutAddMenuEntry("right arm", 3);
	glutAddMenuEntry("left arm", 4);
	glutAddMenuEntry("right leg", 5);
	glutAddMenuEntry("left leg", 6);
	glutAddMenuEntry("right feet", 7);
	glutAddMenuEntry("left feet", 8);
	horizonInt = glutCreateMenu(horizonMenu);
    glutAddMenuEntry("on", 0);
    glutAddMenuEntry("off", 1);
	verticInt = glutCreateMenu(verticMenu);
    glutAddMenuEntry("on", 0);
    glutAddMenuEntry("off", 1);
	spinInt = glutCreateMenu(spinMenu);
    glutAddMenuEntry("on", 0);
    glutAddMenuEntry("off", 1);
    materialInt = glutCreateMenu(materialMenu);
    glutAddMenuEntry("Texture", -1);
    glutAddMenuEntry("Standard", 0);
    glutAddMenuEntry("Greymon", 1);
    glutAddMenuEntry("Black Dragon", 2);
    

	glutCreateMenu(menu);
    glutAddSubMenu("Animate?", animateInt);
	glutAddSubMenu("Horizontal Light?", horizonInt);
	glutAddSubMenu("Vertical Light?", verticInt);
	glutAddSubMenu("Spin?", spinInt);
    glutAddSubMenu("Material?", materialInt);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
	glutTimerFunc(FPS, frameAnimation, 0); 	
    glutMainLoop();

    return 0;
}
