// Machine Ball
//  By Benny Kramek
//  benny@kramekweb.com
//  http://benny.kramekweb.com
//
//
// Source Code is released under the GPL
//

#ifndef __OPTIONS_H_
#define __OPTIONS_H_

struct options_struct
{
	int resolution;
	int colordepth;
	int zdepth;
	int musicvol;
	int soundvol;
	int p1left;
	int p1right;
	int p1up;
	int p1down;
	int p1special;
	int p1fire;
	int p2left;
	int p2right;
	int p2up;
	int p2down;
	int p2special;
	int p2fire;
	int logoshow;
	int fpsshow;
	int camerashake;
};

extern options_struct options;

#endif

