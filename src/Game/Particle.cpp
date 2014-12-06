/*************************************/
/** Particle <source file>			**/
/**									**/
/** Contains functionality that		**/
/** pertains to Particle 			**/
/** functionality. 					**/
/*************************************/

#include <math.h>

#include "Game.h"

/*
	Particle Init

	Purpose: Called once to initialise an particles
			engine data.

	Parameters: Part - a pointer to the particle to
					Affect.

	Return:	Nil
*/

void 			ParticleInit( Particle* part )
{
	part->texture = GameFindTexture( part->runner, "Primary" );
	part->ani.loops = false;
}

/*
	Particle Spawn

	Purpose: Used to spawn a new instance of a particle,
			setting it up to be a specific type.

	Parameters: Part - a pointer to the particle to
					Affect.
				Pos - the position where the particle
					should appear.
				Type - the predefined type we want the
					particle to be.

	Return:	True if we spawn the particle successfully,
			False if we fail
*/

bool 			ParticleSpawn( Particle* part, const Vector2D& pos, PartType type )
{
	// Check that we are not overwritting an already alive particle
	if (part->alive == true)
		return false;

	// Setup Consistent Properties
	part->alive = true;
	part->pos = pos;

	switch(type)
	{
		case PART_EXPLOSION_01:
		{
			part->ani.image_index = 0;
			part->ani.image_speed = 0.2;
			part->ani.image_count = 6;
			part->ani.y_offset = 0;

			return true;
		}
		break;

		default:
		{
			return false;
		}
		break;
	}
}

/*
	Particle Swap

	Purpose: Used to swap the values of two particle
			objects.

	Parameters: P1 - the first particle in the swap.
				P2 - the second particle in the swap.

	Return:	Nil
*/

void 			ParticleSwap( Particle* p1, Particle* p2 )
{
	// Check for difference to swap
	if (p1 == p2)
		return;

	// Create Temporary Particle to Go Between
	Particle temp = *p1;

	// Set P1 to P2
	*p1 = *p2;

	// Set P2 to Temp (previous P1)
	*p2 = temp;

}

/*
	Particle Update

	Purpose: Called to update the particle animation
			and check for particle death.

	Parameters: Part - a pointer to the particle to
					Affect.

	Return:	True if the particle ends the update alive
			False otherwise
*/

bool 			ParticleUpdate( Particle* part, Uint32 dt )
{
	// Update Animation
	AnimationUpdate(&(part->ani));

	// Check For Animation Death
	if (part->ani.image_speed == 0)
		part->alive = false;

	return part->alive;
}

/*
	Particle Render

	Purpose: Called to draw the particle.

	Parameters: Part - a pointer to the particle to
					Affect.

	Return:	Nil
*/

void 			ParticleRender( Particle* part )
{
	int pos_x = (int)roundf(part->pos.x-16);
	int pos_y = (int)roundf(part->pos.y-16);

    TextureRender(	part->texture, part->runner->render,
					pos_x, pos_y, &(part->ani.rect) );
}
