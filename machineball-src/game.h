// Machine Ball
//  By Benny Kramek
//  benny@kramekweb.com
//  http://benny.kramekweb.com
//
//
// Source Code is released under the GPL
//

#ifndef _GAME_H_
#define _GAME_H_

struct gameoptions
{
	int timegoallimit; // 0=timelimit 1=goallimit
	int timegoals; // Number of goals/time in game
	int timegoals2;
	
	int powerupsenabled; // 0=nopowerups 1=powerups
	int powerupfrequency; // number of seconds till powerup
	int powerupfrequency2;
	
	int preventstuck; // Nuke the ball if it gets stuck between the machines

	double turbo, shield, mine, missle, meshatek;
};

void startGame(int, int, int b, gameoptions op);

#endif
