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

	TILE_EDITOR_OVERLAY,
	MAX_SPRITES
};

struct tile
{
	int ID;
};

extern int Sprites[MAX_SPRITES] = {}; // Deklaracja tablicy na obrazki
extern tile Map[16][12];

extern int editor_x, editor_y;
extern int img_background;

void LoadSprites();
void set_tile(int x, int y, int tile_id);
void Process_Editor();
void save_map_to_file();
void load_map_from_file();
void clear_map();
