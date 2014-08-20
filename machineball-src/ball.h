// Machine Ball
//  By Benny Kramek
//  benny@kramekweb.com
//  http://benny.kramekweb.com
//
//
// Source Code is released under the GPL
//

#ifndef _BALL_H_
#define _BALL_H_

#include <ode/ode.h>

class Ball
{
	private:
		int type;

		double radius;
		double mass;
		double mu;
		double bounce;
		double friction;
	public:
		dBodyID body;
		dGeomID geom;

		void init(int type);
		void destroy(void);
		void reset(void);
		double getRadius(void) { return radius; };
		double getBounce(void) { return bounce; };
		double getMu(void) { return mu; };
		double getFriction(void) { return friction; };
		double getMass(void) { return mass; };
};

extern Ball ball;

#endif

