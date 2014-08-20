// Machine Ball
//  By Benny Kramek
//  benny@kramekweb.com
//  http://benny.kramekweb.com
//
//
// Source Code is released under the GPL
//

#define ALLEGRO_STATICLINK
#include "court.h"
#include "court.h"

Court court;

void Court::init(int size)
{
	if(size==0)
	{
		x=100;
		y=65;
	}
	else if(size==1)
	{
		x=200;
		y=100;
	}
	else if(size==2)
	{
		x=300;
		y=150;
	}
	else if(size==3)
	{
		x=400;
		y=200;
	}
	wall[0] = dCreatePlane(0, 0, -1, 0, -y);
	wall[1] = dCreatePlane(0, 0, 1, 0, -y);

	wall[2] = dCreatePlane(0, cos(M_PI/4), -sin(M_PI/4), 0, cos(M_PI/4)*-x - sin(M_PI/4)*(y-20));
	wall[3] = dCreatePlane(0, cos(M_PI/8), -sin(M_PI/8), 0, cos(M_PI/8)*(-x+6) - sin(M_PI/8)*(y-14));
	wall[4] = dCreatePlane(0, sin(M_PI/8), -cos(M_PI/8), 0, sin(M_PI/8)*(-x+14) - cos(M_PI/8)*(y-6));
	
	wall[5] = dCreatePlane(0, -cos(M_PI/4), -sin(M_PI/4), 0, cos(M_PI/4)*-x - sin(M_PI/4)*(y-20));
	wall[6] = dCreatePlane(0, -cos(M_PI/8), -sin(M_PI/8), 0, cos(M_PI/8)*(-x+6) - sin(M_PI/8)*(y-14));
	wall[7] = dCreatePlane(0, -sin(M_PI/8), -cos(M_PI/8), 0, sin(M_PI/8)*(-x+14) - cos(M_PI/8)*(y-6));

	wall[8] = dCreatePlane(0, cos(M_PI/4), sin(M_PI/4), 0, cos(M_PI/4)*-x - sin(M_PI/4)*(y-20));
	wall[9] = dCreatePlane(0, cos(M_PI/8), sin(M_PI/8), 0, cos(M_PI/8)*(-x+6) - sin(M_PI/8)*(y-14));
	wall[10] = dCreatePlane(0, sin(M_PI/8), cos(M_PI/8), 0, sin(M_PI/8)*(-x+14) - cos(M_PI/8)*(y-6));

	wall[11] = dCreatePlane(0, -cos(M_PI/4), sin(M_PI/4), 0, cos(M_PI/4)*-x - sin(M_PI/4)*(y-20));
	wall[12] = dCreatePlane(0, -cos(M_PI/8), sin(M_PI/8), 0, cos(M_PI/8)*(-x+6) - sin(M_PI/8)*(y-14));
	wall[13] = dCreatePlane(0, -sin(M_PI/8), cos(M_PI/8), 0, sin(M_PI/8)*(-x+14) - cos(M_PI/8)*(y-6));

	wall[14] = dCreatePlane(0, 1, 0, 0, -x-30);
	wall[15] = dCreatePlane(0, -1, 0, 0, -x-30);
	
	int i;
	for(i=0; i<4; i++)
		corner[i] = dCreateBox(0, 1000, 1000, 1000);
	dGeomSetPosition(corner[0], -court.x-500, 500+35, 0);
	dGeomSetPosition(corner[1], -court.x-500, -500-35, 0);
	dGeomSetPosition(corner[2], court.x+500, 500+35, 0);
	dGeomSetPosition(corner[3], court.x+500, -500-35, 0);

	floor = dCreatePlane(0, 0, 0, 1, 0);
}

void Court::destroy(void)
{
	int i;
	for(i=0; i<4; i++)
		dGeomDestroy(corner[i]);
	dGeomDestroy(floor);
	for(i=0; i<16; i++)
		dGeomDestroy(wall[i]);
}

