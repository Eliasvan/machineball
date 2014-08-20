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

#include "camera.h"
#include "court.h"
#include "explosion.h"
#include "options.h"

Camera camera;

void Camera::adjust(double x1, double y1, double x2, double y2, double x3, double y3, double h)
{
	double rectwidth, rectheight, minx, miny, maxx, maxy, centerx, centery, height;


	minx=x1;
	if(x2<minx)
		minx=x2;
	if(x3<minx)
		minx=x3;

	miny=y1;
	if(y2<miny)
		miny=y2;
	if(y3<miny)
		miny=y3;


	maxx=x1;
	if(x2>maxx)
		maxx=x2;
	if(x3>maxx)
		maxx=x3;

	maxy=y1;
	if(y2>maxy)
		maxy=y2;
	if(y3>maxy)
		maxy=y3;

	rectwidth=maxx-minx;
	rectheight=maxy-miny;

	if(rectheight*(4.0/3.0)>rectwidth)
		rectwidth=rectheight*(4.0/3.0);
	else
		rectheight=rectwidth*(3.0/4.0);

	centerx=(minx+maxx)/2.0;
	centery=(miny+maxy)/2.0;

	if(rectheight>2*court.y)
	{
		centery=0;
	}
	else if(centery-rectheight/2.0<-court.y) 
		centery=-court.y+rectheight/2.0;
	else if(centery+rectheight/2.0>court.y)
		centery=court.y-rectheight/2.0;
	height=rectwidth*0.94+70;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 4.0/3.0, (height>150?height:150)+h-149, (height>150?height:150)+h+20);
	glMatrixMode(GL_MODELVIEW);
	if(explosions.numexplosions==0 || !options.camerashake)
		gluLookAt(centerx, centery, (height>150?height:150)+h, centerx, centery, 0, 0, 1, 0);
	else
	{
		double deltax, deltay;
		deltax=(rand()%100)/20.0-2.5;
		deltay=(rand()%100)/20.0-2.5;
		gluLookAt(centerx+deltax, centery+deltay, (height>150?height:150)+h, centerx+deltax, centery+deltay, 0, 0, 1, 0);
	}
}
