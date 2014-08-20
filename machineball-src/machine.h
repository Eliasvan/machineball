// Machine Ball
//  By Benny Kramek
//  benny@kramekweb.com
//  http://benny.kramekweb.com
//
//
// Source Code is released under the GPL
//

#ifndef _MACHINE_H_
#define _MACHINE_H_

#define dSINGLE
#include <ode/ode.h>
#include <allegro.h>

#include "mbdata.h"

#define MACHINE_ENGINE_COAST   0
#define MACHINE_ENGINE_BRAKE   1
#define MACHINE_ENGINE_FORWARD 2
#define MACHINE_ENGINE_REVERSE 3

class Machine
{
	private:
		double pushtime;
		double energy;
		double upsidedowntime;
	
	public:
		dBodyID body[2];
		dBodyID wheel[3];
		dGeomID sphere[3];
		dGeomID geom[4];
		dGeomID finE[2];

		dJointID joint;
		dJointID wheeljoint[4];

		double shieldcount;
		double meshatekcount;

		int poweruptype;
		int powerupammo;
		double powerupcount;
		double powerupcharge;

		void init();
		void destroy(void);
		void reset(void);

		void setengine(int mode);

		double steer;
		void adjuststeer(void);
		void setsteer(double s);
		void addsteer(double s);

		void push(void) { if(energy==4) { pushtime=0.5; energy=0; play_sample(&mb_kick_wav, 255, 128, 1000, 0); } };
		void adjustPush(double m);

		void fire(void);

		double getEnergy(void) { return energy; };

		void updateupsidedown(double t);
};

extern Machine machine[2];

#endif

