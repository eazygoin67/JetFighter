/*************************************/
/** Game <source file>				**/
/**									**/
/** Game contains all functionality **/
/** related to the Game Struct 		**/
/** functionality. 					**/
/*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "Game.h"

/*
	Game Init

	Purpose: Sets up all SDL functionality and
			in-game Variables

	Parameters: Game - a pointer to the Game object
					running the game.

	Return:	Whether or not all libraries and assets
			were loaded/initialised correctly.
			<Boolean>
*/

bool 			GameInit( Game* game )
{
	printf("--- Game Init ---\n");

	// Set Random Seed
	srand( time(0) );

	// Start SDL
	if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	// Load SDL_IMG set for loading: PNG
	int img_flags = IMG_INIT_PNG;
	if ( !(IMG_Init(img_flags) & img_flags) )
	{
		printf( "SDL Image Exension could not load. IMG Error: %s\n", IMG_GetError() );
		SDL_Quit();
		return false;
	}

	// Load SDL_TTF
	if ( TTF_Init() == -1 )
	{
		printf( "SDL TTF Extension could not load. TTF Error: %s\n", TTF_GetError() );
		IMG_Quit();
		SDL_Quit();
		return false;
	}

	// Initialise Gamepad
	///TODO: Initialise Gamepad Support

	// Set Running State to True, so we can enter the main loop
	game->running = true;

	// Create the SDL window
	///TODO: Error Check when creating SDL Screen and Renderer Objects
	game->window = SDL_CreateWindow( "JET Fighter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0 );
	game->render = SDL_CreateRenderer( game->window, -1, SDL_RENDERER_ACCELERATED );

	// Set Screen Color
	game->screen_color.r = 0;
	game->screen_color.g = 128;
	game->screen_color.b = 255;
	game->screen_color.a = 255;

	// Return Success
	printf("--- Game Init Successful ---\n\n");
	return true;
}

/*
	Game Setup

	Purpose: Once the game has been initialised, and
			we have loaded all assets, we can properly
			setup game environment, ready for play.

	Parameters: Game - a pointer to the Game object
					running the game.

	Return:	Nil
*/

void 			GameSetup( Game* game )
{
	// Set Playing to Off
	game->playing = false;

	// Set background texture
	game->background_texture = GameFindTexture(game, "Background");

	if ( game->background_texture == nullptr )
		printf( "Error: Background Texture was not correctly Assigned\n" );

	// Set Up Player Object
	game->player.runner = game;
	PlayerSetup( &(game->player) );

	if ( game->player.sprite_texture == nullptr )
		printf( "Error: Player Texture was not correctly Assigned\n" );

	// Set Up Enemies
	for( Uint16 i = 0; i < POOLSIZE_ENEMY; i++ )
	{
        game->enemies[i].runner = game;
        EnemyInit( &(game->enemies[i]) );
	}

	// Setup Particles
	for( Uint16 i = 0; i < POOLSIZE_PARTICLE; i++ )
	{
		game->particles[i].runner = game;
		ParticleInit( &(game->particles[i]) );
	}

	// Setup Projectiles
	for( Uint16 i = 0;
		 i < (POOLSIZE_ENEMY_PROJ > POOLSIZE_FRIENDLY_PROJ ? POOLSIZE_ENEMY_PROJ : POOLSIZE_FRIENDLY_PROJ);
		 i++ )
	{
		// Friendly Projectiles
		if ( i < POOLSIZE_FRIENDLY_PROJ )
		{
			game->projectiles_friendly[i].runner = game;
			ProjectileInit( &(game->projectiles_friendly[i]) );
		}

		// Enemy Projectiles
		if ( i < POOLSIZE_ENEMY_PROJ )
		{
			game->projectiles_enemy[i].runner = game;
			ProjectileInit( &(game->projectiles_enemy[i]) );
		}
	}
}

/*
	Game Reset

	Purpose: Used to reset the state of the game to
			it's initial state. Used when after the
			player is killed.

	Parameters: Game - a pointer to the Game object
					running the game.

	Return:	Nil
*/

void 			GameReset( Game* game )
{
	// Turn off playing
	game->playing 						= false;

	// Switch off needing a reset
	game->needs_reset 					= false;
	game->next_reset					= 0;

	// Reset Player to Default State
    game->player.pos 					= Vector2D( SCREEN_WIDTH*0.5, SCREEN_HEIGHT*0.5 );
    game->player.vel.zero();

	game->player.alive 					= true;
	game->player.hp 					= 100;
	game->player.hp_max 				= 100;

	// Reset all enemies
	game->enemy_count = 0;
	for( Uint16 i = 0; i < POOLSIZE_ENEMY; i++ )
		game->enemies[i].alive = false;

	// Reset all particles
	game->part_count = 0;
	for( Uint16 i = 0; i < POOLSIZE_PARTICLE; i++ )
		game->particles[i].alive = false;

	// Reset all Projectiles
	for( Uint16 i = 0;
		 i < (POOLSIZE_ENEMY_PROJ > POOLSIZE_FRIENDLY_PROJ ? POOLSIZE_ENEMY_PROJ : POOLSIZE_FRIENDLY_PROJ);
		 i++ )
	{
		// Friendly Projectiles
		if ( i < POOLSIZE_FRIENDLY_PROJ )
			game->projectiles_friendly[i].alive = false;

		// Enemy Projectiles
		if ( i < POOLSIZE_ENEMY_PROJ )
			game->projectiles_enemy[i].alive = false;
	}
}

/*
	Game Start Play

	Purpose: Used to start the game, when the player
			hits the button

	Parameters: Game - a pointer to the Game object
					running the game.

	Return:	Nil
*/

void 			GameStartPlay( Game* game )
{
	if (!game->playing)
		GameReset(game);

	game->playing = true;
}

/*
	Game Quit

	Purpose: Called at the end of the programs,
			lifetime, to unload all libraries and
			assets. Should be used after the game
			loop.

	Parameters: Game - a pointer to the Game object
					running the game.

	Return:	Nil
*/

void 			GameQuit( Game* game )
{
	// Close Gamepad
	///TODO: Close Gamepad if it is open

	// Unload SDL and Extensions
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

/*
	Game Load Assets

	Purpose: Loads in-game data assets.

	Parameters: Game - a pointer to the Game object
					running the game.

	Return:	Whether or not loading was successful.
			<Boolean>
*/

bool 			GameLoadAssets( Game* game )
{
	// Load Game Font
	game->game_font = TTF_OpenFont( "Data/Fonts/kenvector_future_thin.ttf", 16 );
	if ( game->game_font == 0 || game->game_font == nullptr )
		return false;

	// Load Start Text
	if ( !TextureInit( &(game->game_start_text), "Start Text" ) )
		return false;
	SDL_Color text_color = {255,255,255,255};
	if (!TextureLoadFromText( &(game->game_start_text), game->render,
							  "Press Enter to Start", game->game_font, text_color ))
		return false;

	// Primary Texture
	if ( !TextureInit( &(game->texture_assets[0]), "Primary" ) )
		return false;
	if ( !TextureLoadFile( &(game->texture_assets[0]), game->render, "Data/Images/Primary.png" ) )
		return false;

	// Background Texture
	if ( !TextureInit( &(game->texture_assets[1]), "Background" ) )
		return false;
	if ( !TextureLoadFile( &(game->texture_assets[1]), game->render, "Data/Images/Background.png" ) )
		return false;

	//Return Success
	return true;
}

/*
	Game Free Assets

	Purpose: Frees all In Game Assets.

	Parameters: Game - a pointer to the Game object
					running the game.

	Return:	Nil
*/

void 			GameFreeAssets( Game* game )
{
	// Unload Font and Text Textures
	TextureFree( &game->game_start_text );
	TTF_CloseFont(game->game_font);

	// Free Textures
	for( Uint16 i = 0; i < TEXTURE_COUNT; i++ )
	{
		// Free Texture Assets
		TextureFree( &(game->texture_assets[i]) );
	}
}

/*
	Game Find Texture

	Purpose: Finds the Texture with a specific name,
			if it is held by the game

	Parameters: Game - a pointer to the Game object
					running the game.
				Name - the name of the asset to find

	Return:	The Asset Texture, if it was found,
			otherwise it will return nullptr
*/

AssetTexture*	GameFindTexture( Game* game, const char* name )
{
	// Search Linearly for the correct texture
    for( Uint16 i = 0; i < TEXTURE_COUNT; i++ )
	{
		if ( strcmp( game->texture_assets[i].name, name ) == 0 )
			return &(game->texture_assets[i]);
	}

	// Texture was not found, Return null
	return nullptr;
}

/*
	Game Update Background

	Purpose: Scrolls the current background
			texture.

	Parameters: Game - a pointer to the Game object
					running the game.
				dt - Delta time, the time that has
					passed since the last update.

	Return:	Nil
*/

void 			GameBackgroundUpdate( Game* game, Uint32 dt )
{
	// Scroll Background Down
	game->background_y += game->background_yspeed;
	game->background_y %= game->background_texture->height;
}

/*
	Game Render Background

	Purpose: Draws the current background texture,
			repeating it to fill the whole screen.

	Parameters: Game - a pointer to the Game object
					running the game.

	Return:	Nil
*/

void 			GameBackgroundRender( Game* game )
{
	// Draw Background Tiled
	int x_pos;
	int y_pos;
    Uint16 repeat_x = SCREEN_WIDTH / game->background_texture->width + 1;
    Uint16 repeat_y = SCREEN_HEIGHT / game->background_texture->height + 2;

	for( Uint16 row = 0; row < repeat_y; row++ )
	{
		x_pos = 0;
		y_pos = (-game->background_texture->height+game->background_y) + (game->background_texture->height*row);

		for( Uint16 col = 0; col < repeat_x; col++ )
		{
			x_pos = game->background_texture->width*col;

			TextureRender( game->background_texture, game->render, x_pos, y_pos );
		}
	}

}

/*
	Game Input Joystick

	Purpose: Called when handling input to update
			the state of the gamepad axis(s).

	Parameters: Game - a pointer to the Game object
					running the game.
				E - A pointer to the event from the
					event loop polled when chacking
					input

	Return:	Nil
*/

void 			GameInputJoystickAxis( Game* game )
{
	///TODO: Implement Gamepad Events
}

/*
	Game Input

	Purpose: Called each gameloop while we have not
			reached the max number of enemies. It
			has the possibility of randomly spawning
			a new enemy.

	Parameters: Game - a pointer to the Game object
					running the game.

	Return:	Nil
*/

void 			GameSpawnRandomEnemy( Game* game )
{
	// Check we haven't hit enemy capacity
	if (game->enemy_count >= POOLSIZE_ENEMY)
		return;

	// Do Random Chance Check
	Uint32 chance = ENEMY_SPAWN_CHANCE + ENEMY_SPAWN_CHANCE*0.1;
	if ( rand()%chance != 0 )
		return;

	// Choose Random Type
	EnemyType type = ENEMY_SIMPLE; //= (EnemyType)rand()%enemy_type_count;

	// Spawn Random Enemy
    EnemySetup( &(game->enemies[game->enemy_count]), type );

    // Increment Enemy Count
    ++game->enemy_count;
}

/*
	Game Kill Enemy

	Purpose: Called each gameloop while we have not
			reached the max number of enemies. It
			has the possibility of randomly spawning
			a new enemy.

	Parameters: Game - a pointer to the Game object
					running the game.
				Enemy - a pointer to the dead enemy

	Return:	Nil
*/

void 			GameKillEnemy( Game* game, Enemy* enemy )
{
	// Set Enemy to Dead
	enemy->alive = false;

	// Swap with the last enemy we have alive
	EnemySwap( enemy, &(game->enemies[game->enemy_count-1]) );

	// Decrement the Enenmy Count
	--(game->enemy_count);
}

/*
	Game Spawn Particle

	Purpose: Spawns a new particle from the particle
			pool.

	Parameters: Game - a pointer to the Game object
					running the game.
				Pos - the position for the particle
				Type - the type of particle

	Return:	Nil
*/

void 			GameSpawnParticle( Game* game, const Vector2D& pos, PartType type )
{
	// Check we haven't reached the limit
    if (game->part_count >= POOLSIZE_PARTICLE)
		return;

	// Spawn Particle
	ParticleSpawn( &(game->particles[game->part_count++]), pos, type );
}

/*
	Game Kill Particle

	Purpose: Kills a specified particle in the memory
			pool.

	Parameters: Game - a pointer to the Game object
					running the game.
				Part - a pointer to the dead particle

	Return:	Nil
*/

void 			GameKillParticle( Game* game, Particle* part )
{
	// Set Particle to Dead
    part->alive = false;

    // Swap with last particle we have alive
    ParticleSwap( part, &(game->particles[game->part_count-1]) );

    // Decrement Particle Count
    --(game->part_count);
}

/*
	Game Spawn Projectile

	Purpose: Spawns a new projectile from the memory
			pool.

	Parameters: Game - a pointer to the Game object
					running the game.
				Friendly - is the projectile friendly
				Pos - the position to spawn
				Vel - the velocity of the particle

	Return:	A pointer to the new projectile, for further
			mutation.
			Returns nullptr if all memory pool is fully
			taken.
*/

Projectile*		GameSpawnProjectile( Game* game, bool friendly, const Vector2D& pos, const Vector2D& vel )
{
	// Find where we are storing the projectile
	Projectile*		proj_array = ( friendly ? game->projectiles_friendly : game->projectiles_enemy );
	Uint16&			proj_count = ( friendly ? game->proj_friendly_count : game->proj_enemy_count );
	const Uint16	pool_size  = ( friendly ? POOLSIZE_FRIENDLY_PROJ : POOLSIZE_ENEMY_PROJ );

	// Do we have room for a new projectile
	if ( proj_count >= pool_size )
		return nullptr;

	// Find and spawn new projectile
	Projectile* proj = &(proj_array[proj_count++]);
	proj->friendly = friendly;
	ProjectileSpawn( proj, pos, vel );

	// Return new projectile
	return proj;
}

/*
	Game Kill Projectile

	Purpose: Kills a specified projectile in the memory
			pool.

	Parameters: Game - a pointer to the Game object
					running the game.
				Proj - a pointer to the dead projectile

	Return:	Nil
*/

void 			GameKillProjectile( Game* game, Projectile* proj )
{
	// Find where we are storing the projectile
	Projectile*		proj_array = ( proj->friendly ? game->projectiles_friendly : game->projectiles_enemy );
	Uint16&			proj_count = ( proj->friendly ? game->proj_friendly_count : game->proj_enemy_count );

	// Set Projectile to Dead
    proj->alive = false;

    // Swap with last projectile we have alive
    ProjectileSwap( proj, &(proj_array[proj_count-1]) );

    // Decrement Projectile Count
    --proj_count;
}

/*
	Game Render Hud

	Purpose: Draws the heads-up display elements of
			game (e.g. health bar, score, etc.)

	Parameters: Game - a pointer to the Game object
					running the game.
				Part - a pointer to the dead particle

	Return:	Nil
*/

void 			GameRenderHud( Game* game )
{
	// HUD While Playing
	if (game->playing == true)
	{
		// Render Player Health Bar
		if (game->player.hp > 0)
		{
			float bar_length = (float)game->player.hp / (float)game->player.hp_max * 150.0f;
			SDL_Rect bar = { 32, 32, (int)bar_length, 4 };
			SDL_SetRenderDrawColor( game->render, 0xFF, 0x00, 0x00, 0xFF );
			SDL_RenderFillRect( game->render, &bar );
		}
	}
	else
	// HUD While Waiting
	{
		// Render "Press Start" Text
		TextureRender( &(game->game_start_text), game->render,
						   SCREEN_WIDTH/2-game->game_start_text.width/2,
						   SCREEN_HEIGHT/2+64 );
	}
}

/*
	Game Loop

	Purpose: Runs the main game loop

	Parameters: Game - a pointer to the Game object
					running the game.

	Return:	Nil
*/

void 			GameLoop( Game* game )
{
	// Time Variables
	const Uint32 time_per_frame = 1000 / FPS;

	Uint32 frame_time = SDL_GetTicks();
	Uint32 acc = 0;

	// Using a semi-fixed timestep
	while(game->running)
	{
		Uint32 new_time = SDL_GetTicks();
		Uint32 diff_time = new_time - frame_time;

		frame_time = new_time;
		acc += diff_time;

		while( acc > time_per_frame )
		{
			// Check for Reset Game
			if (game->needs_reset && new_time >= game->next_reset)
				GameReset(game);

			// Process Events
			GameInput(game);

			// Update Game
			GameUpdate(game, time_per_frame);

			// Fix Timestep
			acc -= time_per_frame;
		}

		// Render Game
		GameRender(game);
	}
}

/*
	Game Input

	Purpose: Updates input devices and processes
			incoming events

	Parameters: Game - a pointer to the Game object
					running the game.

	Return:	Nil
*/

void 			GameInput( Game* game )
{
	// Handle Events
	while( SDL_PollEvent( &(game->event) ) )
	{
		if (game->event.type == SDL_QUIT)
			game->running = false;				// Exit Program
	}

	game->key_states = SDL_GetKeyboardState(0);
	GameInputJoystickAxis( game );

	///TODO: Possibly change so escape just resets the game
	// Check for Escape Key Quit
	if (game->key_states[SDL_SCANCODE_ESCAPE] > 0)
		game->running = false;

	// Check for Begin Play if we haven't already started
	if ( !game->playing && game->key_states[SDL_SCANCODE_RETURN] > 0 )
		GameStartPlay(game);

	game->key_up 	= (game->key_states[SDL_SCANCODE_UP] > 0);
	game->key_down 	= (game->key_states[SDL_SCANCODE_DOWN] > 0);
	game->key_left 	= (game->key_states[SDL_SCANCODE_LEFT] > 0);
	game->key_right	= (game->key_states[SDL_SCANCODE_RIGHT] > 0);
	game->key_shoot	= (game->key_states[SDL_SCANCODE_SPACE] > 0);
}

/*
	Game Update

	Purpose: Updates all game objects and systems

	Parameters: Game - a pointer to the Game object
					running the game.
				dt - Delta time, the time that has
					passed since the last update.

	Return:	Nil
*/

void 			GameUpdate( Game* game, Uint32 dt )
{
	// Update the Background
	GameBackgroundUpdate( game, dt );

	// Check that we are in game before updating entities
	if (game->playing == false)
		return;

	// Randomly Spawn Enemies
	if ( game->enemy_count < POOLSIZE_ENEMY )
		GameSpawnRandomEnemy(game);

	// Update Player
	PlayerUpdate( &(game->player), dt );

	// Update Enemies
	for( Sint32 i = 0; i < game->enemy_count; i++ )
	{
		// Check if the enemy is dead after it's update
		if ( !game->enemies[i].update( &(game->enemies[i]), dt ) )
		{
			GameKillEnemy( game, &(game->enemies[i]) );
			--i;
		}
	}

	// Update Particles
	for( Sint32 i = 0; i < game->part_count; i++ )
	{
		// Check if the particle is dead after it's update
		if ( !ParticleUpdate(&(game->particles[i]), dt) )
		{
			GameKillParticle( game, &(game->particles[i]) );
			--i;
		}
	}

	// Friendly Projectiles
	for( Uint16 i = 0; i < game->proj_friendly_count; i++ )
	{
		if ( !ProjectileUpdate( &(game->projectiles_friendly[i]), dt ) )
		{
			GameKillProjectile( game, &(game->projectiles_friendly[i]) );
			--i;
		}
	}

	// Enemy Projectiles
	for( Uint16 i = 0; i < game->proj_enemy_count; i++ )
	{
		if ( !ProjectileUpdate( &(game->projectiles_enemy[i]), dt ) )
		{
			GameKillProjectile( game, &(game->projectiles_enemy[i]) );
			--i;
		}
	}
}

/*
	Game Render

	Purpose: Renders all game objects

	Parameters: Game - a pointer to the Game object
					running the game.

	Return:	Nil
*/

void 			GameRender( Game* game )
{
	// Clear Screen
	SDL_SetRenderDrawColor( game->render,
							game->screen_color.r, game->screen_color.g, game->screen_color.b,
							255 );
	SDL_RenderClear( game->render );

		// Draw Background
		GameBackgroundRender(game);

		// Draw Particles
		for( Uint16 i = 0; i < game->part_count; i++ )
			ParticleRender( &(game->particles[i]) );

		// Freindly Projectiles
		for( Uint16 i = 0; i < game->proj_friendly_count; i++ )
			ProjectileRender( &(game->projectiles_friendly[i]) );

		// Enemy Projectiles
		for( Uint16 i = 0; i < game->proj_enemy_count; i++ )
			ProjectileRender( &(game->projectiles_enemy[i]) );

		// Draw Enemies
		for( Uint16 i = 0; i < game->enemy_count; i++ )
			EnemyRender( &(game->enemies[i]) );

		// Draw Player
		PlayerRender( &(game->player) );

		// Draw Heads-Up Display
		GameRenderHud(game);

	// Draw to the Screen
    SDL_RenderPresent( game->render );
}
