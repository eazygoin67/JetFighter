/*************************************/
/** Texture <source file>			**/
/**									**/
/** Texture contains all relevant	**/
/** functionality relating to 		**/
/** Texture asset management.		**/
/*************************************/

#include <stdio.h>
#include <string.h>

#include "Game.h"

/*
	Texture Init

	Purpose: Call after creating a texture asset,
			to correctly set it's properties.

	Parameters: Tex - a pointer to the texture
					asset.
				Name - the name this texture
					should be identified by
					when searching for it.

	Return:	True if the texture asset was set up
			correctly, otherwise false <Boolean>
*/

bool 			TextureInit( AssetTexture* tex, const char* name )
{
	// Set Texture Name
	if (tex->name != nullptr)
	{
		delete [] tex->name;
		tex->name = nullptr;
	}

	int name_length = strlen(name)+1;
    tex->name = new char[name_length];
    tex->name = strcpy( tex->name, name );

	// Set Texture Default Size
	tex->width = 0;
	tex->height = 0;

	// Print Success Message
	printf( "Texture Initialised: %s\n", tex->name );

	// Return Success
	return true;
}

/*
	Texture Free

	Purpose: Frees all texture data and returns
			ALL values to the default state.
			(The name is also reset.)

	Parameters: Tex - a pointer to the texture
					asset.

	Return:	nil
*/

void			TextureFree( AssetTexture* tex )
{
	// Clear Name
	if ( tex->name != nullptr )
	{
		delete [] tex->name;
		tex->name = nullptr;
	}

	// Clear Texture
	if ( tex->texture != nullptr )
	{
		SDL_DestroyTexture( tex->texture );
		tex->texture = nullptr;
	}

	// Reset Dimensions
	tex->width = 0;
	tex->height = 0;
}

/*
	Texture Load File

	Purpose: Loads a texture from a passed in
			image file.

	Parameters: Tex - a pointer to the texture
					asset
				Render - a pointer to the render device
					to draw the texture to
				Name - the name of the image file
					to load from

	Return:	True if the texture was successfully
			loaded, otherwise false <Boolean>
*/

bool			TextureLoadFile( AssetTexture* tex, SDL_Renderer* render, const char* filename )
{
	// Clear Texture
	if ( tex->texture != nullptr )
	{
		SDL_DestroyTexture( tex->texture );
		tex->texture = nullptr;
	}

	// Reset Dimensions
	tex->width = 0;
	tex->height = 0;

	// Create a temp spot for the new texture
	SDL_Texture* temp = nullptr;

	// Load the image from the file
	SDL_Surface* surf = IMG_Load( filename );

	// Check for Image Loading Errors
	if ( !surf )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", filename, IMG_GetError() );
		return false;
	}

	// If successful, create new texture from loaded image
	else
	{
		// Color Image
		SDL_SetColorKey( surf, SDL_TRUE, SDL_MapRGB( surf->format, 0, 0xFF, 0xFF ) );

		// Set Texture from Surface
		temp = SDL_CreateTextureFromSurface( render, surf );
		if ( !temp )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", filename, SDL_GetError() );
			SDL_FreeSurface(surf);
			return false;
		}
		else
		{
			tex->texture = temp;
			tex->width = surf->w;
			tex->height = surf->h;
		}

		// We can now get rid of our surface
		SDL_FreeSurface(surf);
	}

	// Print Success Message
	printf( "Texture successfully loaded from file: %s\n", filename );

	// Return Success
	return true;
}

/*
	Texture Load From Text

	Purpose: Creates an image from a font string

	Parameters: Tex - a pointer to the texture
					asset
				Render - a pointer to the render device
					to draw the texture to
				Text - the text to be drawn
				Font - the font to use when rendering
				Text Color - the color of the text

	Return:	True if the texture was successfully
			loaded, otherwise false <Boolean>
*/

bool			TextureLoadFromText( AssetTexture* tex, SDL_Renderer* render, const char* text, TTF_Font* font, SDL_Color text_color )
{
	// Free any pre-existing texture
	if ( tex->texture != nullptr )
	{
		SDL_DestroyTexture( tex->texture );
		tex->texture = nullptr;
	}
	tex->width = 0;
	tex->height = 0;

	// Create the new texture
	SDL_Surface* loaded_surface = TTF_RenderText_Solid( font, text, text_color );
	if ( loaded_surface == 0 )
	{
		printf("Error: Unable to load surface from Text: %s\n", TTF_GetError() );
		return false;
	}

	// Create Texture from Loaded Surface
	tex->texture = SDL_CreateTextureFromSurface( render, loaded_surface );
	if (tex->texture == 0)
	{
		printf( "Error: Unable to create text texture from surface: %s\n", SDL_GetError() );
		SDL_FreeSurface(loaded_surface);
		return false;
	}

	// Set new width and height
	tex->width = loaded_surface->w;
	tex->height = loaded_surface->h;

	// Free loaded surface, we don't need it anymore
	SDL_FreeSurface(loaded_surface);

	// Print Success Message
	printf( "Texture successfully loaded from text: %s\n", text );

	// Return Success
	return true;
}

/*
	Texture Set Color

	Purpose: Changes the color to blend into
			the texture

	Parameters: Tex - a pointer to the texture
					asset.
				R - value of the red channel of the
					desired color.
				G - value of the green channel of the
					desired color.
				B - value of the blue channel of the
					desired color.

	Return:	Nil
*/

void			TextureSetColor( AssetTexture* tex, Uint8 r, Uint8 g, Uint8 b )
{
	// Set the Blend Color
	SDL_SetTextureColorMod( tex->texture, r, g, b );
}

/*
	Texture Set Alpha

	Purpose: Set the alpha (opacity) of the texture
			when rendering.

	Parameters: Tex - a pointer to the texture
					asset.
				Alpha - the value of the desired alpha
					channel

	Return:	Nil
*/

void 			TextureSetAlpha( AssetTexture* tex, Uint8 alpha )
{
	// Set the Alpha Channel of the Texture
	SDL_SetTextureAlphaMod( tex->texture, alpha );
}

/*
	Texture Set Blend

	Purpose: Set the blend mode used when rendering
			the texture.

	Parameters: Tex - a pointer to the texture
					asset.
				Blending - the blend mode to use

	Return:	Nil
*/

void 			TextureSetBlend( AssetTexture* tex, SDL_BlendMode blending )
{
	// Set the Blend Mode of the Texture
	SDL_SetTextureBlendMode( tex->texture, blending );
}

/*
	Texture Render

	Purpose: Used to render the specific part of the
			texture to the screen

	Parameters: Tex - a pointer to the texture
					asset.
				Render - a pointer to the render device
					to draw the texture to
				X - the x position to draw the texture
					on screen.
				Y - the y position to draw the texture
					on screen
				Clip - a rectangle used to determine which
					section of the rectangle to render.
					Can be left as nullptr to render all

	Return:	Nil
*/

void 			TextureRender( AssetTexture* tex, SDL_Renderer* render, int x, int y, SDL_Rect* clip )
{
	// Set the Section to Render
	SDL_Rect render_quad = { x, y, tex->width, tex->height };

	if (clip != nullptr)
	{
		render_quad.w = clip->w;
		render_quad.h = clip->h;
	}

	// Render to the Screen
	SDL_RenderCopy( render, tex->texture, clip, &render_quad );
}
