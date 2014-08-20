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

#include "machine.h"

#include "court.h"
#include "powerup.h"

#include "mbdata.h"

extern dWorldID world;

Machine machine[2];

void Machine::init()
{
	int i;
	pushtime=0;
	energy=4;
	dMass m;
	for(i=0; i<3; i++)
	{
		wheel[i] = dBodyCreate(world);
		dMassSetSphere(&m, 1, 5);
		dMassAdjust(&m, 2);
		dBodySetMass(wheel[i], &m);
		sphere[i] = dCreateSphere(0, 5);
		dGeomSetBody(sphere[i], wheel[i]);
	}
	dBodySetPosition(wheel[0], 0, 12, 6);
	dBodySetPosition(wheel[1], -6, -7, 6);
	dBodySetPosition(wheel[2], 6, -7, 6);

	body[0] = dBodyCreate(world);
	dMassSetBox(&m, 1, 20, 80, 5);
	dMassAdjust(&m, 5);
	dBodySetMass(body[0], &m);
	dBodySetPosition(body[0], 0, 0, 6.5);
	geom[0] = dCreateBox(0, 19, 27, 10);
	dGeomSetBody(geom[0], body[0]);

	body[1] = dBodyCreate(world);
	dMassSetBox(&m, 1, 11, 5, 10);
	dMassAdjust(&m, 0.3);
	dBodySetMass(body[1], &m);
	dBodySetPosition(body[1], 0, 17, 6.5);
	geom[1] = dCreateBox(0, 11, 5, 10);
	dGeomSetBody(geom[1], body[1]);

	joint = dJointCreateSlider(world, 0);
	dJointAttach(joint, body[0], body[1]);
	dJointSetSliderAxis(joint, 0, 1, 0);
	dJointSetSliderParam(joint, dParamLoStop, -9);
	dJointSetSliderParam(joint, dParamHiStop, 0);

	for(i=0; i<2; i++)
	{
		geom[i+2] = dCreateGeomTransform(0);
		dGeomTransformSetCleanup(geom[i+2], 1);
		
		finE[i] = dCreateBox(0, 7, 5, 10);
		dGeomSetPosition(finE[i], i==0?-6.3:6.3, -2, 0);
		dMatrix3 R;
		dRFromAxisAndAngle(R, 0, 0, 1, i==0?M_PI/4:-M_PI/4);
		dGeomSetRotation(finE[i], R);
		dGeomTransformSetGeom(geom[i+2], finE[i]);
		dGeomSetBody(geom[i+2], body[1]);
	}
	
	
	for(i=0; i<3; i++)
	{
		wheeljoint[i] = dJointCreateHinge2(world, 0);
		dJointAttach(wheeljoint[i], body[0], wheel[i]);
		const dReal *wPos = dBodyGetPosition(wheel[i]);
		dJointSetHinge2Anchor(wheeljoint[i], wPos[0], wPos[1], wPos[2]);
		dJointSetHinge2Axis1(wheeljoint[i], 0, 0, 1);
		dJointSetHinge2Axis2(wheeljoint[i], 1, 0, 0);

		dJointSetHinge2Param(wheeljoint[i], dParamSuspensionERP, 0.8);
		dJointSetHinge2Param(wheeljoint[i], dParamSuspensionCFM, 0.01);
		dJointSetHinge2Param(wheeljoint[i], dParamLoStop, 0);
		dJointSetHinge2Param(wheeljoint[i], dParamHiStop, 0);
		dJointSetHinge2Param(wheeljoint[i], dParamCFM, 0.0001);
		dJointSetHinge2Param(wheeljoint[i], dParamStopERP, 0.8);
		dJointSetHinge2Param(wheeljoint[i], dParamStopCFM, 0.0001);
	}

	reset();
}

void Machine::destroy(void)
{
	int i;
	dBodyDestroy(body[0]);
	dBodyDestroy(body[1]);
	dGeomDestroy(geom[0]);
	dJointDestroy(joint);
	for(i=0; i<2; i++)
		dGeomDestroy(geom[i+2]);
	for(i=0; i<3; i++)
	{
		dBodyDestroy(wheel[i]);
		dGeomDestroy(sphere[i]);
	}
}

void Machine::reset(void)
{
	dMatrix3 R;
	if(this==&machine[0])
	{

		dRFromAxisAndAngle(R, 0, 0, 1, -M_PI/2.0);
		dBodySetPosition(body[0], -court.x/2, 0, 26.5);
		dBodySetRotation(body[0], R);
		dBodySetPosition(body[1], -court.x/2+17, 0, 26.5);
		dBodySetRotation(body[1], R);
		int i;
		for(i=0; i<3; i++)
			dBodySetRotation(wheel[i], R);
		dBodySetPosition(wheel[0], 12-court.x/2, 0, 26);
		dBodySetPosition(wheel[1], -7-court.x/2, 6, 26);
		dBodySetPosition(wheel[2], -7-court.x/2, -6, 26);
	}
	else
	{
		dRFromAxisAndAngle(R, 0, 0, 1, M_PI/2.0);
		dBodySetPosition(body[0], court.x/2, 0, 26.5);
		dBodySetRotation(body[0], R);
		dBodySetPosition(body[1], court.x/2-17, 0, 26.5);
		dBodySetRotation(body[1], R);
		int i;
		for(i=0; i<3; i++)
			dBodySetRotation(wheel[i], R);
		dBodySetPosition(wheel[0], -12+court.x/2, 0, 26);
		dBodySetPosition(wheel[1], 7+court.x/2, -6, 26);
		dBodySetPosition(wheel[2], 7+court.x/2, 6, 26);
	}
	steer=0;
	pushtime=0;
	energy=4;
	adjuststeer();
	dBodySetLinearVel(body[0], 0, 0, 0);
	dBodySetLinearVel(body[1], 0, 0, 0);
	dBodySetAngularVel(body[0], 0, 0, 0);
	dBodySetAngularVel(body[1], 0, 0, 0);
	int i;
	for(i=0; i<3; i++)
	{
		dBodySetLinearVel(wheel[i], 0, 0, 0);
		dBodySetAngularVel(wheel[i], 0, 0, 0);
	}
	upsidedowntime=0;

	shieldcount=0;
	meshatekcount=0;

	powerupammo=0;
	powerupcount=0;
	powerupcharge=0;
}

void Machine::setengine(int mode)
{
	int i;
	if(mode==MACHINE_ENGINE_COAST)
	{
		for(i=0; i<3; i++)
			dJointSetHinge2Param(wheeljoint[i], dParamFMax2, 0);
	}
	if(mode==MACHINE_ENGINE_BRAKE)
	{
		for(i=0; i<3; i++)
		{
			dJointSetHinge2Param(wheeljoint[i], dParamVel2, 0);
			dJointSetHinge2Param(wheeljoint[i], dParamFMax2, 10000);
		}
	}
	if(mode==MACHINE_ENGINE_FORWARD)
	{
		for(i=0; i<3; i++)
		{
			dJointSetHinge2Param(wheeljoint[i], dParamVel2, 30);
			dJointSetHinge2Param(wheeljoint[i], dParamFMax2, 1500);
		}
	}
	if(mode==MACHINE_ENGINE_REVERSE)
	{
		for(i=0; i<3; i++)
		{
			dJointSetHinge2Param(wheeljoint[i], dParamVel2, -20);
			dJointSetHinge2Param(wheeljoint[i], dParamFMax2, 1500);
		}
	}
}

void Machine::setsteer(double s)
{
	steer=s;
	if(steer>0.6)
		steer=0.6;
	if(steer<-0.6)
		steer=-0.6;
	adjuststeer();
}

void Machine::addsteer(double s)
{
	steer+=s;
	if(steer>0.6)
		steer=0.6;
	if(steer<-0.6)
		steer=-0.6;
	adjuststeer();
}

void Machine::adjuststeer(void)
{
	dReal v = steer - dJointGetHinge2Angle1 (wheeljoint[0]);
	if (v > 0.1) v = 0.1;
	if (v < -0.1) v = -0.1;
	v *= 50.0;
	dJointSetHinge2Param (wheeljoint[0],dParamVel,v);
	dJointSetHinge2Param (wheeljoint[0],dParamFMax,8000);
	dJointSetHinge2Param (wheeljoint[0],dParamLoStop,-0.6);
	dJointSetHinge2Param (wheeljoint[0],dParamHiStop,0.666666);
	dJointSetHinge2Param (wheeljoint[0],dParamFudgeFactor,0.1);
}

void Machine::adjustPush(double m)
{
	if(pushtime>0)
	{
		dJointSetSliderParam(joint, dParamVel, -100);
		dJointSetSliderParam(joint, dParamFMax, 5000);
	}
	else
	{
		dJointSetSliderParam(joint, dParamVel, 100);
		dJointSetSliderParam(joint, dParamFMax, 1000);
	}
	pushtime-=m;
	energy+=m;
	if(energy>4)
		energy=4;
	if(pushtime<0)
		pushtime=0;
}

void Machine::updateupsidedown(double t)
{
	const dReal *rotation = dBodyGetRotation(body[0]);
	if(rotation[10]>0.3)
		upsidedowntime=0;
	else
		upsidedowntime+=t;
	if(upsidedowntime>5)
		reset();
}

void Machine::fire(void)
{
	if(powerupammo>0 && powerupcharge==0)
	{
		if(poweruptype==0)
		{
			powerupammo--;
			powerupcharge=3;
			powerupcount=1;
			play_sample(&mb_turbo_wav, 255, 128, 1000, 0);
		}
		if(poweruptype==2)
		{
			dVector3 result;
			dBodyGetRelPointPos(body[0], 0, -9, -5, result);
			if(result[2]<5)
			{	
				powerupammo--;
				powerupcharge=0.25;

				if(this==&machine[0])
					mines.addMine(result[0], result[1], 0);
				else
					mines.addMine(result[0], result[1], 1);
				play_sample(&mb_minelay_wav, 255, 128, 1000, 0);
			}
		}
		if(poweruptype==3 || poweruptype==4)
		{
			const dReal * pos0 = dBodyGetPosition(body[0]);
			const dReal * pos1 = dBodyGetPosition(body[1]);
			double length=sqrt(
					(pos0[0]-pos1[0])*(pos0[0]-pos1[0]) +
					(pos0[1]-pos1[1])*(pos0[1]-pos1[1]) +
					(pos0[2]-pos1[2])*(pos0[2]-pos1[2]))+3;
			dVector3 result;
			dBodyGetRelPointPos(body[0], 0, length+5, 0, result);
			if(result[2]<10)
			{
				powerupammo--;
				powerupcharge=0.5;
				const dReal *p = dBodyGetRotation(body[0]);
				double x=p[1], y=p[5];
				length = sqrt(x*x+y*y);
				x/=length;
				y/=length;
				double angle=-acos(x)*(180/M_PI);
				if(y<0)
					angle=-angle;
				angle+=90;
				if(poweruptype==3)
				{
					if(this==&machine[0])
						projectiles.addProjectile(result[0], result[1], x*130, y*100, angle, 0, 0);
					else
						projectiles.addProjectile(result[0], result[1], x*130, y*100, angle, 0, 1);
					play_sample(&mb_mlaunch1_wav, 255, 128, 1000, 0);
				}
				else
				{
					if(this==&machine[0])
						projectiles.addProjectile(result[0], result[1], x*160, y*160, angle, 1, 0);
					else
						projectiles.addProjectile(result[0], result[1], x*160, y*160, angle, 1, 1);
					play_sample(&mb_mlaunch2_wav, 255, 128, 1000, 0);
				}
			}
		}
	}
}

