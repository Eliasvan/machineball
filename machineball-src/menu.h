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
int ballMenu(void);
int courtMenu(void);
int gameoptionsMenu(gameoptions *op);

int optionsMenu(void);
void audioMenu(void);
void videoMenu(void);
void controlsMenu(void);

void humancompmessage(void);
void creditsMenu(void);

#endif
