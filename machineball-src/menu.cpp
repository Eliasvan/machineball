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

#include "mbdata.h"

#include "menu.h"
#include "intro.h"
#include "text.h"
#include "timer.h"
#include "controls.h"
#define dSINGLE
#include "glode.h"
#include "render.h"
#include "options.h"

extern GLuint bennykramekwebtex;

extern AL_DUH_PLAYER *dp;
extern DUH *menuduh;

int mainMenu(void)
{
	allegro_gl_begin();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 4.0/3.0, 1.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLuint pat01tex;
	allegro_gl_set_texture_format(GL_RGB);
	pat01tex = allegro_gl_make_texture(&mb_pattern01_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	GLuint pat02tex;
	allegro_gl_set_texture_format(GL_RGB);
	pat02tex = allegro_gl_make_texture(&mb_pattern02_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	allegro_gl_end();

	GLuint machineballtex;
	allegro_gl_set_texture_format(GL_RGBA);
	BITMAP *bmp = create_bitmap_ex(32, 256, 64);
	blit(&mb_machineball_bmp, bmp, 0, 0, 0, 0, 256, 64);
	machineballtex = allegro_gl_make_masked_texture(bmp);
	destroy_bitmap(bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(menumusicisplaying==0)
	{
		dp = al_start_duh(menuduh, 2, 0, (float)options.musicvol/255.0, 2048, 22050);
		menumusicisplaying=1;
	}

	timer.install();
	double i=0;
	int choice=0;
	int upkey=0, downkey=0;
	while(!(key[KEY_ENTER] || key[KEY_SPACE] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL)))
	{
		rest(1);
		i+=timer.seconds()/2.0;
		timer.reset();

		poll_joystick();

		al_poll_duh(dp);

		if(i>1)
			i=i-1;
		if((key[KEY_DOWN] || controls[0].keydown(KEYDOWN)) && downkey==0)
		{
			choice++;
			downkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if((key[KEY_UP] || controls[0].keydown(KEYUP)) && upkey==0)
		{
			choice--;
			upkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if(!(key[KEY_DOWN] || controls[0].keydown(KEYDOWN)))
			downkey=0;
		if(!(key[KEY_UP] || controls[0].keydown(KEYUP)))
			upkey=0;
		if(choice>3)
			choice=0;
		if(choice<0)
			choice=3;

		allegro_gl_begin();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 800, 0, 600, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pat01tex);
		glEnable(GL_BLEND);
		glColor4f(sin(M_PI*i)/2+0.3, sin(M_PI*i)/2+0.3, 0.7, 0.4);
		glBegin(GL_QUADS);
		glTexCoord2f(-i, i);
		glVertex2i(0, 0);
		glTexCoord2f(800.0/256.0-i, i);
		glVertex2i(800, 0);
		glTexCoord2f(800.0/256.0-i, 600.0/256.0+i);
		glVertex2i(800, 600);
		glTexCoord2f(-i, 600.0/256.0+i);
		glVertex2i(0, 600);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, pat02tex);
		glColor4f(0.4, 0.4, 0.4, sin(M_PI*i)/2*0.6);
		glBegin(GL_QUADS);
		glTexCoord2f(i, i);
		glVertex2i(0, 0);
		glTexCoord2f(800.0/256.0+i, i);
		glVertex2i(800, 0);
		glTexCoord2f(800.0/256.0+i, 600.0/256.0+i);
		glVertex2i(800, 600);
		glTexCoord2f(i, 600.0/256.0+i);
		glVertex2i(0, 600);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, machineballtex);
		glColor3f(1, 1, 1);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex2i(100, 350);
		glTexCoord2f(1, 0);
		glVertex2i(700, 350);
		glTexCoord2f(1, 1);
		glVertex2i(700, 500);
		glTexCoord2f(0, 1);
		glVertex2i(100, 500);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, bennykramekwebtex);
		glColor4f(0, 0, 1, 0.6);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex2i(490, 0);
		glTexCoord2f(1, 0);
		glVertex2i(746, 0);
		glTexCoord2f(1, 1);
		glVertex2i(746, 32);
		glTexCoord2f(0, 1);
		glVertex2i(490, 32);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		
		glTranslated(130, 100+(3-choice)*50, 0);
		glBegin(GL_QUADS);
		glColor4f(0, 0, 1, 0.2);
		glVertex2i(50, 0);
		glVertex2i(450, 0);
		glVertex2i(450, 42);
		glVertex2i(50, 42);
		glEnd();
		glDisable(GL_BLEND);
		glBegin(GL_TRIANGLES);
		glColor3f(0, 0, 1);
		glVertex2i(0, 0);
		glVertex2i(40, 20);
		glVertex2i(0, 40);
		glEnd();

		text.begin();
		glColor4f(0, 0, 1, 0.6);
		text.print(40, 0, "BENNY KRAMEK 2002");
		glColor3f(1, 1, 0.8);
		text.print(200, 250, "HUMAN VS HUMAN");
		text.print(200, 200, "HUMAN VS COMP");
		text.print(200, 150, "OPTIONS");
		text.print(200, 100, "QUIT");
		text.end();

		glFlush();
		allegro_gl_flip();
		allegro_gl_end();
	}
	play_sample(&mb_menusel_wav, 255, 128, 1000, 0);
	while(key[KEY_ENTER] || key[KEY_SPACE] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL))
	{
		poll_joystick();
		al_poll_duh(dp);
	}
	allegro_gl_begin();
	glDeleteTextures(1, &pat01tex);
	glDeleteTextures(1, &pat02tex);
	glDeleteTextures(1, &machineballtex);
	allegro_gl_end();
	return choice;
}

int ballMenu(void)
{
	allegro_gl_begin();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 4.0/3.0, 1.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glLoadIdentity();
	GLfloat LightAmbient[]= { 0.7, 0.7, 0.7, 1.0 };
	GLfloat LightDiffuse[]= { 1.0, 1.0, 1.0, 1.0 };
	GLfloat LightPosition[]= { -10.0, 0.0, 7.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);
	glDisable(GL_LIGHTING);

	GLuint pat01tex;
	allegro_gl_set_texture_format(GL_RGB);
	pat01tex = allegro_gl_make_texture(&mb_pattern01_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	GLuint pat02tex;
	allegro_gl_set_texture_format(GL_RGB);
	pat02tex = allegro_gl_make_texture(&mb_pattern02_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLuint balltex[6];
	allegro_gl_set_texture_format(GL_RGB);
	balltex[0] = allegro_gl_make_texture(&mb_ball01_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	balltex[1] = allegro_gl_make_texture(&mb_ball02_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	balltex[2] = allegro_gl_make_texture(&mb_ball03_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	balltex[3] = allegro_gl_make_texture(&mb_ball04_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	balltex[4] = allegro_gl_make_texture(&mb_ball05_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	balltex[5] = allegro_gl_make_texture(&mb_ball06_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	allegro_gl_end();
	
	timer.install();
	double i=0;
	int choice=0;
	int leftkey=0, rightkey=0;
	while(!(key[KEY_ENTER] || key[KEY_SPACE] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL)))
	{
		rest(10);
		i+=timer.seconds()/2.0;
		timer.reset();

		poll_joystick();
		al_poll_duh(dp);

		if(i>1)
			i=i-1;
		if((key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)) && rightkey==0)
		{
			choice++;
			rightkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if((key[KEY_LEFT] || controls[0].keydown(KEYLEFT)) && leftkey==0)
		{
			choice--;
			leftkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if(!(key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)))
			rightkey=0;
		if(!(key[KEY_LEFT] || controls[0].keydown(KEYLEFT)))
			leftkey=0;
		if(choice>5)
			choice=0;
		if(choice<0)
			choice=5;

		allegro_gl_begin();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 800, 0, 600, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pat01tex);
		glEnable(GL_BLEND);
		glColor4f(sin(M_PI*i)/2+0.3, sin(M_PI*i)/2+0.3, 0.7, 0.4);
		glBegin(GL_QUADS);
		glTexCoord2f(-i, i);
		glVertex2i(0, 0);
		glTexCoord2f(800.0/256.0-i, i);
		glVertex2i(800, 0);
		glTexCoord2f(800.0/256.0-i, 600.0/256.0+i);
		glVertex2i(800, 600);
		glTexCoord2f(-i, 600.0/256.0+i);
		glVertex2i(0, 600);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, pat02tex);
		glColor4f(0.4, 0.4, 0.4, sin(M_PI*i)/2*0.6);
		glBegin(GL_QUADS);
		glTexCoord2f(i, i);
		glVertex2i(0, 0);
		glTexCoord2f(800.0/256.0+i, i);
		glVertex2i(800, 0);
		glTexCoord2f(800.0/256.0+i, 600.0/256.0+i);
		glVertex2i(800, 600);
		glTexCoord2f(i, 600.0/256.0+i);
		glVertex2i(0, 600);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glTranslatef(600, 200, 0);
		glBegin(GL_TRIANGLES);
		glColor3f(0, 0, 1);
		glVertex2i(0, 0);
		glVertex2i(40, 20);
		glVertex2i(0, 40);
		glEnd();
		glLoadIdentity();
		glTranslatef(180, 200, 0);
		glBegin(GL_TRIANGLES);
		glColor3f(0, 0, 1);
		glVertex2i(40, 0);
		glVertex2i(0, 20);
		glVertex2i(40, 40);
		glEnd();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, balltex[choice]);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, 4.0/3.0, 1.0, 300.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0, -5, -50);
		glRotatef(i/1.0*360.0, 0, 1, 0);
		glRotatef(20, 1, 0, 0);
		glDisable(GL_BLEND);
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glColor3f(0.5, 0.5, 0.5);
		if(choice==0)
			draw_sphere(5, 32);
		else if(choice==1)
			draw_sphere(8, 32);
		else if(choice==2)
			draw_sphere(7, 32);
		else if(choice==3)
			draw_sphere(4, 32);
		else if(choice==4)
			draw_sphere(10, 32);
		else if(choice==5)
			draw_sphere(11, 32);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		
		text.begin();
		glColor3f(1, 1, 0.8);
		text.print(230, 450, "SELECT THE BALL");
		if(choice==0)
			text.print(250, 20, "OFFICIAL BALL");
		else if(choice==1)
			text.print(320, 20, "BOULDER");
		else if(choice==2)
			text.print(280, 20, "BEACH BALL");
		else if(choice==3)
			text.print(270, 20, "SOCCER BALL");
		else if(choice==4)
			text.print(220, 20, "BIG ORANGE BALL");
		else if(choice==5)
			text.print(260, 20, "INFLATED TUX");
		text.end();

		glFlush();
		allegro_gl_flip();
		allegro_gl_end();
	}
	play_sample(&mb_menusel_wav, 255, 128, 1000, 0);
	while(key[KEY_ENTER] || key[KEY_SPACE] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL))
	{
		poll_joystick();
		al_poll_duh(dp);
	}
	allegro_gl_begin();
	glDeleteTextures(1, &pat01tex);
	glDeleteTextures(1, &pat02tex);
	glDeleteTextures(6, balltex);
	allegro_gl_end();
	return choice;
}

int courtMenu(void)
{
	allegro_gl_begin();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 4.0/3.0, 1.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glLoadIdentity();
	GLfloat LightAmbient[]= { 0.7, 0.7, 0.7, 1.0 };
	GLfloat LightDiffuse[]= { 1.0, 1.0, 1.0, 1.0 };
	GLfloat LightPosition[]= { -10.0, 0.0, 7.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);
	glDisable(GL_LIGHTING);

	GLuint pat01tex;
	allegro_gl_set_texture_format(GL_RGB);
	pat01tex = allegro_gl_make_texture(&mb_pattern01_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	GLuint pat02tex;
	allegro_gl_set_texture_format(GL_RGB);
	pat02tex = allegro_gl_make_texture(&mb_pattern02_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	allegro_gl_end();
	
	timer.install();
	double i=0;
	int cs=1;
	int leftkey=0, rightkey=0;
	while(!(key[KEY_ENTER] || key[KEY_SPACE] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL)))
	{
		rest(10);
		i+=timer.seconds()/2.0;
		timer.reset();

		poll_joystick();
		al_poll_duh(dp);

		if(i>1)
			i=i-1;
		if((key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)) && rightkey==0)
		{
			cs++;
			rightkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if((key[KEY_LEFT] || controls[0].keydown(KEYLEFT)) && leftkey==0)
		{
			cs--;
			leftkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if(!(key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)))
			rightkey=0;
		if(!(key[KEY_LEFT] || controls[0].keydown(KEYLEFT)))
			leftkey=0;
		if(cs>3)
			cs=0;
		if(cs<0)
			cs=3;

		allegro_gl_begin();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 800, 0, 600, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pat01tex);
		glEnable(GL_BLEND);
		glColor4f(sin(M_PI*i)/2+0.3, sin(M_PI*i)/2+0.3, 0.7, 0.4);
		glBegin(GL_QUADS);
		glTexCoord2f(-i, i);
		glVertex2i(0, 0);
		glTexCoord2f(800.0/256.0-i, i);
		glVertex2i(800, 0);
		glTexCoord2f(800.0/256.0-i, 600.0/256.0+i);
		glVertex2i(800, 600);
		glTexCoord2f(-i, 600.0/256.0+i);
		glVertex2i(0, 600);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, pat02tex);
		glColor4f(0.4, 0.4, 0.4, sin(M_PI*i)/2*0.6);
		glBegin(GL_QUADS);
		glTexCoord2f(i, i);
		glVertex2i(0, 0);
		glTexCoord2f(800.0/256.0+i, i);
		glVertex2i(800, 0);
		glTexCoord2f(800.0/256.0+i, 600.0/256.0+i);
		glVertex2i(800, 600);
		glTexCoord2f(i, 600.0/256.0+i);
		glVertex2i(0, 600);
		glEnd();

		glDisable(GL_TEXTURE_2D);

		glTranslatef(640, 200, 0);
		glBegin(GL_TRIANGLES);
		glColor3f(0, 0, 1);
		glVertex2i(0, 0);
		glVertex2i(40, 20);
		glVertex2i(0, 40);
		glEnd();
		glLoadIdentity();
		glTranslatef(140, 200, 0);
		glBegin(GL_TRIANGLES);
		glColor3f(0, 0, 1);
		glVertex2i(40, 0);
		glVertex2i(0, 20);
		glVertex2i(40, 40);
		glEnd();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, 4.0/3.0, 1.0, 300.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0, -5, -50);
		glRotatef(20, 1, 0, 0);
		glRotatef(i/1.0*360.0, 0, 1, 0);
		glDisable(GL_BLEND);
		glColor3f(0.9, 0.9, 0.9);
		glScalef((cs+0.4)*3.5, (cs+0.4)*3.5, (cs+0.4)*4);
		glBindTexture(GL_TEXTURE_2D, floortex);
		glEnable(GL_TEXTURE_2D);
		glBegin(GL_POLYGON);
		glTexCoord2f(-1, 0.4);
		glVertex3f(-1, 0, 0.4);
		glTexCoord2f(-1, -0.4);
		glVertex3f(-1, 0, -0.4);
		glTexCoord2f(-0.8, -0.6);
		glVertex3f(-0.8, 0, -0.6);
		glTexCoord2f(0.8, -0.6);
		glVertex3f(0.8, 0, -0.6);
		glTexCoord2f(1, -0.4);
		glVertex3f(1, 0, -0.4);
		glTexCoord2f(1, 0.4);
		glVertex3f(1, 0, 0.4);
		glTexCoord2f(0.8, 0.6);
		glVertex3f(0.8, 0, 0.6);
		glTexCoord2f(-0.8, 0.6);
		glVertex3f(-0.8, 0, 0.6);
		glEnd();
		glBegin(GL_QUADS);
		glTexCoord2f(-1.2, 0.2);
		glVertex3f(-1.2, 0, 0.2);
		glTexCoord2f(-1.2, -0.2);
		glVertex3f(-1.2, 0, -0.2);
		glTexCoord2f(-1, -0.2);
		glVertex3f(-1, 0, -0.2);
		glTexCoord2f(-1, 0.2);
		glVertex3f(-1, 0, 0.2);
		glTexCoord2f(1, 0.2);
		glVertex3f(1, 0, 0.2);
		glTexCoord2f(1, -0.2);
		glVertex3f(1, 0, -0.2);
		glTexCoord2f(1.2, -0.2);
		glVertex3f(1.2, 0, -0.2);
		glTexCoord2f(1.2, 0.2);
		glVertex3f(1.2, 0, 0.2);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		glVertex3f(-0.01, 0, 0.6);
		glVertex3f(-0.01, 0, -0.6);
		glVertex3f(0.01, 0, -0.6);
		glVertex3f(0.01, 0, 0.6);
		glVertex3f(-1, 0, 0.3);
		glVertex3f(-0.6, 0, 0.3);
		glVertex3f(-0.6, 0, 0.29);
		glVertex3f(-1, 0, 0.29);
		glVertex3f(-0.59, 0, 0.29);
		glVertex3f(-0.59, 0, -0.29);
		glVertex3f(-0.6, 0, -0.29);
		glVertex3f(-0.6, 0, 0.29);
		glVertex3f(-1, 0, -0.3);
		glVertex3f(-0.6, 0, -0.3);
		glVertex3f(-0.6, 0, -0.29);
		glVertex3f(-1, 0, -0.29);
		glVertex3f(1, 0, 0.3);
		glVertex3f(0.6, 0, 0.3);
		glVertex3f(0.6, 0, 0.29);
		glVertex3f(1, 0, 0.29);
		glVertex3f(0.59, 0, 0.29);
		glVertex3f(0.59, 0, -0.29);
		glVertex3f(0.6, 0, -0.29);
		glVertex3f(0.6, 0, 0.29);
		glVertex3f(1, 0, -0.3);
		glVertex3f(0.6, 0, -0.3);
		glVertex3f(0.6, 0, -0.29);
		glVertex3f(1, 0, -0.29);
		glEnd();
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, walltex);
		glEnable(GL_DEPTH_TEST);
		glBegin(GL_QUAD_STRIP);
		glTexCoord2f(0, 0);
		glVertex3f(-1, 0, 0.4);
		glTexCoord2f(0, 1);
		glVertex3f(-1, 0.2, 0.4);
		glTexCoord2f(0.5, 0);
		glVertex3f(-1, 0, 0.2);
		glTexCoord2f(0.5, 1);
		glVertex3f(-1, 0.2, 0.2);
		glTexCoord2f(1, 0);
		glVertex3f(-1.2, 0, 0.2);
		glTexCoord2f(1, 1);
		glVertex3f(-1.2, 0.2, 0.2);
		glTexCoord2f(1.5, 0);
		glVertex3f(-1.2, 0, -0.2);
		glTexCoord2f(1.5, 1);
		glVertex3f(-1.2, 0.2, -0.2);
		glTexCoord2f(2, 0);
		glVertex3f(-1, 0, -0.2);
		glTexCoord2f(2, 1);
		glVertex3f(-1, 0.2, -0.2);
		glTexCoord2f(2.5, 0);
		glVertex3f(-1, 0, -0.4);
		glTexCoord2f(2.5, 1);
		glVertex3f(-1, 0.2, -0.4);
		glTexCoord2f(3, 0);
		glVertex3f(-0.8, 0, -0.6);
		glTexCoord2f(3, 1);
		glVertex3f(-0.8, 0.2, -0.6);
		glTexCoord2f(6, 0);
		glVertex3f(0.8, 0, -0.6);
		glTexCoord2f(6, 1);
		glVertex3f(0.8, 0.2, -0.6);
		glTexCoord2f(6.5, 0);
		glVertex3f(1, 0, -0.4);
		glTexCoord2f(6.5, 1);
		glVertex3f(1, 0.2, -0.4);
		glTexCoord2f(7, 0);
		glVertex3f(1, 0, -0.2);
		glTexCoord2f(7, 1);
		glVertex3f(1, 0.2, -0.2);
		glTexCoord2f(7.5, 0);
		glVertex3f(1.2, 0, -0.2);
		glTexCoord2f(7.5, 1);
		glVertex3f(1.2, 0.2, -0.2);
		glTexCoord2f(8, 0);
		glVertex3f(1.2, 0, 0.2);
		glTexCoord2f(8, 1);
		glVertex3f(1.2, 0.2, 0.2);
		glTexCoord2f(8.5, 0);
		glVertex3f(1, 0, 0.2);
		glTexCoord2f(8.5, 1);
		glVertex3f(1, 0.2, 0.2);
		glTexCoord2f(9, 0);
		glVertex3f(1, 0, 0.4);
		glTexCoord2f(9, 1);
		glVertex3f(1, 0.2, 0.4);
		glTexCoord2f(9.5, 0);
		glVertex3f(0.8, 0, 0.6);
		glTexCoord2f(9.5, 1);
		glVertex3f(0.8, 0.2, 0.6);
		glTexCoord2f(12.5, 0);
		glVertex3f(-0.8, 0, 0.6);
		glTexCoord2f(12.5, 1);
		glVertex3f(-0.8, 0.2, 0.6);
		glTexCoord2f(13, 0);
		glVertex3f(-1, 0, 0.4);
		glTexCoord2f(13, 1);
		glVertex3f(-1, 0.2, 0.4);
		glEnd();
		glDisable(GL_DEPTH_TEST);
		
		text.begin();
		glColor3f(1, 1, 0.8);
		text.print(160, 450, "SELECT THE COURT SIZE");
		if(cs==0)
			text.print(330, 10, "SMALL");
		else if(cs==1)
			text.print(320, 10, "NORMAL");
		else if(cs==2)
			text.print(330, 10, "LARGE");
		else if(cs==3)
			text.print(340, 10, "HUGE");
		text.end();

		glFlush();
		allegro_gl_flip();
		allegro_gl_end();
	}
	play_sample(&mb_menusel_wav, 255, 128, 1000, 0);
	while(key[KEY_ENTER] || key[KEY_SPACE] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL))
	{
		poll_joystick();
		al_poll_duh(dp);
	}
	allegro_gl_begin();
	glDeleteTextures(1, &pat01tex);
	glDeleteTextures(1, &pat02tex);
	allegro_gl_end();

	return cs;
}

void gameoptionsMenu(gameoptions *op)
{
	op->timegoallimit=0;
	op->timegoals=2;
	op->powerupsenabled=1;
	op->powerupfrequency=1;
	op->preventstuck=1;
	
	op->turbo=200;
	op->shield=200;
	op->mine=200;
	op->missle=200;
	op->meshatek=200;

	allegro_gl_begin();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 4.0/3.0, 1.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLuint pat01tex;
	allegro_gl_set_texture_format(GL_RGB);
	pat01tex = allegro_gl_make_texture(&mb_pattern01_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	GLuint pat02tex;
	allegro_gl_set_texture_format(GL_RGB);
	pat02tex = allegro_gl_make_texture(&mb_pattern02_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	allegro_gl_end();

	timer.install();
	double i=0;
	int choice=0;
	int upkey=0, downkey=0, leftkey=0, rightkey=0;
	while(!(key[KEY_ENTER] || key[KEY_SPACE] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL)))
	{
		rest(10); /* Limit to 100fps, in order to correctly set the powerup freqs */
		i+=timer.seconds()/2.0;
		timer.reset();

		poll_joystick();
		al_poll_duh(dp);

		if(i>1)
			i=i-1;
		if((key[KEY_DOWN] || controls[0].keydown(KEYDOWN)) && downkey==0)
		{
			choice++;
			downkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if((key[KEY_UP] || controls[0].keydown(KEYUP)) && upkey==0)
		{
			choice--;
			upkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if((key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)) && rightkey==0 && choice==0)
		{
			op->timegoallimit=!op->timegoallimit;
			rightkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if((key[KEY_LEFT] || controls[0].keydown(KEYLEFT)) && leftkey==0 && choice==0)
		{
			op->timegoallimit=!op->timegoallimit;
			leftkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if((key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)) && rightkey==0 && choice==2)
		{
			op->preventstuck=!op->preventstuck;
			rightkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if((key[KEY_LEFT] || controls[0].keydown(KEYLEFT)) && leftkey==0 && choice==2)
		{
			op->preventstuck=!op->preventstuck;
			leftkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if((key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)) && rightkey==0 && choice==3)
		{
			op->powerupsenabled=!op->powerupsenabled;
			rightkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if((key[KEY_LEFT] || controls[0].keydown(KEYLEFT)) && leftkey==0 && choice==3)
		{
			op->powerupsenabled=!op->powerupsenabled;
			leftkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if((key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)) && rightkey==0 && choice==1)
		{
			op->timegoals++;
			if(op->timegoals>5)
				op->timegoals=0;
			rightkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if((key[KEY_LEFT] || controls[0].keydown(KEYLEFT)) && leftkey==0 && choice==1)
		{
			op->timegoals--;
			if(op->timegoals<0)
				op->timegoals=5;
			leftkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if((key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)) && rightkey==0 && choice==4)
		{
			op->powerupfrequency++;
			if(op->powerupfrequency>3)
				op->powerupfrequency=0;
			rightkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if((key[KEY_LEFT] || controls[0].keydown(KEYLEFT)) && leftkey==0 && choice==4)
		{
			op->powerupfrequency--;
			if(op->powerupfrequency<0)
				op->powerupfrequency=3;
			leftkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}


		if((key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)) && choice==5)
		{
			op->turbo+=5;
			if(op->turbo>400)
				op->turbo=400;
		}
		if((key[KEY_LEFT] || controls[0].keydown(KEYLEFT)) && choice==5)
		{
			op->turbo-=5;
			if(op->turbo<0)
				op->turbo=0;
		}
		if((key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)) && choice==6)
		{
			op->shield+=5;
			if(op->shield>400)
				op->shield=400;
		}
		if((key[KEY_LEFT] || controls[0].keydown(KEYLEFT)) && choice==6)
		{
			op->shield-=5;
			if(op->shield<0)
				op->shield=0;
		}
		if((key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)) && choice==7)
		{
			op->mine+=5;
			if(op->mine>400)
				op->mine=400;
		}
		if((key[KEY_LEFT] || controls[0].keydown(KEYLEFT)) && choice==7)
		{
			op->mine-=5;
			if(op->mine<0)
				op->mine=0;
		}
		if((key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)) && choice==8)
		{
			op->missle+=5;
			if(op->missle>400)
				op->missle=400;
		}
		if((key[KEY_LEFT] || controls[0].keydown(KEYLEFT)) && choice==8)
		{
			op->missle-=5;
			if(op->missle<0)
				op->missle=0;
		}
		if((key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)) && choice==9)
		{
			op->meshatek+=5;
			if(op->meshatek>400)
				op->meshatek=400;
		}
		if((key[KEY_LEFT] || controls[0].keydown(KEYLEFT)) && choice==9)
		{
			op->meshatek-=5;
			if(op->meshatek<0)
				op->meshatek=0;
		}
		if(!(key[KEY_DOWN] || controls[0].keydown(KEYDOWN)))
			downkey=0;
		if(!(key[KEY_UP] || controls[0].keydown(KEYUP)))
			upkey=0;
		if(!(key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)))
			rightkey=0;
		if(!(key[KEY_LEFT] || controls[0].keydown(KEYLEFT)))
			leftkey=0;
		if(choice>9)
			choice=0;
		if(choice>3 && !op->powerupsenabled)
			choice=0;
		if(choice<0)
		{
			if(op->powerupsenabled)
				choice=9;
			else
				choice=3;
		}

		allegro_gl_begin();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 800, 0, 600, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pat01tex);
		glEnable(GL_BLEND);
		glColor4f(sin(M_PI*i)/2+0.3, sin(M_PI*i)/2+0.3, 0.7, 0.4);
		glBegin(GL_QUADS);
		glTexCoord2f(-i, i);
		glVertex2i(0, 0);
		glTexCoord2f(800.0/256.0-i, i);
		glVertex2i(800, 0);
		glTexCoord2f(800.0/256.0-i, 600.0/256.0+i);
		glVertex2i(800, 600);
		glTexCoord2f(-i, 600.0/256.0+i);
		glVertex2i(0, 600);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, pat02tex);
		glColor4f(0.4, 0.4, 0.4, sin(M_PI*i)/2*0.6);
		glBegin(GL_QUADS);
		glTexCoord2f(i, i);
		glVertex2i(0, 0);
		glTexCoord2f(800.0/256.0+i, i);
		glVertex2i(800, 0);
		glTexCoord2f(800.0/256.0+i, 600.0/256.0+i);
		glVertex2i(800, 600);
		glTexCoord2f(i, 600.0/256.0+i);
		glVertex2i(0, 600);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		
		glBegin(GL_QUADS);
		glColor4f(0, 0, 1, 0.6);
		if(op->timegoallimit==0)
		{
			glVertex2i(330, 500);
			glVertex2i(430, 500);
			glVertex2i(430, 542);
			glVertex2i(330, 542);
		}
		else
		{
			glVertex2i(480, 500);
			glVertex2i(590, 500);
			glVertex2i(590, 542);
			glVertex2i(480, 542);
		}

		glVertex2i(360+op->timegoals*66, 450);
		glVertex2i(415+op->timegoals*66, 450);
		glVertex2i(415+op->timegoals*66, 492);
		glVertex2i(360+op->timegoals*66, 492);
		
		if(op->preventstuck==0)
		{
			glVertex2i(570, 400);
			glVertex2i(640, 400);
			glVertex2i(640, 442);
			glVertex2i(570, 442);}
		else
		{
			glVertex2i(430, 400);
			glVertex2i(530, 400);
			glVertex2i(530, 442);
			glVertex2i(430, 442);
		}

		if(op->powerupsenabled==0)
		{
			glVertex2i(520, 350);
			glVertex2i(710, 350);
			glVertex2i(710, 392);
			glVertex2i(520, 392);}
		else
		{
			glVertex2i(280, 350);
			glVertex2i(470, 350);
			glVertex2i(470, 392);
			glVertex2i(280, 392);
		}

		if(op->powerupsenabled)
		{
			glVertex2i(510+op->powerupfrequency*60, 300);
			glVertex2i(565+op->powerupfrequency*60, 300);
			glVertex2i(565+op->powerupfrequency*60, 342);
			glVertex2i(510+op->powerupfrequency*60, 342);

			glVertex2f(300, 250);
			glVertex2f(300+op->turbo, 250);
			glVertex2f(300+op->turbo, 292);
			glVertex2f(300, 292);
			glVertex2f(300, 200);
			glVertex2f(300+op->shield, 200);
			glVertex2f(300+op->shield, 242);
			glVertex2f(300, 242);
			glVertex2f(300, 150);
			glVertex2f(300+op->mine, 150);
			glVertex2f(300+op->mine, 192);
			glVertex2f(300, 192);
			glVertex2f(300, 100);
			glVertex2f(300+op->missle, 100);
			glVertex2f(300+op->missle, 142);
			glVertex2f(300, 142);
			glVertex2f(300, 50);
			glVertex2f(300+op->meshatek, 50);
			glVertex2f(300+op->meshatek, 92);
			glVertex2f(300, 92);
		}

		glEnd();
		glDisable(GL_BLEND);
		
		glTranslated(0, 350+(3-choice)*50, 0);
		glBegin(GL_TRIANGLES);
		glColor3f(0, 0, 1);
		glVertex2i(50, 0);
		glVertex2i(10, 20);
		glVertex2i(50, 40);

		glVertex2i(790, 20);
		glVertex2i(750, 0);
		glVertex2i(750, 40);
		glEnd();

		text.begin();
		glColor3f(0, 1, 0);
		text.print(250, 550, "MATCH OPTIONS");
		glColor3f(1, 1, 0.8);
		text.print(60, 500, "MATCH TYPE: TIME / GOAL LIMIT");
		if(op->timegoallimit==0)
			text.print(60, 450, "# OF MINUTES:");
		else
			text.print(60, 450, "# OF GOALS:");
		text.print(400, 450, "1  2  5 10 30 60");
		text.print(60, 400, "NUKE STUCK BALL: YES / NO");
		text.print(60, 350, "POWERUPS: ENABLED / DISABLED");
		if(op->powerupsenabled)
		{
			text.print(60, 300, "FREQUENCY (SECONDS): 1  3 10 30");
			text.print(60, 250, "TURBO:");
			text.print(60, 200, "SHIELD:");
			text.print(60, 150, "MINE:");
			text.print(60, 100, "MISSLE:");
			text.print(60,  50, "EMP:");
		}
		text.end();

		glFlush();
		allegro_gl_flip();
		allegro_gl_end();
	}
	play_sample(&mb_menusel_wav, 255, 128, 1000, 0);
	while(key[KEY_ENTER] || key[KEY_SPACE] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL))
	{
		poll_joystick();
		al_poll_duh(dp);
	}
	allegro_gl_begin();
	glDeleteTextures(1, &pat01tex);
	glDeleteTextures(1, &pat02tex);
	allegro_gl_end();

	al_stop_duh(dp);
	menumusicisplaying=0;

	if(op->timegoals==0)
		op->timegoals=1;
	else if(op->timegoals==1)
		op->timegoals=2;
	else if(op->timegoals==2)
		op->timegoals=5;
	else if(op->timegoals==3)
		op->timegoals=10;
	else if(op->timegoals==4)
		op->timegoals=30;
	else if(op->timegoals==5)
		op->timegoals=60;

	if(op->powerupfrequency==0)
		op->powerupfrequency=1;
	else if(op->powerupfrequency==1)
		op->powerupfrequency=3;
	else if(op->powerupfrequency==2)
		op->powerupfrequency=10;
	else if(op->powerupfrequency==3)
		op->powerupfrequency=30;
}

void humancompmessage(void)
{
	allegro_gl_begin();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	text.begin();
	glColor3f(0, 0, 1);
	text.print(200, 440, "SORRY!");
	glColor3f(1, 1, 1);
	text.print(20, 400, "ONLY HUMAN VS HUMAN IS AVAILABLE");
	glColor3f(0, 1, 0);
	text.print(120, 120, "IF YOU ARE A TALENTED AI");
	text.print(65, 80, "PROGRAMMER AND WOULD LIKE TO");
	text.print(60, 40, "PROGRAM SOME COMPUTER AI FOR");
	text.print(0, 0, "MACHINEBALL THEN PLEASE CONTACT ME.");
	text.end();
	
	glFlush();
	allegro_gl_flip();
	allegro_gl_end();

	while(!(key[KEY_ENTER] || key[KEY_SPACE] || key[KEY_ESC] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL)))
	{
		poll_joystick();
		al_poll_duh(dp);
	}
	play_sample(&mb_menusel_wav, 255, 128, 1000, 0);
	while(key[KEY_ENTER] || key[KEY_SPACE] || key[KEY_ESC] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL))
	{
		poll_joystick();
		al_poll_duh(dp);
	}
}

int optionsMenu(void)
{
	allegro_gl_begin();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 4.0/3.0, 1.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLuint pat01tex;
	allegro_gl_set_texture_format(GL_RGB);
	pat01tex = allegro_gl_make_texture(&mb_pattern01_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	GLuint pat02tex;
	allegro_gl_set_texture_format(GL_RGB);
	pat02tex = allegro_gl_make_texture(&mb_pattern02_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	allegro_gl_end();

	timer.install();
	double i=0;
	int choice=0;
	int upkey=0, downkey=0;
	while(!(key[KEY_ENTER] || key[KEY_SPACE] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL)))
	{
		rest(1);
		i+=timer.seconds()/2.0;
		timer.reset();

		poll_joystick();
		al_poll_duh(dp);

		if(i>1)
			i=i-1;
		if((key[KEY_DOWN] || controls[0].keydown(KEYDOWN)) && downkey==0)
		{
			choice++;
			downkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if((key[KEY_UP] || controls[0].keydown(KEYUP)) && upkey==0)
		{
			choice--;
			upkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if(!(key[KEY_DOWN] || controls[0].keydown(KEYDOWN)))
			downkey=0;
		if(!(key[KEY_UP] || controls[0].keydown(KEYUP)))
			upkey=0;
		if(choice>3)
			choice=0;
		if(choice<0)
			choice=3;

		allegro_gl_begin();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 800, 0, 600, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pat01tex);
		glEnable(GL_BLEND);
		glColor4f(sin(M_PI*i)/2+0.3, sin(M_PI*i)/2+0.3, 0.7, 0.4);
		glBegin(GL_QUADS);
		glTexCoord2f(-i, i);
		glVertex2i(0, 0);
		glTexCoord2f(800.0/256.0-i, i);
		glVertex2i(800, 0);
		glTexCoord2f(800.0/256.0-i, 600.0/256.0+i);
		glVertex2i(800, 600);
		glTexCoord2f(-i, 600.0/256.0+i);
		glVertex2i(0, 600);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, pat02tex);
		glColor4f(0.4, 0.4, 0.4, sin(M_PI*i)/2*0.6);
		glBegin(GL_QUADS);
		glTexCoord2f(i, i);
		glVertex2i(0, 0);
		glTexCoord2f(800.0/256.0+i, i);
		glVertex2i(800, 0);
		glTexCoord2f(800.0/256.0+i, 600.0/256.0+i);
		glVertex2i(800, 600);
		glTexCoord2f(i, 600.0/256.0+i);
		glVertex2i(0, 600);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		
		glTranslated(130, 200+(3-choice)*50, 0);
		glBegin(GL_QUADS);
		glColor4f(0, 0, 1, 0.2);
		glVertex2i(50, 0);
		glVertex2i(500, 0);
		glVertex2i(500, 42);
		glVertex2i(50, 42);
		glEnd();
		glDisable(GL_BLEND);
		glBegin(GL_TRIANGLES);
		glColor3f(0, 0, 1);
		glVertex2i(0, 0);
		glVertex2i(40, 20);
		glVertex2i(0, 40);
		glEnd();

		text.begin();
		glColor3f(0, 1, 0);
		text.print(310, 500, "OPTIONS");
		glColor3f(1, 1, 0.8);
		text.print(300, 350, "CONTROLS");
		text.print(330, 300, "AUDIO");
		text.print(330, 250, "VIDEO");
		text.print(200, 200, "BACK TO MAIN MENU");
		text.end();

		glFlush();
		allegro_gl_flip();
		allegro_gl_end();
	}
	play_sample(&mb_menusel_wav, 255, 128, 1000, 0);
	while(key[KEY_ENTER] || key[KEY_SPACE] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL))
	{
		poll_joystick();
		al_poll_duh(dp);
	}
	allegro_gl_begin();
	glDeleteTextures(1, &pat01tex);
	glDeleteTextures(1, &pat02tex);
	allegro_gl_end();
	return choice;
}

void audioMenu(void)
{
	allegro_gl_begin();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 4.0/3.0, 1.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLuint pat01tex;
	allegro_gl_set_texture_format(GL_RGB);
	pat01tex = allegro_gl_make_texture(&mb_pattern01_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	GLuint pat02tex;
	allegro_gl_set_texture_format(GL_RGB);
	pat02tex = allegro_gl_make_texture(&mb_pattern02_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	allegro_gl_end();

	timer.install();
	double i=0;
	int choice=0;
	int upkey=0, downkey=0;
	while(!(key[KEY_ENTER] || key[KEY_SPACE] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL)) || choice!=2)
	{
		rest(10); /* Limit to 100fps, in order to correctly set the volumes */
		i+=timer.seconds()/2.0;
		timer.reset();

		poll_joystick();
		al_poll_duh(dp);

		if(i>1)
			i=i-1;
		if((key[KEY_DOWN] || controls[0].keydown(KEYDOWN)) && downkey==0)
		{
			choice++;
			downkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if((key[KEY_UP] || controls[0].keydown(KEYUP)) && upkey==0)
		{
			choice--;
			upkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if(!(key[KEY_DOWN] || controls[0].keydown(KEYDOWN)))
			downkey=0;
		if(!(key[KEY_UP] || controls[0].keydown(KEYUP)))
			upkey=0;
		if(choice>2)
			choice=0;
		if(choice<0)
			choice=2;

		if((key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)) && choice==1)
		{
			options.musicvol+=2;
			if(options.musicvol>255)
				options.musicvol=255;
			al_duh_set_volume(dp, (float)options.musicvol/255.0);
		}
		if((key[KEY_LEFT] || controls[0].keydown(KEYLEFT)) && choice==1)
		{
			options.musicvol-=2;
			if(options.musicvol<0)
				options.musicvol=0;
			al_duh_set_volume(dp, (float)options.musicvol/255.0);
		}

		if((key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)) && choice==0)
		{
			options.soundvol+=2;
			if(options.soundvol>255)
				options.soundvol=255;
			set_volume(options.soundvol, -1);
		}
		if((key[KEY_LEFT] || controls[0].keydown(KEYLEFT)) && choice==0)
		{
			options.soundvol-=2;
			if(options.soundvol<0)
				options.soundvol=0;
			set_volume(options.soundvol, -1);
		}

		allegro_gl_begin();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 800, 0, 600, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pat01tex);
		glEnable(GL_BLEND);
		glColor4f(sin(M_PI*i)/2+0.3, sin(M_PI*i)/2+0.3, 0.7, 0.4);
		glBegin(GL_QUADS);
		glTexCoord2f(-i, i);
		glVertex2i(0, 0);
		glTexCoord2f(800.0/256.0-i, i);
		glVertex2i(800, 0);
		glTexCoord2f(800.0/256.0-i, 600.0/256.0+i);
		glVertex2i(800, 600);
		glTexCoord2f(-i, 600.0/256.0+i);
		glVertex2i(0, 600);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, pat02tex);
		glColor4f(0.4, 0.4, 0.4, sin(M_PI*i)/2*0.6);
		glBegin(GL_QUADS);
		glTexCoord2f(i, i);
		glVertex2i(0, 0);
		glTexCoord2f(800.0/256.0+i, i);
		glVertex2i(800, 0);
		glTexCoord2f(800.0/256.0+i, 600.0/256.0+i);
		glVertex2i(800, 600);
		glTexCoord2f(i, 600.0/256.0+i);
		glVertex2i(0, 600);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		
		glTranslated(130, 200+(3-choice)*50, 0);
		glBegin(GL_QUADS);
		glColor4f(0, 0, 1, 0.2);
		glVertex2i(50, 0);
		glVertex2i(560, 0);
		glVertex2i(560, 42);
		glVertex2i(50, 42);
		glEnd();
		glDisable(GL_BLEND);
		glBegin(GL_TRIANGLES);
		glColor3f(0, 0, 1);
		glVertex2i(0, 0);
		glVertex2i(40, 20);
		glVertex2i(0, 40);
		glEnd();
		glLoadIdentity();
		glBegin(GL_QUADS);
		glColor4f(0, 0, 1, 0.3);
		glVertex2i(430, 350);
		glVertex2i(430+options.soundvol, 350);
		glVertex2i(430+options.soundvol, 390);
		glVertex2i(430, 390);
		glVertex2i(430, 300);
		glVertex2i(430+options.musicvol, 300);
		glVertex2i(430+options.musicvol, 340);
		glVertex2i(430, 340);
		glEnd();

		text.begin();
		glColor3f(0, 1, 0);
		text.print(240, 500, "AUDIO OPTIONS");
		glColor3f(1, 1, 0.8);
		text.print(190, 350, "MASTER-VOL:");
		text.print(190, 300, "MUSIC VOL:");
		text.print(190, 250, "BACK TO OPTIONS");
		text.end();

		glFlush();
		allegro_gl_flip();
		allegro_gl_end();
	}
	play_sample(&mb_menusel_wav, 255, 128, 1000, 0);
	while(key[KEY_ENTER] || key[KEY_SPACE] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL))
	{
		poll_joystick();
		al_poll_duh(dp);
	}
	allegro_gl_begin();
	glDeleteTextures(1, &pat01tex);
	glDeleteTextures(1, &pat02tex);
	allegro_gl_end();
}

void videoMenu(void)
{
	allegro_gl_begin();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 4.0/3.0, 1.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLuint pat01tex;
	allegro_gl_set_texture_format(GL_RGB);
	pat01tex = allegro_gl_make_texture(&mb_pattern01_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	GLuint pat02tex;
	allegro_gl_set_texture_format(GL_RGB);
	pat02tex = allegro_gl_make_texture(&mb_pattern02_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	allegro_gl_end();

	timer.install();
	double i=0;
	int choice=0;
	int upkey=0, downkey=0;
	while(!(key[KEY_ENTER] || key[KEY_SPACE] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL)) || choice!=3)
	{
		rest(1);
		i+=timer.seconds()/2.0;
		timer.reset();

		poll_joystick();
		al_poll_duh(dp);

		if(i>1)
			i=i-1;
		if((key[KEY_DOWN] || controls[0].keydown(KEYDOWN)) && downkey==0)
		{
			choice++;
			downkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if((key[KEY_UP] || controls[0].keydown(KEYUP)) && upkey==0)
		{
			choice--;
			upkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if(!(key[KEY_DOWN] || controls[0].keydown(KEYDOWN)))
			downkey=0;
		if(!(key[KEY_UP] || controls[0].keydown(KEYUP)))
			upkey=0;
		if(choice>3)
			choice=0;
		if(choice<0)
			choice=3;

		if((key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)) && choice==0)
			options.camerashake=0;
		if((key[KEY_LEFT] || controls[0].keydown(KEYLEFT)) && choice==0)
			options.camerashake=1;

		if((key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)) && choice==1)
			options.logoshow=0;
		if((key[KEY_LEFT] || controls[0].keydown(KEYLEFT)) && choice==1)
			options.logoshow=1;

		if((key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)) && choice==2)
			options.fpsshow=0;
		if((key[KEY_LEFT] || controls[0].keydown(KEYLEFT)) && choice==2)
			options.fpsshow=1;
			
		
		allegro_gl_begin();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 800, 0, 600, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pat01tex);
		glEnable(GL_BLEND);
		glColor4f(sin(M_PI*i)/2+0.3, sin(M_PI*i)/2+0.3, 0.7, 0.4);
		glBegin(GL_QUADS);
		glTexCoord2f(-i, i);
		glVertex2i(0, 0);
		glTexCoord2f(800.0/256.0-i, i);
		glVertex2i(800, 0);
		glTexCoord2f(800.0/256.0-i, 600.0/256.0+i);
		glVertex2i(800, 600);
		glTexCoord2f(-i, 600.0/256.0+i);
		glVertex2i(0, 600);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, pat02tex);
		glColor4f(0.4, 0.4, 0.4, sin(M_PI*i)/2*0.6);
		glBegin(GL_QUADS);
		glTexCoord2f(i, i);
		glVertex2i(0, 0);
		glTexCoord2f(800.0/256.0+i, i);
		glVertex2i(800, 0);
		glTexCoord2f(800.0/256.0+i, 600.0/256.0+i);
		glVertex2i(800, 600);
		glTexCoord2f(i, 600.0/256.0+i);
		glVertex2i(0, 600);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		
		glTranslated(10, 200+(3-choice)*50, 0);
		glBegin(GL_QUADS);
		glColor4f(0, 0, 1, 0.2);
		glVertex2i(50, 0);
		glVertex2i(770, 0);
		glVertex2i(770, 42);
		glVertex2i(50, 42);
		glEnd();
		glDisable(GL_BLEND);
		glBegin(GL_TRIANGLES);
		glColor3f(0, 0, 1);
		glVertex2i(0, 0);
		glVertex2i(40, 20);
		glVertex2i(0, 40);
		glEnd();
		glLoadIdentity();
		glBegin(GL_QUADS);
		glColor4f(0, 0, 1, 0.3);
		glVertex2i(!options.camerashake*90+455, 350);
		glVertex2i(!options.camerashake*90+455+80, 350);
		glVertex2i(!options.camerashake*90+455+80, 390);
		glVertex2i(!options.camerashake*90+455, 390);
		glVertex2i(!options.logoshow*90+495, 300);
		glVertex2i(!options.logoshow*90+495+80, 300);
		glVertex2i(!options.logoshow*90+495+80, 340);
		glVertex2i(!options.logoshow*90+495, 340);
		glVertex2i(!options.fpsshow*90+620, 250);
		glVertex2i(!options.fpsshow*90+620+80, 250);
		glVertex2i(!options.fpsshow*90+620+80, 290);
		glVertex2i(!options.fpsshow*90+620, 290);
		glEnd();

		text.begin();
		glColor3f(0, 1, 0);
		text.print(240, 500, "VIDEO OPTIONS");
		glColor3f(1, 1, 0.8);
		text.print(70, 350, "CAMERA-SHAKE FX: YES NO");
		text.print(70, 300, "SHOW LOGO IN GAME: YES NO");
		text.print(70, 250, "SHOW FRAMES-PER-SECOND: YES NO");
		text.print(70, 200, "BACK TO OPTIONS");
		text.end();

		glFlush();
		allegro_gl_flip();
		allegro_gl_end();
	}
	play_sample(&mb_menusel_wav, 255, 128, 1000, 0);
	while(key[KEY_ENTER] || key[KEY_SPACE] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL))
	{
		poll_joystick();
		al_poll_duh(dp);
	}
	allegro_gl_begin();
	glDeleteTextures(1, &pat01tex);
	glDeleteTextures(1, &pat02tex);
	allegro_gl_end();
}

void controlsMenu(void)
{
	char keynames[128+((4*4+12)*4)][10];
	int blahblah;
	for(blahblah=0; blahblah<128; blahblah++)
		strcpy(keynames[blahblah], "-?!?-");
	strcpy(keynames[KEY_A], "A");
	strcpy(keynames[KEY_B], "B");
	strcpy(keynames[KEY_C], "C");
	strcpy(keynames[KEY_D], "D");
	strcpy(keynames[KEY_E], "E");
	strcpy(keynames[KEY_F], "F");
	strcpy(keynames[KEY_G], "G");
	strcpy(keynames[KEY_H], "H");
	strcpy(keynames[KEY_I], "I");
	strcpy(keynames[KEY_J], "J");
	strcpy(keynames[KEY_K], "K");
	strcpy(keynames[KEY_L], "L");
	strcpy(keynames[KEY_M], "M");
	strcpy(keynames[KEY_N], "N");
	strcpy(keynames[KEY_O], "O");
	strcpy(keynames[KEY_P], "P");
	strcpy(keynames[KEY_Q], "Q");
	strcpy(keynames[KEY_R], "R");
	strcpy(keynames[KEY_S], "S");
	strcpy(keynames[KEY_T], "T");
	strcpy(keynames[KEY_U], "U");
	strcpy(keynames[KEY_V], "V");
	strcpy(keynames[KEY_W], "W");
	strcpy(keynames[KEY_X], "X");
	strcpy(keynames[KEY_Y], "Y");
	strcpy(keynames[KEY_Z], "Z");
	strcpy(keynames[KEY_0], "0");
	strcpy(keynames[KEY_1], "1");
	strcpy(keynames[KEY_2], "2");
	strcpy(keynames[KEY_3], "3");
	strcpy(keynames[KEY_4], "4");
	strcpy(keynames[KEY_5], "5");
	strcpy(keynames[KEY_6], "6");
	strcpy(keynames[KEY_7], "7");
	strcpy(keynames[KEY_8], "8");
	strcpy(keynames[KEY_9], "9");
	strcpy(keynames[KEY_0_PAD], "0 PAD");
	strcpy(keynames[KEY_1_PAD], "1 PAD");
	strcpy(keynames[KEY_2_PAD], "2 PAD");
	strcpy(keynames[KEY_3_PAD], "3 PAD");
	strcpy(keynames[KEY_4_PAD], "4 PAD");
	strcpy(keynames[KEY_5_PAD], "5 PAD");
	strcpy(keynames[KEY_6_PAD], "6 PAD");
	strcpy(keynames[KEY_7_PAD], "7 PAD");
	strcpy(keynames[KEY_8_PAD], "8 PAD");
	strcpy(keynames[KEY_9_PAD], "9 PAD");
	strcpy(keynames[KEY_F1], "F1");
	strcpy(keynames[KEY_F2], "F2");
	strcpy(keynames[KEY_F3], "F3");
	strcpy(keynames[KEY_F4], "F4");
	strcpy(keynames[KEY_F5], "F5");
	strcpy(keynames[KEY_F6], "F6");
	strcpy(keynames[KEY_F7], "F7");
	strcpy(keynames[KEY_F8], "F8");
	strcpy(keynames[KEY_F9], "F9");
	strcpy(keynames[KEY_F10], "F10");
	strcpy(keynames[KEY_F11], "F11");
	strcpy(keynames[KEY_F12], "F12");
	strcpy(keynames[KEY_TILDE], "TILDE");
	strcpy(keynames[KEY_MINUS], "-");
	strcpy(keynames[KEY_EQUALS], "=");
	strcpy(keynames[KEY_BACKSPACE], "BS");
	strcpy(keynames[KEY_TAB], "TAB");
	strcpy(keynames[KEY_OPENBRACE], "[");
	strcpy(keynames[KEY_CLOSEBRACE], "]");
	strcpy(keynames[KEY_ENTER], "ENTER");
	strcpy(keynames[KEY_COLON], ";");
	strcpy(keynames[KEY_QUOTE], "'");
	strcpy(keynames[KEY_BACKSLASH], "\\");
	strcpy(keynames[KEY_BACKSLASH2], "\\");
	strcpy(keynames[KEY_COMMA], ",");
	strcpy(keynames[KEY_STOP], ".");
	strcpy(keynames[KEY_SLASH], "/");
	strcpy(keynames[KEY_SPACE], "SPACE");
	strcpy(keynames[KEY_INSERT], "INSERT");
	strcpy(keynames[KEY_DEL], "DELETE");
	strcpy(keynames[KEY_HOME], "HOME");
	strcpy(keynames[KEY_END], "END");
	strcpy(keynames[KEY_PGUP], "PGUP");
	strcpy(keynames[KEY_PGDN], "PGDN");
	strcpy(keynames[KEY_LEFT], "LEFT");
	strcpy(keynames[KEY_RIGHT], "RIGHT");
	strcpy(keynames[KEY_UP], "UP");
	strcpy(keynames[KEY_DOWN], "DOWN");
	strcpy(keynames[KEY_SLASH_PAD], "/ PAD");
	strcpy(keynames[KEY_ASTERISK], "*");
	strcpy(keynames[KEY_MINUS_PAD], "- PAD");
	strcpy(keynames[KEY_PLUS_PAD], "+ PAD");
	strcpy(keynames[KEY_DEL_PAD], "DEL-PAD");
	strcpy(keynames[KEY_ENTER_PAD], "ENTER-PAD");
	strcpy(keynames[KEY_PRTSCR], "PRTSCR");
	strcpy(keynames[KEY_PAUSE], "PAUSE");
	strcpy(keynames[KEY_ABNT_C1], "ABNT-C1");
	strcpy(keynames[KEY_YEN], "YEN");
	strcpy(keynames[KEY_KANA], "KANA");
	strcpy(keynames[KEY_CONVERT], "CONVERT");
	strcpy(keynames[KEY_NOCONVERT], "NOCONVERT");
	strcpy(keynames[KEY_AT], "AT");
	strcpy(keynames[KEY_CIRCUMFLEX], "CIRCUMFLE");
	strcpy(keynames[KEY_KANJI], "KANJI");
	strcpy(keynames[KEY_MODIFIERS], "MOD");
	strcpy(keynames[KEY_LSHIFT], "L-SHIFT");
	strcpy(keynames[KEY_RSHIFT], "R-SHIFT");
	strcpy(keynames[KEY_LCONTROL], "L-CTRL");
	strcpy(keynames[KEY_RCONTROL], "R-CTRL");
	strcpy(keynames[KEY_ALT], "ALT");
	strcpy(keynames[KEY_ALTGR], "ALTGR");
	strcpy(keynames[KEY_LWIN], "L-WIN");
	strcpy(keynames[KEY_RWIN], "R-WIN");
	strcpy(keynames[KEY_MENU], "MENU");
	strcpy(keynames[KEY_SCRLOCK], "SCRLOCK");
	strcpy(keynames[KEY_NUMLOCK], "NUMLOCK");
	strcpy(keynames[KEY_CAPSLOCK], "CAPSLOCK");

	strcpy(keynames[128], "J1-UP");
	strcpy(keynames[129], "J1-DOWN");
	strcpy(keynames[130], "J1-LEFT");
	strcpy(keynames[131], "J1-RIGHT");
	strcpy(keynames[132], "J1-UP2");
	strcpy(keynames[133], "J1-DOWN2");
	strcpy(keynames[134], "J1-LEFT2");
	strcpy(keynames[135], "J1-RIGHT2");
	strcpy(keynames[136], "J1-UP3");
	strcpy(keynames[137], "J1-DOWN3");
	strcpy(keynames[138], "J1-LEFT3");
	strcpy(keynames[139], "J1-RIGHT3");
	strcpy(keynames[140], "J1-UP4");
	strcpy(keynames[141], "J1-DOWN4");
	strcpy(keynames[142], "J1-LEFT4");
	strcpy(keynames[143], "J1-RIGHT4");
	strcpy(keynames[144], "J1-BUT1");
	strcpy(keynames[145], "J1-BUT2");
	strcpy(keynames[146], "J1-BUT3");
	strcpy(keynames[147], "J1-BUT4");
	strcpy(keynames[148], "J1-BUT5");
	strcpy(keynames[149], "J1-BUT6");
	strcpy(keynames[150], "J1-BUT7");
	strcpy(keynames[151], "J1-BUT8");
	strcpy(keynames[152], "J1-BUT9");
	strcpy(keynames[153], "J1-BUT10");
	strcpy(keynames[154], "J1-BUT11");
	strcpy(keynames[155], "J1-BUT12");

	strcpy(keynames[156], "J2-UP");
	strcpy(keynames[157], "J2-DOWN");
	strcpy(keynames[158], "J2-LEFT");
	strcpy(keynames[159], "J2-RIGHT");
	strcpy(keynames[160], "J2-UP2");
	strcpy(keynames[161], "J2-DOWN2");
	strcpy(keynames[162], "J2-LEFT2");
	strcpy(keynames[163], "J2-RIGHT2");
	strcpy(keynames[164], "J2-UP3");
	strcpy(keynames[165], "J2-DOWN3");
	strcpy(keynames[166], "J2-LEFT3");
	strcpy(keynames[167], "J2-RIGHT3");
	strcpy(keynames[168], "J2-UP4");
	strcpy(keynames[169], "J2-DOWN4");
	strcpy(keynames[170], "J2-LEFT4");
	strcpy(keynames[171], "J2-RIGHT4");
	strcpy(keynames[172], "J2-BUT1");
	strcpy(keynames[173], "J2-BUT2");
	strcpy(keynames[174], "J2-BUT3");
	strcpy(keynames[175], "J2-BUT4");
	strcpy(keynames[176], "J2-BUT5");
	strcpy(keynames[177], "J2-BUT6");
	strcpy(keynames[178], "J2-BUT7");
	strcpy(keynames[179], "J2-BUT8");
	strcpy(keynames[180], "J2-BUT9");
	strcpy(keynames[181], "J2-BUT10");
	strcpy(keynames[182], "J2-BUT11");
	strcpy(keynames[183], "J2-BUT12");

	strcpy(keynames[184], "J3-UP");
	strcpy(keynames[185], "J3-DOWN");
	strcpy(keynames[186], "J3-LEFT");
	strcpy(keynames[187], "J3-RIGHT");
	strcpy(keynames[188], "J3-UP2");
	strcpy(keynames[189], "J3-DOWN2");
	strcpy(keynames[190], "J3-LEFT2");
	strcpy(keynames[191], "J3-RIGHT2");
	strcpy(keynames[192], "J3-UP3");
	strcpy(keynames[193], "J3-DOWN3");
	strcpy(keynames[194], "J3-LEFT3");
	strcpy(keynames[195], "J3-RIGHT3");
	strcpy(keynames[196], "J3-UP4");
	strcpy(keynames[197], "J3-DOWN4");
	strcpy(keynames[198], "J3-LEFT4");
	strcpy(keynames[199], "J3-RIGHT4");
	strcpy(keynames[200], "J3-BUT1");
	strcpy(keynames[201], "J3-BUT2");
	strcpy(keynames[202], "J3-BUT3");
	strcpy(keynames[203], "J3-BUT4");
	strcpy(keynames[204], "J3-BUT5");
	strcpy(keynames[205], "J3-BUT6");
	strcpy(keynames[206], "J3-BUT7");
	strcpy(keynames[207], "J3-BUT8");
	strcpy(keynames[208], "J3-BUT9");
	strcpy(keynames[209], "J3-BUT10");
	strcpy(keynames[210], "J3-BUT11");
	strcpy(keynames[211], "J3-BUT12");

	strcpy(keynames[212], "J4-UP");
	strcpy(keynames[213], "J4-DOWN");
	strcpy(keynames[214], "J4-LEFT");
	strcpy(keynames[215], "J4-RIGHT");
	strcpy(keynames[216], "J4-UP2");
	strcpy(keynames[217], "J4-DOWN2");
	strcpy(keynames[218], "J4-LEFT2");
	strcpy(keynames[219], "J4-RIGHT2");
	strcpy(keynames[220], "J4-UP3");
	strcpy(keynames[221], "J4-DOWN3");
	strcpy(keynames[222], "J4-LEFT3");
	strcpy(keynames[223], "J4-RIGHT3");
	strcpy(keynames[224], "J4-UP4");
	strcpy(keynames[225], "J4-DOWN4");
	strcpy(keynames[226], "J4-LEFT4");
	strcpy(keynames[227], "J4-RIGHT4");
	strcpy(keynames[228], "J4-BUT1");
	strcpy(keynames[229], "J4-BUT2");
	strcpy(keynames[230], "J4-BUT3");
	strcpy(keynames[231], "J4-BUT4");
	strcpy(keynames[232], "J4-BUT5");
	strcpy(keynames[233], "J4-BUT6");
	strcpy(keynames[234], "J4-BUT7");
	strcpy(keynames[235], "J4-BUT8");
	strcpy(keynames[236], "J4-BUT9");
	strcpy(keynames[237], "J4-BUT10");
	strcpy(keynames[238], "J4-BUT11");
	strcpy(keynames[239], "J4-BUT12");


	allegro_gl_begin();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 4.0/3.0, 1.0, 300.0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	GLuint pat01tex;
	allegro_gl_set_texture_format(GL_RGB);
	pat01tex = allegro_gl_make_texture(&mb_pattern01_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	GLuint pat02tex;
	allegro_gl_set_texture_format(GL_RGB);
	pat02tex = allegro_gl_make_texture(&mb_pattern02_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	allegro_gl_end();

	timer.install();
	double i=0;
	int choicex=0, choicey=0;;
	int upkey=0, downkey=0;
	int waitforkey=0;
	while(!(key[KEY_ENTER] || key[KEY_SPACE] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL)) || choicey!=6)
	{
		rest(1);
		i+=timer.seconds()/2.0;
		timer.reset();

		poll_joystick();
		al_poll_duh(dp);

		if(i>1)
			i=i-1;
		if((key[KEY_DOWN] || controls[0].keydown(KEYDOWN)) && downkey==0)
		{
			choicey++;
			downkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if((key[KEY_UP] || controls[0].keydown(KEYUP)) && upkey==0)
		{
			choicey--;
			upkey=1;
			play_sample(&mb_menumove_wav, 255, 128, 1000, 0);
		}
		if(!(key[KEY_DOWN] || controls[0].keydown(KEYDOWN)))
			downkey=0;
		if(!(key[KEY_UP] || controls[0].keydown(KEYUP)))
			upkey=0;
		if(choicey>6)
			choicey=0;
		if(choicey<0)
			choicey=6;

		if((key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)))
			choicex=1;
		if((key[KEY_LEFT] || controls[0].keydown(KEYLEFT)))
			choicex=0;

		if(waitforkey==1)
		{
			int pressedkey=0;
			int i;
			while(pressedkey==0)
			{
				poll_joystick();
				al_poll_duh(dp);
				for(i=1; i<128; i++)
					if(key[i] && i!=KEY_ESC)
						pressedkey=i;
				for(i=0; i<num_joysticks; i++)
				{
					int j;
					if(joy[i].num_sticks>=1)
					{
						if(joy[i].stick[0].num_axis>=1)
						{
							if(joy[i].stick[0].axis[0].d1)
								pressedkey=128+i*28+2+0;
							if(joy[i].stick[0].axis[0].d2)
								pressedkey=128+i*28+3+0;
						}
						if(joy[i].stick[0].num_axis>=2)
						{
							if(joy[i].stick[0].axis[1].d1)
								pressedkey=128+i*28+0+0;
							if(joy[i].stick[0].axis[1].d2)
								pressedkey=128+i*28+1+0;
						}
					}
					if(joy[i].num_sticks>=2)
					{
						if(joy[i].stick[1].num_axis>=1)
						{
							if(joy[i].stick[1].axis[0].d1)
								pressedkey=128+i*28+2+4;
							if(joy[i].stick[1].axis[0].d2)
								pressedkey=128+i*28+3+4;
						}
						if(joy[i].stick[1].num_axis>=2)
						{
							if(joy[i].stick[1].axis[1].d1)
								pressedkey=128+i*28+0+4;
							if(joy[i].stick[1].axis[1].d2)
								pressedkey=128+i*28+1+4;
						}
					}
					if(joy[i].num_sticks>=3)
					{
						if(joy[i].stick[2].num_axis>=1)
						{
							if(joy[i].stick[2].axis[0].d1)
								pressedkey=128+i*28+2+8;
							if(joy[i].stick[2].axis[0].d2)
								pressedkey=128+i*28+3+8;
						}
						if(joy[i].stick[2].num_axis>=2)
						{
							if(joy[i].stick[2].axis[1].d1)
								pressedkey=128+i*28+0+8;
							if(joy[i].stick[2].axis[1].d2)
								pressedkey=128+i*28+1+8;
						}
					}
					if(joy[i].num_sticks>=4)
					{
						if(joy[i].stick[3].num_axis>=1)
						{
							if(joy[i].stick[3].axis[0].d1)
								pressedkey=128+i*28+2+12;
							if(joy[i].stick[3].axis[0].d2)
								pressedkey=128+i*28+3+12;
						}
						if(joy[i].stick[3].num_axis>=2)
						{
							if(joy[i].stick[3].axis[1].d1)
								pressedkey=128+i*28+0+12;
							if(joy[i].stick[3].axis[1].d2)
								pressedkey=128+i*28+1+12;
						}
					}

					for(j=0; j<joy[i].num_buttons; j++)
					{
						if(joy[i].button[j].b)
							pressedkey=128+16+i*28+j;
					}
				}
			}
			if(choicex==0 && choicey==0)
				options.p1up=pressedkey;
			if(choicex==0 && choicey==1)
				options.p1down=pressedkey;
			if(choicex==0 && choicey==2)
				options.p1left=pressedkey;
			if(choicex==0 && choicey==3)
				options.p1right=pressedkey;
			if(choicex==0 && choicey==4)
				options.p1special=pressedkey;
			if(choicex==0 && choicey==5)
				options.p1fire=pressedkey;
			if(choicex==1 && choicey==0)
				options.p2up=pressedkey;
			if(choicex==1 && choicey==1)
				options.p2down=pressedkey;
			if(choicex==1 && choicey==2)
				options.p2left=pressedkey;
			if(choicex==1 && choicey==3)
				options.p2right=pressedkey;
			if(choicex==1 && choicey==4)
				options.p2special=pressedkey;
			if(choicex==1 && choicey==5)
				options.p2fire=pressedkey;
			waitforkey=0;
			if(pressedkey<115)
				while(key[pressedkey])
					al_poll_duh(dp);
			else
				while(special_keydown(pressedkey))
				{
					poll_joystick();
					al_poll_duh(dp);
				}
		}

		if(choicey!=6 && (key[KEY_ENTER]||key[KEY_SPACE]||controls[0].keydown(KEYFIRE)||controls[0].keydown(KEYSPECIAL)))
		{
			while(key[KEY_ENTER] || key[KEY_SPACE] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL))
			{
				poll_joystick();
				al_poll_duh(dp);
			}
			waitforkey=1;
		}

		allegro_gl_begin();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 800, 0, 600, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, pat01tex);
		glEnable(GL_BLEND);
		glColor4f(sin(M_PI*i)/2+0.3, sin(M_PI*i)/2+0.3, 0.7, 0.4);
		glBegin(GL_QUADS);
		glTexCoord2f(-i, i);
		glVertex2i(0, 0);
		glTexCoord2f(800.0/256.0-i, i);
		glVertex2i(800, 0);
		glTexCoord2f(800.0/256.0-i, 600.0/256.0+i);
		glVertex2i(800, 600);
		glTexCoord2f(-i, 600.0/256.0+i);
		glVertex2i(0, 600);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, pat02tex);
		glColor4f(0.4, 0.4, 0.4, sin(M_PI*i)/2*0.6);
		glBegin(GL_QUADS);
		glTexCoord2f(i, i);
		glVertex2i(0, 0);
		glTexCoord2f(800.0/256.0+i, i);
		glVertex2i(800, 0);
		glTexCoord2f(800.0/256.0+i, 600.0/256.0+i);
		glVertex2i(800, 600);
		glTexCoord2f(i, 600.0/256.0+i);
		glVertex2i(0, 600);
		glEnd();

		glDisable(GL_TEXTURE_2D);
		
		if(choicey<6)
		{
			glTranslated(choicex*400+220, (6-choicey)*40, 0);
			glBegin(GL_QUADS);
			glColor4f(0, 0, 1, 0.6);
			glVertex2i(0, 0);
			glVertex2i(200, 0);
			glVertex2i(200, 42);
			glVertex2i(0, 42);
			glEnd();
		}
		else
		{
			glTranslated(190, 0, 0);
			glBegin(GL_QUADS);
			glColor4f(0, 0, 1, 0.6);
			glVertex2i(0, 0);
			glVertex2i(360, 0);
			glVertex2i(360, 42);
			glVertex2i(0, 42);
			glEnd();
		}
//		glLoadIdentity();
//		glBegin(GL_QUADS);
//		glColor4f(0, 0, 1, 0.3);
//		glVertex2i(!options.camerashake*90+455, 350);
//		glVertex2i(!options.camerashake*90+455+80, 350);
//		glVertex2i(!options.camerashake*90+455+80, 390);
//		glVertex2i(!options.camerashake*90+455, 390);
//		glVertex2i(!options.logoshow*90+495, 300);
//		glVertex2i(!options.logoshow*90+495+80, 300);
//		glVertex2i(!options.logoshow*90+495+80, 340);
//		glVertex2i(!options.logoshow*90+495, 340);
//		glVertex2i(!options.fpsshow*90+620, 250);
//		glVertex2i(!options.fpsshow*90+620+80, 250);
//		glVertex2i(!options.fpsshow*90+620+80, 290);
//		glVertex2i(!options.fpsshow*90+620, 290);
//		glEnd();

		text.begin();
		glColor3f(0, 1, 0);
		text.print(240, 560, "CONTROLS OPTIONS");
		glColor3f(1, 1, 0.8);
		text.print(20, 480, "DETECTED INPUT DEVICES:");
		glColor3f(0.7, 0.7, 1);
		text.print(20, 440, "KEYBOARD");
		if(num_joysticks>0)
			text.print(220, 440, ", JOYSTICK1");
		if(num_joysticks>1)
			text.print(450, 440, ", JOYSTICK2");
		if(num_joysticks>2)
			text.print(20, 400, "JOYSTICK3");
		if(num_joysticks>3)
			text.print(220, 400, ", JOYSTICK4");

		glColor3f(1, 1, 0.8);
		text.print(40, 300, "PLAYER 1");
		text.print(20, 240, "FORWARD:");
		text.print(20, 200, "REVERSE:");
		text.print(20, 160, "LEFT:");
		text.print(20, 120, "RIGHT:");
		text.print(20, 80, "KICK:");
		text.print(20, 40, "FIRE:");

		text.print(440, 300, "PLAYER 2");
		text.print(420, 240, "FORWARD:");
		text.print(420, 200, "REVERSE:");
		text.print(420, 160, "LEFT:");
		text.print(420, 120, "RIGHT:");
		text.print(420, 80, "KICK:");
		text.print(420, 40, "FIRE:");

		text.print(200, 0, "BACK TO OPTIONS");

		glColor3f(0.7, 1, 0.7);
		if(waitforkey==0)
		{
			text.print(220, 240, keynames[options.p1up]);
			text.print(220, 200, keynames[options.p1down]);
			text.print(220, 160, keynames[options.p1left]);
			text.print(220, 120, keynames[options.p1right]);
			text.print(220, 80, keynames[options.p1special]);
			text.print(220, 40, keynames[options.p1fire]);
			text.print(620, 240, keynames[options.p2up]);
			text.print(620, 200, keynames[options.p2down]);
			text.print(620, 160, keynames[options.p2left]);
			text.print(620, 120, keynames[options.p2right]);
			text.print(620, 80, keynames[options.p2special]);
			text.print(620, 40, keynames[options.p2fire]);
		}
		else
		{
			if(choicex==0 && choicey==0)
				text.print(220, 240, "???"); 
			else
				text.print(220, 240, keynames[options.p1up]);
			if(choicex==0 && choicey==1)
				text.print(220, 200, "???"); 
			else
				text.print(220, 200, keynames[options.p1down]);
			if(choicex==0 && choicey==2)
				text.print(220, 160, "???"); 
			else
				text.print(220, 160, keynames[options.p1left]);
			if(choicex==0 && choicey==3)
				text.print(220, 120, "???"); 
			else
				text.print(220, 120, keynames[options.p1right]);
			if(choicex==0 && choicey==4)
				text.print(220, 80, "???"); 
			else
				text.print(220, 80, keynames[options.p1special]);
			if(choicex==0 && choicey==5)
				text.print(220, 40, "???"); 
			else
				text.print(220, 40, keynames[options.p1fire]);
			if(choicex==1 && choicey==0)
				text.print(620, 240, "???"); 
			else
				text.print(620, 240, keynames[options.p2up]);
			if(choicex==1 && choicey==1)
				text.print(620, 200, "???"); 
			else
				text.print(620, 200, keynames[options.p2down]);
			if(choicex==1 && choicey==2)
				text.print(620, 160, "???"); 
			else
				text.print(620, 160, keynames[options.p2left]);
			if(choicex==1 && choicey==3)
				text.print(620, 120, "???"); 
			else
				text.print(620, 120, keynames[options.p2right]);
			if(choicex==1 && choicey==4)
				text.print(620, 80, "???"); 
			else
				text.print(620, 80, keynames[options.p2special]);
			if(choicex==1 && choicey==5)
				text.print(620, 40, "???"); 
			else
				text.print(620, 40, keynames[options.p2fire]);

		}
		text.end();

		glFlush();
		allegro_gl_flip();
		allegro_gl_end();
	}
	play_sample(&mb_menusel_wav, 255, 128, 1000, 0);
	while(key[KEY_ENTER] || key[KEY_SPACE] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL))
	{
		poll_joystick();
		al_poll_duh(dp);
	}
	allegro_gl_begin();
	glDeleteTextures(1, &pat01tex);
	glDeleteTextures(1, &pat02tex);
	allegro_gl_end();
}


void creditsMenu(void)
{
	GLuint machineballtex;
	allegro_gl_set_texture_format(GL_RGBA);
	BITMAP *bmp = create_bitmap_ex(32, 256, 64);
	blit(&mb_machineball_bmp, bmp, 0, 0, 0, 0, 256, 64);
	machineballtex = allegro_gl_make_masked_texture(bmp);
	destroy_bitmap(bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	allegro_gl_begin();

	glClearColor(0, 0, 0.1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, machineballtex);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2i(200, 380);
	glTexCoord2f(1, 0);
	glVertex2i(600, 380);
	glTexCoord2f(1, 1);
	glVertex2i(600, 480);
	glTexCoord2f(0, 1);
	glVertex2i(200, 480);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, bennykramekwebtex);
	glColor3f(0.3, 0.3, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2i(144, 270);
	glTexCoord2f(1, 0);
	glVertex2i(656, 270);
	glTexCoord2f(1, 1);
	glVertex2i(656, 334);
	glTexCoord2f(0, 1);
	glVertex2i(144, 334);
	glEnd();	

	text.begin();
	glColor3f(0.3, 0.3, 1);
	text.print(240, 340, "BENNY KRAMEK 2002");
	glColor3f(0.7, 0.7, 0.7);
	text.print(10, 120, "OPEN-DYNAMICS-ENGINE : Q12.ORG/ODE");
	text.print(10, 80, "ALLEGRO : ALLEG.SF.NET");
	text.print(10, 40, "OPENGL : OPENGL.ORG");
	text.print(10, 0, "DUMB : DUMB.SF.NET");
	text.end();
	
	glFlush();
	allegro_gl_flip();
	allegro_gl_end();

	while(!(key[KEY_ENTER] || key[KEY_SPACE] || key[KEY_ESC] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL)))
	{
		poll_joystick();
		al_poll_duh(dp);
	}
	while(key[KEY_ENTER] || key[KEY_SPACE] || key[KEY_ESC] || controls[0].keydown(KEYFIRE) || controls[0].keydown(KEYSPECIAL))
	{
		poll_joystick();
		al_poll_duh(dp);
	}

	glDeleteTextures(1, &machineballtex);
}

