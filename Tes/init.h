// Init.h, just a wrapper


#include <windows.h>
#include <string>
#include <cmath>
#include <ctime>
#include <iostream>
#include "GLee\GLee.h"
#include <GL/glut.h>
#include "glm/glm.h"

extern GLuint kaktus_dp;
extern GLuint sayap_dp;
extern GLuint nemo_body_dp;
extern GLuint nemo_sirip;
extern GLuint nemo_buntut;

void initDisplayList();
void glShadowProjection(float * l, float * e, float * n);
