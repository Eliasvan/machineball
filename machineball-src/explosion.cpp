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

#include "mbdata.h"

#include "explosion.h"

void Explosion::init(double xp, double yp, double zp, double startradius, double speedp, double endradiusp)
{
	active=1;
	
	life=0;
	lifetime=(endradiusp-startradius)/speedp;
	x=xp;
	y=yp;
	z=zp;
	curradius=startradius;
	speed=speedp;
	endradius=endradiusp;
}

void Explosion::update(double t)
{
	curradius+=speed*t;
	life+=t;
}

void ExplosionList::init(void)
{
	numexplosions=0;
	freepos=0;

	int i;
	for(i=0; i<MAX_EXPLOSIONS; i++)
		list[i].active=0;
}

void ExplosionList::addExplosion(double xp, double yp, double zp, double startradius, double speedp, double endradiusp)
{
	list[freepos].init(xp, yp, zp, startradius, speedp, endradiusp);
	numexplosions++;
	while(list[freepos].active)
		freepos++;
	play_sample(&mb_explosion_wav, 255, 128, 1000, 0);
}


Explosion & ExplosionList::getExplosion(int n)
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

void ExplosionList::Update(double t)
{
	int i;
	for(i=0; i<MAX_EXPLOSIONS; i++)
	{
		if(list[i].active)
		{
			list[i].update(t);
			if(list[i].curradius>list[i].endradius)
			{
				// Explosion dies:
				list[i].active=0;
				if(i<freepos)
					freepos=i;
				numexplosions--;
			}
		}
	}
}

void ExplosionList::clearall(void)
{
	int i;
	numexplosions=0;

	freepos=0;

	for(i=0; i<MAX_EXPLOSIONS; i++)
		list[i].active=0;
}

ExplosionList explosions;
