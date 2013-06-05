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