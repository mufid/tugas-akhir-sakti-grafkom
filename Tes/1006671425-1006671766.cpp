
#define GLUT_DISABLE_ATEXIT_HACK
#undef WIN32_LEAN_AND_MEAN 
#define _CRT_SECURE_NO_DEPRECATE

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
#include "camera.h"
using namespace std;

// Variables

// Perspective Constant
CCamera Camera;
#define FOVY (double) 80.0
#define ZNEAR (double) 0.6
#define ZFAR (double) 98.0
#define DRAGON_SCALE (double) 0.6
#define SNOW_SCALE (double) 0.6
#define FPS 10

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

double lightDegree = 0, lightInc = 1; // Derajat Lampu

int lockedPart = 0; // Index joint yang sedang aktif
double jointDegree[9] = {20, 0, 0, 0, 0, 0, 0, 0, 0}; // Derajat tiap joint
double jointInc = 10;

GLfloat rot = 0.0, rotInc = 0.12; // Derajat perputaran model
bool isSpin = true; // Apakah model perlu berputar?

// Apakah sedang dalam wireframe mode atau shading mode. TRUE
// apabila sedang dalam wireframe mode. FALSE jika dalam kondisi
// yang lain
bool wireframe = false;
// Light
bool isLight[] = {true, true, true, true};
// Kamera jadul vs. Baru
bool kamerajadul = true;
// Siapa interaktif yang sedang dipilih? Dragon atau Nemo?
bool dragonInteractive = false;
bool nemoInteractive = false;

GLfloat /* light 1: atas depan*/
lamp1A[] = {0.5, 0.5, 0.5, 1.0},
lamp1D[] = {0.9, 0.8, 0.8, 1.0}, 
lamp1S[] = {0.6, 0.6, 0.6, 1.0},
lamp1P[] ={0.0, 5.0, 5.0, 1.0};

GLfloat /*light 2: kiri atas*/
lamp2A[] = {0.0, 0.0, 0.0, 1.0},
lamp2D[] = {0.9, 0.0, 0.9, 1.0}, // ungu
lamp2S[] = {0.6, 0.6, 0.6, 1.0},
lamp2P[] = {-18.0, 15.0, 0.0, 10.0};

GLfloat /*light 3: kanan atas*/
lamp3A[] = {0.0, 0.0, 0.0, 1.0},
lamp3D[] = {0.0, 0.6, 0.9, 1.0}, // biru muda
lamp3S[] = {0.6, 0.6, 0.6, 1.0},
lamp3P[] = {18.0, 15.0, 0.0, 10.0};

GLfloat /* Horizontal Light */
horizonA[] = {0.3, 0.3, 0.3, 1.0},
horizonD[] = {0.8, 0.0, 0.0, 1.0}, 
horizonS[] = {0.6, 0.6, 0.6, 1.0},
horizonP[] = {10.0, 3.0, 0.0, 1.0};

// Camera
bool isMoving = true, isDragon = true, isZudomon = true;

// lamp positions
float lamp4[] = {10.0, 3.0, 0.0};
float lamp[4][3] = {{0.0, 17.0, -10.0}, {-18.0, 17.0, 10.0}, {18.0, 17.0, 10.0}, {10.0, 3.0, 0.0}}; 

/* variabel untuk pembentukan bayangan */
// vektor normal untuk setiap sisi ruangan
// bawah belakang kiri kanan
float vNormal[4][3] = {{0.0, -1.0, 0.0}, {0.0, 0.0, -1.0}, {-1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}};

// titik-titik yang terletak pada masing-masing sisi ruangan
// bawah belakang kiri kanan
float vPoint[4][3] = {{0.0, -9.95, -17.5}, {0.0, 0.0, -29.0}, {-18.0, 0.0, -17.5}, {18.0, 0.0, -17.5}};

// Animation Variables
bool isAnimate = true; // Apakah model beranimasi?
double dragV = 0, dragVInc = -0.005;
int dragStat = 0;

// Texture Variables
bool showTexture = true;
GLuint shadowMapTexture;
GLuint dragonTex;
GLuint snowTex;
GLuint pineTex;
GLuint floorTex;
GLuint wallTex;
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
    width = 600;
    height = 600;
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
	
	glLightfv(GL_LIGHT1, GL_POSITION, lamp1P);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lamp1A);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lamp1D);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lamp1S);
	
	glLightfv(GL_LIGHT2, GL_POSITION, lamp2P);
    glLightfv(GL_LIGHT2, GL_AMBIENT, lamp2A);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lamp2D);
    glLightfv(GL_LIGHT2, GL_SPECULAR, lamp2S);
	
	glLightfv(GL_LIGHT3, GL_POSITION, lamp3P);
    glLightfv(GL_LIGHT3, GL_AMBIENT, lamp3A);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, lamp3D);
    glLightfv(GL_LIGHT3, GL_SPECULAR, lamp3S);

    glLightfv(GL_LIGHT4, GL_POSITION, horizonP);
    glLightfv(GL_LIGHT4, GL_AMBIENT, horizonA);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, horizonD);
    glLightfv(GL_LIGHT4, GL_SPECULAR, horizonS);
	
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_LIGHT4);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
	
    dragonTex = LoadTextureRAW("dark_blue.raw", TRUE);
    if (!dragonTex) printf("Texture File not found !\n");
	
    pineTex = LoadTextureRAW("yellow_dot.raw", TRUE);
    if (!pineTex) printf("Texture File not found !\n");
	
    snowTex = LoadTextureRAW("bloody_violet.raw", TRUE);
    if (!snowTex) printf("Texture File not found !\n");
	
    wallTex = LoadTextureRAW("sunset_wall.raw", TRUE);
    if (!wallTex) printf("Texture File not found !\n");

    floorTex = LoadTextureRAW("green_floor.raw", TRUE);
    if (!floorTex) printf("Texture File not found !\n");

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
    printf("Dirubah ukurannya ke %dx%d\n", w, h);
    gluPerspective(FOVY, aspect, ZNEAR, ZFAR);
    glGetFloatv(GL_MODELVIEW_MATRIX, cameraProjectionMatrix);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

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
	glRotatef(initAngle, 0.0, 1.0, 0.0);
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
	
	glRotatef(initAngle, 0.0, 1.0, 0.0);
	glScalef(DRAGON_SCALE, DRAGON_SCALE+0.05, DRAGON_SCALE);
	glRotatef(-10+dragV*20/2, 0.0, 0.0, 1.0);
	DrawBody();
	glRotatef(-10+dragV*20/2, .0, 0.0, -1.0);
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
	
	
    if (showTexture) {
    	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteS);
    	glMaterialfv(GL_FRONT, GL_AMBIENT, whiteA);
	    glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteD);
	    glMaterialf(GL_FRONT, GL_SHININESS, whiteShine);
		glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_GEN_S); // Enable Texture Coord Generation For S ( NEW )
        glEnable(GL_TEXTURE_GEN_T); // Enable Texture Coord Generation For T ( NEW )
		glBindTexture(GL_TEXTURE_2D, pineTex); //binding texture
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
    if (showTexture) {
        glDisable(GL_TEXTURE_GEN_S); // Disable Texture Coord Generation For S ( NEW )
        glDisable(GL_TEXTURE_GEN_T); // Disable Texture Coord Generation For T ( NEW )
		glDisable(GL_TEXTURE_2D);
    }
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
    	
    if (showTexture) {
    	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteS);
    	glMaterialfv(GL_FRONT, GL_AMBIENT, whiteA);
	    glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteD);
	    glMaterialf(GL_FRONT, GL_SHININESS, whiteShine);
		glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_GEN_S); // Enable Texture Coord Generation For S ( NEW )
        glEnable(GL_TEXTURE_GEN_T); // Enable Texture Coord Generation For T ( NEW )
		glBindTexture(GL_TEXTURE_2D, snowTex); //binding texture
    }

	glTranslatef(0.0, -5.8 -(4), -15.0);
	glRotatef(initAngle, 0.0, 1.0, 0.0);
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
    if (showTexture) {
        glDisable(GL_TEXTURE_GEN_S); // Disable Texture Coord Generation For S ( NEW )
        glDisable(GL_TEXTURE_GEN_T); // Disable Texture Coord Generation For T ( NEW )
		glDisable(GL_TEXTURE_2D);
    }
	glPopMatrix();
}

void DrawScene(bool);

void drawPlane(void) {
     // menggambar lantai
    if (showTexture) {
		glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_GEN_S); // Enable Texture Coord Generation For S ( NEW )
        glEnable(GL_TEXTURE_GEN_T); // Enable Texture Coord Generation For T ( NEW )
		glBindTexture(GL_TEXTURE_2D, floorTex); //binding texture
    }

    // Init untuk wireframe apabila dia wireframe mode
    // Ini akan menjadi dasar kehidupan berbangsa dan bernegara
    // apakah dia ingin menjadikan sebuah tampilan yang hanya
    // garis-garis atau tampilan indah.
    if (wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    

    glColor3f(0.0, 0.5, 0.8);
    glBegin(GL_POLYGON);
    glVertex3f(-18.5, -10.0, -30);
    glVertex3f(-18.5, -10.0, -10);
    glVertex3f(18.5, -10.0, -10);
    glVertex3f(18.5, -10.0, -30);
    glEnd();
	
    if (showTexture) {
        glDisable(GL_TEXTURE_GEN_S); // Disable Texture Coord Generation For S ( NEW )
        glDisable(GL_TEXTURE_GEN_T); // Disable Texture Coord Generation For T ( NEW )
		glDisable(GL_TEXTURE_2D);
    }

    // menggambar dinding kiri
    if (showTexture) {
		glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_GEN_S); // Enable Texture Coord Generation For S ( NEW )
        glEnable(GL_TEXTURE_GEN_T); // Enable Texture Coord Generation For T ( NEW )
		glBindTexture(GL_TEXTURE_2D, wallTex); //binding texture
    }
    glColor3f(0.0, 0.3, 0.5);
    glBegin(GL_POLYGON);
    glVertex3f(-18.5, -10, -30);
    glVertex3f(-18.5, -10, -10);
    glVertex3f(-18.5, 17, -10);
    glVertex3f(-18.5, 17, -30);
    glEnd();
	
	
    if (showTexture) {
        glDisable(GL_TEXTURE_GEN_S); // Disable Texture Coord Generation For S ( NEW )
        glDisable(GL_TEXTURE_GEN_T); // Disable Texture Coord Generation For T ( NEW )
		glDisable(GL_TEXTURE_2D);
    }
    // menggambar dinding kanan
    if (showTexture) {
		glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_GEN_S); // Enable Texture Coord Generation For S ( NEW )
        glEnable(GL_TEXTURE_GEN_T); // Enable Texture Coord Generation For T ( NEW )
		glBindTexture(GL_TEXTURE_2D, wallTex); //binding texture
    }
    glColor3f(0.0, 0.3, 0.5);
    glBegin(GL_POLYGON);
    glVertex3f(18.5, -10, -30);
    glVertex3f(18.5, -10, -10);
    glVertex3f(18.5, 17, -10);
    glVertex3f(18.5, 17, -30);
    glEnd();
	
    if (showTexture) {
        glDisable(GL_TEXTURE_GEN_S); // Disable Texture Coord Generation For S ( NEW )
        glDisable(GL_TEXTURE_GEN_T); // Disable Texture Coord Generation For T ( NEW )
		glDisable(GL_TEXTURE_2D);
    }

    // menggambar dinding belakang
    if (showTexture) {
		glEnable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_GEN_S); // Enable Texture Coord Generation For S ( NEW )
        glEnable(GL_TEXTURE_GEN_T); // Enable Texture Coord Generation For T ( NEW )
		glBindTexture(GL_TEXTURE_2D, wallTex); //binding texture
    }
    glColor3f(0.0, 0.4, 0.6);
    glBegin(GL_POLYGON);
    glVertex3f(-18.5, -10, -30);
    glVertex3f(-18.5, 17, -30);
    glVertex3f(18.5, 17, -30);
    glVertex3f(18.5, -10, -30);
    glEnd();
	
    if (showTexture) {
        glDisable(GL_TEXTURE_GEN_S); // Disable Texture Coord Generation For S ( NEW )
        glDisable(GL_TEXTURE_GEN_T); // Disable Texture Coord Generation For T ( NEW )
		glDisable(GL_TEXTURE_2D);
    }

}

void DrawScene(bool texture) {
    // Menggambar objek utama
	glPushMatrix();
	displayDragonObject(0.0, -8.0-dragV*5/2, 8.0+dragV, -2.0);
	displayPineObject(0.0, 15.0, 1.5, -11.0);
	displaySnowObject(0.0, 5, 5.5, -4.0);
	glPopMatrix();
        
	//Gambar kaktus 1 yey
	glPushMatrix();
	glTranslatef(-7.f, -10.f, -17.f);
	glScalef(1.3f, 1.3f, 1.3f);
	drawKaktus(texture);
    glPopMatrix();

    //Gambar kaktus 2 yey
    glPushMatrix();
	glTranslatef(8.f, -10.f, -17.f);
	glScalef(1.3f,1.3f,1.3f);
	drawKaktus(texture);
    glPopMatrix();

    // Gambar kaktus 3 yey
    glPushMatrix();
	glTranslatef(0.f, -10.f, -17.f);
	glScalef(1.3f,1.3f,1.3f);
	drawKaktus(texture);
    glPopMatrix();

    // Gambar Nemo yey
    glPushMatrix();
	glTranslatef(0.f, -3.f, -17.f);
	glScalef(1.3f,1.3f,1.3f);
	drawNemo(dragV, texture, isAnimate);
    glPopMatrix();

}

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
    VECTOR3D lightPosition;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
	glDisable(GL_LIGHTING);

	if (kamerajadul)
    {
	    glTranslatef(Camera.Position.x, Camera.Position.y, Camera.Position.z);
	    glRotatef(Camera.RotatedX, 1.0, 0, 0);
	    glRotatef(Camera.RotatedY, 0.0, 1.0, 0);
	    glRotatef(Camera.RotatedZ, 0.0, 0, 1.0);
    }
    else
    {
        // Gunakan gluLookAt;
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt( 0, dragV * 3 + 5.f, 0.f,
		           zudomonposx, zudomonposy, zudomonposz-10.f,
		           0, 1, 0);
    }
	glPushMatrix();
    drawPlane();
    glPopMatrix();
    

	// Lampu horizontal
    glPushMatrix(); 
    if (isHorizon) {
        glEnable(GL_LIGHT4);
        glColor3f(0.2, 0.2, 0.85);
    } else {
        glDisable(GL_LIGHT4);
        glColor3f(0.0, 0.0, 0.0);
    }

    glColor3f(0.85, 0.2, 0.2);
    glTranslatef(0.0, -1.0, -15.0);
    glRotatef(lightDegree, 0.0, 1.0, 0.0);     
	    
    glGetFloatv(GL_MODELVIEW_MATRIX, lightProjectionMatrix1);
    glTranslatef(10.0, 4.0, -15.0);
    if (isHorizon) glLightfv(GL_LIGHT4, GL_POSITION, horizonP);
    glutSolidSphere(0.3, 10, 10);
    glPopMatrix();
	
    glEnable(GL_LIGHTING);
    DrawScene(showTexture);
	glDisable(GL_LIGHTING);
	
	glColor3f(0.0, 0.0, 0.0);

	for(int ii = 0; ii < 3; ++ii) {
		if (isLight[ii]) {
			for(int jj = 0; jj < 4; ++jj) {
				glPushMatrix();
				glShadowProjection(lamp[ii], vPoint[jj], vNormal[jj]);
                // Draw scene with no texture
				DrawScene(false);
				glPopMatrix();
			}
		}
	}

    // pengaturan texture
    if (showTexture) {
        showTextures();
    } else {
        glDisable(GL_TEXTURE_2D);
    }
	glLoadIdentity();

	
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

// Fungsi menu pengaturan mode
void modeMenu(int id) {
	if (id == 0) {
		isAnimate = true;
	} else {
		isAnimate = false;
	}
}

// Fungsi menu pengaturan animasi
void animateMenu(int id) {
	lockedPart = id;
    dragonInteractive = true;
    nemoInteractive = false;
}

void animateMenuNemo(int id) {
    lockedPart = id;
    printf("Locked part: %d\n", id);
    nemoInteractive = true;
    dragonInteractive = false;
}

// Fungsi menu pengaturan camera
void camMenu(int id) {
    kamerajadul = true;
	if(id == 1 || id == 2) {
		isMoving = false;
		Camera.Reset();
		if (id == 1) {
			dragV = 0;
			isDragon = true;
			Camera.MoveForward(5.1);
			Camera.MoveUpward(-5.0);
			Camera.StrafeRight(8.0);
			Camera.RotateX(-27.0);
			Camera.MoveForward(-1.1);
			Camera.RotateY(15.0);
			Camera.RotateX(35.0);
			Camera.MoveForward(-4.1);
			Camera.RotateZ(5.0);
		} else {
			isDragon = false;
            kamerajadul = false;
		}
	} else {
		isMoving = true;
		if (id == 3) {
			Camera.Reset();
		}
	}
}

// Fungsi pengaturan material
void materialMenu(int id) 
{
    // Bagus mode
	if(id == -1) {
		showTexture = true;
    }
    // Wireframe mode
    else if (id == 3) {
        showTexture = true;
        wireframe = true;
    // Only material mode
	} else {
		currentMaterial = id + 1;
		showTexture = false;
	}
}

// Fungsi menu pengaturan lampu
void lampMenu(int id) {
    isLight[id] = !isLight[id];
}

// Fungsi yang mengatur gerak joint
void mouseClick(int btn, int state, int x, int y) {
    if (lockedPart != -1 && nemoInteractive && !isAnimate)
    {
		if (btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
			if(nemokeyframes[lockedPart] + jointInc >= 360) {
				nemokeyframes[lockedPart] = 0;
			} else {
				nemokeyframes[lockedPart] += .1;
			}
		} else if (btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN) {
			if(nemokeyframes[lockedPart] - jointInc <= 0) {
				nemokeyframes[lockedPart] = 360;
			} else {
				nemokeyframes[lockedPart] -= .1;
			}
		}
    }
    else if(lockedPart != -1 && dragonInteractive && !isAnimate) {
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
	} else if (lockedPart == -1) {
		if (btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
			lamp[0][1]+=3;
		} else if (btn==GLUT_RIGHT_BUTTON && state==GLUT_DOWN) {
			lamp[0][1]-=3;
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
		if(!isMoving && isDragon) {
			Camera.RotateY(dragVInc*17);
			Camera.StrafeRight(dragVInc*5);
			Camera.MoveUpward(-dragVInc*1.5);
			Camera.MoveForward(-dragVInc*3);		
			Camera.RotateX(dragVInc*8);
		}
	}
	glutTimerFunc(FPS, frameAnimation, 0); 	
}

void KeyDown(unsigned char key, int x, int y)
{
	if(key == 27) {
		PostQuitMessage(0);
	} else if (isMoving) {
		switch (key) {
		case 'l':		
			Camera.RotateY(5.0);
			display();
			break;
		case 'j':		
			Camera.RotateY(-5.0);
			display();
			break;
		case 's':		
			Camera.MoveForward( -0.1 ) ;
			display();
			break;
		case 'w':		
			Camera.MoveForward( 0.1 ) ;
			display();
			break;
		case 'i':		
			Camera.RotateX(5.0);
			display();
			break;
		case 'k':		
			Camera.RotateX(-5.0);
			display();
			break;
		case 'd':		
			Camera.StrafeRight(-0.1);
			display();
			break;
		case 'a':		
			Camera.StrafeRight(0.1);
			display();
			break;
		case 'q':
			Camera.MoveUpward(-0.3);
			display();
			break;
		case 'e':
			Camera.MoveUpward(0.3);
			display();
			break;

		case 'u':
			Camera.RotateZ(-5.0);
			display();
			break;
		case 'o':
			Camera.RotateZ(5.0);
			display();
			break;
		}
	}
}

// Main Function
int main(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    // Solving unknown ATI Problem on Mufid's computer
    glutInitWindowSize(1212, 708);
    glutCreateWindow("TugasAkhir_Fajar Setyo Nugroho_1006671425_Muhammad Mufid Afif_1006671766");

	init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
	glutMouseFunc(mouseClick);
	glutIdleFunc(animation);
	
    initDisplayList();

    int modeInt, animateInt, materialInt, lampInt, camInt, nemoInt;
	modeInt = glutCreateMenu(modeMenu);
    glutAddMenuEntry("Demo", 0);
    glutAddMenuEntry("Interaktif", 1);
	animateInt = glutCreateMenu(animateMenu);
	glutAddMenuEntry("spotlight", -1);
    glutAddMenuEntry("neck", 0);
	glutAddMenuEntry("right shoulder", 1);
	glutAddMenuEntry("left shoulder", 2);
	glutAddMenuEntry("right arm", 3);
	glutAddMenuEntry("left arm", 4);
	glutAddMenuEntry("right leg", 5);
	glutAddMenuEntry("left leg", 6);
	glutAddMenuEntry("right feet", 7);
	glutAddMenuEntry("left feet", 8);
    nemoInt = glutCreateMenu(animateMenuNemo);
    glutAddMenuEntry("sirip-1", 0);
	glutAddMenuEntry("sirip-2", 1);
	glutAddMenuEntry("sirip-3", 2);
	glutAddMenuEntry("sirip-4", 3);
	glutAddMenuEntry("sirip-5", 4);
	glutAddMenuEntry("sirip-6", 5);
	glutAddMenuEntry("sirip-7", 6);
	glutAddMenuEntry("buntut", 7);
	lampInt = glutCreateMenu(lampMenu);
    glutAddMenuEntry("spotlight", 0);
    glutAddMenuEntry("left light", 1);
    glutAddMenuEntry("right light", 2);
    glutAddMenuEntry("horizontal light", 3);
    materialInt = glutCreateMenu(materialMenu);
    glutAddMenuEntry("Texture", -1);
    glutAddMenuEntry("Standard", 0);
    glutAddMenuEntry("Greymon", 1);
    glutAddMenuEntry("Black Dragon", 2);
    glutAddMenuEntry("Wireframe Mode", 3);
	camInt = glutCreateMenu(camMenu);
    glutAddMenuEntry("Moving Camera", 0);
    glutAddMenuEntry("Dragon View", 1);
    glutAddMenuEntry("Nemodomon", 2);
    glutAddMenuEntry("Reset View", 3);
    
	
	glutKeyboardFunc(KeyDown);
	glutCreateMenu(menu);
    glutAddSubMenu("Mode?", modeInt);
    glutAddSubMenu("Animate Dragon?", animateInt);
    glutAddSubMenu("Animate Nemo?", nemoInt);
	glutAddSubMenu("Toggle Lamp?", lampInt);
    glutAddSubMenu("Material?", materialInt);
    glutAddSubMenu("Camera?", camInt);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);
	glutTimerFunc(FPS, frameAnimation, 0); 	
    glutMainLoop();

    return 0;
}
