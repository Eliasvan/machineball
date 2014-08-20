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
#include <alleggl.h>
#include <GL/glu.h>
#define dSINGLE
#include <ode/ode.h>
#include <aldumb.h>

#include "game.h"
#include "timer.h"
#include "ball.h"
#include "glode.h"
#include "text.h"
#include "camera.h"
#include "render.h"
#include "court.h"
#include "machine.h"
#include "controls.h"
#include "powerup.h"
#include "explosion.h"
#include "options.h"

#include "mbdata.h"

extern AL_DUH_PLAYER *dp;
extern DUH *songduh[12];

int currentsong;

int goals[2];
double goalcount;

double camerazoomtime;

dWorldID world;
dJointGroupID contactgroup;

dGeomID explosionsphere;

// Seconds of current frame  (1/framespersecond)
double m;

// Time left in game in seconds if timed game
double gametime;

void collisiondetection(void);

extern int rendertime;

int gameover;
int gamepaused;

void startGame(int mode, int cs, int b, gameoptions op)
{
	// Initialize
	// ...
	// ...


	// Do crazy shit:
	double sum=op.turbo + op.shield + op.mine + op.missle + op.meshatek;
	if(sum>0)
	{
		op.turbo=op.turbo/sum;
		op.shield=op.turbo+op.shield/sum;
		op.mine=op.shield+op.mine/sum;
		op.missle=op.mine+op.missle/sum;
		op.meshatek=op.missle+op.meshatek/sum;
	}
	else
		op.powerupsenabled=0;

	goals[0]=0;
	goals[1]=0;

	if(op.timegoallimit==0)
		rendertime=1;
	else
		rendertime=0;

	world = dWorldCreate();
	dWorldSetGravity(world, 0, 0, -150);
	contactgroup = dJointGroupCreate(0);
	ball.init(b);

	court.init(cs);

	machine[0].init();
	machine[1].init();

	powerups.init((int)court.x-30, (int)court.y-10);
	mines.init();
	explosions.init();
	projectiles.init();

	bodymodel.load((char*)"0");
	pushermodel.load((char*)"1");

	allegro_gl_begin();

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	allegro_gl_set_texture_format(GL_RGB);
	if(b==0)
		balltex = allegro_gl_make_texture(&mb_ball01_bmp);
	else if(b==1)
		balltex = allegro_gl_make_texture(&mb_ball02_bmp);
	else if(b==2)
		balltex = allegro_gl_make_texture(&mb_ball03_bmp);
	else if(b==3)
		balltex = allegro_gl_make_texture(&mb_ball04_bmp);
	else if(b==4)
		balltex = allegro_gl_make_texture(&mb_ball05_bmp);
	else if(b==5)
		balltex = allegro_gl_make_texture(&mb_ball06_bmp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 4.0/3.0, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	allegro_gl_end();

	dGeomID powerupsphere;
	dGeomID minesphere;
	dGeomID projectilesphere;

	powerupsphere = dCreateSphere(0, 5);
	minesphere = dCreateSphere(0, 3);

	explosionsphere = dCreateSphere(0, 1);
	projectilesphere = dCreateSphere(0, 5);

	double powerupready;

	powerupready=op.powerupfrequency;

	double preventstucktime=5;
	
	gametime = op.timegoals*60;

	timer.install();

	goalcount=0;
	camerazoomtime=1;

	currentsong=rand()%12;


	AL_DUH_PLAYER *dp;
	dp = al_start_duh(songduh[currentsong], 2, 0, (float)options.musicvol/255.0, 2048, 22050);
	
	gameover=0;
	gamepaused=0;

	
	int esckeydown=0;
	while(!((gameover && key[KEY_ESC]) || (gamepaused && key[KEY_F11])))
	{
		/* m may never be 0, otherwise an assert in ODE will get
		   triggered */
		do {
			rest(1);
			m=timer.getsecondsandreset();
		} while (m <= 0);

		poll_joystick();
		al_poll_duh(dp);


		m=m/4;
		if(m>1.0/100.0)
			m=1.0/100.0;

		int tt;

		if(gameover==0)
		{
			if(key[KEY_ESC] && gamepaused==0 && esckeydown==0)
			{
				gamepaused=1;
				esckeydown=1;
			}
			if(!key[KEY_ESC])
				esckeydown=0;
			if(gamepaused && esckeydown==0 && key[KEY_ESC])
			{
				gamepaused=0;
				esckeydown=1;
			}
		}

		if(gamepaused)
		{
			int keyrightdown = 0, keyleftdown = 0;
			int needchange = 0;
			if(!(key[KEY_LEFT] || controls[0].keydown(KEYLEFT)))
					keyleftdown=0;
			if(!(key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)))
					keyrightdown=0;
			if((key[KEY_LEFT] || controls[0].keydown(KEYLEFT)) && keyleftdown==0)
			{
				currentsong--;
				keyleftdown=1;
				needchange=1;
			}
			if((key[KEY_RIGHT] || controls[0].keydown(KEYRIGHT)) && keyrightdown==0)
			{
				currentsong++;
				keyrightdown=1;
				needchange=1;
			}
			if(currentsong>11)
				currentsong=0;
			if(currentsong<0)
				currentsong=11;
			if(needchange)
			{
				al_stop_duh(dp);
				dp = al_start_duh(songduh[currentsong], 2, 0, (float)options.musicvol/255.0, 2048, 22050);
			}
		}
		else
		for(tt=0; tt<4; tt++)
		{
			//
			// Work on this:
			const dReal * ballpos = dBodyGetPosition(ball.body);
			if(ballpos[1]>court.y-2*ball.getRadius())
				dBodyAddForce(ball.body, 0, -2*ball.getMass(), 0);
			if(ballpos[1]<-court.y+2*ball.getRadius())
				dBodyAddForce(ball.body, 0, 2*ball.getMass(), 0);

			int i, j;
			for(i=0; i<2; i++)
			{
				if(controls[i].keydown(KEYSPECIAL))
					machine[i].push();
				machine[i].adjustPush(m);
				machine[i].setengine(MACHINE_ENGINE_COAST);
				if(machine[i].meshatekcount>0)
					machine[i].setengine(MACHINE_ENGINE_BRAKE);
				if(machine[i].meshatekcount==0)
				{
					if(controls[i].keydown(KEYUP))
					{
						const dReal * vel = dBodyGetLinearVel(machine[i].body[0]);
						dVector3 relvel;
						dBodyVectorFromWorld(machine[i].body[0], vel[0], vel[1], vel[2], relvel);
						if(relvel[1]>=0)
							machine[i].setengine(MACHINE_ENGINE_FORWARD);
						else
							machine[i].setengine(MACHINE_ENGINE_BRAKE);
					}
					if(controls[i].keydown(KEYDOWN))
					{
						const dReal * vel = dBodyGetLinearVel(machine[i].body[0]);
						dVector3 relvel;
						dBodyVectorFromWorld(machine[i].body[0], vel[0], vel[1], vel[2], relvel);
						if(relvel[1]<=0)
							machine[i].setengine(MACHINE_ENGINE_REVERSE);
						else
							machine[i].setengine(MACHINE_ENGINE_BRAKE);
					}

					if(!controls[i].keydown(KEYLEFT) && !controls[i].keydown(KEYRIGHT))
						machine[i].setsteer(0);
					if(controls[i].keydown(KEYRIGHT))
					{
						if(machine[i].steer<0)
							machine[i].setsteer(0);
						machine[i].addsteer(3*m);
					}
					if(controls[i].keydown(KEYLEFT))
					{
						if(machine[i].steer>0)
							machine[i].setsteer(0);
						machine[i].addsteer(-3*m);
					}
					if(controls[i].keydown(KEYFIRE))
						machine[i].fire();
				}

				if(machine[i].powerupcharge>0)
				{
					machine[i].powerupcharge-=m;
					if(machine[i].powerupcharge<0)
						machine[i].powerupcharge=0;
				}
				if(machine[i].shieldcount>0)
				{
					machine[i].shieldcount-=m;
					if(machine[i].shieldcount<0)
						machine[i].shieldcount=0;
				}
				if(machine[i].meshatekcount>0)
				{
					machine[i].meshatekcount-=m;
					if(machine[i].meshatekcount<0)
						machine[i].meshatekcount=0;
				}

				if(machine[i].powerupcount>0)
				{
					machine[i].powerupcount-=m;
					if(machine[i].powerupcount<0)
						machine[i].powerupcount=0;
					if(machine[i].poweruptype==0)
						dBodyAddRelForce(machine[i].body[0], 0, 6000, 0);
				}
				machine[i].updateupsidedown(m);
			}

			if(goalcount>0)
			{
				goalcount-=m;
				if(goalcount<0)
				{
					goalcount=0;
					camerazoomtime=1;
					ball.reset();
					machine[0].reset();
					machine[1].reset();
					while(powerups.numpowerups>0)
						powerups.removePowerup(0);
					while(mines.nummines>0)
						mines.removeMine(0);
					while(projectiles.numprojectiles>0)
						projectiles.removeProjectile(0);
					explosions.clearall();
				}
			}
			else
				gametime-=m;

			if(op.timegoallimit==0)
			{
				if(gametime<=0)
					gameover=1;
			}
			else
				if(goals[0]>=op.timegoals || goals[1]>=op.timegoals)
					gameover=1;

			explosions.Update(m);

			if(camerazoomtime>0)
			{
				camerazoomtime-=m;
				if(camerazoomtime<0)
					camerazoomtime=0;
			}

			for(i=0; i<powerups.numpowerups; i++)
			{
				dContactGeom dummycontactgeom;
				for(j=0; j<2; j++)
				{
					dGeomSetPosition(powerupsphere, powerups.getPowerup(i).x, powerups.getPowerup(i).y, 5);
					if(
							dCollide(machine[j].geom[0], powerupsphere, 1, &dummycontactgeom,
								sizeof(dContactGeom))>0 ||
							dCollide(machine[j].geom[1], powerupsphere, 1, &dummycontactgeom,
								sizeof(dContactGeom))>0 ||
							dCollide(machine[j].geom[2], powerupsphere, 1, &dummycontactgeom,
								sizeof(dContactGeom))>0 ||
							dCollide(machine[j].geom[3], powerupsphere, 1, &dummycontactgeom,
								sizeof(dContactGeom))>0)
					{
						if(powerups.getPowerup(i).type==1)
						{
							machine[j].shieldcount+=20;
							play_sample(&mb_shield_wav, 255, 128, 1000, 0);
						}
						else
						{
							if(machine[j].poweruptype!=powerups.getPowerup(i).type)
							{	
								machine[j].poweruptype=powerups.getPowerup(i).type;
								machine[j].powerupammo=0;
								machine[j].powerupcharge=0;
								machine[j].powerupcount=0;
							}
							machine[j].powerupammo++;
							if(machine[j].poweruptype==0)
								play_sample(&mb_turbo2_wav, 255, 128, 1000, 0);
							else if(machine[j].poweruptype==2)
							{
								play_sample(&mb_mines_wav, 255, 128, 1000, 0);
								machine[j].powerupammo++;
							}
							else if(machine[j].poweruptype==3)
								play_sample(&mb_missle_wav, 255, 128, 1000, 0);
							else if(machine[j].poweruptype==4)
								play_sample(&mb_emp_wav, 255, 128, 1000, 0);
						}
						powerups.removePowerup(i);
					}
				}
			}

			for(i=0; i<mines.nummines; i++)
			{
				dContactGeom dummycontactgeom;
				dGeomSetPosition(minesphere, mines.getMine(i).x, mines.getMine(i).y, 3);
				if(
						dCollide(machine[0].geom[0], minesphere, 1, &dummycontactgeom,
							sizeof(dContactGeom))>0 ||
						dCollide(machine[0].geom[1], minesphere, 1, &dummycontactgeom,
							sizeof(dContactGeom))>0 ||
						dCollide(machine[0].geom[2], minesphere, 1, &dummycontactgeom,
							sizeof(dContactGeom))>0 ||
						dCollide(machine[0].geom[3], minesphere, 1, &dummycontactgeom,
							sizeof(dContactGeom))>0)
				{
					if(mines.getMine(i).danger==1 || mines.getMine(i).owner==1)
					{
						// Mine Blows up
						explosions.addExplosion(mines.getMine(i).x, mines.getMine(i).y, 0, 8, 45, 40);
						mines.removeMine(i);
					}

				}
				else if(

						dCollide(machine[1].geom[0], minesphere, 1, &dummycontactgeom,
							sizeof(dContactGeom))>0 ||
						dCollide(machine[1].geom[1], minesphere, 1, &dummycontactgeom,
							sizeof(dContactGeom))>0 ||
						dCollide(machine[1].geom[2], minesphere, 1, &dummycontactgeom,
							sizeof(dContactGeom))>0 ||
						dCollide(machine[1].geom[3], minesphere, 1, &dummycontactgeom,
							sizeof(dContactGeom))>0)
				{
					if(mines.getMine(i).danger==1 || mines.getMine(i).owner==0)
					{
						// Mine Blows up
						explosions.addExplosion(mines.getMine(i).x, mines.getMine(i).y, 0, 8, 45, 40);
						mines.removeMine(i);
					}
				}
				else
				{
					mines.getMine(i).danger=1;
				}
				if(dCollide(ball.geom, minesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0)
				{
					// Mine Blows Up
					explosions.addExplosion(mines.getMine(i).x, mines.getMine(i).y, 0, 5, 45, 40);
					mines.removeMine(i);
				}
			}


			if(op.powerupsenabled)
			{
				powerupready-=m;
				powerups.spin+=40*m;
				if(powerups.spin>360)
					powerups.spin-=360;
				if(powerupready<0)
				{
					if(powerups.numpowerups<5)
					{
						double ppp=(rand()%10000)/10000.0;
						if(ppp<=op.turbo)
							powerups.addPowerup(0);
						else if(ppp<=op.shield)
							powerups.addPowerup(1);
						else if(ppp<=op.mine)
							powerups.addPowerup(2);
						else if(ppp<=op.missle)
							powerups.addPowerup(3);
						else if(ppp<=op.meshatek)
							powerups.addPowerup(4);
					}
					powerupready=op.powerupfrequency;
				}
			}

			for(i=0; i<projectiles.numprojectiles; i++)
			{
				dContactGeom dummycontactgeom;
				dGeomSetPosition(projectilesphere, projectiles.getProjectile(i).x, projectiles.getProjectile(i).y, 6);
				if(projectiles.getProjectile(i).owner==1 && (
						dCollide(machine[0].geom[0], projectilesphere, 1, &dummycontactgeom,
							sizeof(dContactGeom))>0 ||
						dCollide(machine[0].geom[1], projectilesphere, 1, &dummycontactgeom,
							sizeof(dContactGeom))>0 ||
						dCollide(machine[0].geom[2], projectilesphere, 1, &dummycontactgeom,
							sizeof(dContactGeom))>0 ||
						dCollide(machine[0].geom[3], projectilesphere, 1, &dummycontactgeom,
							sizeof(dContactGeom))>0))
				{
					if(projectiles.getProjectile(i).type==0)
					{
						explosions.addExplosion(projectiles.getProjectile(i).x, projectiles.getProjectile(i).y, 5, 20, 60, 50);
						projectiles.removeProjectile(i);
					}
					else
					{
						if(machine[0].shieldcount==0)
						{
							machine[0].meshatekcount+=9;
							play_sample(&mb_shituk_wav, 255, 128, 1000, 0);
						}
						projectiles.removeProjectile(i);
					}
				}
				else if(projectiles.getProjectile(i).owner==0 && (
						dCollide(machine[1].geom[0], projectilesphere, 1, &dummycontactgeom,
							sizeof(dContactGeom))>0 ||
						dCollide(machine[1].geom[1], projectilesphere, 1, &dummycontactgeom,
							sizeof(dContactGeom))>0 ||
						dCollide(machine[1].geom[2], projectilesphere, 1, &dummycontactgeom,
							sizeof(dContactGeom))>0 ||
						dCollide(machine[1].geom[3], projectilesphere, 1, &dummycontactgeom,
							sizeof(dContactGeom))>0))
				{
					if(projectiles.getProjectile(i).type==0)
					{
						explosions.addExplosion(projectiles.getProjectile(i).x, projectiles.getProjectile(i).y, 5, 20, 60, 50);
						projectiles.removeProjectile(i);
					}
					else
					{
						if(machine[1].shieldcount==0)
						{
							machine[1].meshatekcount+=9;
							play_sample(&mb_shituk_wav, 255, 128, 1000, 0);
						}
						projectiles.removeProjectile(i);
					}
				}
				else if( 
						dCollide(court.corner[0], projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0 ||
						dCollide(court.corner[1], projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0 ||
						dCollide(court.corner[2], projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0 ||
						dCollide(court.corner[3], projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0 ||
						dCollide(court.wall[0], projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0 ||
						dCollide(court.wall[1], projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0 ||
						dCollide(court.wall[2], projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0 ||
						dCollide(court.wall[3], projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0 ||
						dCollide(court.wall[4], projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0 ||
						dCollide(court.wall[5], projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0 ||
						dCollide(court.wall[6], projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0 ||
						dCollide(court.wall[7], projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0 ||
						dCollide(court.wall[8], projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0 ||
						dCollide(court.wall[9], projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0 ||
						dCollide(court.wall[10], projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0 ||
						dCollide(court.wall[11], projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0 ||
						dCollide(court.wall[12], projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0 ||
						dCollide(court.wall[13], projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0 ||
						dCollide(court.wall[14], projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0 ||
						dCollide(court.wall[15], projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0 ||
						dCollide(ball.geom, projectilesphere, 1, &dummycontactgeom, sizeof(dContactGeom))>0)
				{

					if(projectiles.getProjectile(i).type==0)
						explosions.addExplosion(projectiles.getProjectile(i).x, projectiles.getProjectile(i).y, 5, 20, 60, 50);
					projectiles.removeProjectile(i);
				}

			}
			
			projectiles.update(m);


			const dReal * pos = dBodyGetPosition(ball.body);
			const dReal * ballVel = dBodyGetLinearVel(ball.body);
			const dReal * m1 = dBodyGetPosition(machine[0].body[0]);
			const dReal * m2 = dBodyGetPosition(machine[1].body[0]);
			if(op.preventstuck)
			{
				if(ballVel[0]*ballVel[0] + ballVel[1]*ballVel[1] + ballVel[2]*ballVel[2] < 16 &&
						(m1[0]-pos[0])*(m1[0]-pos[0]) + (m1[1]-pos[1])*(m1[1]-pos[1]) + (m1[2]-pos[2])*(m1[2]-pos[2]) < 1200 &&
						(m2[0]-pos[0])*(m2[0]-pos[0]) + (m2[1]-pos[1])*(m2[1]-pos[1]) + (m2[2]-pos[2])*(m2[2]-pos[2]) < 1200)
				{
					preventstucktime-=m;
					if(preventstucktime<=0)
					{
						preventstucktime=5;
						explosions.addExplosion(pos[0]+((rand()%500)-1000)/500.0, pos[1]+((rand()%500)-1000)/500.0, 0, 5, 70, 40);
					}
				}
				else
					preventstucktime=5;
			}

			if(gameover==0)
			{
				if(pos[0]+ball.getRadius() < -court.x && goalcount==0)
				{
					goals[1]++;
					goalcount=4;
					play_sample(&mb_goal_wav, 255, 128, 1000, 0);
				}
				else if(pos[0]-ball.getRadius() > court.x && goalcount==0)
				{
					goals[0]++;
					goalcount=4;
					play_sample(&mb_goal_wav, 255, 128, 1000, 0);
				}
			}

			collisiondetection();
			dWorldStep(world, m);
			dJointGroupEmpty(contactgroup);
		}

		render();
	}
	al_stop_duh(dp);

	// Destroy:
	// ...
	// ...

	bodymodel.kill();
	pushermodel.kill();

	dGeomDestroy(powerupsphere);
	dGeomDestroy(minesphere);
	dGeomDestroy(explosionsphere);

	glDeleteTextures(1, &balltex);
	
	machine[0].destroy();
	machine[1].destroy();

	dJointGroupDestroy(contactgroup);
	court.destroy();
	ball.destroy();
	dWorldDestroy(world);
}

void collisiondetection(void)
{
	int i, j, k;

	const int MAX_CONTACTS=100;
	dContact contact[MAX_CONTACTS];
	int cn; // Contact Number

	cn=0;

	const dReal * pos = dBodyGetPosition(ball.body);
	if(pos[2]-ball.getRadius() <= 0)
	{
		contact[cn].surface.mode = dContactBounce;
		contact[cn].surface.bounce = ball.getBounce();
		contact[cn].surface.bounce_vel = 10;
		contact[cn].surface.mu = ball.getMu();
		contact[cn].geom.pos[0]=pos[0];
		contact[cn].geom.pos[1]=pos[1];
		contact[cn].geom.pos[2]=0;
		contact[cn].geom.normal[0]=0;
		contact[cn].geom.normal[1]=0;
		contact[cn].geom.normal[2]=1;
		contact[cn].geom.depth=-pos[2]+ball.getRadius();
		contact[cn].geom.g1=0;
		contact[cn].geom.g2=0;
		dJointID c = dJointCreateContact(world, contactgroup, &contact[cn]);
		dJointAttach(c, ball.body, 0);
		const dReal * ballvel = dBodyGetLinearVel(ball.body);
		dBodyAddForce(ball.body, -ball.getFriction()*ballvel[0], -ball.getFriction()*ballvel[1], -ball.getFriction()*ballvel[2]);
		const dReal * ballavel = dBodyGetAngularVel(ball.body);
		dBodyAddTorque(ball.body, -ball.getFriction()*ballavel[0], -ball.getFriction()*ballavel[1], -ball.getFriction()*ballavel[2]);
		cn++;
	}
	if(pos[1]-ball.getRadius() <= -court.y || pos[1]+ball.getRadius() >= court.y)
	{
		contact[cn].surface.mode = dContactBounce;
		contact[cn].surface.bounce = ball.getBounce();
		contact[cn].surface.bounce_vel = 0;
		contact[cn].surface.mu = ball.getMu()*5.0;
		contact[cn].geom.pos[0]=pos[0];
		contact[cn].geom.pos[1]=pos[1]<0?-court.y:court.y;
		contact[cn].geom.pos[2]=pos[2];
		contact[cn].geom.normal[0]=0;
		contact[cn].geom.normal[1]=pos[1]<0?1:-1;
		contact[cn].geom.normal[2]=0;
		contact[cn].geom.depth=pos[1]<0?(-pos[1]+ball.getRadius()-court.y):(pos[1]+ball.getRadius()-court.y);
		contact[cn].geom.g1=0;
		contact[cn].geom.g2=0;
		dJointID c = dJointCreateContact(world, contactgroup, &contact[cn]);
		dJointAttach(c, ball.body, 0);
		cn++;
	}
	if(pos[0]-ball.getRadius() <= -court.x-30 || pos[0]+ball.getRadius() >= court.x+30)
	{
		contact[cn].surface.mode = dContactBounce;
		contact[cn].surface.bounce = 0;
		contact[cn].surface.bounce_vel = 0;
		contact[cn].surface.mu = ball.getMu()*5.0;
		contact[cn].geom.pos[0]=pos[0]<0?-court.x-30:court.x+30;
		contact[cn].geom.pos[1]=pos[1];
		contact[cn].geom.pos[2]=pos[2];
		contact[cn].geom.normal[0]=pos[0]<0?1:-1;
		contact[cn].geom.normal[1]=0;
		contact[cn].geom.normal[2]=0;
		contact[cn].geom.depth=pos[0]<0?(-pos[0]+ball.getRadius()-court.x-30):(pos[0]+ball.getRadius()-court.x-30);
		contact[cn].geom.g1=0;
		contact[cn].geom.g2=0;
		dJointID c = dJointCreateContact(world, contactgroup, &contact[cn]);
		dJointAttach(c, ball.body, 0);
		cn++;
	}

	for(i=0; i<4; i++)
	{
		if(dCollide(ball.geom, court.corner[i], 1, &contact[cn].geom, sizeof(dContactGeom)) > 0)
		{
			contact[cn].surface.mode = dContactBounce;
			contact[cn].surface.bounce = ball.getBounce();
			contact[cn].surface.bounce_vel = 0;
			contact[cn].surface.mu = ball.getMu()*5.0;
			dJointID c = dJointCreateContact(world, contactgroup, &contact[cn]);
			dJointAttach(c, ball.body, 0);
			cn++;
		}
	}

	if(-court.x-3<=pos[0]-ball.getRadius() && court.x+3>=pos[0]+ball.getRadius())
	{
		for(i=0; i<12; i++)
		{
			if(dCollide(ball.geom, court.wall[i+2], 1, &contact[cn].geom, sizeof(dContactGeom)) > 0)
			{
				contact[cn].surface.mode = dContactBounce;
				contact[cn].surface.bounce = ball.getBounce();
				contact[cn].surface.bounce_vel = 0;
				contact[cn].surface.mu = ball.getMu()*5.0;
				dJointID c = dJointCreateContact(world, contactgroup, &contact[cn]);
				dJointAttach(c, ball.body, 0);
				cn++;
			}
		}
	}

	for(j=0; j<explosions.numexplosions; j++)
	{
		dGeomSetPosition(explosionsphere, explosions.getExplosion(j).x, explosions.getExplosion(j).y, explosions.getExplosion(j).z);
		dGeomSphereSetRadius(explosionsphere, explosions.getExplosion(j).curradius);
		int n;
		if((n=dCollide(ball.geom, explosionsphere, 4, &contact[cn].geom, sizeof(dContact))) > 0)
		{
			contact[cn].surface.mode = dContactSoftERP | dContactSoftCFM;
			contact[cn].surface.mu = 5000;
			contact[cn].surface.soft_erp = 0.16;
			contact[cn].surface.soft_cfm = 0.02;
			dJointID c = dJointCreateContact(world, contactgroup, &contact[cn]);
			dJointAttach(c, ball.body, 0);
			cn++;
		}
	}

	for(i=0; i<2; i++)
	{

		for(j=0; j<16; j++)
		{
			for(k=0; k<4; k++)
			{
				int n;
				if((n=dCollide(machine[i].geom[k], court.wall[j], 4, &contact[cn].geom, sizeof(dContact))) > 0)
				{
					if(contact[cn].geom.pos[1]<-37 || contact[cn].geom.pos[1]>37 || j>13)
					{
						contact[cn].surface.mode = dContactBounce;
						contact[cn].surface.bounce = 0.2;
						contact[cn].surface.bounce_vel = 5;
						contact[cn].surface.mu = 0;
						dJointID c = dJointCreateContact(world, contactgroup, &contact[cn]);
						dJointAttach(c, machine[i].body[!!k], 0);
						cn++;
					}
				}
			}
		}
		for(j=0; j<4; j++)
		{
			for(k=0; k<4; k++)
			{
				int n;
				if((n=dCollide(machine[i].geom[k], court.corner[j], 4, &contact[cn].geom, sizeof(dContact))) > 0)
				{
					contact[cn].surface.mode = dContactBounce;
					contact[cn].surface.bounce = 0.2;
					contact[cn].surface.bounce_vel = 5;
					contact[cn].surface.mu = 0;
					dJointID c = dJointCreateContact(world, contactgroup, &contact[cn]);
					dJointAttach(c, machine[i].body[!!k], 0);
					cn++;
				}
			}
		}

		for(j=0; j<3; j++)
		{
			if(dCollide(machine[i].sphere[j], court.floor, 1, &contact[cn].geom, sizeof(dContactGeom)) > 0)
			{
				contact[cn].surface.mode = dContactBounce;
				contact[cn].surface.bounce = 0.3;
				contact[cn].surface.bounce_vel = 5;
				contact[cn].surface.mu = 4000;
				dJointID c = dJointCreateContact(world, contactgroup, &contact[cn]);
				dJointAttach(c, machine[i].wheel[j], 0);
				cn++;
			}
		}
		for(j=0; j<3; j++)
		{
			if(dCollide(machine[i].sphere[j], ball.geom, 1, &contact[cn].geom, sizeof(dContactGeom)) > 0)
			{
				contact[cn].surface.mode = dContactBounce;
				contact[cn].surface.bounce = 0.2;
				contact[cn].surface.bounce_vel = 0;
				contact[cn].surface.mu = 0;
				dJointID c = dJointCreateContact(world, contactgroup, &contact[cn]);
				dJointAttach(c, machine[i].wheel[j], ball.body);
				cn++;
			}
		}
		for(k=0; k<4; k++)
		{
			int n;
			if((n=dCollide(machine[i].geom[k], court.floor, 4, &contact[cn].geom, sizeof(dContact))) > 0)
			{
				for(j=0; j<n; j++)
				{
					contact[cn].surface.mode = dContactBounce;
					contact[cn].surface.bounce = 0.2;
					contact[cn].surface.bounce_vel = 5;
					contact[cn].surface.mu = 300;
					dJointID c = dJointCreateContact(world, contactgroup, &contact[cn]);
					dJointAttach(c, machine[i].body[!!k], 0);
					cn++;
				}
			}
		}
		for(k=0; k<4; k++)
		{
			if(dCollide(ball.geom, machine[i].geom[k], 1, &contact[cn].geom, sizeof(dContactGeom)) > 0)
			{
				contact[cn].surface.mode = dContactBounce;
				contact[cn].surface.bounce = 0;
				contact[cn].surface.bounce_vel = 0;
				contact[cn].surface.mu = 0;
				dJointID c = dJointCreateContact(world, contactgroup, &contact[cn]);
				dJointAttach(c, ball.body, machine[i].body[!!k]);
				cn++;
			}
		}


		if(machine[i].shieldcount==0)
		{
			for(j=0; j<explosions.numexplosions; j++)
			{
				dGeomSetPosition(explosionsphere, explosions.getExplosion(j).x, explosions.getExplosion(j).y, explosions.getExplosion(j).z);
				dGeomSphereSetRadius(explosionsphere, explosions.getExplosion(j).curradius);
				for(k=0; k<4; k++)
				{
					int n;
					if((n=dCollide(machine[i].geom[k], explosionsphere, 4, &contact[cn].geom, sizeof(dContact))) > 0)
					{
						contact[cn].surface.mode = dContactSoftERP | dContactSoftCFM;
						contact[cn].surface.mu = 0;
						contact[cn].surface.soft_erp = 0.16;
						contact[cn].surface.soft_cfm = 0.02;
						dJointID c = dJointCreateContact(world, contactgroup, &contact[cn]);
						dJointAttach(c, machine[i].body[!!k], 0);
						cn++;
					}
				}
			}
		}

	}
	int n;
	for(i=0; i<4; i++)
	{
		for(k=0; k<4; k++)
		{
			if((n=dCollide(machine[0].geom[i], machine[1].geom[k], 4, &contact[cn].geom, sizeof(dContact))) > 0)
			{
				for(j=0; j<n; j++)
				{
					contact[cn].surface.mode = dContactBounce;
					contact[cn].surface.bounce = 0.5;
					contact[cn].surface.bounce_vel = 5;
					contact[cn].surface.mu = 0;
					dJointID c = dJointCreateContact(world, contactgroup, &contact[cn]);
					dJointAttach(c, machine[0].body[!!i], machine[1].body[!!k]);
					cn++;
				}
			}
		}
	}
}

