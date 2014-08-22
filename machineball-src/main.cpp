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
#define dSINGLE
#include <ode/ode.h>

#include <aldumb.h>

#include "window.h"
#include "intro.h"
#include "menu.h"
#include "game.h"
#include "text.h"
#include "render.h"
#include "options.h"

#include "mbdata.h"

AL_DUH_PLAYER *dp;
DUH *menuduh;
DUH *songduh[12];

void loadProgramData(void);
void freeProgramData(void);

int main(int argc, char **argv)
{
	// Parse command line args:
	int i;
	int forc=1;
	int do_configure=0;

	char rcfile[1024];
#ifndef ALLEGRO_WINDOWS
	sprintf(rcfile, "%s/.machineballrc", getenv("HOME"));
#endif
#ifdef ALLEGRO_WINDOWS
	strcpy(rcfile, "./machineball.cfg");
#endif
	
	for(i=0; i<argc; i++)
	{
		if(strcmp(argv[i], "--help")==0 || strcmp(argv[i], "-h")==0)
		{
			printf("Machine Ball   Version 1.0\n");
			printf("  By Benny Kramek\n");
			printf("  benny@kramekweb.com\n");
			printf("  http://benny.kramekweb.com\n\n");
			printf("Usage:\n");
			printf("  machineball [OPTIONS]\n\n");
			printf("  -v, -V, --version      show version information\n");
			printf("  -h, --help             show usage information (this info)\n");
			printf("  -ns, --nosound         disable sound\n");
			printf("  -f, --rcfile [arg]     configuration file\n");
			exit(0);
		}
		if(strcmp(argv[i], "--version")==0 || strcmp(argv[i], "-v")==0 || strcmp(argv[i], "-V")==0)
		{
			printf("Machine Ball   Version 1.0\n");
			printf("  By Benny Kramek\n");
			printf("  benny@kramekweb.com\n");
			printf("  http://benny.kramekweb.com\n");
			exit(0);
		}
		if(strcmp(argv[i], "-ns")==0 || strcmp(argv[i], "--nosound")==0)
		{
			forc=0;
		}
		if(strcmp(argv[i], "-f")==0 || strcmp(argv[i], "--rcfile")==0)
		{
			if(i == argc-1)
			{
				printf("Machine Ball   Version 1.0\n");
				printf("  By Benny Kramek\n");
				printf("  benny@kramekweb.com\n");
				printf("  http://benny.kramekweb.com\n\n");
				printf("Usage:\n");
				printf("  machineball [OPTIONS]\n\n");
				printf("  -v, -V, --version      show version information\n");
				printf("  -h, --help             show usage information (this info)\n");
				printf("  -ns, --nosound         disable sound\n");
				printf("  -f, --rcfile [arg]     configuration file\n");
				exit(-1);
			}
			else
			{
				strcpy(rcfile, argv[i+1]);
			}
		}
	}

	printf("Machine Ball   Version 1.0\n");   
	printf("  By Benny Kramek\n");
	printf("  benny@kramekweb.com\n");
	printf("  http://benny.kramekweb.com\n\n");
	if(forc==0)
		printf("=== Sound is disabled ===\n");
	printf("Configuration file: %s\n", rcfile);

	FILE *fp = fopen(rcfile, "r");
	if(fp==NULL)
	{
		printf("=== Cannot Open %s for reading. Using default Configuration ===\n", rcfile);
		do_configure=1;
		// Default values:
		options.resolution=6;
		options.colordepth=1;
		options.zdepth=1;
		options.musicvol=196;
		options.soundvol=255;
		options.p1left=KEY_LEFT;
		options.p1right=KEY_RIGHT;
		options.p1up=KEY_UP;
		options.p1down=KEY_DOWN;
		options.p1special=KEY_RSHIFT;
		options.p1fire=KEY_RCONTROL;
		options.p2left=KEY_A;
		options.p2right=KEY_D;
		options.p2up=KEY_W;
		options.p2down=KEY_S;
		options.p2special=KEY_LSHIFT;
		options.p2fire=KEY_LCONTROL;
		options.logoshow=1;
		options.fpsshow=0;
		options.camerashake=1;
	}
	else
	{
		// Load configuration from file:
		char tempdummy[256];
		int versiontest;
		if (
				fgets(tempdummy, 256, fp) == NULL || 
				fscanf(fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", 
					&versiontest,
					&options.resolution, 
					&options.colordepth,
					&options.zdepth,
					&options.musicvol,
					&options.soundvol,
					&options.p1left,
					&options.p1right,
					&options.p1up,
					&options.p1down,
					&options.p1special,
					&options.p1fire,
					&options.p2left,
					&options.p2right,
					&options.p2up,
					&options.p2down,
					&options.p2special,
					&options.p2fire,
					&options.logoshow,
					&options.fpsshow,
					&options.camerashake
				) != 21 ||
				versiontest != 1000 // 1000 is magic number for version 1.0
		) {
			fclose(fp);
			allegro_message("=== Configuration file is corrupt or from an incompatible version of Machine Ball ===\n");
			exit(-1);
		}
		fclose(fp);
	}
	fp = fopen(rcfile, "w");
	if(fp==NULL)
		printf("=== Cannot Open %s for writing. Configuration will not be saved ===\n", rcfile);
	else
	{
		fprintf(fp, "# Machine Ball Configuration file. http://benny.kramekweb.com DO NOT HAND EDIT!\n");
		fprintf(fp, "%d ", 1000);
		fprintf(fp, "%d ", options.resolution);
		fprintf(fp, "%d ", options.colordepth);
		fprintf(fp, "%d ", options.zdepth);
		fprintf(fp, "%d ", options.musicvol);
		fprintf(fp, "%d ", options.soundvol);
		fprintf(fp, "%d ", options.p1left);
		fprintf(fp, "%d ", options.p1right);
		fprintf(fp, "%d ", options.p1up);
		fprintf(fp, "%d ", options.p1down);
		fprintf(fp, "%d ", options.p1special);
		fprintf(fp, "%d ", options.p1fire);
		fprintf(fp, "%d ", options.p2left);
		fprintf(fp, "%d ", options.p2right);
		fprintf(fp, "%d ", options.p2up);
		fprintf(fp, "%d ", options.p2down);
		fprintf(fp, "%d ", options.p2special);
		fprintf(fp, "%d ", options.p2fire);
		fprintf(fp, "%d ", options.logoshow);
		fprintf(fp, "%d ", options.fpsshow);
		fprintf(fp, "%d\n", options.camerashake);
		fprintf(fp, "# Machine Ball Configuration file. http://benny.kramekweb.com DO NOT HAND EDIT!\n");
		fclose(fp);
	}

	srand(time(0));
	dInitODE();
	createWindow(forc, do_configure); // The 1st argument tells us if we want sound or not
	
	loadProgramData();
	dumb_resampling_quality=2;
	
	showIntro();

	int menuChoice;
	int courtSize = 1;
	int ballChoice = 0;
	int responce = 0;
	int escpress = 0;

	gameoptions o;
	gameoptions p;
	
	int timegoallimit=0;
	int timegoals=2;
	int powerupsenabled=1;
	int powerupfrequency=1;
	int preventstuck=1;

	int turbo=200;
	int shield=200;
	int mine=200;
	int missle=200;
	int meshatek=200;
	
	do
	{
	    int setup=0;
	    while(setup == 0){
		setup = 1;
		menuChoice = mainMenu();
		if(menuChoice==0)
		{
			while(setup == 1)
			{
			  int cs = courtSize;
			  courtSize = courtMenu(courtSize);
			  setup++;
			  if(courtSize==4){setup = 0; courtSize = cs;}
			  while(setup == 2)
			  {
			    
			    if(setup == 2 && escpress == 0)
			    {
			      int bc = ballChoice;
			      ballChoice = ballMenu(ballChoice);
			      setup++;
			      if(ballChoice==6){setup = 1;ballChoice = bc;}
			    }
			    if(setup == 3 && escpress == 0)
			    {
			      p=o;
			      
			      responce = gameoptionsMenu(&o,timegoallimit,timegoals,powerupsenabled,powerupfrequency,preventstuck,turbo,shield,mine,missle,meshatek);
			      setup++;
			      timegoallimit = o.timegoallimit; timegoals = o.timegoals; powerupsenabled = o.powerupsenabled; powerupfrequency = o.powerupfrequency; 
			      preventstuck = o.preventstuck; turbo = o.turbo; shield = o.shield; mine = o.mine; missle = o.missle; meshatek = o.meshatek;
			      if(responce == 1){o = p;setup = 2;escpress = 1;}
			      
			      
			    }
			    if(!key[KEY_ESC]){escpress = 0;}
			  }
			  
			}
			if(setup == 4){
			  startGame(0, courtSize, ballChoice, o);}
		}
		//else if(menuChoice==1)
		//{
		//	courtSize = courtMenu();
		//	ballChoice = ballMenu();
		//	gameoptionsMenu(&o);
		//	startGame(1, courtSize, ballChoice, o);
		//}
		else if(menuChoice==1)
			humancompmessage();
		else if(menuChoice==2)
		{
			int optionsmenuchoice;
			do
			{
			optionsmenuchoice = optionsMenu();
			if(optionsmenuchoice==0)
				controlsMenu();
			else if(optionsmenuchoice==1)
				audioMenu();
			else if(optionsmenuchoice==2)
				videoMenu();
			} while(optionsmenuchoice!=3);
		}
	    }
	} while(menuChoice!=3);
	creditsMenu();

	fp = fopen(rcfile, "w");
	if(fp==NULL)
		printf("=== Cannot Open %s for writing. Configuration will not be saved ===\n", rcfile);
	else
	{
		fprintf(fp, "# Machine Ball Configuration file. http://benny.kramekweb.com DO NOT HAND EDIT!\n");
		fprintf(fp, "%d ", 1000);
		fprintf(fp, "%d ", options.resolution);
		fprintf(fp, "%d ", options.colordepth);
		fprintf(fp, "%d ", options.zdepth);
		fprintf(fp, "%d ", options.musicvol);
		fprintf(fp, "%d ", options.soundvol);
		fprintf(fp, "%d ", options.p1left);
		fprintf(fp, "%d ", options.p1right);
		fprintf(fp, "%d ", options.p1up);
		fprintf(fp, "%d ", options.p1down);
		fprintf(fp, "%d ", options.p1special);
		fprintf(fp, "%d ", options.p1fire);
		fprintf(fp, "%d ", options.p2left);
		fprintf(fp, "%d ", options.p2right);
		fprintf(fp, "%d ", options.p2up);
		fprintf(fp, "%d ", options.p2down);
		fprintf(fp, "%d ", options.p2special);
		fprintf(fp, "%d ", options.p2fire);
		fprintf(fp, "%d ", options.logoshow);
		fprintf(fp, "%d ", options.fpsshow);
		fprintf(fp, "%d\n", options.camerashake);
		fprintf(fp, "# Machine Ball Configuration file. http://benny.kramekweb.com DO NOT HAND EDIT!\n");
		fclose(fp);
	}

	printf("\nThanks for Playing!\nVisit http://benny.kramekweb.com\n");

	freeProgramData();

	return(0);
}
END_OF_MAIN();

void loadProgramData(void)
{
	text.init();


	shadowtex = allegro_gl_make_texture_ex(AGL_TEXTURE_RESCALE|AGL_TEXTURE_FLIP, &mb_shadow_tga, GL_ALPHA4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	poweruptex = allegro_gl_make_texture_ex(AGL_TEXTURE_RESCALE|AGL_TEXTURE_FLIP, &mb_powerup_tga, GL_ALPHA4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	allegro_gl_set_texture_format(GL_RGB);
	floortex = allegro_gl_make_texture(&mb_floor01_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	allegro_gl_set_texture_format(GL_RGB);
	walltex = allegro_gl_make_texture(&mb_wall01_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	allegro_gl_set_texture_format(GL_RGB);
	machinetex = allegro_gl_make_texture(&mb_machinetex_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	allegro_gl_set_texture_format(GL_RGB);
	reflecttex = allegro_gl_make_texture(&mb_reflect_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	allegro_gl_set_texture_format(GL_RGBA);
	BITMAP * bmp = create_bitmap_ex(32, 256, 128);
	blit(&mb_goal_bmp, bmp, 0, 0, 0, 0, 256, 128);
	goaltex = allegro_gl_make_masked_texture(bmp);
	destroy_bitmap(bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	bennykramekwebtex = allegro_gl_make_texture_ex(AGL_TEXTURE_RESCALE|AGL_TEXTURE_FLIP, &mb_bennykramekweb_tga, GL_ALPHA4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	allegro_gl_set_texture_format(GL_RGBA);
	bmp = create_bitmap_ex(32, 128, 128);
	blit(&mb_weapons_bmp, bmp, 0, 0, 0, 0, 128, 128);
	weaponstex = allegro_gl_make_masked_texture(bmp);
	destroy_bitmap(bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	allegro_gl_set_texture_format(GL_RGBA);
	explosiontex = allegro_gl_make_texture(&mb_explosion_tga);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// Music:
	DUMBFILE *f = dumbfile_open_memory((char*)mb_menu_xm, 51355);
	menuduh = dumb_read_xm(f);
	dumbfile_close(f);

	f = dumbfile_open_memory((char*)mb_song01_s3m, 39778);
	songduh[0] = dumb_read_s3m(f);
	dumbfile_close(f);
	f = dumbfile_open_memory((char*)mb_song02_mod, 40450);
	songduh[1] = dumb_read_mod(f);
	dumbfile_close(f);
	f = dumbfile_open_memory((char*)mb_song03_mod, 19772);
	songduh[2] = dumb_read_mod(f);
	dumbfile_close(f);
	f = dumbfile_open_memory((char*)mb_song04_mod, 86546);
	songduh[3] = dumb_read_mod(f);
	dumbfile_close(f);
	f = dumbfile_open_memory((char*)mb_song05_xm, 53969);
	songduh[4] = dumb_read_xm(f);
	dumbfile_close(f);
	f = dumbfile_open_memory((char*)mb_song06_xm, 42300);
	songduh[5] = dumb_read_xm(f);
	dumbfile_close(f);
	f = dumbfile_open_memory((char*)mb_song07_mod, 99704);
	songduh[6] = dumb_read_mod(f);
	dumbfile_close(f);
	f = dumbfile_open_memory((char*)mb_song08_xm, 59748);
	songduh[7] = dumb_read_xm(f);
	dumbfile_close(f);
	f = dumbfile_open_memory((char*)mb_song09_xm, 37452);
	songduh[8] = dumb_read_xm(f);
	dumbfile_close(f);
	f = dumbfile_open_memory((char*)mb_song10_mod, 60500);
	songduh[9] = dumb_read_mod(f);
	dumbfile_close(f);
	f = dumbfile_open_memory((char*)mb_song11_mod, 24636);
	songduh[10] = dumb_read_mod(f);
	dumbfile_close(f);
	f = dumbfile_open_memory((char*)mb_song12_mod, 29262);
	songduh[11] = dumb_read_mod(f);
	dumbfile_close(f);

}


void freeProgramData(void)
{
	text.destroy();
	glDeleteTextures(1, &shadowtex);
	glDeleteTextures(1, &poweruptex);
	glDeleteTextures(1, &floortex);
	glDeleteTextures(1, &walltex);
	glDeleteTextures(1, &goaltex);
	glDeleteTextures(1, &machinetex);
	glDeleteTextures(1, &reflecttex);
	glDeleteTextures(1, &bennykramekwebtex);
	glDeleteTextures(1, &explosiontex);
	
	unload_duh(menuduh);
	int i;
	for(i=0; i<12; i++)
		unload_duh(songduh[i]);
	
	dCloseODE();
}

