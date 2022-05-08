#include "stdafx.h"
#include "Editor.h"
#include <xors3d.h>
#include <fstream>

bool player_exists = false;
int editor_x = 0;
int editor_y = 0;
int img_background;

tile Map[16][12]; // Tworzymy siatke

void set_tile(int x, int y, int id)
{
	printf("setting %d %d\n", x, y);
	Map[x][y].ID = id;
}

void LoadSprites()
{
		Sprites[BLANK] = xLoadImage("Data\\Gfx\\Transparent.png");
		Sprites[FLOOR_SPR] = xLoadImage("Data\\Gfx\\Floor.png");
		Sprites[WALL_SPR] = xLoadImage("Data\\Gfx\\Wall.png");
		Sprites[PLAYER_SPR] = xLoadImage("Data\\Gfx\\Player.png");
		Sprites[BOX1_SPR] = xLoadImage("Data\\Gfx\\Box1.png");
		Sprites[BOX2_SPR] = xLoadImage("Data\\Gfx\\Box2.png");
		Sprites[TARGET_SPR] = xLoadImage("Data\\Gfx\\Target.png");

		Sprites[TILE_EDITOR_OVERLAY] = xLoadImage("Data\\Gfx\\Overlay.png");
		xImageColor(Sprites[TILE_EDITOR_OVERLAY], 255, 0, 0);
		xImageAlpha(Sprites[TILE_EDITOR_OVERLAY], 0.4f);

		img_background = xLoadImage("Data\\Gfx\\Background.png"); // Ladowanie tla
}

void UpdateMap()
{
	int x_pos = 0;
    int y_pos = 0;
    
    for (int y=0; y<12; y++)
    {
        for (int x=0; x<16; x++)
        {
            int SpriteID = Map[x][y].ID;

			xDrawImage(Sprites[SpriteID], x_pos, y_pos);

            x_pos += 64;
        }

		y_pos += 64;
        x_pos = 0;        
    }

	Process_Editor();
}

void Process_Editor()
{	
	xDrawImage(Sprites[TILE_EDITOR_OVERLAY], editor_x * 64.0, editor_y * 64);
	
	// PORUSZANIE SIE PO EDYTORZE
    if (xKeyHit(xKEY_UP) || xKeyHit(xKEY_W) && editor_y > 0)
        editor_y--;

    if (xKeyHit(xKEY_DOWN) || xKeyHit(xKEY_S) && editor_y < 11)
        editor_y++;

    if (xKeyHit(xKEY_LEFT) || xKeyHit(xKEY_A) && editor_x > 0)
        editor_x--;

    if (xKeyHit(xKEY_RIGHT) || xKeyHit(xKEY_D) && editor_x < 15)
        editor_x++;

	// WYBOR OBRAZKOW

	if (xKeyHit(xKEY_1))
	{
		if(Map[editor_x][editor_y].ID == 3)
			player_exists = false;
        set_tile(editor_x, editor_y, BLANK);
	}

	if (xKeyHit(xKEY_2))
	{
		if(Map[editor_x][editor_y].ID == 3)
			player_exists = false;
        set_tile(editor_x, editor_y, FLOOR_SPR);
	}

    if (xKeyHit(xKEY_3))
	{
		if(Map[editor_x][editor_y].ID == 3)
			player_exists = false;
        set_tile(editor_x, editor_y, WALL_SPR);
	}

	if (xKeyHit(xKEY_4) && !player_exists)
	{
		player_exists = true;
        set_tile(editor_x, editor_y, PLAYER_SPR);
	}

	if (xKeyHit(xKEY_5))
	{
		if(Map[editor_x][editor_y].ID == 3)
			player_exists = false;
        set_tile(editor_x, editor_y, BOX1_SPR);
	}

	if (xKeyHit(xKEY_6))
	{
		if(Map[editor_x][editor_y].ID == 3)
			player_exists = false;
        set_tile(editor_x, editor_y, BOX2_SPR);
	}

	if (xKeyHit(xKEY_7))
	{
		if(Map[editor_x][editor_y].ID == 3)
			player_exists = false;
        set_tile(editor_x, editor_y, TARGET_SPR);
	}

	// LADOWANIE I ZAPIS MAPY

	if (xKeyHit(xKEY_F5))
		save_map_to_file();

	if (xKeyHit(xKEY_F9))
		load_map_from_file();

	// CZYSZCZENIE MAPY

	if (xKeyHit(xKEY_C))
		clear_map();

}

void save_map_to_file()
{
	std::ofstream oFile("Data\\Levels\\level.map",std::ofstream::binary);
	oFile.write((char*)Map, sizeof(Map));
	oFile.close();
}


void load_map_from_file()
{
	std::ifstream pFile("Data\\Levels\\level.map",std::ifstream::binary);
	pFile.read((char*)Map, sizeof(Map));
	pFile.close();
}

void clear_map()
{
	for(int i=0; i<16; i++)
	{
		for(int j=0; j<12; j++)
		{
			Map[i][j].ID = BLANK;
		}
	}
}


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
        char msgBuffer[129];
		xSetEngineSetting("Splash::TilingTime", "0"); // Wylacz animacje intra
		xSetEngineSetting("Splash::AfterTilingTime", "0"); // Wylacz intro
        xGraphics3D(1024, 768, 32, false, true);
		xAppTitle("Somblocks Editor");

		Handle font = xLoadFont("Arial", 18);	// Laduj czcionke
		xSetFont(font); // Ustaw czcionke											

        int camera = xCreateCamera();

		LoadSprites();
		
        while(!(xKeyHit(KEY_ESCAPE) || xWinMessage("WM_CLOSE")))
        {
                xRenderWorld();
				
				xDrawImage(img_background, 0, 0);
				UpdateMap();

                sprintf(msgBuffer, "SOMBLOCKS EDITOR");
                xText(10, 10, msgBuffer);
				sprintf(msgBuffer, "Adam Kozinski © 2022");
                xText(10, 40, msgBuffer);
				sprintf(msgBuffer, "'1-7' - Sprites, 'C' - Clear Map, 'F5' - Save Map, 'F9' - Load Map");
                xText(320, 10, msgBuffer);

                xFlip();
        }

        return 0;
}

