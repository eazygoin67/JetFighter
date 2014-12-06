/*************************************/
/** Utility <source file>			**/
/**									**/
/** Contains functionality that		**/
/** are used for multiple purposes, **/
/** potentially by multiple objects **/
/** all throughout the game.		**/
/*************************************/

#include "Game.h"

/*
	Utility AABB Collision

	Purpose: Checks if two Axis Aligned Boxes
			collide with each other.

	Parameters: R1 - a pointer to the first
					box to check
				R2 - a pointer to the second
					box to check

	Return:	True if the two rectangles collide
			False if they don't collide
*/

bool			UtilityAabbCheck( SDL_Rect* r1, SDL_Rect* r2 )
{
	if ( r2->x+r2->w < r1->x ) 		return false;
	if ( r2->x >= r1->x+r1->w )		return false;

	if ( r2->y+r2->h < r1->y ) 		return false;
	if ( r2->y >= r1->y+r1->h )		return false;

	return true;
}
