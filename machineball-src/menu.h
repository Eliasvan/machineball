// Machine Ball
//  By Benny Kramek
//  benny@kramekweb.com
//  http://benny.kramekweb.com
//
//
// Source Code is released under the GPL
//

#ifndef _MENU_H_
#define _MENU_H_

#include "game.h"

int mainMenu(void);
int ballMenu(int ballchoice);
int courtMenu(int cs1);
int gameoptionsMenu(gameoptions *op,int o1,int o2,int o3,int o4,int o5,int o6,int o7,int o8,int o9,int o10);

int optionsMenu(void);
void audioMenu(void);
void videoMenu(void);
void controlsMenu(void);

void humancompmessage(void);
void creditsMenu(void);

#endif
