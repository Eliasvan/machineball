// Machine Ball
//  By Benny Kramek
//  benny@kramekweb.com
//  http://benny.kramekweb.com
//
//
// Source Code is released under the GPL
//

#ifndef _COURT_H_
#define _COURT_H_

#define dSINGLE
#include <ode/ode.h>

//         ____0_____
//      2 /
//       /
//      |
//      |
//     -+
//     |
//     |
//     -+
//      |
//      |
//       \
//        \____1_____

class Court
{
	private:
	public:
		double x;
		double y;
		dGeomID corner[4];
		dGeomID wall[16];
                dGeomID floor;
		void init(int size);
		void destroy(void);
};

extern Court court;

#endif

