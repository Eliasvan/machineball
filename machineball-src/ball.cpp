// Machine Ball
//  By Benny Kramek
//  benny@kramekweb.com
//  http://benny.kramekweb.com
//
//
// Source Code is released under the GPL
//

#define ALLEGRO_STATICLINK
#include "ball.h"

extern dWorldID world;

Ball ball;

void Ball::init(int type)
{
	body = dBodyCreate(world);
	if(type==0)
	{
		radius=5;
		mass=10;
		mu=5000;
		bounce=0.7;
		friction=4;
	}
	if(type==1)
	{
		radius=8;
		mass=50;
		mu=20000;
		bounce=0.2;
		friction=15;
	}
	if(type==2)
	{
		radius=7;
		mass=3;
		mu=2000;
		bounce=0.5;
		friction=0.5;
	}
	if(type==3)
	{
		radius=4;
		mass=8;
		mu=3000;
		bounce=0.6;
		friction=3;
	}
	if(type==4)
	{
		radius=10;
		mass=25;
		mu=6000;
		bounce=0.4;
		friction=10;
	}
	if(type==5)
	{
		radius=11;
		mass=5;
		mu=2000;
		bounce=0.8;
		friction=1;
	}
	dMass m;
	dMassSetSphere(&m, 1, radius);
	dMassAdjust(&m, mass);
	dBodySetMass(body, &m);
	dBodySetPosition(body, 0, 0, radius+30);

	geom = dCreateSphere(0, radius);
	dGeomSetBody(geom, body);

	reset();
}

void Ball::destroy(void)
{
	dGeomDestroy(geom);
	dBodyDestroy(body);
}

void Ball::reset(void)
{
	dBodySetPosition(body, 0, 0, radius+30);
	dBodySetLinearVel(body, 0, 0, 0);
	dBodySetAngularVel(body, 0, 0, 0);
	
	dBodySetLinearVel(body, 0, (rand()%2000)/100.0-10, 75);
}

