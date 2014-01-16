 /******************************************************************************/
/*!
\file     ParticleSystem.c
\par      Project: Rev
\author   Oland Pelton
\brief
  All content © 2013 DigiPen (USA) Corporation, all rights reserved.

*/
/******************************************************************************/

#include "Global.h"
#include "ParticleSystem.h"
#include "ParticleSystemR.h"

// ParticleSystem Description
const void *oParticleSystem;
// ParticleSystemClass Description
const void *oParticleSystemClass;

static void AddParticle(ParticleSystem *em);
static void SwapParticleArray(ParticleSystem *em, unsigned indexL, unsigned indexR);

Mesh* ParticleColor[P_NUM_COLORS];

//////////////////////////////////////////////////////////
// Public Functions
//////////////////////////////////////////////////////////
void InitializeParticleColors(void)
{
	int i;
	float size = 0.5f;
	for(i = 0; i < P_NUM_COLORS; ++i)
	{
		switch(i)
		{
			case P_RED:
				ParticleColor[i] = CreateColoredTriangleMesh(size, size, 0xFFFF0000);
				break;
			case P_GREEN:
				ParticleColor[i] = CreateColoredTriangleMesh(size, size, 0xFF00FF00);
				break;
			case P_BLUE:
				ParticleColor[i] = CreateColoredTriangleMesh(size, size, 0xFF0000FF);
				break;
			case P_WHITE:
				ParticleColor[i] = CreateColoredTriangleMesh(size, size, 0xFFFFFFFF);
				break;
			case P_PURPLE:
				ParticleColor[i] = CreateColoredTriangleMesh(size, size, 0xFFFF4BDF);
        break;
      case P_VIOLET:
        ParticleColor[i] = CreateColoredTriangleMesh(size, size, 0xFF7F00FF);
        break;
      case P_YELLOW:
        ParticleColor[i] = CreateColoredTriangleMesh(size, size, 0xFFFFFF00);
        break;
			default:
				break;
		}
	}
}

void DeleteParticleColors(void)
{
	int i;
	for(i = 0; i < P_NUM_COLORS; ++i)
		DestroyMesh(ParticleColor[i]);
}
/////////////////////////////////////////////////////////////////////////////////////

void TriggerEmitter(void *em, BOOL flag)
{
	((ParticleSystem*)em)->activated = flag;
}

void FinishEmitter(void *emitter)
{
  ((ParticleSystem*)emitter)->activated = TRUE;
  ((ParticleSystem*)emitter)->finite = TRUE;
  ((ParticleSystem*)emitter)->ammo = 0;
}

void ToggleEmitter(void *emitter)
{
  ((ParticleSystem*)emitter)->activated = !((ParticleSystem*)emitter)->activated;
}

void SetEmitAngle(void *emitter, float minAngle, float maxAngle)
{
  ((ParticleSystem*)emitter)->velocityAngleMin = minAngle;
  ((ParticleSystem*)emitter)->velocityAngleMax = maxAngle;
}

void SetEmitColor(void *emitter, enum PARTICLECOLOR color)
{
  ((ParticleSystem*)emitter)->color = color;
}

////////////////////////////
//Functions
////////////////////////////
void *MakeEmitter(float xPos, float yPos, float zOrder,
                  enum PARTICLECOLOR color, BOOL finite, unsigned ammo, unsigned maxParticles,
					        unsigned simultaneous, float spawnDelay, float lifespanMin, float lifespanMax,
					        float velocityMin, float velocityMax, float velAngleMin, float velAngleMax,
					        float accelMin, float accelMax, float accelAngleMin, float accelAngleMax, BOOL replay)
{
  //(oSprite, float xPos, float yPos, float xMov, float yMov, float scale, float ZOrder)
  ParticleSystem *pSystem = (ParticleSystem *)New(oParticleSystem, xPos, yPos, 0.0f, 0.0f, 1.0f, zOrder,
                                finite, ammo, color, maxParticles, simultaneous, spawnDelay, lifespanMin, lifespanMax,
                                velocityMin, velocityMax, velAngleMin, velAngleMax, accelMin, accelMax,
                                accelAngleMin, accelAngleMax, replay);
  return pSystem;
}

void DeleteReplayParticles(void *particleReplay)
{
  ParticleSystem *pSys = (ParticleSystem *)particleReplay;

  free(pSys->pos);
  free(pSys->dur);
}

////////////////////////////
//Get Functions
////////////////////////////

////////////////////////////
//Set Functions
////////////////////////////

//////////////////////////////////////////////////////////
// Private Functions (pointed to by the objects)
// These should all be static.
//////////////////////////////////////////////////////////
void SwapParticleArray(ParticleSystem *em, unsigned indexL, unsigned indexR)
{
	Point2D tempPos;
  Vector2D tempVel;
  Vector2D tempAcc;
  float tempDur;
  Mesh *tempCol;

  tempPos = em->pos[indexL];
  tempVel = em->vel[indexL];
  tempAcc = em->acc[indexL];
  tempDur = em->dur[indexL];
  tempCol = em->col[indexL];

	em->pos[indexL] = em->pos[indexR];
  em->vel[indexL] = em->vel[indexR];
  em->acc[indexL] = em->acc[indexR];
  em->dur[indexL] = em->dur[indexR];
  em->col[indexL] = em->col[indexR];

  em->pos[indexR] = tempPos;
  em->vel[indexR] = tempVel;
  em->acc[indexR] = tempAcc;
  em->dur[indexR] = tempDur;
  em->col[indexR] = tempCol;
}

void AddParticle(ParticleSystem *em)
{
  Point2D pos = GetPosition(em);

	if(em->activeParticles >= em->maxParticles)	//if max number of particles has been reached, exit function
		return;
	else
	{
		float min, max, fTemp;
		Vector2D vTemp;
    Point2D pTemp;
		int index = em->activeParticles;

		//position
    switch(em->type)
    {
    case EMITTER_BOX:
      min = pos.x - em->paramX;		max = pos.x + em->paramX;
		  pTemp.x = min + (float)rand()/((float)RAND_MAX/(max-min));

      min = pos.y - em->paramY;		max = pos.y + em->paramY;
		  pTemp.y = min + (float)rand()/((float)RAND_MAX/(max-min));
      break;

    case EMITTER_CIRCLE:
      min = 0.0f;		max = 360.0f;
		  fTemp = min + (float)rand()/((float)RAND_MAX/(max-min));
      vTemp = AngleToVector(fTemp);

      min = em->paramY;		max = em->paramX;
		  fTemp = min + (float)rand()/((float)RAND_MAX/(max-min));
      ScaleVector(&vTemp, fTemp);

      pTemp.x = vTemp.x + pos.x;
      pTemp.y = vTemp.y + pos.y;
      break;

    case EMITTER_LINE:
      vTemp = AngleToVector(em->paramX);

      min = 0.0f;		max = em->paramY;
		  fTemp = min + (float)rand()/((float)RAND_MAX/(max-min));
      ScaleVector(&vTemp, fTemp);

      pTemp.x = vTemp.x + pos.x;
      pTemp.y = vTemp.y + pos.y;
      break;

    default:
      pTemp.x = pos.x;
      pTemp.y = pos.y;
      break;
    }
		em->pos[index] = pTemp;

		//color index
    em->col[index] = ParticleColor[em->color];

		//velocity
		min = em->velocityAngleMin;		max = em->velocityAngleMax;
		fTemp = min + (float)rand()/((float)RAND_MAX/(max-min));
		vTemp = AngleToVector(fTemp);

		min = em->velocityMin;			max = em->velocityMax;
		fTemp = min + (float)rand()/((float)RAND_MAX/(max-min));
		ScaleVector(&vTemp, fTemp);

		em->vel[index] = vTemp;

		//acceleration
		min = em->accelAngleMin;		max = em->accelAngleMax;
		fTemp = min + (float)rand()/((float)RAND_MAX/(max-min));
		vTemp = AngleToVector(fTemp);

		min = em->accelMin;			max = em->accelMax;
		fTemp = min + (float)rand()/((float)RAND_MAX/(max-min));
		ScaleVector(&vTemp, fTemp);

		em->acc[index] = vTemp;

		//duration
		min = em->particleLifespanMin;	max = em->particleLifespanMax;
		em->dur[index] = min + (float)rand()/((float)RAND_MAX/(max-min));

		//increment counter
		++em->activeParticles;
    if(em->finite)
      --em->ammo;
	}
}

////////////////////////////
//Object Methods
////////////////////////////

static void ParticleSystemUpdate(void *_self)
{
  ParticleSystem *self = (ParticleSystem *)_self;
	size_t i;
	float time = GetTime();

      //update durations
	  for(i = 0; i < self->activeParticles; ++i)
	  {	
      if(!IsPlayingReplay(self))
		    self->dur[i] -= time;

		  if(self->dur[i] <= 0.0f)
			  SwapParticleArray(self, i, --self->activeParticles);
    }

  if(!IsPlayingReplay(_self))
  {
	  //time = GetTime();
	  self->timer -= time;

    if(self->timer <= 0.0f && self->activated)
	  {
		  for(i = 0; i < self->simultaneous; ++i)
      {
        if(self->finite == FALSE || self->ammo > 0)
			    AddParticle(self);
      }

		  self->timer = self->spawnDelay;
	  }

    //I promise I have a good reason for doing them all separately



    //update velocities
    for(i = 0; i < self->activeParticles; ++i)
    {
      self->vel[i].x = (self->acc[i].x * time) + self->vel[i].x;
		  self->vel[i].y = (self->acc[i].y * time) + self->vel[i].y;
	  }

    //update positions
    for(i = 0; i < self->activeParticles; ++i)
    {
      self->pos[i].x = (self->vel[i].x * time) + self->pos[i].x;
		  self->pos[i].y = (self->vel[i].y * time) + self->pos[i].y;	
    }

    if(self->finite == TRUE && self->ammo == 0 && self->activeParticles <= 0)
    {
	    TriggerEmitter(self, FALSE);
      SetDeleteFlag(self, TRUE);
    }
  }
}

static void ParticleSystemDraw(void *_self)
{
	ParticleSystem *self = (ParticleSystem *)_self;
	size_t i;

	for(i = 0; i < self->activeParticles; ++i)
	{
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetTransparency(1.0f);
		DrawImage(self->col[i], self->pos[i].x, self->pos[i].y);
	}
}

static void ParticleSystemUpdateReplay(void *self, void *data)
{
  ParticleSystem *pSysData;
  ParticleSystem *pSys = (ParticleSystem *)self;
  unsigned i;

  if(!data)
  {
    // Create the data
    data = NewShell(oParticleSystem, sizeof(ParticleSystem));
  }

  pSysData = (ParticleSystem *)data;

  SuperUpdateReplay(oParticleSystem, self, data);

  pSysData->pos = (Point2D *)malloc(sizeof(Point2D) * pSys->maxParticles);
  //pSysData->vel = pSys->vel;
 // pSysData->acc = pSys->acc;
  pSysData->dur = (float *)malloc(sizeof(Point2D) * pSys->maxParticles);

  pSysData->activeParticles = pSys->activeParticles;

  for(i = 0; i < pSys->maxParticles; i++)
  {
    pSysData->pos[i] = pSys->pos[i];
    pSysData->dur[i] = pSys->dur[i];
  }

  //pSysData->col = pSys->col;

  pSysData->finite = pSys->finite;
  pSysData->ammo = pSys->ammo;
  pSysData->timer = pSys->timer;
  pSysData->activated = pSys->activated;

  data = pSysData;

  if(GetGameObjectID(self) == PARTICLESYSTEM)
  {
    // Write to the ParticleSystem's Replay
    UpdateReplayData(GetReplay(self), data);
  }
}

static void ParticleSystemReadFromReplay(void *self, void *pReplayData)
{
  ParticleSystem *pSysData = (ParticleSystem *)pReplayData;
  ParticleSystem *pSys = (ParticleSystem *)self;
  unsigned i;

  SuperReadFromReplay(oParticleSystem, self, pReplayData);

  // Update the ParticleSystem's data members from the replay here

  pSys->activeParticles = pSysData->activeParticles;
  for(i = 0; i < pSys->maxParticles; i++)
  {
    pSys->pos[i] = pSysData->pos[i];
    pSys->dur[i] = pSysData->dur[i];
  }

  pSys->finite = pSysData->finite;
  pSys->ammo = pSysData->ammo;
  pSys->timer = pSysData->timer;
  // Only update data member's unique to ParticleSystem.  For example, you shouldn't be
  // updating anything like position since it is already handled
  // by the GameObject.
}

void EmitterMode(void *emitter, enum EMITTERTYPE type, float paramX, float paramY)
{
  ParticleSystem *self = (ParticleSystem*)emitter;
  self->type = type;
  self->paramX = paramX;
  self->paramY = paramY;
}

////////////////////////////
//Object Get Methods
////////////////////////////

////////////////////////////
//Object Set Methods
////////////////////////////

//////////////////////////////////////////////////////////
// Constuctor for ParticleSystem
//////////////////////////////////////////////////////////
/*(oParticleSystem, float xPos, float yPos, float xMov, float yMov, float scale, float ZOrder,
   enum PARTICLECOLOR color, unsigned maxParticles, unsigned simultaneous, float spawnDelay,
	 float lifespanMin, float lifespanMax, float velocityMin, float velocityMax,
	 float velAngleMin, float velAngleMax, float accelMin, float accelMax,
	 float accelAngleMin, float accelAngleMax)*/
static void * ParticleSystemConstructor(void *_self, va_list *app)
{
  ParticleSystem *self = (ParticleSystem *)SuperConstructor(oParticleSystem, _self, app);
  
  // Setup ParticleSystem here
	size_t i;

	self->timer						      = 0.0f;
  self->type                  = EMITTER_POINT;
  self->paramX                = 0.0f;
  self->paramY                = 0.0f;

  self->finite                = va_arg(*app, BOOL);
  self->ammo                  = va_arg(*app, unsigned);
  self->color						      = va_arg(*app, enum PARTICLECOLOR);
  self->maxParticles		  		= va_arg(*app, unsigned);
  self->simultaneous		  		= va_arg(*app, unsigned);

  self->spawnDelay			    	= (float)va_arg(*app, double);
  self->particleLifespanMin		= (float)va_arg(*app, double);
	self->particleLifespanMax		= (float)va_arg(*app, double);

  self->velocityMin				    = (float)va_arg(*app, double);
	self->velocityMax				    = (float)va_arg(*app, double);
  self->velocityAngleMin			= (float)va_arg(*app, double);
	self->velocityAngleMax			= (float)va_arg(*app, double);

  self->accelMin				    	= (float)va_arg(*app, double);
	self->accelMax					    = (float)va_arg(*app, double);
	self->accelAngleMin				  = (float)va_arg(*app, double);
	self->accelAngleMax				  = (float)va_arg(*app, double);

	self->activeParticles			  = 0;

	self->pos					          = (Point2D*)malloc(sizeof(Point2D) * self->maxParticles);
  self->vel					          = (Vector2D*)malloc(sizeof(Vector2D) * self->maxParticles);
  self->acc					          = (Vector2D*)malloc(sizeof(Vector2D) * self->maxParticles);
  self->dur					          = (float*)malloc(sizeof(float) * self->maxParticles);
  self->col					          = (Mesh**)malloc(sizeof(Mesh*) * self->maxParticles);
	
  self->activated					    = TRUE;
	
	//all particles initialized to zero
	for(i = 0; i < self->maxParticles; ++i)
	{
		Vector2D vZero  = {0, 0};
		Point2D pZero   = {0, 0};
		self->acc[i]    = vZero;
		self->col[i]    = NULL;
		self->dur[i]    = 0;
		self->pos[i]    = pZero;
		self->vel[i]    = vZero;
	}

	//if there is no spawn delay, max number of particles are emitted
	if(self->spawnDelay <= EPSILON && self->spawnDelay >= EPSILON)	
	{
		for(i = 0; i < self->maxParticles; ++i)
			AddParticle(self);
	}
  // Be sure to add this GameObjectID to the enumeration in the
  // file GameObjectIDs.h!
  SetGameObjectID(self, PARTICLESYSTEM);

  // Should be a BOOL that tells the particles whether or not they need a replay.
  if(va_arg(*app, BOOL) == TRUE)
    SetReplay(self, CreateNewReplay(self));

  return self;
}

//////////////////////////////////////////////////////////
// Constuctor for ParticleSystemClass
//////////////////////////////////////////////////////////

static void *ParticleSystemClassConstructor(void *_self, va_list *app)
{
  ParticleSystemClass *self = (ParticleSystemClass *)SuperConstructor(oParticleSystemClass, _self, app);

  // This is a macro that will start the selector
  // The selectors will give the metaclass the proper pointers
  //SELECTOR_SETUP
  //(
  //  if SELECTOR(self, FunctionName)
  //)

  return self;
}

//////////////////////////////////////////////////////////
// Destructor for ParticleSystem
//////////////////////////////////////////////////////////

static void *ParticleSystemDestructor(void *_self)
{
  ParticleSystem *self;
  free(((ParticleSystem*)_self)->pos);
  free(((ParticleSystem*)_self)->vel);
  free(((ParticleSystem*)_self)->acc);
  free(((ParticleSystem*)_self)->dur);
  free(((ParticleSystem*)_self)->col);
  self = (ParticleSystem *)SuperDestructor(oParticleSystem , _self);
  return self;
}

//////////////////////////////////////////////////////////
// Initalization of each Object Description.
// This must be called before these objects
// are being used or bad things will happen.
//////////////////////////////////////////////////////////
void InitParticleSystem()
{
  InitGameObject();

  if(!oParticleSystemClass)
  {
    oParticleSystemClass = New(oGameObjectClass, "ParticleSystemClass",
                  oGameObjectClass, sizeof(ParticleSystemClass),
                  METHOD_SET(ParticleSystemClass, Constructor),
                  0);
  }

  if(!oParticleSystem)
  {
    oParticleSystem = New(oParticleSystemClass, "ParticleSystem",
             oGameObject , sizeof(ParticleSystem),
             METHOD_SET(ParticleSystem, Constructor),
             METHOD_SET(ParticleSystem, Destructor),
             METHOD_SET(ParticleSystem, Update),
             METHOD_SET(ParticleSystem, Draw),
             METHOD_SET(ParticleSystem, UpdateReplay),
             METHOD_SET(ParticleSystem, ReadFromReplay),
             0);
  }
}

void FreeParticleSystem()
{
  // Have to cast it to a void * since it is const void *
  free((void *)oParticleSystem);
  free((void *)oParticleSystemClass);
}