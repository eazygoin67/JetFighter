#ifndef GAME_H
#define GAME_H

//////////////////////////////////////////////////////
// Inlcudes											//
//////////////////////////////////////////////////////

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Vector2D.h"

//////////////////////////////////////////////////////
// Defines											//
//////////////////////////////////////////////////////

#define FPS 60
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define TEXTURE_COUNT 2

#define POOLSIZE_ENEMY 20
#define POOLSIZE_PARTICLE 20
#define POOLSIZE_FRIENDLY_PROJ 10
#define POOLSIZE_ENEMY_PROJ 40

#define ENEMY_SPAWN_CHANCE 60

#define CONTROLLER_COUNT 1

//////////////////////////////////////////////////////
// Forward Declares									//
//////////////////////////////////////////////////////

struct AssetTexture;
struct Animation;

struct Game;
struct Player;
struct Enemy;
struct Projectile;
struct Particle;

//////////////////////////////////////////////////////
// Enumerations										//
//////////////////////////////////////////////////////

enum EnemyType : Uint16
{
	ENEMY_SIMPLE = 0,
	ENEMY_STRAIGHTSHOOT,
	ENEMY_STRAFESHOOT,
	// Add more as needed

	enemy_type_count
};

enum PartType : Uint16
{
	PART_EXPLOSION_01 = 0,
	// Add more as needed

	part_type_count
};

//////////////////////////////////////////////////////
// Structures										//
//////////////////////////////////////////////////////

/*** Point ***/

struct Point
{
	Sint32 					x = 0;
	Sint32					y = 0;
};

/*** Asset Texture ***/

struct AssetTexture
{
	SDL_Texture*			texture = nullptr;

	char*					name = nullptr;
	Uint16 					width = 0;
	Uint16					height = 0;
};

/*** Animation ***/

struct Animation
{
	float 					image_index = 0;
	float 					image_speed = 0;
	Uint16					image_count = 1;

	bool					loops = true;

	Uint16 					y_offset = 0; // How far down the sprite sheet we sit

	SDL_Rect				rect = { 0, 0, 32, 32 };
};

/*** Player ***/

struct Player
{
	Game*					runner = nullptr;

    bool 					alive = true;
    Sint16 					hp = 100;
    Sint16					hp_max = 100;

    float 					move_speed = 0.5;
    float 					move_fric = 0.9;
    float 					move_dead = 0.1;
    float 					move_max = 4;

    Uint32 					next_shot = 0;

    Vector2D 				pos;
	Vector2D				vel;

	SDL_Rect				collision = { 0, 0, 24, 24 };
	Point					col_offset;

	Animation				ship_ani;
    AssetTexture*			sprite_texture = nullptr;
};

/*** Enemy ***/

union EnemyData
{
	struct {
		Uint32 	cooldown;
	} straight_data;
};

struct Enemy
{
	Game* 					runner = nullptr;
	EnemyType				type = ENEMY_SIMPLE;

	Vector2D				pos;
	Vector2D				vel;

	bool					alive = true;
	Sint16					hp = 10;
	Sint16					hp_max = 10;
	Uint32					score_value = 10;

	SDL_Rect				collision = { 0, 0, 24, 24 };
	Point					col_offset;

	Animation				ship_ani;
	AssetTexture*			sprite_texture;

	EnemyData				data;

	bool					(*update)(Enemy*, Uint32);
};

/*** Particle ***/

struct Particle
{
	Game*					runner = nullptr;

	Vector2D				pos;

	bool 					alive = false;

	Animation				ani;
	AssetTexture*			texture = nullptr;
};

/*** Projectile ***/

struct Projectile
{
	Game*					runner = nullptr;

	bool 					alive = false;
	Uint8					damage = 10;
	bool					friendly = false;

	Vector2D				pos;
	Vector2D				vel;

	SDL_Rect 				collision = { 0,0, 5,32 };
	Point 					col_offset;

	Animation				ani;
	AssetTexture*			texture;
};

/*** Game ***/

struct Game
{
	Player					player;

	Enemy					enemies[POOLSIZE_ENEMY];
	Uint16					enemy_count = 0;

	Particle				particles[POOLSIZE_PARTICLE];
	Uint16					part_count = 0;

	Projectile				projectiles_friendly[POOLSIZE_FRIENDLY_PROJ];
	Projectile				projectiles_enemy[POOLSIZE_ENEMY_PROJ];
	Uint16					proj_friendly_count = 0;
	Uint16					proj_enemy_count = 0;

	bool 					running = false;
	bool					playing = false;
	SDL_Window*				window = nullptr;
	SDL_Renderer*			render = nullptr;
	SDL_Event				event;

	bool					needs_reset = false;
	Uint32					next_reset = 0;

	SDL_Color				screen_color;

	TTF_Font*				game_font;
	AssetTexture			game_start_text;

	AssetTexture			texture_assets[TEXTURE_COUNT];

	AssetTexture*			background_texture = nullptr;
	Uint16					background_y = 0;
	Uint16 					background_yspeed = 4;

	Uint32					score = 0xFFFFFFFF;
	Uint32					high_score = 0xFFFFFFFF;
	char					score_text[8] = "0";
	char					highscore_text[8] = "0";
	AssetTexture			score_text_font;
	AssetTexture			highscore_text_font;

	bool					key_up = false;
	bool					key_down = false;
	bool					key_left = false;
	bool					key_right = false;
	bool					key_shoot = false;

	const Uint8*			key_states = nullptr;
};

//////////////////////////////////////////////////////
// Functions										//
//////////////////////////////////////////////////////

/*** Texture Functions ***/

bool 			TextureInit( AssetTexture* tex, const char* name );
void			TextureFree( AssetTexture* tex );
bool			TextureLoadFile( AssetTexture* tex, SDL_Renderer* render, const char* filename );
bool			TextureLoadFromText( AssetTexture* tex, SDL_Renderer* render, const char* text, TTF_Font* font, SDL_Color text_color );

void			TextureSetColor( AssetTexture* tex, Uint8 r, Uint8 g, Uint8 b );
void 			TextureSetAlpha( AssetTexture* tex, Uint8 alpha );
void 			TextureSetBlend( AssetTexture* tex, SDL_BlendMode blending );

void 			TextureRender( AssetTexture* tex, SDL_Renderer* render, int x, int y, SDL_Rect* clip = nullptr );

/*** Animation Functions ***/

void 			AnimationUpdate( Animation* ani );

/*** Game Functions ***/

bool 			GameInit( Game* game );
void 			GameSetup( Game* game );
void 			GameReset( Game* game );
void 			GameStartPlay( Game* game );
void 			GameQuit( Game* game );

bool 			GameLoadAssets( Game* game );
void 			GameFreeAssets( Game* game );

AssetTexture*	GameFindTexture( Game* game, const char* name );

void 			GameBackgroundUpdate( Game* game, Uint32 dt );
void 			GameBackgroundRender( Game* game );

void 			GameSpawnRandomEnemy( Game* game );
void 			GameKillEnemy( Game* game, Enemy* enemy );

void 			GameSpawnParticle( Game* game, const Vector2D& pos, PartType type );
void 			GameKillParticle( Game* game, Particle* part );

Projectile*		GameSpawnProjectile( Game* game, bool friendly, const Vector2D& pos, const Vector2D& vel );
void 			GameKillProjectile( Game* game, Projectile* proj );

void 			GameScoreSet( Game* game, Uint32 value );
void 			GameScoreAdd( Game* game, Uint32 value );
void 			GameHighScoreSet( Game* game, Uint32 value );

void 			GameRenderHud( Game* game );

void 			GameLoop( Game* game );
void 			GameInput( Game* game );
void 			GameUpdate( Game* game, Uint32 dt );
void 			GameRender( Game* game );

/*** Player Functions ***/

void 			PlayerSetup( Player* player );

void			PlayerUpdate( Player* player, Uint32 dt );
void 			PlayerRender( Player* player );

/*** Enemy Functionality ***/

void 			EnemyInit( Enemy* enemy );
bool 			EnemySetup( Enemy* enemy, EnemyType type, bool spawn_random = true );
void 			EnemySwap( Enemy* e1, Enemy* e2 );
void 			EnemyRender( Enemy* enemy );

// Update Functions of Several Different Enemy Types
bool 			EnemyUpdateSimple( Enemy* enemy, Uint32 dt );
bool			EnemyUpdateStraightShoot( Enemy* enemy, Uint32 dt );
bool 			EnemyUpdateStrafeShoot( Enemy* enemy, Uint32 dt );

/*** Particle Functions ***/

void 			ParticleInit( Particle* part );
bool 			ParticleSpawn( Particle* part, const Vector2D& pos, PartType type );
void 			ParticleSwap( Particle* p1, Particle* p2 );
bool 			ParticleUpdate( Particle* part, Uint32 dt );
void 			ParticleRender( Particle* part );

/*** Projectile Functions ***/

void			ProjectileInit( Projectile* proj );
bool 			ProjectileSpawn( Projectile* proj, const Vector2D& pos, const Vector2D& vel );
void 			ProjectileSwap( Projectile* p1, Projectile* p2 );
bool 			ProjectileUpdate( Projectile* proj, Uint32 dt );
void 			ProjectileRender( Projectile* proj );

/*** Utility Functions ***/

bool			UtilityAabbCheck( SDL_Rect* r1, SDL_Rect* r2 );

#endif // GAME_H
