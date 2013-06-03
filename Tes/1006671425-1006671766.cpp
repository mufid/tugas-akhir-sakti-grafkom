
#define GLUT_DISABLE_ATEXIT_HACK
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string>
#include <cmath>
#include <ctime>
#include <iostream>
#include <GL\glut.h>
using namespace std;

// Variables

// Perspective Constant
#define FOVY (double) 80.0
#define ZNEAR (double) 0.6
#define ZFAR (double) 98.0

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
GLUquadricObj *head, *neck, *body, *rChest, *lChest, *rShoulder, *lShoulder, *rArm, *lArm, *rLeg, *lLeg, *rFeet, *lFeet;

// Light Color
GLfloat lightD[3][4] = {{0.0, 0.0, 0.8, 1.0}, 
					    {0.8, 0.0, 0.0, 1.0},
                        {1.0, 1.0, 1.0, 1.0}};

// Horizontal Light
GLfloat horizonA[] = {0.0, 0.0, 0.0, 1.0},
		horizonS[] = {0.6, 0.6, 0.6, 1.0},
		horizonP[] = {10.0, 3.0, -30.0, 1.0};

// Vertical Light
GLfloat verticA[] = {0.0, 0.0, 0.0, 1.0},
		verticS[] = {0.6, 0.6, 0.6, 1.0},
		verticP[] = {10.0, 3.0, -30.0, 1.0};

// Materials
// silver, gold, ruby, emerald

GLfloat materialA[4][3] = {{ 0.19225, 0.19225, 0.19225 },
						   { 0.24725, 0.1995, 0.0745 },
						   { 0.1745, 0.01175, 0.01175 },
						   { 0.0215, 0.1745, 0.0215}},
		materialD[4][3] = {{ 0.50754, 0.50754, 0.50754 },
						   { 0.75164, 0.60648, 0.22648 },
                           { 0.61424, 0.04136, 0.04136 },
                           { 0.07568, 0.61424, 0.07568 }},
        materialS[4][3] = {{ 0.508273, 0.508273, 0.508273 },
                           { 0.628281, 0.555802, 0.366065 },
                           { 0.727811, 0.626959, 0.626959 },
                           { 0.633, 0.727811, 0.633}},
		materialShine[4] = { 0.4, 0.4, 0.6, 0.6};

int activeMaterial = 0; // Material apa yang sedang aktif

bool isPlay = false; // Apakah animasi bermain?
bool isHorizon = true, isVertic = true; // Apakah lampu menyala?

double lightDegree = 0, lightInc = 0.09; // Derajat Lampu

int lockedPart = 0; // Index joint yang sedang aktif
double jointDegree[9] = {20, 0, 0, 0, 0, 0, 0, 0, 0}, jointInc = 10; // Derajat tiap joint

GLfloat rot = 0.0, rotInc = 0.05; // Derajat perputaran model
bool isSpin = true; // Apakah model perlu berputar?

// Function

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

    glLightfv(GL_LIGHT1, GL_POSITION, horizonP);
    glLightfv(GL_LIGHT1, GL_AMBIENT, horizonA);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightD[0]);
    glLightfv(GL_LIGHT1, GL_SPECULAR, horizonS);

    glLightfv(GL_LIGHT2, GL_POSITION, verticP);
    glLightfv(GL_LIGHT2, GL_AMBIENT, verticA);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightD[1]);
    glLightfv(GL_LIGHT2, GL_SPECULAR, verticS);

	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);
}

// Reshape Function
void reshape(int w, int h) {
	const float aspect = (float) w / (float) h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(FOVY, aspect, ZNEAR, ZFAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void DrawLArm() {
	glPushMatrix();
	glTranslatef(-0.2, 0.7 - BICEPT_HEIGHT, BICEPT_HEIGHT - 1.9);
	glRotatef(jointDegree[4], 1.0, 0.0, 0.0);
	glRotatef(0, 0.0, 1.0, 0.0);
	gluCylinder(lArm, ARM_LOWER, ARM_UPPER, ARM_HEIGHT, 10, 10);
	glRotatef(0, 0.0, 1.0, 0.0);
	glPopMatrix();
}

void DrawLShoulder() {
	glPushMatrix();
	glTranslatef(CHEST_HEIGHT, 0.5, 0.0);
	glRotatef(jointDegree[2], 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(-30, 1.0, 0.0, 0.0);
	gluCylinder(lShoulder, SHOULDER_LOWER, SHOULDER_UPPER, SHOULDER_HEIGHT, 10, 10);
	glRotatef(30, 1.0, 0.0, 0.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	
	glTranslatef(SHOULDER_HEIGHT/7, 0.0, 0.0);
	glRotatef(60, 1.0, 0.0, 0.0);
	gluCylinder(lShoulder, BICEPT_UPPER, BICEPT_LOWER, BICEPT_HEIGHT, 10, 10);
	glRotatef(-60, 1.0, 0.0, 0.0);
	DrawLArm();	
	glPopMatrix();
}

void DrawRArm() {
	glPushMatrix();
	glTranslatef(0.2, 0.7 - BICEPT_HEIGHT, BICEPT_HEIGHT - 1.9);
	glRotatef(jointDegree[3], 1.0, 0.0, 0.0);
	glRotatef(0, 0.0, 1.0, 0.0);
	gluCylinder(rArm, ARM_LOWER, ARM_UPPER, ARM_HEIGHT, 10, 10);
	glRotatef(0, 0.0, 1.0, 0.0);
	glPopMatrix();
}

void DrawRShoulder() {
	glPushMatrix();
	glTranslatef(-CHEST_HEIGHT, 0.5, 0.0);
	glRotatef(jointDegree[1], 1.0, 0.0, 0.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(-30, 1.0, 0.0, 0.0);
	gluCylinder(rShoulder, SHOULDER_LOWER, SHOULDER_UPPER, SHOULDER_HEIGHT, 10, 10);
	glRotatef(30, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	
	glTranslatef(-SHOULDER_HEIGHT/7, 0.0, 0.0);
	glRotatef(60, 1.0, 0.0, 0.0);
	gluCylinder(rShoulder, BICEPT_UPPER, BICEPT_LOWER, BICEPT_HEIGHT, 10, 10);
	glRotatef(-60, 1.0, 0.0, 0.0);
	DrawRArm();	
	glPopMatrix();
}

void DrawNeck() {
	glPushMatrix();
	glTranslatef(0.0, 0.6, 0.8);
	glRotatef(-25, 1.0, 0.0, 0.0);
	gluCylinder(neck, NECK_LOWER, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(25, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.4, 0.9);
	glRotatef(-10, 1.0, 0.0, 0.0);
	gluCylinder(neck, NECK_LOWER, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(10, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.2, 0.9);
	glRotatef(0, 1.0, 0.0, 0.0);
	gluCylinder(neck, NECK_LOWER, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 0.9);
	glRotatef(0, 1.0, 0.0, 0.0);
	gluCylinder(neck, NECK_LOWER, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(0, 1.0, 0.0, 0.0);
	glPopMatrix();
}

void DrawLFeet() {
	glTranslatef(2.8, -1.2, 2.3);	
	glRotatef(jointDegree[8], 1.0, 0.0, 0.0);
	glPushMatrix();
	glRotatef(140.0, 1.0, 0.0, 0.0);
	gluCylinder(lLeg, CALF_LOWER, CALF_UPPER, CALF_HEIGHT, 10, 10);
	glRotatef(-140.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -3.3, -2.7);
	glRotatef(30.0, 1.0, 0.0, 0.0);
	glScalef(FEET_WIDTH, FEET_HEIGHT, FEET_LENGTH);
	glutSolidCube(1.0);
	glScalef(1.0/FEET_WIDTH, 1.0/FEET_HEIGHT, 1.0/FEET_LENGTH);
	glRotatef(30.0, -1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 1.0);	
	glPopMatrix();
}


void DrawRFeet() {
	glTranslatef(-2.8, -1.2, 2.3);	
	glRotatef(jointDegree[7], 1.0, 0.0, 0.0);
	glPushMatrix();
	glRotatef(140.0, 1.0, 0.0, 0.0);
	gluCylinder(lLeg, CALF_LOWER, CALF_UPPER, CALF_HEIGHT, 10, 10);
	glRotatef(-140.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -3.3, -2.7);
	glRotatef(30.0, 1.0, 0.0, 0.0);
	glScalef(FEET_WIDTH, FEET_HEIGHT, FEET_LENGTH);
	glutSolidCube(1.0);
	glScalef(1.0/FEET_WIDTH, 1.0/FEET_HEIGHT, 1.0/FEET_LENGTH);
	glRotatef(30.0, -1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 1.0);	
	glPopMatrix();
}

void DrawLLeg() {
	glPushMatrix();
	glTranslatef(-0.5, -0.1, 0.0);
	glRotatef(-40, 1.0, 1.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(30, 1.0, 0.0, 0.0);
	gluCylinder(lLeg, LEG_LOWER, LEG_UPPER, LEG_HEIGHT, 10, 10);
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
	gluCylinder(rLeg, LEG_LOWER, LEG_UPPER, LEG_HEIGHT, 10, 10);
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
	gluCylinder(neck, NECK_LOWER, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(70, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.5, 0.5);
	glRotatef(-55, 1.0, 0.0, 0.0);
	gluCylinder(neck, NECK_LOWER, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(55, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.6, 0.6);
	glRotatef(-40, 1.0, 0.0, 0.0);
	gluCylinder(neck, NECK_LOWER, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(40, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.6, 0.8);
	glRotatef(-25, 1.0, 0.0, 0.0);
	gluCylinder(neck, NECK_LOWER, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(25, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.4, 0.9);
	glRotatef(-10, 1.0, 0.0, 0.0);
	gluCylinder(neck, NECK_LOWER, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(10, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.2, 0.9);
	glRotatef(0, 1.0, 0.0, 0.0);
	gluCylinder(neck, NECK_LOWER, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 0.9);
	glRotatef(0, 1.0, 0.0, 0.0);
	gluCylinder(neck, NECK_LOWER, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 0.9);
	gluCylinder(neck, NECK_LOWER, 0.01, NECK_HEIGHT*4, 10, 10);
	glPopMatrix();
}

void DrawLeftWing() {
	glPushMatrix();
	glTranslatef(1.0, STOMACH_HEIGHT, 0);
	glRotatef(-30, 0.0, 0.0, 1.0);
	glRotatef(-130, 1.0, 0.0, 0.0);
	gluCylinder(neck, 0.4, 0.4, 5.0, 10, 10);
	glRotatef(130, 1.0, 0.0, 0.0);
	glRotatef(30, 0.0, 0.0, 1.0);
	glPopMatrix();
}

void DrawBody(GLfloat initAngle) {
	glPushMatrix();

	glTranslatef(0.0, -5.8 -(4), -15.0);
	
	glRotatef(rot, 0.0, 1.0, 0.0);
	//glRotatef(120, 0.0, 1.0, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(body, STOMACH_LOWER, STOMACH_UPPER, STOMACH_HEIGHT, 10, 10);	
	glRotatef(90, 1.0, 0.0, 0.0);

	glTranslatef(0.0, STOMACH_HEIGHT, 0);

	glRotatef(90, 0.0, 1.0, 0.0);
	gluCylinder(lChest, CHEST_LOWER, CHEST_UPPER, CHEST_HEIGHT, 10, 10);
	glRotatef(-90, 0.0, 1.0, 0.0);
	DrawLShoulder();

	glRotatef(-90, 0.0, 1.0, 0.0);
	gluCylinder(rChest, CHEST_LOWER, CHEST_UPPER, CHEST_HEIGHT, 10, 10);
	glRotatef(90, 0.0, 1.0, 0.0);	
	DrawRShoulder();

	glPushMatrix();
	glRotatef(jointDegree[0], 1.0, 0.0, 0.0);
	glTranslatef(0.0, STOMACH_HEIGHT - 1.4, 0.0);
	glRotatef(-70, 1.0, 0.0, 0.0);
	gluCylinder(neck, NECK_LOWER, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(70, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.5, 0.5);
	glRotatef(-55, 1.0, 0.0, 0.0);
	gluCylinder(neck, NECK_LOWER, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(55, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.6, 0.6);
	glRotatef(-40, 1.0, 0.0, 0.0);
	gluCylinder(neck, NECK_LOWER, NECK_UPPER, NECK_HEIGHT, 10, 10);
	glRotatef(40, 1.0, 0.0, 0.0);
	DrawNeck();
	glPopMatrix();
	glTranslatef(0.0, -STOMACH_HEIGHT-0.5, 0.5);
	glPushMatrix();
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(body, STOMACH_UPPER, STOMACH_LOWER, STOMACH_HEIGHT/4, 10, 10);
	glRotatef(90, 1.0, 0.0, 0.0);
	glPopMatrix();
	
	DrawTail();
	DrawLeftWing();


	glTranslatef(0.0, 0.0, 0.0);
	
	glRotatef(90, 1.0, 0.0, 0.0);
	gluCylinder(body, ULEG_LOWER, ULEG_UPPER, ULEG_HEIGHT, 10, 10);
	glRotatef(-90, 1.0, 0.0, 0.0);
	
	glPushMatrix();
	glRotatef(jointDegree[6], 1.0, 0.0, 0.0);
	glTranslatef(1.2, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glRotatef(-30, 1.0, 0.0, 0.0);
	gluCylinder(body, LLEG_LOWER, LLEG_UPPER, LLEG_HEIGHT, 10, 10);
	glRotatef(30, 1.0, 0.0, 0.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	DrawLLeg();
	glPopMatrix();
	
	glPushMatrix();
	glRotatef(jointDegree[5], 1.0, 0.0, 0.0);
	glTranslatef(-1.2, 0.0, 0.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
	glRotatef(-30, 1.0, 0.0, 0.0);
	gluCylinder(body, LLEG_LOWER, LLEG_UPPER, LLEG_HEIGHT, 10, 10);
	glRotatef(30, 1.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	DrawRLeg();
	glPopMatrix();

	
	glPopMatrix();
}

// Konstruksi Objek
void displayObject(GLfloat initAngle, GLfloat initX, GLfloat initY, GLfloat initZ) {
	glTranslatef(initX, initY, initZ);
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialS[activeMaterial]);
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialA[activeMaterial]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialD[activeMaterial]);
	glMaterialf(GL_FRONT, GL_SHININESS, materialShine[activeMaterial]);
	DrawBody(initAngle);
	glPopMatrix();
}

// Display Function
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
	glDisable(GL_LIGHTING);
	glPushMatrix();
    glPopMatrix();
	
	// Horizontal Light
    glPushMatrix(); 
    if (isHorizon) glColor3f(0.85, 0.2, 0.2);
    else glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0.0, -1.0, -15.0);
    glRotatef(lightDegree, 0.0, 1.0, 0.0);     
	glLightfv(GL_LIGHT1, GL_POSITION, horizonP);
    glTranslatef(10.0, 4.0, -15.0);
    glutSolidSphere(0.3, 10, 10);
    glPopMatrix();
    glPopMatrix();

	// Vertical Light
    glPushMatrix(); 
    if (isVertic) glColor3f(0.2, 0.2, 0.85);
    else glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0.0, -1.0, -15.0);
    glRotatef(lightDegree, 1.0, 0.0, 0.0);     
	glLightfv(GL_LIGHT2, GL_POSITION, verticP);
    glTranslatef(10.0, 4.0, -15.0);
    glutSolidSphere(0.3, 10, 10);
    glPopMatrix();
    glPopMatrix();
	
    glEnable(GL_LIGHTING);
	glPushMatrix();
	displayObject(0.0, 0.0, 9.0, 0.0);
	glPopMatrix();
	glDisable(GL_LIGHTING);

	
    glDisable(GL_TEXTURE_2D);
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
	if(id == 0) {
		isHorizon = true;
		glEnable(GL_LIGHT1);
	} else if(id == 1) {
		isHorizon = false;
		glDisable(GL_LIGHT1);
	}
}

// Fungsi menu pengaturan lampu vertical
void verticMenu(int id) {
	if(id == 0) {
		isVertic = true;
		glEnable(GL_LIGHT2);
	} else if(id == 1) {
		isVertic = false;
		glDisable(GL_LIGHT2);
	}
}

// Fungsi menu pengaturan perputaran model
void spinMenu(int id) {
	if(id == 0)
		isSpin = true;
	else if(id == 1)
		isSpin = false;
}

// Fungsi menu pengaturan material
void materialMenu(int id) {
	activeMaterial = id;
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

// Main Function
int main(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1024, 600);
    glutCreateWindow("WS2_Fajar Setyo Nugroho_1006671425");

	init();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
	glutMouseFunc(mouseClick);
	glutIdleFunc(animation);
	
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
    glutAddMenuEntry("silver", 0);
    glutAddMenuEntry("gold", 1);
    glutAddMenuEntry("ruby", 2);
    glutAddMenuEntry("emerald", 3);

	glutCreateMenu(menu);
    glutAddSubMenu("Animate?", animateInt);
	glutAddSubMenu("Horizontal Light?", horizonInt);
	glutAddSubMenu("Vertical Light?", verticInt);
	glutAddSubMenu("Material?", materialInt);
	glutAddSubMenu("Spin?", spinInt);

	glutAttachMenu(GLUT_MIDDLE_BUTTON);
	
    glutMainLoop();

    return 0;
}