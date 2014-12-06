/*************************************/
/** Enenmy <source file>			**/
/**									**/
/** Contains all functionality 		**/
/** relating to functioning of 		**/
/** enemy objects of all types.		**/
/*************************************/

#include <stdlib.h>
#include <math.h>

#include "Game.h"

/*
	Enemy Init

	Purpose: Called once to initialise an enemies
			engine data.

	Parameters: Enemy - a pointer to the enemy to
					Affect.

	Return:	Nil
*/

void 			EnemyInit( Enemy* enemy )
{
	enemy->alive = false;
	enemy->sprite_texture = GameFindTexture( enemy->runner, "Primary" );
}

/*
	Enemy Setup

	Purpose: Called to Setup an enemy to be
			active and of a specific type. The enemy
			must be dead to be setup.

	Parameters: Enemy - a pointer to the enemy to
					Change.
				Type - the type of enemy to spawn.
				Spawn Random - Whether or not we should
						affect the position to be
						random, based on the type.

	Return:	True if the enemy was successfully initialised.
			False returned otherwise.
*/

bool 			EnemySetup( Enemy* enemy, EnemyType type, bool spawn_random )
{
	if (enemy->alive != false)
		return false;

	switch( type )
	{
		/*** Simple Enemy ***/
		case (ENEMY_SIMPLE):
		{
			enemy->type 					= type;

			enemy->pos.zero();
			enemy->vel						= Vector2D( 0.0f, 3.0 + rand()%200 * 0.01 );

			enemy->alive 					= true;
			enemy->hp 						= 1;
			enemy->hp_max					= 1;
			enemy->score_value 				= 10;

			enemy->collision.w 				= 24;
			enemy->collision.h 				= 24;
			enemy->col_offset.x 			= 4;
			enemy->col_offset.y 			= 4;

			enemy->ship_ani.image_count 	= 1;
			enemy->ship_ani.image_speed 	= 0;
			enemy->ship_ani.image_index 	= 0;
			enemy->ship_ani.y_offset 		= 32*3;
			enemy->ship_ani.rect.w 			= 32;
			enemy->ship_ani.rect.h 			= 32;
			enemy->ship_ani.loops 			= false;

			enemy->update					= EnemyUpdateSimple;

			if ( spawn_random )
				enemy->pos = Vector2D( 32+rand()%SCREEN_WIDTH-64, -64 );

			return true;
		}
		break;

		/*** Fail Case ***/
		default:
			return false;
		break;
	}
}

/*
	Enemy Swap

	Purpose: Swaps the values of two enemy objects.

	Parameters: E1 - a pointer to the first enemy to
					be swapped.
				E2 - a pointer to the second enemy to
					be swapped.

	Return:	Nil
*/

void 			EnemySwap( Enemy* e1, Enemy* e2 )
{
	// Check that we aren't trying to swap the same value
    if (e1 == e2)
		return;

	Enemy temp = *e1;
	*e1 = *e2;
	*e2 = temp;
}

/*
	Enemy Render

	Purpose: Called to draw an enemy to the screen

	Parameters: Enemy - a pointer to the enemy to
					Affect.

	Return:	Nil
*/

void 			EnemyRender( Enemy* enemy )
{
	AnimationUpdate( &(enemy->ship_ani) );

	int pos_x = (int)roundf(enemy->pos.x-16);
	int pos_y = (int)roundf(enemy->pos.y-16);

    TextureRender(	enemy->sprite_texture, enemy->runner->render,
					pos_x, pos_y, &(enemy->ship_ani.rect) );
}

/*
	Enemy Update Simple

	Purpose: <Enemy Update Function>
			 This Simple enemy will fly in a straight
			 vertical pattern, until it goes out of
			 screen.

	Parameters: Enemy - a pointer to the enemy to
					Affect.

	Return:	True if the enemy ends the update still alive
			Otherwise false
*/

bool			EnemyUpdateSimple( Enemy* enemy, Uint32 dt )
{
	// Move Positions
	enemy->pos += enemy->vel;

	// Update Collision Box
	enemy->collision.x = (int)enemy->pos.x-16 + enemy->col_offset.x;
	enemy->collision.y = (int)enemy->pos.y-16 + enemy->col_offset.y;

	// Cache Game Pointer for Ease
	Game* game = enemy->runner;

	// Check Player Collision
	Player* player = &(game->player);
	if (player->alive)
	{
		if (UtilityAabbCheck( &(enemy->collision), &(player->collision) ))
		{
			player->hp -= 25;
			enemy->alive = false;
			GameSpawnParticle( game, enemy->pos, PART_EXPLOSION_01 );
		}
	}

	// Check Projectile Collisions
	Projectile* proj;	for( Uint16 i = 0; i < game->proj_friendly_count; i++ )
	{
		proj = &(game->projectiles_friendly[i]);

		if ( UtilityAabbCheck( &(enemy->collision), &(proj->collision) ) )
		{
			enemy->hp -= proj->damage;
			proj->alive = false;
		}
	}

	// Check for Out of Screen
	if ( enemy->pos.y > SCREEN_HEIGHT+64 )
		enemy->alive = false;

	// Check for no more health
	if (enemy->hp <= 0)
	{
		GameSpawnParticle( game, enemy->pos, PART_EXPLOSION_01 );
		enemy->alive = false;
	}

	return enemy->alive;
}
