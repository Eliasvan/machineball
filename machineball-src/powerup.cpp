// Machine Ball
//  By Benny Kramek
//  benny@kramekweb.com
//  http://benny.kramekweb.com
//
//
// Source Code is released under the GPL
//

#define ALLEGRO_STATICLINK
#include <math.h>
#include <stdlib.h>
#include "machine.h"
#include "powerup.h"

void Powerup::init(double xp, double yp, int typep, int questionp)
{
	active=1;
	
	x=xp;
	y=yp;
	type=typep;
	question=questionp;
}

void Mine::init(double xp, double yp)
{
	active=1;
	danger=0;
	x=xp;
	y=yp;
}

void PowerupList::init(int maxxp, int maxyp)
{
	maxx=maxxp;
	maxy=maxyp;

	numpowerups=0;
	freepos=0;

	int i;
	for(i=0; i<MAX_POWERUPS; i++)
		list[i].active=0;

	spin=0;
}

void PowerupList::addPowerup(int t)
{
	const dReal * pos1 = dBodyGetPosition(machine[0].body[0]);
	const dReal * pos2 = dBodyGetPosition(machine[0].body[1]);
	int xpos = rand()%maxx*2-maxx;
	int ypos = rand()%maxy*2-maxy;
	
	while((xpos-pos1[0])*(xpos-pos1[0])+(ypos-pos1[1])*(ypos-pos1[1]) < (maxy*maxy/4) || (xpos-pos2[0])*(xpos-pos2[0])+(ypos-pos2[1])*(ypos-pos2[1]) < (maxy*maxy/4))
	{
		xpos = rand()%maxx*2-maxx;
		ypos = rand()%maxy*2-maxy;
	}
	
	list[freepos].init(xpos, ypos, t, 0);
	numpowerups++;
	while(list[freepos].active)
		freepos++;
// 	list[freepos].init(rand()%maxx*2-maxx, rand()%maxy*2-maxy, t, 0);
// 	numpowerups++;
// 	while(list[freepos].active)
// 		freepos++;
}


Powerup & PowerupList::getPowerup(int n)
{
	int i, j;
	i=0;
	j=0;
	do
	{
		if(list[i].active)
			j++;
		i++;
	} while(j<n+1);
	return list[i-1];
}

void PowerupList::removePowerup(int n)
{
	int i, j;
	i=0;
	j=0;
	do
	{
		if(list[i].active)
			j++;
		i++;
	} while(j<n+1);
	list[i-1].active=0;
	if(i-1<freepos)
		freepos=i-1;
	numpowerups--;
}

void MineList::init(void)
{
	nummines=0;
	freepos=0;

	int i;
	for(i=0; i<MAX_MINES; i++)
		list[i].active=0;
}

void MineList::addMine(double xp, double yp, int o)
{
	list[freepos].init(xp, yp);
	nummines++;
	list[freepos].owner=o;
	while(list[freepos].active)
		freepos++;
}


Mine & MineList::getMine(int n)
{
	int i, j;
	i=0;
	j=0;
	do
	{
		if(list[i].active)
			j++;
		i++;
	} while(j<n+1);
	return list[i-1];
}

void MineList::removeMine(int n)
{
	int i, j;
	i=0;
	j=0;
	do
	{
		if(list[i].active)
			j++;
		i++;
	} while(j<n+1);
	list[i-1].active=0;
	if(i-1<freepos)
		freepos=i-1;
	nummines--;
}

void ProjectileList::init(void)
{
	numprojectiles=0;
	freepos=0;

	int i;
	for(i=0; i<MAX_PROJECTILES; i++)
		list[i].active=0;
}

void ProjectileList::addProjectile(double xp, double yp, double velxp, double velyp, double a, int t, int o)
{
	list[freepos].active=1;
	list[freepos].owner=o;
	list[freepos].x=xp;
	list[freepos].y=yp;
	list[freepos].velx=velxp;
	list[freepos].vely=velyp;
	list[freepos].type=t;
	list[freepos].angle=a;
	list[freepos].length=20;
	numprojectiles++;
	while(list[freepos].active)
		freepos++;
}


Projectile & ProjectileList::getProjectile(int n)
{
	int i, j;
	i=0;
	j=0;
	do
	{
		if(list[i].active)
			j++;
		i++;
	} while(j<n+1);
	return list[i-1];
}

void ProjectileList::removeProjectile(int n)
{
	int i, j;
	i=0;
	j=0;
	do
	{
		if(list[i].active)
			j++;
		i++;
	} while(j<n+1);
	list[i-1].active=0;
	if(i-1<freepos)
		freepos=i-1;
	numprojectiles--;
}

void ProjectileList::update(double m)
{
	int i;
	for(i=0; i<MAX_PROJECTILES; i++)
	{
		if(list[i].active)
		{
			list[i].x+=list[i].velx*m;
			list[i].y+=list[i].vely*m;
			double length=sqrt(list[i].x*list[i].x + list[i].y*list[i].y);
			list[i].velx+=list[i].velx/length*40*m;
			list[i].vely+=list[i].vely/length*40*m;
			list[i].length+=30*m;
		}
	}
}

PowerupList powerups;
MineList mines;
ProjectileList projectiles;

