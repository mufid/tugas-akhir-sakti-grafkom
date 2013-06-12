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
    // ====================================================================
    //                           K A K T U S
    // ====================================================================

    GLMmodel* kaktus;

    kaktus = (GLMmodel*)malloc(sizeof(GLMmodel));
	kaktus = glmReadOBJ("kaktus.obj");
	
    // Kaktus textured, prepping
    glEnable(GL_TEXTURE);
    glEnable(GL_TEXTURE_2D);
    // Kaktus textured
	kaktus_dp = glGenLists(1);
	glNewList(kaktus_dp, GL_COMPILE);
        glmDraw(kaktus, GLM_SMOOTH | GLM_MATERIAL | GLM_COLOR);
	glEndList();

    glDisable(GL_TEXTURE);
    glDisable(GL_TEXTURE_2D);

    // Kaktus untextured
	kaktus_dp_notexture = glGenLists(1);
	glNewList(kaktus_dp_notexture, GL_COMPILE);
        glmDraw(kaktus, GLM_SMOOTH);
	glEndList();

    // ====================================================================
    //                       N E M O    U T A M A
    // ====================================================================

    GLMmodel* nemobodi_model;

    nemobodi_model = (GLMmodel*)malloc(sizeof(GLMmodel));
	nemobodi_model = glmReadOBJ("nemo-bodi.obj");

    // Nemo textured, prepping
    glEnable(GL_TEXTURE);
    glEnable(GL_TEXTURE_2D);
    // Nemo textured
    nemo_body_dp = glGenLists(1);
	glNewList(nemo_body_dp, GL_COMPILE);
        glmDraw(nemobodi_model, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
	glEndList();

    glDisable(GL_TEXTURE);
    glDisable(GL_TEXTURE_2D);

    // Nemo untextured
    nemo_body_dp_notexture = glGenLists(1);
    glNewList(nemo_body_dp_notexture, GL_COMPILE);
        glmDraw(nemobodi_model, GLM_SMOOTH);
    glEndList();

    // ====================================================================
    //                       N E M O    S I R I P
    // ====================================================================

    nemobodi_model = (GLMmodel*)malloc(sizeof(GLMmodel));
	nemobodi_model = glmReadOBJ("nemo-sirip.obj");

    // Nemo textured, prepping
    glEnable(GL_TEXTURE);
    glEnable(GL_TEXTURE_2D);
    // Nemo textured
    nemo_sirip_dp = glGenLists(1);
	glNewList(nemo_sirip_dp, GL_COMPILE);
        glmDraw(nemobodi_model, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
	glEndList();

    glDisable(GL_TEXTURE);
    glDisable(GL_TEXTURE_2D);

    // Nemo untextured
    nemo_sirip_dp_notexture = glGenLists(1);
    glNewList(nemo_sirip_dp_notexture, GL_COMPILE);
        glmDraw(nemobodi_model, GLM_SMOOTH);
    glEndList();

    // ====================================================================
    //                       N E M O    B U N T U T
    // ====================================================================

    // Buntut  yea
    nemobodi_model = (GLMmodel*)malloc(sizeof(GLMmodel));
	nemobodi_model = glmReadOBJ("nemo-belakang.obj");

    glEnable(GL_TEXTURE);
    glEnable(GL_TEXTURE_2D);

    // Nemo textured, prepping
    glEnable(GL_TEXTURE);
    glEnable(GL_TEXTURE_2D);
    // Nemo textured
    nemo_buntut_dp = glGenLists(1);
	glNewList(nemo_buntut_dp, GL_COMPILE);
        glmDraw(nemobodi_model, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
	glEndList();

    glDisable(GL_TEXTURE);
    glDisable(GL_TEXTURE_2D);

    // Nemo untextured
    nemo_buntut_dp_notexture = glGenLists(1);
    glNewList(nemo_buntut_dp_notexture, GL_COMPILE);
        glmDraw(nemobodi_model, GLM_SMOOTH);
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

