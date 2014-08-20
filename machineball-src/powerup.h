// Machine Ball
//  By Benny Kramek
//  benny@kramekweb.com
//  http://benny.kramekweb.com
//
//
// Source Code is released under the GPL
//

#ifndef _POWERUP_H_
#define _POWERUP_H_

class Powerup
{
	public:
		int active; // 1=alive
		
		double x, y; // Position of powerup
		int type; // What powerup contains:
		// 0 - turbo
		// 1 - shield
		// 2 - mine
		// 3 - missle
		// 4 - meshatek
		int question; // If powerup is displayed as a question mark

		void init(double xp, double yp, int typep, int questionp);
};


class PowerupList
{
	private:
		int maxx, maxy;
		
		enum { MAX_POWERUPS=10 };
		Powerup list[MAX_POWERUPS];
		int freepos;

	public:
		double spin;
		
		int numpowerups;
		void init(int maxxp, int maxyp);
		
		void addPowerup(int t);
		Powerup & getPowerup(int n);
		void removePowerup(int n);
};
		
extern PowerupList powerups;

class Mine
{
	public:
		int active;
		double x, y;

		int danger;

		int owner;

		void init(double xp, double yp);
};

class MineList
{
	private:
		enum { MAX_MINES=200 };
		Mine list[MAX_MINES];
		int freepos;
		
	public:
		int nummines;
		void init(void);

		void addMine(double xp, double yp, int o);
		Mine & getMine(int n);
		void removeMine(int n);
};

extern MineList mines;

class Projectile
{
	public: 
		int active;

		int owner;
		int type;

		double angle;
		double length;
		double x, y;
		double velx, vely;
};

class ProjectileList
{
	private:
		enum { MAX_PROJECTILES=10 };
		Projectile list[MAX_PROJECTILES];
		int freepos;
		
	public:
		int numprojectiles;
		void init(void);
		void update(double m);

		void addProjectile(double xp, double yp, double velxp, double velyp, double a, int t, int o);
		Projectile & getProjectile(int n);
		void removeProjectile(int n);
};

extern ProjectileList projectiles;

#endif

