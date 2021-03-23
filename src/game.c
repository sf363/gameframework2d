#include <SDL.h>

#include <math.h>

#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"

#include "camera.h"
#include "entity.h"
#include "player.h"
#include "level.h"
#include "font.h"
#include "collision.h"

int main(int argc, char * argv[])
{
	/*variable declarations*/
	int done = 0;
	const Uint8 * keys;
	Level *level;
	Font *font;
	TextLine fps_text;
	Level *level2;

	int mx, my;
	float mf = 0;
	Sprite *mouse;
	Entity *player;
	Entity *mage;
	Entity *tank;
	Entity *ranger;
	Entity *monster;
	Entity *monster2;
	Entity *imp;
	Entity *zombie;
	Entity *zombie2;
	Entity *skel;
	Entity *skel2;
	Entity *soul;
	Entity *soul2;
	Entity *NPC;
	Vector4D mouseColor = { 255, 100, 255, 200 };
	/*program initializtion*/
	init_logger("gf2d.log");
	slog("---==== BEGIN ====---");
	gf2d_graphics_initialize(
		"gf2d",
		1200,
		720,
		1200,
		720,
		vector4d(0, 0, 0, 255),
		0);
	gf2d_graphics_set_frame_delay(16);
	camera_set_dimensions(vector2d(1200, 720));
	camera_set_position(vector2d(0, 0));
	gf2d_sprite_init(1024);
	font_init(10);
	entity_manager_init(100);


	SDL_ShowCursor(SDL_DISABLE);
	

	/*demo setup*/
	mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16);
	level = level_load("levels/Town.json");
	level2 = level_load("levels/Town.json");
	//spawns player
	player = player_spawn(vector2d(500, 100));
	mage= mage_spawn(vector2d(100, 300));
	tank = tank_spawn(vector2d(100, 100));
	ranger= ranger_spawn(vector2d(100, 200));
	font = font_load("fonts/colony_wars.ttf", 16);
	//spawns monsters
	monster= monster_spawn(vector2d(1200, 600));
	monster2 = monster_spawn(vector2d(1400, 700));
	imp= imp_spawn(vector2d(700, 500));
	zombie = zombie_spawn(vector2d(1200,100));
	zombie2 = zombie_spawn(vector2d(1500,600));
	skel = skeleton_spawn(vector2d(600, 800));
	skel2 = skeleton_spawn(vector2d(950, 400));
	soul = soul_reaper_spawn(vector2d(800, 200));
	soul2 = soul_reaper_spawn(vector2d(1600,400));//spawns the monster
	door_spawn(vector2d(400,800));
	NPC = NPC_spawn(vector2d(200, 200));
	
	if (player->target == NULL)
	{
		player->target = monster, imp, monster2, zombie, zombie2, skel, skel2, soul, soul2, NPC;
		mage->target = player;
		tank->target = mage;
		ranger->target = tank;
		monster->target = player;
		monster2->target = player;
		imp->target = player;
		zombie->target = player;
		zombie2->target = player;
		skel->target = player;
		skel2->target = player;
		soul->target = player;
		soul2->target = player;
		NPC->target = player;
	}
	if (player->target != NULL)
	{
		player->target = NULL;
		mage->target = NULL;
		tank->target = NULL;
		ranger->target = NULL;
		monster->target = NULL;
		monster2->target = NULL;
		imp->target = NULL;
		zombie->target = NULL;
		zombie2->target = NULL;
		skel->target = NULL;
		skel2->target = NULL;
		soul->target = NULL;
		soul2->target = NULL;
		NPC->target = NULL;

	}

	/*main game loop*/
	while (!done)
	{
		SDL_PumpEvents();   // update SDL's internal event structures
		keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
		/*update things here*/
		SDL_GetMouseState(&mx, &my);
		mf += 0.1;
		if (mf >= 16.0)mf = 0;

		entity_manager_update_entities();
		entity_manager_think_entities();

		level_update(level);

		gf2d_graphics_clear_screen();// clears drawing buffers
		// all drawing should happen betweem clear_screen and next_frame
		//backgrounds drawn first
		level_draw(level);
		/*if(player(vector2d(1100, 300)))
		{
			level_update(level2);
			gf2d_graphics_clear_screen();
			level_draw(level2);

		}*/
		entity_manager_draw_entities();

		//UI elements last
		gf2d_sprite_draw(
			mouse,
			vector2d(mx, my),
			NULL,
			NULL,
			NULL,
			NULL,
			&mouseColor,
			(int)mf);
		gf2d_grahics_next_frame();
		font_render(font, fps_text, vector2d(32, 32), gfc_color8(255, 0, 255, 255));
		// render current draw frame and skip to the next frame
		/*
		if (keys[SDL_SCANCODE_RIGHT])
		{
			camera_move(vector2d(10,0));
		}
		if (keys[SDL_SCANCODE_LEFT])
		{
			camera_move(vector2d(-10, 0));
		}
		if (keys[SDL_SCANCODE_UP])
		{
			camera_move(vector2d(0, -10));
		}
		if (keys[SDL_SCANCODE_DOWN])
		{
			camera_move(vector2d(0, 10));
		}
		if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
		//        slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());*/
		gf2d_grahics_next_frame();
		if (keys[SDL_SCANCODE_ESCAPE])done = 1;
		gfc_line_sprintf(fps_text, "Player Health: %f ");
		//gfc_line_sprintf(fps_text,"Rendering at %f FPS", gf2d_graphics_get_frames_per_second());
		font_render(font,fps_text,vector2d(32,32),gfc_color8(255,0,0,255));
		
	}
	slog("---==== END ====---");
	return 0;
}