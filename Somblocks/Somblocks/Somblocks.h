#pragma once

#include "resource.h"

enum eSprites
{
	BLANK,
	FLOOR_SPR,
	WALL_SPR,
	PLAYER_SPR,
	BOX1_SPR,
	BOX2_SPR,
	TARGET_SPR,

	MAX_SPRITES
};

enum eSounds
{
	WALK_SND,
	BOX_SND,
	APLAUZ_SND,
	LAUGH_SND,

	MAX_SOUNDS
};

struct tile
{
	int ID;
};

extern int Sprites[MAX_SPRITES] = {}; // Deklaracja tablicy na obrazki
extern int Sounds[MAX_SOUNDS] = {};
extern tile Map[16][12];
extern int player_x, player_y;
extern int blocks, targets;
extern int current_level;
extern bool is_finished;
extern int img_background;

void LoadSprites();
void set_tile(int x, int y, int tile_id);
void Process_Controls();
void load_map_from_file();
void load_best_score();
void load_game();
void save_game();
void find_elements();
void win_check();