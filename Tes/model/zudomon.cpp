#include "zudomon.h"

GLuint kaktus_dp;
GLuint kaktus_dp_notexture;
GLuint nemo_body_dp;
GLuint nemo_body_dp_notexture;
GLuint nemo_sirip_dp;
GLuint nemo_sirip_dp_notexture;
GLuint nemo_buntut_dp;
GLuint nemo_buntut_dp_notexture;

float keyframe_sayapatas_1;
float keyframe_sayapatas_2;
float keyframe_sayapatas_3;
float keyframe_sayapbawah_1;
float keyframe_sayapbawah_2;
float keyframe_sayapbawah_3;
float keyframe_sayapbawah_4;
float keyframe_buntut;

float zudomonposx = 0;
float zudomonposy = 0;
float zudomonposz = -20.f;

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
    zudomonposy = 0;
    zudomonposx = sinf(keyframe*2) * 10.f;
    zudomonposz = 0;
    glTranslatef(zudomonposx, zudomonposy, zudomonposz);
    glRotatef(90, 0.f, 0.f, 1.f);
    glRotatef(90, 1.f, 0.f, 0.f);
    
    // Gambar bodi dulu
    // Nothing todo with body LOL LOL
    glPushMatrix();
    glCallList(texture ? nemo_body_dp : nemo_body_dp_notexture);
    glPopMatrix();

    // Terus gambar sirip
    glPushMatrix();
    glTranslatef(3.7f, 1.f, 0);
    // y: kemiringan terhadap posisi badan
    // z: kemiringan berenang
    glRotatef(sinf(keyframe*10.f) * 20, 0.f, 0.f, 1.f);
    glCallList(texture ? nemo_sirip_dp : nemo_sirip_dp_notexture);
    glPopMatrix();

    // Terus gambar sirip-2 atas
    glPushMatrix();
    glRotatef(-40.f, 0, 1.f, 0);
    glTranslatef(3.7f, 1.f, 0.f);
    glRotatef(sinf(keyframe*10.f - 3.f) * 20, 0.f, 0.f, 1.f);
    glCallList(texture ? nemo_sirip_dp : nemo_sirip_dp_notexture);
    glPopMatrix();

    // Terus gambar sirip-3 atas
    glPushMatrix();
    glRotatef(-70.f, 0, 1.f, 0);
    glTranslatef(3.7f, 1.f, 0.f);
    glRotatef(sinf(keyframe*10.f - 5.7f) * 20, 0.f, 0.f, 1.f);
    glCallList(texture ? nemo_sirip_dp : nemo_sirip_dp_notexture);
    glPopMatrix();

    // Sirip, bagian bawah
    glPushMatrix();
    glRotatef(180.f, 0.f, 1.f, 0);
    glTranslatef(3.7f, 1.f, 0);
    glRotatef(sinf(keyframe*10.f) * 20, 0.f, 0.f, 1.f);
    glCallList(texture ? nemo_sirip_dp : nemo_sirip_dp_notexture);
    glPopMatrix();

    // Sirip-2, bagian bawah
    glPushMatrix();
    glRotatef(180.f, 0.f, 1.f, 0);
    glRotatef(20.f, 0, 1.f, 0);
    glTranslatef(3.7f, 1.f, 0);
    glRotatef(sinf(keyframe*10.f - 3.f) * 20, 0.f, 0.f, 1.f);
    glCallList(texture ? nemo_sirip_dp : nemo_sirip_dp_notexture);
    glPopMatrix();

    // BUNTUT NEMO
    //    ---------------------------------------------
    //    |++++                                        \\\\\\\
    //  /++                                             |||\\\\\
    //  |++                                             |||\\\\\
    //    |++++                                       ////////
    //    ---------------------------------------------
    //                                                    ^^^^
    //                                                INI BUNTUT

    glPushMatrix();
    glRotatef(180, 1.f, 0, 0.f);
    glTranslatef(0.f, 0.f, -5.6432f);
    glRotatef(sinf(keyframe*20.f) * 20, 0.f, 0.f, 4.f);
    glCallList(texture ? nemo_buntut_dp : nemo_body_dp_notexture);
    glPopMatrix();
}