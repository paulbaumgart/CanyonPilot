#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include <GLUT/glut.h>
#include <math.h>
#include "../loadppm.h"

extern int textureCount;

class Skybox {
public:
  Skybox(int size) {
    this->size = size;
    
    int width, height;
    unsigned char* image;
    
    image = loadPPM("Textures/desertsky_ft.pbm", width, height);
    frontTid = textureCount++;
    
    glBindTexture(GL_TEXTURE_2D, frontTid);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, 
        height, 0, GL_RGB, GL_UNSIGNED_BYTE, 
        image);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
        GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
        GL_NEAREST);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
    
    free(image);
    image = loadPPM("Textures/desertsky_bk.pbm", width, height);
    backTid = textureCount++;
    
    glBindTexture(GL_TEXTURE_2D, backTid);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, 
        height, 0, GL_RGB, GL_UNSIGNED_BYTE, 
        image);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
        GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
        GL_NEAREST);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
    
    free(image);
    image = loadPPM("Textures/desertsky_up.pbm", width, height);
    upTid = textureCount++;
    
    glBindTexture(GL_TEXTURE_2D, upTid);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, 
        height, 0, GL_RGB, GL_UNSIGNED_BYTE, 
        image);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
        GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
        GL_NEAREST);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
    
    free(image);
    image = loadPPM("Textures/desertsky_dn.pbm", width, height);
    downTid = textureCount++;
    
    glBindTexture(GL_TEXTURE_2D, downTid);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, 
        height, 0, GL_RGB, GL_UNSIGNED_BYTE, 
        image);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
        GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
        GL_NEAREST);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
    
    free(image);
    image = loadPPM("Textures/desertsky_lf.pbm", width, height);
    leftTid = textureCount++;
    
    glBindTexture(GL_TEXTURE_2D, leftTid);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, 
        height, 0, GL_RGB, GL_UNSIGNED_BYTE, 
        image);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
        GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
        GL_NEAREST);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
    
    free(image);
    image = loadPPM("Textures/desertsky_rt.pbm", width, height);
    rightTid = textureCount++;
    
    glBindTexture(GL_TEXTURE_2D, rightTid);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, 
        height, 0, GL_RGB, GL_UNSIGNED_BYTE, 
        image);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
        GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
        GL_NEAREST);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
    
    free(image);
  }
  
  void draw(Vector3 velocity) {
    glRotated(atan2(velocity[Z], velocity[X]) * 180 / M_PI, 0, 1, 0);
    
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glDisable(GL_FOG);
    
    glBindTexture(GL_TEXTURE_2D, downTid);
    glBegin(GL_QUADS);
    
    glNormal3d(0, 1, 0);
    glTexCoord2f(0.0, 0.0);
    glVertex3d( size/2, -size/2,  size/2);
    glTexCoord2f(1.0, 0.0);
    glVertex3d( size/2, -size/2, -size/2);
    glTexCoord2f(1.0, 1.0);
    glVertex3d(-size/2, -size/2, -size/2);
    glTexCoord2f(0.0, 1.0);
    glVertex3d(-size/2, -size/2,  size/2);
    
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, upTid);
    glBegin(GL_QUADS);
    
    glNormal3d(0, -1, 0);
    glTexCoord2f(1.0, 1.0);
    glVertex3d( size/2,  size/2, -size/2);
    glTexCoord2f(0.0, 1.0);
    glVertex3d( size/2,  size/2,  size/2);
    glTexCoord2f(0.0, 0.0);
    glVertex3d(-size/2,  size/2,  size/2);
    glTexCoord2f(1.0, 0.0);
    glVertex3d(-size/2,  size/2, -size/2);
    
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, leftTid);
    glBegin(GL_QUADS);
    
    glNormal3d(1, 0, 0);
    glTexCoord2f(0.0, 1.0);
    glVertex3d(-size/2, -size/2, -size/2);
    glTexCoord2f(0.0, 0.0);
    glVertex3d(-size/2,  size/2, -size/2);
    glTexCoord2f(1.0, 0.0);
    glVertex3d(-size/2,  size/2,  size/2);
    glTexCoord2f(1.0, 1.0);
    glVertex3d(-size/2, -size/2,  size/2);
    
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, rightTid);
    glBegin(GL_QUADS);
    
    glNormal3d(-1, 0, 0);
    glTexCoord2f(0.0, 1.0);
    glVertex3d( size/2, -size/2,  size/2);
    glTexCoord2f(0.0, 0.0);
    glVertex3d( size/2,  size/2,  size/2);
    glTexCoord2f(1.0, 0.0);
    glVertex3d( size/2,  size/2, -size/2);
    glTexCoord2f(1.0, 1.0);
    glVertex3d( size/2, -size/2, -size/2);
    
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, frontTid);
    glBegin(GL_QUADS);
    
    glNormal3d(0, 0, 1);
    glTexCoord2f(1.0, 0.0);
    glVertex3d(-size/2,  size/2, -size/2);
    glTexCoord2f(1.0, 1.0);
    glVertex3d(-size/2, -size/2, -size/2);
    glTexCoord2f(0.0, 1.0);
    glVertex3d( size/2, -size/2, -size/2);
    glTexCoord2f(0.0, 0.0);
    glVertex3d( size/2,  size/2, -size/2);
    
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, backTid);
    glBegin(GL_QUADS);
    
    glNormal3d(0, 0, -1);
    glTexCoord2f(0.0, 1.0);
    glVertex3d(-size/2, -size/2,  size/2);
    glTexCoord2f(0.0, 0.0);
    glVertex3d(-size/2,  size/2,  size/2);
    glTexCoord2f(1.0, 0.0);
    glVertex3d( size/2,  size/2,  size/2);
    glTexCoord2f(1.0, 1.0);
    glVertex3d( size/2, -size/2,  size/2);
    
    glEnd();
    glPopAttrib();
  }
private:
  int size, frontTid, backTid, upTid, downTid, leftTid, rightTid;
};


#endif
