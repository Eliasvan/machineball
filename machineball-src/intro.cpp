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
#include <GL/glu.h>

#include <aldumb.h>

#include "intro.h"
#include "text.h"
#include "timer.h"
#include "options.h"

#include "mbdata.h"

extern AL_DUH_PLAYER *dp;
extern DUH *menuduh;

int menumusicisplaying;

extern GLuint bennykramekwebtex;

void showIntro(void)
{
	allegro_gl_begin();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
	allegro_gl_end();

	GLuint machineballtex;
	allegro_gl_set_texture_format(GL_RGBA);
	BITMAP *bmp = create_bitmap_ex(32, 256, 64);
	blit(&mb_machineball, bmp, 0, 0, 0, 0, 256, 64);
	machineballtex = allegro_gl_make_masked_texture(bmp);
	destroy_bitmap(bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	dp = al_start_duh(menuduh, 2, 0, (float)options.musicvol/255.0, 2048, 22050);
	menumusicisplaying=1;

	timer.install();
	double time;
	time=0;
	while(!(key[KEY_ESC] || time>23))
	{
		rest(1);
		time=timer.seconds();

		al_poll_duh(dp);

		allegro_gl_begin();
		
		glLoadIdentity();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(time<4)
		{
			glColor3f(time/4, time/4, time/4);
			text.print(120, 280, "BENNY KRAMEK PRESENTS");
			glBindTexture(GL_TEXTURE_2D, bennykramekwebtex);
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex2i(242, 180);
			glTexCoord2f(1, 0);
			glVertex2i(498, 180);
			glTexCoord2f(1, 1);
			glVertex2i(498, 212);
			glTexCoord2f(0, 1);
			glVertex2i(242, 212);
			glEnd();
		}
		else if(time<8)
		{
			glColor3f(1-((time-4)/4), 1-((time-4)/4), 1-((time-4)/4));
			text.print(120, 280, "BENNY KRAMEK PRESENTS");
			glBindTexture(GL_TEXTURE_2D, bennykramekwebtex);
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex2i(242, 180);
			glTexCoord2f(1, 0);
			glVertex2i(498, 180);
			glTexCoord2f(1, 1);
			glVertex2i(498, 212);
			glTexCoord2f(0, 1);
			glVertex2i(242, 212);
			glEnd();
		}

		glColor3f(0.5, 0.5, 0.5);
		text.begin();
		text.print(20, (int)(60*(time-8.7)), "THIS IS THE FUTURE.");
		text.print(20, (int)(60*(time-8.7))-50, "A FUTURE WHERE PRIMITIVE SPORTS");
		text.print(20, (int)(60*(time-8.7))-100, "NO LONGER EXIST. IN THIS WORLD,");
		text.print(20, (int)(60*(time-8.7))-150, "THERE IS ONLY ONE SPORT.");
		text.print(20, (int)(60*(time-8.7))-200, "THERE IS ONLY...");
		text.end();

		if(time>22 && time<=22.5)
		{
			glColor3f(1, 1, 1);
			glBindTexture(GL_TEXTURE_2D, machineballtex);
			glTranslatef(400, 300, 0);
			glScalef((time-22)*200, (time-22)*200, (time-22)*200);
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex2i(-4, -1);
			glTexCoord2f(1, 0);
			glVertex2i(4, -1);
			glTexCoord2f(1, 1);
			glVertex2i(4, 1);
			glTexCoord2f(0, 1);
			glVertex2i(-4, 1);
			glEnd();

		}
		if(time>22.5)
		{
			glColor3f(1, 1, 1);
			glBindTexture(GL_TEXTURE_2D, machineballtex);
			glTranslatef(400, 300+(time-22.5)*240, 0);
			glScalef(100-(time-22.5)*50, 100-(time-22.5)*50, 100-(time-22.5)*50);
			glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex2i(-4, -1);
			glTexCoord2f(1, 0);
			glVertex2i(4, -1);
			glTexCoord2f(1, 1);
			glVertex2i(4, 1);
			glTexCoord2f(0, 1);
			glVertex2i(-4, 1);
			glEnd();
			glColor4f(1, 1, 1, 1-((time-22.5)));
			glLoadIdentity();
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_QUADS);
			glVertex2i(0, 0);
			glVertex2i(800, 0);
			glVertex2i(800, 600);
			glVertex2i(0, 600);
			glEnd();
			glEnable(GL_TEXTURE_2D);
		}
		
		glFlush();
		allegro_gl_flip();
		allegro_gl_end();
	}

	glDeleteTextures(1, &machineballtex);
}
