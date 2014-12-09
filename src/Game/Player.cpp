/*************************************/
/** Player <source file>			**/
/**									**/
/** Player contains all relevant 	**/
/** functionality regarding the 	**/
/** player.							**/
/*************************************/

#include <stdio.h>
#include <math.h>

#include "Game.h"

/*
	Player Setup

	Purpose: Sets up a player to a default state,
			just before the game starts

	Parameters: Player - a pointer pointing to the
					player object.

	Return:	Nil
*/

void 			PlayerSetup( Player* player )
{
    player->pos 					= Vector2D( SCREEN_WIDTH*0.5, SCREEN_HEIGHT*0.5 );
    player->vel 					= Vector2D(0,0);

	player->alive 					= true;
	player->hp 						= 100;
	player->hp_max 					= 100;

	player->sprite_texture 			= GameFindTexture( player->runner, "Primary" );

	player->col_offset.x 			= 4;
	player->col_offset.y 			= 4;

	player->ship_ani.image_count 	= 1;
	player->ship_ani.image_speed 	= 0;
	player->ship_ani.image_index 	= 0;
	player->ship_ani.y_offset 		= 32;
	player->ship_ani.rect.w 		= 32;
	player->ship_ani.rect.h 		= 32;
	player->ship_ani.loops 			= false;
}

/*
	Player Update

	Purpose: Updates the Player Object, called
			each update.

	Parameters: Player - a pointer pointing to the
					player object.
				DT - the delta time, time passed
					between this frame and the
					previous.

	Return:	Nil
*/

void			PlayerUpdate( Player* player, Uint32 dt )
{
	if (player->alive == false)
		return;

	// Slow Velocity
	player->vel *= player->move_fric;

	// Cache Game for Simplicity
	Game* game = player->runner;

	// Check for Movement Input
	Vector2D input;
		if ( game->key_up )
			input.y -= player->move_speed;
		if ( game->key_down )
			input.y += player->move_speed;
		if ( game->key_left )
			input.x -= player->move_speed;
		if ( game->key_right )
			input.x += player->move_speed;
	input.truncate(player->move_speed);

	// Add Input to Velocity
	player->vel += input;
	player->vel.truncate( player->move_max );

	// Check Dead Zones
	if ( fabsf(player->vel.x) < player->move_dead )
		player->vel.x = 0;
	if ( fabsf(player->vel.y) < player->move_dead )
		player->vel.y = 0;

	// Add Velocity to Position
	player->pos += player->vel;

	// Clamp Position to stay inside screen
	if (player->pos.x  < 0)					player->pos.x = 0; else
	if (player->pos.x > SCREEN_WIDTH)		player->pos.x = SCREEN_WIDTH;
	if (player->pos.y  < 0)					player->pos.y = 0; else
	if (player->pos.y > SCREEN_HEIGHT)		player->pos.y = SCREEN_HEIGHT;

	// Set Collision Box Position
	player->collision.x = (int)player->pos.x-16 + player->col_offset.x;
	player->collision.y = (int)player->pos.y-16 + player->col_offset.y;

	// Check for Shooting
	if (game->key_shoot)
	{
		if ( player->next_shot < SDL_GetTicks() )
		{
			// Set Cooldown
			player->next_shot = SDL_GetTicks() + 250;

			// Get Position and Velocity of Projectile
			Vector2D pos = player->pos;
			Vector2D vel( 0, -10 );
			pos.y -= 32;

			// Spawn Projectile
			Projectile* proj = GameSpawnProjectile( game, true, pos, vel );

			// Additional Properties
			if (proj != nullptr)
			{
				proj->ani.image_count = 1;
				proj->ani.y_offset = 32*7;
				proj->damage = 10;
			}
		}
	}

	// Check Projectile Collisions
	Projectile* proj;	for( Uint16 i = 0; i < game->proj_enemy_count; i++ )
	{
		proj = &(game->projectiles_enemy[i]);

		if ( UtilityAabbCheck( &(player->collision), &(proj->collision) ) )
		{
			player->hp -= proj->damage;
			proj->alive = false;
		}
	}

	// Check for Death
	if ( player->hp <= 0 )
	{
		player->alive = false;
		GameSpawnParticle( game, player->pos, PART_EXPLOSION_01 );

		game->needs_reset = true;
		game->next_reset = SDL_GetTicks() + 2000;
	}

}

/*
	Player Render

	Purpose: Calls the Player to execute all it's
			draw functionality.

	Parameters: Player - a pointer pointing to the
					player object.

	Return:	Nil
*/

void 			PlayerRender( Player* player )
{
	if (player->alive == false)
		return;

	AnimationUpdate( &(player->ship_ani) );

	int pos_x = (int)roundf(player->pos.x-16);
	int pos_y = (int)roundf(player->pos.y-16);

    TextureRender(	player->sprite_texture, player->runner->render,
					pos_x, pos_y, &(player->ship_ani.rect) );

	// Render Collision Box (For Debugging Only)
	//SDL_SetRenderDrawColor( player->runner->render, 255, 0, 0, 128 );
	//SDL_RenderFillRect( player->runner->render, &(player->collision) );
}
