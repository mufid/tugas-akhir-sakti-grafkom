#include <windows.h>
#include <string>
#include <cmath>
#include <ctime>
#include <iostream>
#include "GLee\GLee.h"
#include <GL/glut.h>
#include "glm/glm.h"
#include "init.h"

// DisplayList dipergunakan untuk menggambar sayap yang cukup kompleks
void initDisplayList()
{
    GLMmodel* kaktus;

    kaktus = (GLMmodel*)malloc(sizeof(GLMmodel));
	kaktus = glmReadOBJ("kaktus.obj");
	
	kaktus_dp=glGenLists(1);
	glNewList(kaktus_dp,GL_COMPILE);
        glmDraw(kaktus, GLM_SMOOTH | GLM_TEXTURE | GLM_COLOR);
		//glmDraw(kaktus, GLM_SMOOTH);
	glEndList();

    GLMmodel* nemobodi_model;

    nemobodi_model = (GLMmodel*)malloc(sizeof(GLMmodel));
	nemobodi_model = glmReadOBJ("nemo-bodi.obj");
	//glmUnitize(kubus);
    glmFacetNormals(nemobodi_model);
    glmLinearTexture(nemobodi_model);
	glmVertexNormals(nemobodi_model, 90.0, GL_TRUE);
	nemo_body_dp = glGenLists(1);
    glEnable(GL_TEXTURE);
    glEnable(GL_TEXTURE_2D);

	glNewList(nemo_body_dp,GL_COMPILE);
        glmDraw(nemobodi_model, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
	glEndList();
    
    glDisable(GL_TEXTURE);
    glDisable(GL_TEXTURE_2D);

    // Sirip yea

    nemobodi_model = (GLMmodel*)malloc(sizeof(GLMmodel));
	nemobodi_model = glmReadOBJ("nemo-sirip.obj");
    glmFacetNormals(nemobodi_model);
    glmLinearTexture(nemobodi_model);
	glmVertexNormals(nemobodi_model, 90.0, GL_TRUE);
	nemo_sirip = glGenLists(1);
    glEnable(GL_TEXTURE);
    glEnable(GL_TEXTURE_2D);

	glNewList(nemo_sirip, GL_COMPILE);
        glmDraw(nemobodi_model, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
	glEndList();

    // Buntut  yea
    nemobodi_model = (GLMmodel*)malloc(sizeof(GLMmodel));
	nemobodi_model = glmReadOBJ("nemo-belakang.obj");
    glmFacetNormals(nemobodi_model);
    glmLinearTexture(nemobodi_model);
	glmVertexNormals(nemobodi_model, 90.0, GL_TRUE);
	nemo_buntut = glGenLists(1);
    glEnable(GL_TEXTURE);
    glEnable(GL_TEXTURE_2D);

	glNewList(nemo_buntut, GL_COMPILE);
        glmDraw(nemobodi_model, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
	glEndList();

 //   kubus = (GLMmodel*)malloc(sizeof(GLMmodel));
	//kubus = glmReadOBJ("nemo.obj");
	////glmUnitize(kubus);
 //   glmFacetNormals(kubus);
 //   glmLinearTexture(kubus);
	//glmVertexNormals(kubus, 90.0, GL_TRUE);
	//kubus_dp=glGenLists(1);
 //   glEnable(GL_TEXTURE);
 //   glEnable(GL_TEXTURE_2D);

	//glNewList(kubus_dp,GL_COMPILE);
 //       glmDraw(kubus, GLM_SMOOTH | GLM_TEXTURE | GLM_MATERIAL);
	//	//glmDraw(kaktus, GLM_SMOOTH);
	//glEndList();
 //   
 //   glDisable(GL_TEXTURE);
 //   glDisable(GL_TEXTURE_2D);

	return;
    GLMmodel* sayap;
    sayap = (GLMmodel*) malloc(sizeof(GLMmodel));
	sayap = glmReadOBJ("sayap.obj");
	
	sayap_dp=glGenLists(1);
	glNewList(sayap_dp, GL_COMPILE);
		glmList(sayap, GLM_SMOOTH);
		//glmDraw(sayap, GLM_SMOOTH);
	glEndList();
}

void glShadowProjection(float * l, float * e, float * n) {
    float d, c;
    float mat[16];

    // These are c and d

    d = n[0] * l[0] + n[1] * l[1] + n[2] * l[2];
    c = e[0] * n[0] + e[1] * n[1] + e[2] * n[2] - d;

    // membuat matriks

    mat[0] = l[0] * n[0] + c;
    mat[4] = n[1] * l[0];
    mat[8] = n[2] * l[0];
    mat[12] = -l[0] * c - l[0] * d;

    mat[1] = n[0] * l[1];
    mat[5] = l[1] * n[1] + c;
    mat[9] = n[2] * l[1];
    mat[13] = -l[1] * c - l[1] * d;

    mat[2] = n[0] * l[2];
    mat[6] = n[1] * l[2];
    mat[10] = l[2] * n[2] + c;
    mat[14] = -l[2] * c - l[2] * d;

    mat[3] = n[0];
    mat[7] = n[1];
    mat[11] = n[2];
    mat[15] = -d;

    // Mengalikan matriks
    glMultMatrixf(mat);
}

