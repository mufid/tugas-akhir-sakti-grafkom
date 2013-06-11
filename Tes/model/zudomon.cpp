#include "zudomon.h"

GLuint kaktus_dp;
GLuint kaktus_dp_notexture;
GLuint nemo_body_dp;
GLuint nemo_body_dp_notexture;
GLuint nemo_sirip_dp;
GLuint nemo_sirip_dp_notexture;
GLuint nemo_buntut_dp;
GLuint nemo_buntut_dp_notexture;

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

void drawKaktus(bool tekstur) {
    glCallList(tekstur ? kaktus_dp : kaktus_dp_notexture);
}

void drawNemo(float keyframe, bool texture) {
    glRotatef(90, 1.f, 0.f, 0);
    glCallList(texture ? nemo_body_dp : nemo_body_dp_notexture);

}