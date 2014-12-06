/*************************************/
/** Projectile <source file>		**/
/**									**/
/** Contains functionality for 		**/
/** fireable projectiles.			**/
/*************************************/

#include <math.h>

#include "Game.h"

/*
	Projectile Init

	Purpose: Called once to initialise an projectile
			engine data.

	Parameters: Proj - a pointer to the projectile to
					Affect.

	Return:	Nil
*/

void			ProjectileInit( Projectile* proj )
{
	proj->alive = false;
	proj->texture = GameFindTexture( proj->runner, "Primary" );

	//Set Collision Properties
    proj->col_offset.x = 10;
    proj->col_offset.y = 0;
}

/*
	Projectile Spawn

	Purpose: Spawns an alive projectile at a specified
			position.

	Parameters: Proj - a pointer to the projectile to
					Affect.
				Pos - the position to spawn the projectile
				Vel - the velocity of the projectile

	Return:	True if the projectil is successfully spawned,
			Otherwise False
*/

bool 			ProjectileSpawn( Projectile* proj, const Vector2D& pos, const Vector2D& vel )
{
	// Set Properties
    proj->alive 			= true;
    proj->pos 				= pos;
    proj->vel 				= vel;
    proj->ani.image_index 	= 0;

	// Return Success
	return true;
}

/*
	Projectile Swap

	Purpose: Swaps the data of two projectiles

	Parameters: P1 - the first projectile to swap
				P2 - the second projectile to swap

	Return:	Nil
*/

void 			ProjectileSwap( Projectile* p1, Projectile* p2 )
{
	// Check for Self Swapping
	if (p1 == p2)
		return;

	// Create Temp Object to P1
	Projectile temp = *p1;

	//Set P1 to P2
	*p1 = *p2;

	// Set P2 to temp (previous p1)
	*p2 = temp;
}

/*
	Projectile Update

	Purpose: Updates the projectile.

	Parameters: Proj - a pointer to the projectile to
					Affect.

	Return:	True if the projectil ends the update alive,
			Otherwise False
*/

bool 			ProjectileUpdate( Projectile* proj, Uint32 dt )
{
	// Check we are alive
	if ( !(proj->alive) )
		return false;

	// Update Animation
	AnimationUpdate( &(proj->ani) );

    // Update the Position
    proj->pos += proj->vel;

	// Check for outside screen
	if ( proj->pos.x < -16 )				proj->alive = false; else
	if ( proj->pos.x > SCREEN_WIDTH+16 )	proj->alive = false; else
	if ( proj->pos.y < -16 )				proj->alive = false; else
	if ( proj->pos.y > SCREEN_HEIGHT+16 )	proj->alive = false;

	// Update Collision Position
	proj->collision.x = (Sint32)proj->pos.x-16 + proj->col_offset.x;
	proj->collision.y = (Sint32)proj->pos.y-16 + proj->col_offset.y;

	// Return Alive
	return proj->alive;
}

/*
	Projectile Render

	Purpose: Renders the projectile.

	Parameters: Proj - a pointer to the projectile to
					Affect.

	Return:	Nil
*/

void 			ProjectileRender( Projectile* proj )
{
	int pos_x = (int)roundf(proj->pos.x-16);
	int pos_y = (int)roundf(proj->pos.y-16);

    TextureRender(	proj->texture, proj->runner->render,
					pos_x, pos_y, &(proj->ani.rect) );
}
