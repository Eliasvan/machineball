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
#include <GL/gl.h>
#include <ode/ode.h>

#include <stdio.h>

#include "glode.h"
#include "ball.h"
#include "camera.h"
#include "text.h"
#include "court.h"
#include "machine.h"
#include "powerup.h"
#include "render.h"
#include "explosion.h"
#include "options.h"

extern int goals[2];
extern double goalcount;

extern double camerazoomtime;

extern int currentsong;

BodyModel bodymodel, pushermodel;

GLuint balltex;
GLuint shadowtex;
GLuint floortex;
GLuint walltex;
GLuint goaltex;
GLuint machinetex;
GLuint reflecttex;
GLuint poweruptex;
GLuint bennykramekwebtex;
GLuint weaponstex;
GLuint explosiontex;

extern double m, gametime;

extern int gameover;
extern int gamepaused;

int rendertime;

void BodyModel::kill(void)
{
	glDeleteLists(dlist, 1);
}

void BodyModel::render(void)
{
	glCallList(dlist);
}

void render(void)
{
	int i;

	allegro_gl_begin();

	glClearColor(0.1, 0.2, 0.2, 1);
	glClear(GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	const dReal * ballpos = dBodyGetPosition(ball.body);
	const dReal * machpos0 = dBodyGetPosition(machine[0].body[0]);
	const dReal * machpos1 = dBodyGetPosition(machine[1].body[0]);
	camera.adjust(ballpos[0], ballpos[1], machpos0[0], machpos0[1], machpos1[0], machpos1[1], camerazoomtime*court.x*2-50*camerazoomtime);
	GLfloat LightAmbient[]= { 0.7, 0.7, 0.7, 1.0 };
	GLfloat LightDiffuse[]= { 1.0, 1.0, 1.0, 1.0 };
	GLfloat LightPosition[]= { -300.0, -300.0, 1000.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	

	
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, floortex);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glColor3f(0.8, 0.8, 0.8);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-court.x-300, -court.y-300, 0);
	glTexCoord2f(10, 0);
	glVertex3f(court.x+300, -court.y-300, 0);
	glTexCoord2f(10, 10);
	glVertex3f(court.x+300, court.y+300, 0);
	glTexCoord2f(0, 10);
	glVertex3f(-court.x-300, court.y+300, 0);
	glEnd();

	glColor3f(1, 1, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	
	glBindTexture(GL_TEXTURE_2D, floortex);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 0);
	glVertex3f(-court.x-30, -court.y, 0);
	glTexCoord2f(court.x/30, 0);
	glVertex3f(court.x+30, -court.y, 0);
	glTexCoord2f(court.x/30, court.y/30);
	glVertex3f(court.x+30, court.y, 0);
	glTexCoord2f(0, court.y/30);
	glVertex3f(-court.x-30, court.y, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	glBegin(GL_QUADS);

	glNormal3f(0, 0, 1);

	glVertex3f(-1, -court.y, 0);
	glVertex3f(1, -court.y, 0);
	glVertex3f(1, court.y, 0);
	glVertex3f(-1, court.y, 0);
	
	glVertex3f(-court.x, court.y/2, 0);
	glVertex3f(-court.x/1.5, court.y/2, 0);
	glVertex3f(-court.x/1.5, court.y/2-1, 0);
	glVertex3f(-court.x, court.y/2-1, 0);

	glVertex3f(-court.x, -court.y/2, 0);
	glVertex3f(-court.x/1.5, -court.y/2, 0);
	glVertex3f(-court.x/1.5, -court.y/2+1, 0);
	glVertex3f(-court.x, -court.y/2+1, 0);

	glVertex3f(-court.x/1.5, court.y/2, 0);
	glVertex3f(-court.x/1.5+1, court.y/2, 0);
	glVertex3f(-court.x/1.5+1, -court.y/2, 0);
	glVertex3f(-court.x/1.5, -court.y/2, 0);

	glVertex3f(court.x, court.y/2, 0);
	glVertex3f(court.x/1.5, court.y/2, 0);
	glVertex3f(court.x/1.5, court.y/2-1, 0);
	glVertex3f(court.x, court.y/2-1, 0);

	glVertex3f(court.x, -court.y/2, 0);
	glVertex3f(court.x/1.5, -court.y/2, 0);
	glVertex3f(court.x/1.5, -court.y/2+1, 0);
	glVertex3f(court.x, -court.y/2+1, 0);

	glVertex3f(court.x/1.5, court.y/2, 0);
	glVertex3f(court.x/1.5+1, court.y/2, 0);
	glVertex3f(court.x/1.5+1, -court.y/2, 0);
	glVertex3f(court.x/1.5, -court.y/2, 0);

	glVertex3f(-court.x-1, -35, 0);
	glVertex3f(-court.x, -35, 0);
	glVertex3f(-court.x, 35, 0);
	glVertex3f(-court.x-1, 35, 0);

	glVertex3f(court.x, -35, 0);
	glVertex3f(court.x+1, -35, 0);
	glVertex3f(court.x+1, 35, 0);
	glVertex3f(court.x, 35, 0);
	
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, shadowtex);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glColor4f(0, 0, 0, 0.6);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(ballpos[0]-ball.getRadius()*1.2, ballpos[1]-ball.getRadius()*1.2, 0);
	glTexCoord2f(1, 0);
	glVertex3f(ballpos[0]+ball.getRadius()*1.2, ballpos[1]-ball.getRadius()*1.2, 0);
	glTexCoord2f(1, 1);
	glVertex3f(ballpos[0]+ball.getRadius()*1.2, ballpos[1]+ball.getRadius()*1.2, 0);
	glTexCoord2f(0, 1);
	glVertex3f(ballpos[0]-ball.getRadius()*1.2, ballpos[1]+ball.getRadius()*1.2, 0);
	glEnd();
	glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);

	glBindTexture(GL_TEXTURE_2D, walltex);
	glColor3f(1, 1, 1);

	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(0, 0);
	glVertex3f(-court.x+28.48581, court.y, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-court.x+28.48581, court.y, 30);
	glTexCoord2f(15, 0);
	glVertex3f(court.x-28.48581, court.y, 0);
	glTexCoord2f(15, 1);
	glVertex3f(court.x-28.48581, court.y, 30);
	glTexCoord2f(16, 0);
	glVertex3f(court.x-14, court.y-6, 0);
	glTexCoord2f(16, 1);
	glVertex3f(court.x-14, court.y-6, 30);
	glTexCoord2f(17, 0);
	glVertex3f(court.x-6, court.y-14, 0);
	glTexCoord2f(17, 1);
	glVertex3f(court.x-6, court.y-14, 30);
	glTexCoord2f(18, 0);
	glVertex3f(court.x, court.y-28.48581, 0);
	glTexCoord2f(18, 1);
	glVertex3f(court.x, court.y-28.48581, 30);
	glTexCoord2f(21, 0);
	glVertex3f(court.x, 35, 0);
	glTexCoord2f(21, 1);
	glVertex3f(court.x, 35, 30);
	glTexCoord2f(23, 0);
	glVertex3f(court.x+30, 35, 0);
	glTexCoord2f(23, 1);
	glVertex3f(court.x+30, 35, 30);
	glTexCoord2f(25, 0);
	glVertex3f(court.x+30, -35, 0);
	glTexCoord2f(25, 1);
	glVertex3f(court.x+30, -35, 30);
	glTexCoord2f(27, 0);
	glVertex3f(court.x, -35, 0);
	glTexCoord2f(27, 1);
	glVertex3f(court.x, -35, 30);
	glTexCoord2f(29, 0);
	glVertex3f(court.x, -court.y+28.48581, 0);
	glTexCoord2f(29, 1);
	glVertex3f(court.x, -court.y+28.48581, 30);
	glTexCoord2f(32, 0);
	glVertex3f(court.x-6, -court.y+14, 0);
	glTexCoord2f(32, 1);
	glVertex3f(court.x-6, -court.y+14, 30);
	glTexCoord2f(33, 0);
	glVertex3f(court.x-14, -court.y+6, 0);
	glTexCoord2f(33, 1);
	glVertex3f(court.x-14, -court.y+6, 30);
	glTexCoord2f(34, 0);
	glVertex3f(court.x-28.48581, -court.y, 0);
	glTexCoord2f(34, 1);
	glVertex3f(court.x-28.48581, -court.y, 30);
	glTexCoord2f(49, 0);
	glVertex3f(-court.x+28.48581, -court.y, 0);
	glTexCoord2f(49, 1);
	glVertex3f(-court.x+28.48581, -court.y, 30);
	glTexCoord2f(50, 0);
	glVertex3f(-court.x+14, -court.y+6, 0);
	glTexCoord2f(50, 1);
	glVertex3f(-court.x+14, -court.y+6, 30);
	glTexCoord2f(51, 0);
	glVertex3f(-court.x+6, -court.y+14, 0);
	glTexCoord2f(51, 1);
	glVertex3f(-court.x+6, -court.y+14, 30);
	glTexCoord2f(52, 0);
	glVertex3f(-court.x, -court.y+28.48581, 0);
	glTexCoord2f(52, 1);
	glVertex3f(-court.x, -court.y+28.48581, 30);
	glTexCoord2f(54, 0);
	glVertex3f(-court.x, -35, 0);
	glTexCoord2f(54, 1);
	glVertex3f(-court.x, -35, 30);
	glTexCoord2f(56, 0);
	glVertex3f(-court.x-30, -35, 0);
	glTexCoord2f(56, 1);
	glVertex3f(-court.x-30, -35, 30);
	glTexCoord2f(59, 0);
	glVertex3f(-court.x-30, 35, 0);
	glTexCoord2f(59, 1);
	glVertex3f(-court.x-30, 35, 30);
	glTexCoord2f(61, 0);
	glVertex3f(-court.x, 35, 0);
	glTexCoord2f(61, 1);
	glVertex3f(-court.x, 35, 30);
	glTexCoord2f(63, 0);
	glVertex3f(-court.x, court.y-28.48581, 0);
	glTexCoord2f(63, 1);
	glVertex3f(-court.x, court.y-28.48581, 30);
	glTexCoord2f(64, 0);
	glVertex3f(-court.x+6, court.y-14, 0);
	glTexCoord2f(64, 1);
	glVertex3f(-court.x+6, court.y-14, 30);
	glTexCoord2f(65, 0);
	glVertex3f(-court.x+14, court.y-6, 0);
	glTexCoord2f(65, 1);
	glVertex3f(-court.x+14, court.y-6, 30);
	glTexCoord2f(66, 0);
	glVertex3f(-court.x+28.48581, court.y, 0);
	glTexCoord2f(66, 1);
	glVertex3f(-court.x+28.48581, court.y, 30);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(0, 0);
	glVertex3f(-court.x/3*2, court.y*2, 0);
	glTexCoord2f(0, 2);
	glVertex3f(-court.x/3*2, court.y*2, 200);
	glTexCoord2f(1, 0);
	glVertex3f(-court.x/3*2, court.y+40, 0);
	glTexCoord2f(1, 2);
	glVertex3f(-court.x/3*2, court.y*1.5+20, 200);
	glTexCoord2f(2, 0);
	glVertex3f(-court.x/3*1, court.y+40, 0);
	glTexCoord2f(2, 2);
	glVertex3f(-court.x/3*1, court.y*1.5+20, 200);
	glTexCoord2f(3, 0);
	glVertex3f(-court.x/3*1, court.y*2, 0);
	glTexCoord2f(3, 2);
	glVertex3f(-court.x/3*1, court.y*2, 200);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(0, 0);
	glVertex3f(court.x/3*2, court.y*2, 0);
	glTexCoord2f(0, 2);
	glVertex3f(court.x/3*2, court.y*2, 200);
	glTexCoord2f(1, 0);
	glVertex3f(court.x/3*2, court.y+40, 0);
	glTexCoord2f(1, 2);
	glVertex3f(court.x/3*2, court.y*1.5+20, 200);
	glTexCoord2f(2, 0);
	glVertex3f(court.x/3*1, court.y+40, 0);
	glTexCoord2f(2, 2);
	glVertex3f(court.x/3*1, court.y*1.5+20, 200);
	glTexCoord2f(3, 0);
	glVertex3f(court.x/3*1, court.y*2, 0);
	glTexCoord2f(3, 2);
	glVertex3f(court.x/3*1, court.y*2, 200);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(0, 0);
	glVertex3f(-court.x/3*2, -court.y*2, 0);
	glTexCoord2f(0, 2);
	glVertex3f(-court.x/3*2, -court.y*2, 200);
	glTexCoord2f(1, 0);
	glVertex3f(-court.x/3*2, -court.y-40, 0);
	glTexCoord2f(1, 2);
	glVertex3f(-court.x/3*2, -court.y*1.5-20, 200);
	glTexCoord2f(2, 0);
	glVertex3f(-court.x/3*1, -court.y-40, 0);
	glTexCoord2f(2, 2);
	glVertex3f(-court.x/3*1, -court.y*1.5-20, 200);
	glTexCoord2f(3, 0);
	glVertex3f(-court.x/3*1, -court.y*2, 0);
	glTexCoord2f(3, 2);
	glVertex3f(-court.x/3*1, -court.y*2, 200);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glTexCoord2f(0, 0);
	glVertex3f(court.x/3*2, -court.y*2, 0);
	glTexCoord2f(0, 2);
	glVertex3f(court.x/3*2, -court.y*2, 200);
	glTexCoord2f(1, 0);
	glVertex3f(court.x/3*2, -court.y-40, 0);
	glTexCoord2f(1, 2);
	glVertex3f(court.x/3*2, -court.y*1.5-20, 200);
	glTexCoord2f(2, 0);
	glVertex3f(court.x/3*1, -court.y-40, 0);
	glTexCoord2f(2, 2);
	glVertex3f(court.x/3*1, -court.y*1.5-20, 200);
	glTexCoord2f(3, 0);
	glVertex3f(court.x/3*1, -court.y*2, 0);
	glTexCoord2f(3, 2);
	glVertex3f(court.x/3*1, -court.y*2, 200);
	glEnd();

	
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, balltex);
	glPushMatrix();
	position_gl(ball.body);
	glColor3f(0.5, 0.5, 0.5);
	draw_sphere(ball.getRadius(), 24);
	glPopMatrix();
	int j;
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, machinetex);
	glEnable(GL_CULL_FACE);
	for(i=0; i<2; i++)
	{
		glPushMatrix();
		position_gl(machine[i].body[0]);
		if(i==0)
			glColor3f(0.5, 0.3, 0.2);
		else
			glColor3f(0.5, 0.5, 0.5);
		bodymodel.render();
		if(machine[i].powerupcount>0 && machine[i].poweruptype==0)
		{
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glDisable(GL_LIGHTING);
			glDisable(GL_CULL_FACE);
			glBegin(GL_QUADS);
			glColor4f(1, 0, 0, 0.7);
			glVertex3f(5.5, -12, 0);
			glVertex3f(3.5, -12, 0);
			glColor4f(1, 1, 0, 0);
			glVertex3f(4, -25, 0);
			glVertex3f(5, -25, 0);

			glColor4f(1, 0, 0, 0.7);
			glVertex3f(-3.5, -12, 0);
			glVertex3f(-5.5, -12, 0);
			glColor4f(1, 1, 0, 0);
			glVertex3f(-5, -25, 0);
			glVertex3f(-4, -25, 0);
			glEnd();
			glEnable(GL_CULL_FACE);
			glEnable(GL_LIGHTING);
			glDisable(GL_BLEND);
			glEnable(GL_TEXTURE_2D);
		}
		glPopMatrix();

		glPushMatrix();
		position_gl(machine[i].body[1]);
		if(machine[i].getEnergy()==4)
			glColor3f(0.5, 0.5, 0.5);
		else
			glColor3f(0.35, 0.3, 0.3);
		pushermodel.render();
		glPopMatrix();
	}
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	glBindTexture(GL_TEXTURE_2D, reflecttex);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glColor3f(1, 1, 1);
	for(i=0; i<2; i++)
	{
		glPushMatrix();
		position_gl(machine[i].body[0]);
		glBegin(GL_QUADS);
		glNormal3f(0, 0.8, 0.4);
		glVertex3f(0, 7.4298439026, 1.25);
		glNormal3f(0, 0.7, 0.7);
		glVertex3f(0, 4.8880519867, 2.75);
		glNormal3f(0.6, 0.7, 0.7);
		glVertex3f(7.4881892204, 4.8880519867, 2.75);
		glNormal3f(0.6, 0.8, 0.4);
		glVertex3f(7.4881892204, 7.4298439026, 1.25);
		glNormal3f(0, 0.8, 0.4);
		glVertex3f(0, 7.4298439026, 1.25);
		glNormal3f(0, 0.7, 0.7);
		glVertex3f(0, 4.8880519867, 2.75);
		glNormal3f(-0.6, 0.7, 0.7);
		glVertex3f(-7.4881892204, 4.8880519867, 2.75);
		glNormal3f(-0.6, 0.8, 0.4);
		glVertex3f(-7.4881892204, 7.4298439026, 1.25);
		glEnd();
		glPopMatrix();
	}

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	for(i=0; i<2; i++)
	{
		if(machine[i].shieldcount>0)
		{
			const dReal * pos0 = dBodyGetPosition(machine[i].body[0]);
			const dReal * pos1 = dBodyGetPosition(machine[i].body[1]);
			double length=sqrt(
					(pos0[0]-pos1[0])*(pos0[0]-pos1[0]) +
					(pos0[1]-pos1[1])*(pos0[1]-pos1[1]) +
					(pos0[2]-pos1[2])*(pos0[2]-pos1[2]))+3;
			glEnable(GL_BLEND);
			glEnable(GL_LIGHTING);
			glPushMatrix();
			position_gl(machine[i].body[0]);
			glColor4f(0, 1, 0, machine[i].shieldcount>1?((machine[i].shieldcount-(int)machine[i].shieldcount)/6.0+0.2):machine[i].shieldcount/2);
			glBegin(GL_QUADS);
			glNormal3f(0, 0, 1);
			glVertex3f(-12, -14, 6);
			glVertex3f(12, -14, 6);
			glVertex3f(12, length, 6);
			glVertex3f(-12, length, 6);
			glNormal3f(0, 0, -1);
			glVertex3f(12, -14, -6);
			glVertex3f(-12, -14, -6);
			glVertex3f(-12, length, -6);
			glVertex3f(12, length, -6);
			glNormal3f(0, -1, 0);
			glVertex3f(-12, -14, -6);
			glVertex3f(12, -14, -6);
			glVertex3f(12, -14, 6);
			glVertex3f(-12, -14, 6);
			glNormal3f(0, 1, 0);
			glVertex3f(12, length, -6);
			glVertex3f(-12, length, -6);
			glVertex3f(-12, length, 6);
			glVertex3f(12, length, 6);
			glNormal3f(-1, 0, 0);
			glVertex3f(-12, length, -6);
			glVertex3f(-12, -14, -6);
			glVertex3f(-12, -14, 6);
			glVertex3f(-12, length, 6);
			glNormal3f(1, 0, 0);
			glVertex3f(12, -14, -6);
			glVertex3f(12, length, -6);
			glVertex3f(12, length, 6);
			glVertex3f(12, -14, 6);
			glEnd();
			glPopMatrix();
			glDisable(GL_BLEND);
			glDisable(GL_LIGHTING);
		}
		if(machine[i].meshatekcount>0)
		{
			const dReal * pos0 = dBodyGetPosition(machine[i].body[0]);
			const dReal * pos1 = dBodyGetPosition(machine[i].body[1]);
			double length=sqrt(
					(pos0[0]-pos1[0])*(pos0[0]-pos1[0]) +
					(pos0[1]-pos1[1])*(pos0[1]-pos1[1]) +
					(pos0[2]-pos1[2])*(pos0[2]-pos1[2]))+3;
			glEnable(GL_BLEND);
			glEnable(GL_LIGHTING);
			glPushMatrix();
			position_gl(machine[i].body[0]);
			glColor4f(1, 1, 1, machine[i].meshatekcount>1?((machine[i].meshatekcount-(int)machine[i].meshatekcount)/6.0+0.2):machine[i].meshatekcount/2);
			glBegin(GL_QUADS);
			glNormal3f(0, 0, 1);
			glVertex3f(-12, -14, 6);
			glVertex3f(12, -14, 6);
			glVertex3f(12, length, 6);
			glVertex3f(-12, length, 6);
			glNormal3f(0, 0, -1);
			glVertex3f(12, -14, -6);
			glVertex3f(-12, -14, -6);
			glVertex3f(-12, length, -6);
			glVertex3f(12, length, -6);
			glNormal3f(0, -1, 0);
			glVertex3f(-12, -14, -6);
			glVertex3f(12, -14, -6);
			glVertex3f(12, -14, 6);
			glVertex3f(-12, -14, 6);
			glNormal3f(0, 1, 0);
			glVertex3f(12, length, -6);
			glVertex3f(-12, length, -6);
			glVertex3f(-12, length, 6);
			glVertex3f(12, length, 6);
			glNormal3f(-1, 0, 0);
			glVertex3f(-12, length, -6);
			glVertex3f(-12, -14, -6);
			glVertex3f(-12, -14, 6);
			glVertex3f(-12, length, 6);
			glNormal3f(1, 0, 0);
			glVertex3f(12, -14, -6);
			glVertex3f(12, length, -6);
			glVertex3f(12, length, 6);
			glVertex3f(12, -14, 6);
			glEnd();
			glPopMatrix();
			glDisable(GL_BLEND);
			glDisable(GL_LIGHTING);
		}
	}
	glDisable(GL_CULL_FACE);
	
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, poweruptex);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glDepthMask(GL_FALSE);
	for(i=0; i<powerups.numpowerups; i++)
	{
		glPushMatrix();
		glTranslatef(powerups.getPowerup(i).x, powerups.getPowerup(i).y, 5);
		glRotatef(powerups.spin, 1, 0, 0);
		if(powerups.getPowerup(i).type==0)
			glColor3f(0.33, 0.33, 1);
		else if(powerups.getPowerup(i).type==1)
			glColor3f(0.33, 1, 0.33);
		else if(powerups.getPowerup(i).type==2)
			glColor3f(1, 0.33, 1);
		else if(powerups.getPowerup(i).type==3)
			glColor3f(1, 1, 0.33);
		else if(powerups.getPowerup(i).type==4)
			glColor3f(0.4, 0.4, 0.4);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-5, -5, 0);
		glTexCoord2f(1, 0);
		glVertex3f(5, -5, 0);
		glTexCoord2f(1, 1);
		glVertex3f(5, 5, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-5, 5, 0);
		glEnd();
		glRotatef(90+powerups.spin*2, 0, 1, 0);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-5, -5, 0);
		glTexCoord2f(1, 0);
		glVertex3f(5, -5, 0);
		glTexCoord2f(1, 1);
		glVertex3f(5, 5, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-5, 5, 0);
		glEnd();
		glRotatef(90+powerups.spin*4, 1, 0, 0);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-5, -5, 0);
		glTexCoord2f(1, 0);
		glVertex3f(5, -5, 0);
		glTexCoord2f(1, 1);
		glVertex3f(5, 5, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-5, 5, 0);
		glEnd();
		glPopMatrix();
	}

	glBindTexture(GL_TEXTURE_2D, explosiontex);
	for(i=0; i<explosions.numexplosions; i++)
	{
		glPushMatrix();
		glTranslatef(explosions.getExplosion(i).x, explosions.getExplosion(i).y, explosions.getExplosion(i).z);
		glScalef(explosions.getExplosion(i).curradius*1.4, explosions.getExplosion(i).curradius*1.4, 1);
		glColor4f(1, 1, 1, 1.0-explosions.getExplosion(i).life/explosions.getExplosion(i).lifetime);
		glBegin(GL_QUADS);
		if(explosions.getExplosion(i).life/explosions.getExplosion(i).lifetime<=0.25)
		{
			glTexCoord2f(0, 0.5);
			glVertex3f(-1, -1, 0);
			glTexCoord2f(0.5, 0.5);
			glVertex3f(1, -1, 0);
			glTexCoord2f(0.5, 1);
			glVertex3f(1, 1, 0);
			glTexCoord2f(0, 1);
			glVertex3f(-1, 1, 0);
		}
		else if(explosions.getExplosion(i).life/explosions.getExplosion(i).lifetime<=0.5)
		{
			glTexCoord2f(0.5, 0.5);
			glVertex3f(-1, -1, 0);
			glTexCoord2f(1, 0.5);
			glVertex3f(1, -1, 0);
			glTexCoord2f(1, 1);
			glVertex3f(1, 1, 0);
			glTexCoord2f(0.5, 1);
			glVertex3f(-1, 1, 0);
		}
		else if(explosions.getExplosion(i).life/explosions.getExplosion(i).lifetime<=0.75)
		{
			glTexCoord2f(0, 0);
			glVertex3f(-1, -1, 0);
			glTexCoord2f(0.5, 0);
			glVertex3f(1, -1, 0);
			glTexCoord2f(0.5, 0.5);
			glVertex3f(1, 1, 0);
			glTexCoord2f(0, 0.5);
			glVertex3f(-1, 1, 0);
		}
		else
		{
			glTexCoord2f(0.5, 0);
			glVertex3f(-1, -1, 0);
			glTexCoord2f(1, 0);
			glVertex3f(1, -1, 0);
			glTexCoord2f(1, 0.5);
			glVertex3f(1, 1, 0);
			glTexCoord2f(0.5, 0.5);
			glVertex3f(-1, 1, 0);
		}
		glEnd();
		glPopMatrix();
	}
	
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, weaponstex);

	glColor3f(1, 1, 1);
	for(i=0; i<mines.nummines; i++)
	{
		glPushMatrix();
		if(mines.getMine(i).danger)
		{
			glTranslatef(mines.getMine(i).x, mines.getMine(i).y, 6);
			glBegin(GL_QUADS);
			glTexCoord2f(0.5, 0);
			glVertex3f(-3, -3, 0);
			glTexCoord2f(1, 0);
			glVertex3f(3, -3, 0);
			glTexCoord2f(1, 0.5);
			glVertex3f(3, 3, 0);
			glTexCoord2f(0.5, 0.5);
			glVertex3f(-3, 3, 0);
			glEnd();
		}
		else
		{
			glTranslatef(mines.getMine(i).x, mines.getMine(i).y, 3);
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(-3, -3, 0);
			glTexCoord2f(0.5, 0);
			glVertex3f(3, -3, 0);
			glTexCoord2f(0.5, 0.5);
			glVertex3f(3, 3, 0);
			glTexCoord2f(0, 0.5);
			glVertex3f(-3, 3, 0);
			glEnd();
		}
		glPopMatrix();
	}

	glDepthMask(GL_TRUE);

	for(i=0; i<projectiles.numprojectiles; i++)
	{
		glPushMatrix();
		glTranslatef(projectiles.getProjectile(i).x, projectiles.getProjectile(i).y, 6);
		glRotatef(projectiles.getProjectile(i).angle, 0, 0, -1);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		glColor4f(1, 0, 0, 0.7);
		glVertex3f(1, 0, 0);
		glVertex3f(-1, 0, 0);
		glColor4f(1, 1, 0, 0);
		glVertex3f(-0.5, -projectiles.getProjectile(i).length, 0);
		glVertex3f(0.5, -projectiles.getProjectile(i).length, 0);
		glEnd();
		glEnable(GL_TEXTURE_2D);
		glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
		if(projectiles.getProjectile(i).type==0)
		{
			glTexCoord2f(0, 0.5);
			glVertex3f(-5, -5, 0);
			glTexCoord2f(0.5, 0.5);
			glVertex3f(5, -5, 0);
			glTexCoord2f(0.5, 1);
			glVertex3f(5, 5, 0);
			glTexCoord2f(0, 1);
			glVertex3f(-5, 5, 0);
		}
		else
		{
			glTexCoord2f(0.5, 0.5);
			glVertex3f(-5, -5, 0);
			glTexCoord2f(1, 0.5);
			glVertex3f(5, -5, 0);
			glTexCoord2f(1, 1);
			glVertex3f(5, 5, 0);
			glTexCoord2f(0.5, 1);
			glVertex3f(-5, 5, 0);
		}
		glEnd();
		glPopMatrix();
	}

		
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glDisable(GL_DEPTH_TEST);

	if(options.logoshow)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 800, 0, 600, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glBindTexture(GL_TEXTURE_2D, bennykramekwebtex);
		glColor4f(1, 1, 1, 0.7);
		glEnable(GL_BLEND);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex2i(544, 0);
		glTexCoord2f(1, 0);
		glVertex2i(800, 0);
		glTexCoord2f(1, 1);
		glVertex2i(800, 32);
		glTexCoord2f(0, 1);
		glVertex2i(544, 32);
		glEnd();
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	if(goalcount>0)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 800, 0, 600, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glColor4f(0.8, 1, 0.8, goalcount/6.0);
		glBegin(GL_QUADS);
		glVertex2i(0, 0);
		glVertex2i(800, 0);
		glVertex2i(800, 600);
		glVertex2i(0, 600);
		glEnd();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, goaltex);
		glColor4f(1, 1, 1, goalcount/4.0);
		glTranslatef(400, 300, 0);
		glScalef(1-goalcount/4.0, 1-goalcount/4.0, 0);
		glRotatef(goalcount/4.0*360+180, 0, 0, 1);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex2i(-400, -300);
		glTexCoord2f(1, 0);
		glVertex2i(400, -300);
		glTexCoord2f(1, 1);
		glVertex2i(400, 300);
		glTexCoord2f(0, 1);
		glVertex2i(-400, 300);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	if(gameover)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 800, 0, 600, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glColor4f(0, 0, 0, 0.6);
		glBegin(GL_QUADS);
		glVertex2i(100, 100);
		glVertex2i(700, 100);
		glVertex2i(700, 500);
		glVertex2i(100, 500);
		glEnd();
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	if(gamepaused)
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 800, 0, 600, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glColor4f(0, 0, 0, 0.6);
		glBegin(GL_QUADS);
		glVertex2i(100, 100);
		glVertex2i(700, 100);
		glVertex2i(700, 500);
		glVertex2i(100, 500);
		glEnd();
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	
	text.begin();
	char a[10];
	if(options.fpsshow)
	{
		glColor3f(0.3, 0.3, 0.3);
		sprintf(a, "%2.0f", 1.0/(m*4));
		text.print(0, 0, a);
	}
	if(rendertime && gametime>=0)
	{
		sprintf(a, "%2.2d:%2.2d:%2.2d", (int)(gametime/60), (int)(gametime)%60, (int)((gametime-(int)(gametime))*100));
		glColor3f(0, 0, 0);
		text.print(332, 558, a);
		glColor3f(1, 1, 1);
		if(gametime<=10)
			glColor3f(1, 0, 0);
		text.print(330, 560, a);
	}
	if(rendertime && gametime<0)
	{
		sprintf(a, "%2.2d:%2.2d:%2.2d", 0, 0, 0);
		glColor3f(0, 0, 0);
		text.print(332, 558, a);
		glColor3f(1, 1, 1);
		glColor3f(1, 0, 0);
		text.print(330, 560, a);
	}
	glColor3f(0, 0, 0);
	sprintf(a, "%d", (int)goals[0]);
	text.print(102, 558, a);
	glColor3f(0, 0, 1);
	text.print(100, 560, a);
	glColor3f(0, 0, 0);
	sprintf(a, "%d", (int)goals[1]);
	text.print(682, 558, a);
	glColor3f(0, 0, 1);
	text.print(680, 560, a);
	if(gameover)
	{
		glColor3f(1, 1, 1);
		text.print(290, 400, "GAME OVER");
		if(goals[0]>goals[1])
		{
			glColor3f(1, 0.6, 0.4);
			text.print(240, 280, "PLAYER 1 WINS!");
			glColor3f(1, 1, 1);
			text.print(200, 240, "PLAYER 2: YOU SUCK!");
		}
		if(goals[1]>goals[0])
		{
			glColor3f(1, 1, 1);
			text.print(240, 280, "PLAYER 2 WINS!");
			glColor3f(1, 0.6, 0.4);
			text.print(200, 240, "PLAYER 1: YOU SUCK!");
		}
		if(goals[0]==goals[1])
		{
			text.print(300, 280, "TIE GAME!");
			text.print(240, 240, "YOU BOTH SUCK!");
		}
		glColor3f(1, 1, 1);
		text.print(250, 140, "PRESS ESCAPE");
	}
	if(gamepaused)
	{
		glColor3f(1, 0, 0);
		text.print(340, 400, "PAUSE");

		glColor3f(1, 1, 1);
		text.print(120, 340, "CURRENT SONG:");
		text.print(105, 280, "<");
		text.print(680, 280, ">");
		int mama_of;
		for(mama_of=0; mama_of<12; mama_of++)
		{
			char turd[3];
			sprintf(turd, "%d", mama_of+1);
			if(mama_of==currentsong)
				glColor3f(1, 1, 0);
			else
				glColor3f(0.6, 0.6, 1);
			text.print(135+mama_of*45, 280, turd);
		}
					

		glColor3f(1, 1, 1);
		text.print(120, 160, "PRESS ESCAPE TO RESUME");
		text.print(200, 120, "PRESS F11 TO QUIT");
	}
	text.end();
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	glFlush();
	allegro_gl_flip();
	allegro_gl_end();
}

