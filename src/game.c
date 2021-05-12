#include <SDL.h>

#include <math.h>

#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gfc_input.h"
#include "camera.h"
#include "entity.h"
#include "player.h"
#include "level.h"
#include "font.h"
#include "collision.h"
#include "item.h"
#include "gfc_audio.h"




int main(int argc, char * argv[])
{
	/*variable declarations*/
	int i;
	int fullscreen = 0;
	Sprite *background = NULL;
	Space *space = NULL;
	Collision collision;
	CollisionFilter filter = { 0 };


	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--fullscreen") == 0)
		{
			fullscreen = 1;
		}
	}
	int done = 0;
	int done2 = 1;
	int done3 = 1;
	const Uint8 * keys;
	Level *level;
	Font *font;
	TextLine fps_text;
	Level *level2;
	Mix_Music *music;
	int mx, my;
	float mf = 0;
	Sprite *mouse;
	Entity *player;
	Entity *mage;
	Entity *tank;
	Entity *ranger;
	Entity *monster;
	//Entity *monster2;
	Entity *imp;
	Entity *imp2;
	Entity *zombie;
	Entity *zombie2;
	Entity *skel;
	Entity *skel2;
	Entity *soul;
	Entity *soul2;
	//Entity *boss1;
	Entity *NPC;
	Entity *key;
	Entity *key2;
	Entity *key3;
	Entity *key4;
	Entity *key5;
	Sprite *sprite;
	Sprite *sprite2;

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
		fullscreen);
	gf2d_graphics_set_frame_delay(16);
	gf2d_audio_init(256, 16, 4, 1, 1, 1);
	camera_set_dimensions(vector2d(1200, 720));
	camera_set_position(vector2d(0, 0));
	gf2d_sprite_init(1024);
	font_init(10);
	gf2d_action_list_init(128);
	//gf2d_font_init("config/font.cfg");
	//gfc_input_init("config/input.cfg"); //F4
	entity_manager_init(100);

	SDL_ShowCursor(SDL_DISABLE);


	/*demo setup*/
	music = Mix_LoadMUS("music/ITZY.mp3");
	Mix_PlayMusic(music, -1);
	mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16);
	level = level_load("levels/Town.json");
	level2 = level_load("levels/Town.json");
	//spawns player
	player = player_spawn(vector2d(500, 100));
	mage = mage_spawn(vector2d(500, 120));
	tank = tank_spawn(vector2d(500, 130));
	ranger = ranger_spawn(vector2d(500, 140));
	font = font_load("fonts/colony_wars.ttf", 16);
	//spawns monsters
	monster = monster_spawn(vector2d(500, 800));
	//monster2 = monster_spawn(vector2d(1400, 800));
	soul = soul_reaper_spawn(vector2d(2000, 800));
	soul2 = soul_reaper_spawn(vector2d(3500, 800));//spawns the monster
	//imp = imp_spawn(vector2d(800, 800));

	imp2 = imp_spawn(vector2d(500, 100));
	zombie = zombie_spawn(vector2d(1000, 100));
	zombie2 = zombie_spawn(vector2d(2500, 100));
	skel = skeleton_spawn(vector2d(3500, 100));
	skel2 = skeleton_spawn(vector2d(1550, 100));


	door_spawn(vector2d(3800, 765));
	NPC = NPC_spawn(vector2d(100, 500));
	key = item_spawn(vector2d(500,500)); //place the keys 
	key2 = item_spawn(vector2d(1200, 376));
	key3 = item_spawn(vector2d(1200, 700));
	key4 = item_spawn(vector2d(700, 400));
	key5 = item_spawn(vector2d(2200, 50));
	//boss1 = boss_spawn(vector2d(3800, 500));
	if (player->target == NULL)
	{
		player->target = monster, zombie, zombie2, skel, skel2, soul, soul2, NPC;
		//	mage->target = player;
		//	tank->target = mage;
		//	ranger->target = tank;
		monster->target = player;
		zombie->target = player;
		zombie2->target = player;
		skel->target = player;
		skel2->target = player;
		soul->target = player;
		soul2->target = player;
		NPC->target = player;
		key->target = player;
	}
	/*if (player->keys >= 0)
	{
		boss = boss_spawn(vector2d(3800, 500));
	}*/
	sprite = gf2d_sprite_load_image("images/gamemenu.png");
	sprite2 = gf2d_sprite_load_image("images/lose.png");
	/*main game loop*/
	while (!done)
	{
		SDL_PumpEvents();   // update SDL's internal event structures
		keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
		/*update things here*/
		SDL_GetMouseState(&mx, &my);
		mf += 0.1;
		if (mf >= 16.0)mf = 0;
		gf2d_graphics_clear_screen();// clears drawing buffers
		// all drawing should happen betweem clear_screen and next_frame
		//backgrounds drawn first
		gf2d_sprite_draw_image(sprite, vector2d(0, 0));

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
		gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
		entity_manager_update_entities();
		entity_manager_think_entities();
		if (keys[SDL_SCANCODE_UP])
		{
			done = 1;
			done2 = 0;
		}
		if (keys[SDL_SCANCODE_DOWN])
		{
			done = 1;
			done2 = 1;
		}
	}
	while (!done2)
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
		if (player->health <= 0)
		{
			done2 = 1;
			done3 = 0;
		}
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
		gf2d_grahics_next_frame();
		gfc_line_sprintf(fps_text, "Player's level is %p ", player_level());

		if (keys[SDL_SCANCODE_ESCAPE])
		{
			done = 0;
			done2 = 1;
		}
		//gfc_line_sprintf(fps_text, "Player Health: %f ");
		//gfc_line_sprintf(fps_text,"Rendering at %f FPS", gf2d_graphics_get_frames_per_second());
		//font_render(font, fps_text, vector2d(32, 32), gfc_color8(255, 0, 0, 255));

	}
	while (!done3)
	{
		SDL_PumpEvents();   // update SDL's internal event structures
		keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
		/*update things here*/
		SDL_GetMouseState(&mx, &my);
		mf += 0.1;
		if (mf >= 16.0)mf = 0;
		gf2d_graphics_clear_screen();// clears drawing buffers
		// all drawing should happen betweem clear_screen and next_frame
		//backgrounds drawn first
		gf2d_sprite_draw_image(sprite2, vector2d(0, 0));

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
		gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
		entity_manager_update_entities();
		entity_manager_think_entities();
		if (keys[SDL_SCANCODE_ESCAPE])
		{
			done = 1;
			done2 = 1;
			done3 = 1;
		}
		

	}
	Mix_HaltMusic();
	Mix_FreeMusic(music);
	slog("---==== END ====---");
	return 0;
}

//Menus 5/6-5/7
//Dungeaon 5/8-5/9
//fixing things, visual, music 5/10
//finish everything lose ends


