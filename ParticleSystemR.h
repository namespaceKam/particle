/******************************************************************************/
/*!
\file     ParticleSystemR.h
\par      Project: Rev
\author   Oland Pelton
\brief
  All content © 2013 DigiPen (USA) Corporation, all rights reserved.

*/
/******************************************************************************/
// Only include this file into the .c file and into any reserved header
// file that inherits this object

// The 'R' at the end of the file name represents that this header is a
// 'reserved header'
#ifndef PARTICLESYSTEMR_H_
#define PARTICLESYSTEMR_H_

// Include the reserved header of the inherited object
#include "GameObjectR.h"
// Include any other headers here if needed

// ParticleSystem : GameObject
//typedef struct Particle
//{
//	Point2D pos;	//particle position
//	Vector2D vel;	//particle velocity
//	Vector2D acc;	//particle acceleration (if any)
//	float dur;		//particle lifespan
//	Mesh *color;	//particle color
//}Particle;

typedef struct ParticleSystem
{
  const GameObject _;           // The inherited object MUST be first and have the name '_'
  
  //Particle *particle;				  //particles stored in an array
  Point2D *pos;                 //array of particle positions
  Vector2D *vel;                //array of particle velocities
  Vector2D *acc;                //array of particle accelerations
  float *dur;                   //array of particle durations
  Mesh **col;                   //array of particle colors

  Point2D systemOrigin;			    //origin of particles
  float paramX;                 //varies depending on emitter mode
  float paramY;                 //varies depending on emitter mode

  enum EMITTERTYPE type;        //changes how the emitter behaves
	enum PARTICLECOLOR color;		  //initial color of a spawning particle
	unsigned activeParticles;		  //number of particles that are currently active
	unsigned maxParticles;			  //maximum number of particles that can exist simultaneously
	unsigned simultaneous;			  //number of particles spawned per single emission
	unsigned ammo;		    	      //how many particles left before system becomes inactive

	float spawnDelay;				      //delay between particles being emitted. 0 means all active particles will be emitted simultaneously
	float timer;					        //counter for delay
	float particleLifespanMin;		//min lifespan of a particle
	float particleLifespanMax;		//max lifespan of a particle

	float velocityMin;				    //min speed of particle
	float velocityMax;				    //max speed of particles
	float velocityAngleMin;			  //min angle of particle velocity (degrees)
	float velocityAngleMax;			  //max angle of particle velocity (degrees)

	float accelMin;					      //min acceleration
	float accelMax;					      //max acceleration
	float accelAngleMin;			    //min angle of particle acceleration (degrees)
	float accelAngleMax;			    //max angle of particle acceleration (degrees)

  BOOL finite;				          //if true, system will expend ammo and then become inactive
  BOOL activated;					      //bool to show if system is active
}ParticleSystem;


// ParticleSystemClass : GameObjectClass
typedef struct ParticleSystemClass
{
  const GameObjectClass _; // The inherited class MUST be first and have the name '_'

  // Methods



  // Get Methods



  // Set Methods


}ParticleSystemClass;

#endif