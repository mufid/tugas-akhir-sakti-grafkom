#include <GL\glut.h>

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

void drawZudomon() {
    //SOLID_CLOSED_CYLINDER(neck, 0.4, MAX_WING, 7.0, 10, 10);
}
