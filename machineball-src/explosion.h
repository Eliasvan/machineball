// Machine Ball
//  By Benny Kramek
//  benny@kramekweb.com
//  http://benny.kramekweb.com
//
//
// Source Code is released under the GPL
//

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include <allegro.h>

class Explosion
{
	public:
		int active; // 1=alive
		
		double x, y, z;
		double curradius;
		double speed;
		double endradius;
		double life;
		double lifetime;

		void init(double xp, double yp, double zp, double startradius, double speedp, double endradiusp);
		void update(double t);
};


class ExplosionList
{
	private:
		enum { MAX_EXPLOSIONS=10 };
		Explosion list[MAX_EXPLOSIONS];
		int freepos;

	public:
		int numexplosions;
		void init(void);
		
		void addExplosion(double xp, double yp, double zp, double startradius, double speedp, double endradiusp);
		Explosion & getExplosion(int n);

		void clearall(void);
		void Update(double t);
};
		
extern ExplosionList explosions;

#endif

