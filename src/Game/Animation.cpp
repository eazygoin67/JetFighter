/*************************************/
/** Animation <source file>			**/
/**									**/
/** Contains all functionality 		**/
/** relating to the updating and 	**/
/** maintaining of an Animation.	**/
/*************************************/

#include <math.h>

#include "Game.h"

/*
	Sign

	Purpose: Returns whether a value is larger or
			smaller than 0.

	Parameters: Value - the value we are checking.

	Return:	0 - if the value is 0
			1 - if the value is greater than 0
		   -1 - if the value is less than 0
*/

inline float sign( float value )
{
	if (value == 0)
		return 0;

	return ( value > 0 ? 1 : -1 );
}


/*
	Animation Update

	Purpose: Called to run the Animation and
			reposition the Animation
			rectangle based on the animation's
			attributes.

	Parameters: Ani - a pointer to the animation
			that we are updating.

	Return:	Nil
*/

void 			AnimationUpdate( Animation* ani )
{
	// Make sure we have at least 1 image in the sprite
	if (ani->image_count == 0)
		return;

    // Update Current Frame
    ani->image_index += ani->image_speed;

    // Make sure we don't animate faster that the number
    // of images we have
    if ( fabsf(ani->image_speed) > ani->image_count )
		ani->image_speed = (ani->image_count-1) * sign( ani->image_speed );

    // Make Animation Frame Roll Over if Nessesary
    if (ani->loops == true )
	{
		ani->image_index = fmodf(ani->image_index, (float)ani->image_count );
		if ( ani->image_index < 0 )
			ani->image_index = ani->image_count+ani->image_index;
	}
    else
	// We Stop the Animation When It Ends
	{
		if (ani->image_index < 0)
		{
			ani->image_index = 0;
			ani->image_speed = 0;
		}

		if (ani->image_index >= ani->image_count)
		{
			ani->image_speed = 0;
			ani->image_index = ani->image_count-1;
		}
	}

	// Update the SDL Rect Position
	int pos_x = ani->rect.w * (int)ani->image_index;

	ani->rect.x = pos_x;
	ani->rect.y = ani->y_offset;
}
