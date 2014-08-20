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

#include "controls.h"
#include "options.h"

Controls controls[2];

int Controls::keydown(int k)
{
	if(this==&controls[0])
	{
		switch (k) {
		case KEYUP:
			if(options.p1up<128)
				return key[options.p1up];
			else
				return special_keydown(options.p1up);
		case KEYDOWN:
			if(options.p1down<128)
				return key[options.p1down];
			else
				return special_keydown(options.p1down);
		case KEYLEFT:
			if(options.p1left<128)
				return key[options.p1left];
			else
				return special_keydown(options.p1left);
		case KEYRIGHT:
			if(options.p1right<128)
				return key[options.p1right];
			else
				return special_keydown(options.p1right);
		case KEYSPECIAL:
			if(options.p1special<128)
				return key[options.p1special];
			else
				return special_keydown(options.p1special);
		case KEYFIRE:
			if(options.p1fire<128)
				return key[options.p1fire];
			else
				return special_keydown(options.p1fire);
		default:
			return 0;
		}
	}
	else
	{
		switch (k) {
		case KEYUP:
			if(options.p2up<128)
				return key[options.p2up];
			else
				return special_keydown(options.p2up);
		case KEYDOWN:
			if(options.p2down<128)
				return key[options.p2down];
			else
				return special_keydown(options.p2down);
		case KEYLEFT:
			if(options.p2left<128)
				return key[options.p2left];
			else
				return special_keydown(options.p2left);
		case KEYRIGHT:
			if(options.p2right<128)
				return key[options.p2right];
			else
				return special_keydown(options.p2right);
		case KEYSPECIAL:
			if(options.p2special<128)
				return key[options.p2special];
			else
				return special_keydown(options.p2special);
		case KEYFIRE:
			if(options.p2fire<128)
				return key[options.p2fire];
			else
				return special_keydown(options.p2fire);
		default:
			return 0;
		}
	}
}

int special_keydown(int k)
{
	int devicenum, k2;
	devicenum=(k-128)/28;
	k2 = (k-128) % 28;
	
	if(devicenum>num_joysticks-1 || k2 < 0)
		return 0;
	
	if (k2 >= 16)
		return joy[devicenum].button[k2-16].b;
	
	const JOYSTICK_AXIS_INFO axis = joy[devicenum].stick[k2/4].axis[((k2+2)%4)/2];
	return (k2 % 2) ? axis.d2 : axis.d1;

}
