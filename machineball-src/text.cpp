// Machine Ball
//  By Benny Kramek
//  benny@kramekweb.com
//  http://benny.kramekweb.com
//
//
// Source Code is released under the GPL
//

#define ALLEGRO_STATICLINK
#include <string.h>

#include <allegro.h>
#include "mbdata.h"

#include "text.h"

Text text;

void Text::init(void)
{
	// Position of each character in the original BITMAP
	int char_row[128];
	int char_start[128];
	int char_end[128];

	BITMAP * bmp = create_bitmap_ex(32, 256, 256);
	blit(&mb_font, bmp, 0, 0, 0, 0, 256, 256);


	// Find out how many rows of characters are in the original BITMAP
	int i;
	rows=0;
	for(i=0; i<256; i++)
		if(getr(getpixel(bmp, 0, i))==255 && getg(getpixel(bmp, 0, i))==0)
			rows++;

	// Calculate the position of each character in the original BITMAP
	int cur_row=0;
	chars=0;
	i=0;
	while(cur_row<rows)
	{
		char_row[chars]=cur_row;
		char_start[chars]=i;
		while(!(getr(getpixel(bmp, i, (int)((float)cur_row/rows*256)+128/rows))==255 && getg(getpixel(bmp, i, (int)((float)cur_row/rows*256)+128/rows))==0))
			i++;
		char_end[chars]=i-1;
		chars++;
		i++;
		if(getr(getpixel(bmp, i, (int)((float)cur_row/rows*256)+128/rows))==255 && getg(getpixel(bmp, i, (int)((float)cur_row/rows*256)+128/rows))==0)
		{
			cur_row++;
			i=0;
		}
	}
	chars--;

	// Erase the pink guide lines from the original BITMAP
	int x, y;
	for(y=0; y<256; y++)
		for(x=0; x<256; x++)
			if(getr(getpixel(bmp, x, y))==255 && getg(getpixel(bmp, x, y))==0)
				putpixel(bmp, x, y, makeacol(0, 0, 0, 0));


	// Create the OpenGL texture from the BITMAP
	allegro_gl_set_texture_format(GL_INTENSITY4);
	texture = allegro_gl_make_texture(bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// Create the OpenGL display lists for each character
	list = glGenLists(chars);
	glBindTexture(GL_TEXTURE_2D, texture);
	for(i=0; i<chars; i++)
	{
		glNewList(list+i, GL_COMPILE);
		glBegin(GL_QUADS);
		glTexCoord2f((float)char_start[i]/256.0, 1-((char_row[i]+1)*((int)(256/rows))-1)/256.0);
		glVertex2i(0, 0);
		glTexCoord2f((float)char_end[i]/256.0, 1-((char_row[i]+1)*((int)(256/rows))-1)/256.0);
		glVertex2i(char_end[i]-char_start[i], 0);
		glTexCoord2f((float)char_end[i]/256.0, 1-(char_row[i]*((int)(256/rows)))/256.0);
		glVertex2i(char_end[i]-char_start[i], 256/rows-1);
		glTexCoord2f((float)char_start[i]/256.0, 1-(char_row[i]*((int)(256/rows)))/256.0);
		glVertex2i(0, 256/rows-1);
		glEnd();
		glTranslated(char_end[i]-char_start[i]+2, 0, 0);
		glEndList();

	}
	
	destroy_bitmap(bmp);
}

void Text::destroy(void)
{
	glDeleteLists(list, chars);
	glDeleteTextures(1, &texture);
}

void Text::setup(void)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	if(glIsEnabled(GL_DEPTH_TEST)==GL_TRUE)
	{
		depth_test=1;
		glDisable(GL_DEPTH_TEST);
	}
	else
		depth_test=0;
	if(glIsEnabled(GL_TEXTURE_2D)==GL_TRUE)
		texture_2d=1;
	else
	{
		texture_2d=0;
		glEnable(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, texture);
}

void Text::restore(void)
{
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	if(depth_test==1)
		glEnable(GL_DEPTH_TEST);
	if(texture_2d==0)
		glDisable(GL_TEXTURE_2D);
}

void Text::print(int x, int y, char * s)
{
	if(staterestore==0)
		setup();

	glLoadIdentity();
	glEnable(GL_BLEND);
	glTranslated(x, y, 0);
	glListBase(list-32);
	glCallLists(strlen(s), GL_BYTE, s);
	if(staterestore==0)
		restore();
}

