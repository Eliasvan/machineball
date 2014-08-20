// Machine Ball
//  By Benny Kramek
//  benny@kramekweb.com
//  http://benny.kramekweb.com
//
//
// Source Code is released under the GPL
//

#ifndef _CONTROLS_H_
#define _CONTROLS_H_

#define KEYUP      0
#define KEYDOWN    1
#define KEYLEFT    2
#define KEYRIGHT   3
#define KEYSPECIAL 4
#define KEYFIRE    5

class Controls
{
	public:
		int keydown(int k);
};

extern Controls controls[2];

int special_keydown(int k);

#endif

