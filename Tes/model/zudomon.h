
#include "../init.h"

// Untuk bermain interaktif
extern float keyframe_sayapatas_1;
extern float keyframe_sayapatas_2;
extern float keyframe_sayapatas_3;
extern float keyframe_sayapbawah_1;
extern float keyframe_sayapbawah_2;
extern float keyframe_sayapbawah_3;
extern float keyframe_sayapbawah_4;
extern float keyframe_buntut;

// Untuk pergerakan dinamis dari objek
extern float zudomonposx;
extern float zudomonposy;
extern float zudomonposz;

void drawKaktus(bool tekstur);
void drawNemo(float keyframe, bool texture);