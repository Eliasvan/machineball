// Machine Ball
//  By Benny Kramek
//  benny@kramekweb.com
//  http://benny.kramekweb.com
//
//
// Source Code is released under the GPL
//

#ifndef _GLODE_H_
#define _GLODE_H_

#define dSINGLE
#include <ode/ode.h>

void position_gl(dBodyID b);
void position_gl(dGeomID g);
void draw_box(double length, double width, double height);
void draw_sphere(double r, int n=8);

#endif
