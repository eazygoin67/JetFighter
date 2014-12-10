#include "Game/Game.h"

///TODO: Draw Score to Screen
///TODO: Add One More New Enemy
///TODO: Bullet Hit Particle
///NOTE: Change Poolsizes to accommodate more things at once

int main( int argc, char** argv )
{
	// Create Game
    Game game;

    // Initialise Game; Return Failure if we unsuccessful
    if ( !GameInit(&game) )
		return -1;

	// Load Game Assets; Return Failure if we unsuccessful
	if ( !GameLoadAssets(&game) )
	{
		// Quit and Return Failure
		GameQuit(&game);
		return -2;
	}

	// Do Initial Setup of game, after resources are loaded
	GameSetup(&game);

	// Enter Main Game Loop
	GameLoop(&game);

	// Unload Game Assets
	GameFreeAssets(&game);

    // Unload Game Data when Exitting
    GameQuit(&game);

    // Return Success
    return 0;
}
