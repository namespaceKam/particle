/******************************************************************************/
/*!
\file     ParticleSystem.h
\par      Project: Rev
\author   Oland Pelton
\brief
  All content © 2013 DigiPen (USA) Corporation, all rights reserved.

*/
/******************************************************************************/

// This header can be included in any file that needs it
#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

// Include the header of the inherited object
#include "GameObject.h"
// Also include any other needed headers here
#include "Global.h"

enum PARTICLECOLOR
{
	P_RED,
	P_GREEN,
	P_BLUE,
	P_WHITE,
	P_PURPLE,
	P_VIOLET,
  P_YELLOW,
	P_NUM_COLORS,
};

enum EMITTERTYPE
{
  EMITTER_POINT,
  EMITTER_BOX,
  EMITTER_CIRCLE,
  EMITTER_LINE
};

//Functions

//call first, this sets up the array of particle colors. Will update later to support textures
void InitializeParticleColors(void);

//call when level is uninitialized
void DeleteParticleColors(void);

/*Sets the properties of a ParticleSystem.  If you mean to mount the emitter to an object,
it doesn't actually matter what position you give it at first. Emitter is inactive upon creation*/
void *MakeEmitter(float xPos, float yPos, float zOrder,
                  enum PARTICLECOLOR color, BOOL finite, unsigned ammo, unsigned maxParticles,
					        unsigned simultaneous, float spawnDelay, float lifespanMin, float lifespanMax,
					        float velocityMin, float velocityMax, float velAngleMin, float velAngleMax,
					        float accelMin, float accelMax, float accelAngleMin, float accelAngleMax, BOOL replay);

void DeleteReplayParticles(void *particleReplay);

//sets the active status of the emitter to TRUE or FALSE
void TriggerEmitter(void *emitter, BOOL flag);

//toggles emitter on or off
void ToggleEmitter(void *emitter);

/*  the emitter will stop emitting new particles, finish updating particles that
 *  are still alive, and then delete the emitter once remaining particles
 *  are dead */
void FinishEmitter(void *emitter);

/* changes the emit mode of the given emitter
 * for box emit, paramX and paramY are the X and Y dimensions of the box, respectively
 * for line emit, paramX is angle, paramY is magnitude
 * for circle emit, paramX is max emit radius, paramY is the min */
void EmitterMode(void *emitter, enum EMITTERTYPE type, float paramX, float paramY);

//changes the color of the emitter
void SetEmitColor(void *emitter, enum PARTICLECOLOR color);

//changes the angle particles will emit from
void SetEmitAngle(void *emitter, float minAngle, float maxAngle);

// Initializtion of description function

void InitParticleSystem();
void FreeParticleSystem();

extern const void *oParticleSystem;
extern const void *oParticleSystemClass;

#endif