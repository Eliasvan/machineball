// Machine Ball
//  By Benny Kramek
//  benny@kramekweb.com
//  http://benny.kramekweb.com
//
//
// Source Code is released under the GPL
//

#define ALLEGRO_STATICLINK
#include <allegro.h>
#include <alleggl.h>
#include "glode.h"
#include <GL/gl.h>

void position_gl(dBodyID b)
{
	const dReal *p;
	float m[16];
	p = dBodyGetPosition(b);
	glTranslatef(p[0], p[1], p[2]);
	
	p = dBodyGetRotation(b);
	m[0]=p[0];	m[4]=p[1];	m[8]=p[2];	m[12]=0;
	m[1]=p[4];	m[5]=p[5];	m[9]=p[6];	m[13]=0;
	m[2]=p[8];	m[6]=p[9];	m[10]=p[10];	m[14]=0;
	m[3]=0;		m[7]=0;		m[11]=0;	m[15]=1;
		
	glMultMatrixf(m);
}
void position_gl(dGeomID g)
{
	const dReal *p;
	float m[16];
	p = dGeomGetPosition(g);
	glTranslatef(p[0], p[1], p[2]);
	
	p = dGeomGetRotation(g);
	m[0]=p[0];	m[4]=p[1];	m[8]=p[2];	m[12]=0;
	m[1]=p[4];	m[5]=p[5];	m[9]=p[6];	m[13]=0;
	m[2]=p[8];	m[6]=p[9];	m[10]=p[10];	m[14]=0;
	m[3]=0;		m[7]=0;		m[11]=0;	m[15]=1;
		
	glMultMatrixf(m);
}

void draw_box(double length, double width, double height)
{
	double x=length/2, y=width/2, z=height/2;

	glBegin(GL_QUADS);
	glNormal3f( 0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y,  z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( x, -y,  z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( x,  y,  z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,  y,  z);
	glNormal3f( 0.0f, 0.0f,-1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y, -z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y, -z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y, -z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y, -z);
	glNormal3f( 0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,  y, -z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-x,  y,  z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( x,  y,  z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( x,  y, -z);
	glNormal3f( 0.0f,-1.0f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-x, -y, -z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( x, -y, -z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y,  z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y,  z);
	glNormal3f( 1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( x, -y, -z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( x,  y, -z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y,  z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y,  z);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y, -z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y,  z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y,  z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,  y, -z);
	glEnd();
}

void draw_sphere(double r, int n)
{
   int i,j;
   double theta1,theta2,theta3;
   double ex, ey, ez;
   if (r < 0)
      r = -r;
   if (n < 0)
      n = -n;
   if (n < 1 || r <= 0) {
      glBegin(GL_POINTS);
      glVertex3f(0,0,0);
      glEnd();
      return;
   }
   for (j=0;j<n/2;j++) {
      theta1 = j * M_PI*2 / n - M_PI/2;
      theta2 = (j + 1) * M_PI*2 / n - M_PI/2;
      glBegin(GL_QUAD_STRIP);
      for (i=0;i<=n;i++) {
         theta3 = i * M_PI*2 / n;
         ex = r * cos(theta2) * cos(theta3);
         ey = r * sin(theta2);
         ez = r * cos(theta2) * sin(theta3);
         glNormal3f(ex/r,ey/r,ez/r);
         glTexCoord2f(i/(double)n,2*(j+1)/(double)n);
         glVertex3f(ex,ey,ez);
         ex = r * cos(theta1) * cos(theta3);
         ey = r * sin(theta1);
         ez = r * cos(theta1) * sin(theta3);
         glNormal3f(ex/r,ey/r,ez/r);
         glTexCoord2f(i/(double)n,2*j/(double)n);
         glVertex3f(ex,ey,ez);
      }
      glEnd();
   }
}
