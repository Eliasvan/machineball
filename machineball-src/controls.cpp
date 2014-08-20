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
		if(k==KEYUP) 
			if(options.p1up<128)
				return key[options.p1up];
			else
				return special_keydown(options.p1up);
		if(k==KEYDOWN)
			if(options.p1down<128)
				return key[options.p1down];
			else
				return special_keydown(options.p1down);
		if(k==KEYLEFT)
			if(options.p1left<128)
				return key[options.p1left];
			else
				return special_keydown(options.p1left);
		if(k==KEYRIGHT)
			if(options.p1right<128)
				return key[options.p1right];
			else
				return special_keydown(options.p1right);
		if(k==KEYSPECIAL)
			if(options.p1special<128)
				return key[options.p1special];
			else
				return special_keydown(options.p1special);
		if(k==KEYFIRE)
			if(options.p1fire<128)
				return key[options.p1fire];
			else
				return special_keydown(options.p1fire);
		return 0;
	}
	else
	{
		if(k==KEYUP) 
			if(options.p2up<128)
				return key[options.p2up];
			else
				return special_keydown(options.p2up);
		if(k==KEYDOWN)
			if(options.p2down<128)
				return key[options.p2down];
			else
				return special_keydown(options.p2down);
		if(k==KEYLEFT)
			if(options.p2left<128)
				return key[options.p2left];
			else
				return special_keydown(options.p2left);
		if(k==KEYRIGHT)
			if(options.p2right<128)
				return key[options.p2right];
			else
				return special_keydown(options.p2right);
		if(k==KEYSPECIAL)
			if(options.p2special<128)
				return key[options.p2special];
			else
				return special_keydown(options.p2special);
		if(k==KEYFIRE)
			if(options.p2fire<128)
				return key[options.p2fire];
			else
				return special_keydown(options.p2fire);
		return 0;
	}
}

int special_keydown(int k)
{
	int devicenum;
	devicenum=(k-128)/28;
	
	if(devicenum>num_joysticks-1)
		return 0;
	
	if((k-128)%28==0)
		return joy[devicenum].stick[0].axis[1].d1;
	if((k-128)%28==1)
		return joy[devicenum].stick[0].axis[1].d2;
	if((k-128)%28==2)
		return joy[devicenum].stick[0].axis[0].d1;
	if((k-128)%28==3)
		return joy[devicenum].stick[0].axis[0].d2;
	if((k-128)%28==4)
		return joy[devicenum].stick[1].axis[1].d1;
	if((k-128)%28==5)
		return joy[devicenum].stick[1].axis[1].d2;
	if((k-128)%28==6)
		return joy[devicenum].stick[1].axis[0].d1;
	if((k-128)%28==7)
		return joy[devicenum].stick[1].axis[0].d2;
	if((k-128)%28==8)
		return joy[devicenum].stick[2].axis[1].d1;
	if((k-128)%28==9)
		return joy[devicenum].stick[2].axis[1].d2;
	if((k-128)%28==10)
		return joy[devicenum].stick[2].axis[0].d1;
	if((k-128)%28==11)
		return joy[devicenum].stick[2].axis[0].d2;
	if((k-128)%28==12)
		return joy[devicenum].stick[3].axis[1].d1;
	if((k-128)%28==13)
		return joy[devicenum].stick[3].axis[1].d2;
	if((k-128)%28==14)
		return joy[devicenum].stick[3].axis[0].d1;
	if((k-128)%28==15)
		return joy[devicenum].stick[3].axis[0].d2;

	return joy[devicenum].button[(k-128)%28-16].b;

}
