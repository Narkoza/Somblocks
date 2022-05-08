#include "stdafx.h"
#include "Somblocks.h"
#include <xors3d.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>

int img_background;

bool player_on_target = false;
bool is_finished = false;
int player_x = 0;
int player_y = 0;
int targets = 0;
int blocks = 0;
int current_level = 1;
int best = 0;
clock_t now = 0;
std::string best_sec = "00";
std::string best_min = "00";
std::string best_mili = "0000";

tile Map[16][12]; // Tworzymy siatke

std::string int_to_string(int x)
{
	std::ostringstream temp;
	temp << x;

	return temp.str();
}

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

		img_background = xLoadImage("Data\\Gfx\\Background.png"); // Ladowanie tla
}

void LoadSounds()
{
		Sounds[WALK_SND] = xLoadSound("Data\\Sounds\\walk.wav");
		Sounds[BOX_SND] = xLoadSound("Data\\Sounds\\box.wav");
		Sounds[APLAUZ_SND] = xLoadSound("Data\\Sounds\\aplauz.wav");
		Sounds[LAUGH_SND] = xLoadSound("Data\\Sounds\\laugh.wav");

		xSoundVolume(Sounds[WALK_SND], 6.0f);
		xSoundVolume(Sounds[BOX_SND], 6.0f);
		xSoundVolume(Sounds[APLAUZ_SND], 0.6f);
		xSoundVolume(Sounds[LAUGH_SND], 0.6f);
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
	
	Process_Controls();
}

void Process_Controls()
{
	if(is_finished)
	{
		if(xKeyHit(xKEY_RETURN))
			exit(0);
		return;
	}
///////////////////////////////// WCZYTYWANIE GRY /////////////////////////////////////////////

	if (xKeyHit(xKEY_F9))
	{
		load_game();
	}

//////////////////////////////////// ZAPIS GRY ////////////////////////////////////////////////

	if (xKeyHit(xKEY_F5))
	{
		save_game();
	}

///////////////////////////////// RESTART POZIOMU /////////////////////////////////////////////

	if (xKeyHit(xKEY_R))
	{
		player_x = 0;
		player_y = 0;
		targets = 0;
		blocks = 0;
		now = 0;
		player_on_target = false;
		xPlaySound(Sounds[LAUGH_SND]);
		load_map_from_file();
	}


///////////////////////////////// STRZALKA W GORE ////////////////////////////////////////////

	if (xKeyHit(xKEY_UP) || xKeyHit(xKEY_W))
	{
		if(Map[player_x][player_y-1].ID == TARGET_SPR)
		{
			xPlaySound(Sounds[WALK_SND]);
			if(player_on_target)
				set_tile(player_x, player_y, TARGET_SPR);
			else
				set_tile(player_x, player_y, FLOOR_SPR);

			player_on_target = true;
		}
		else if(Map[player_x][player_y-1].ID == WALL_SPR)
		{
			return;
		}
		else if(Map[player_x][player_y-1].ID == BOX1_SPR)
		{
			if(Map[player_x][player_y-2].ID == BOX1_SPR || Map[player_x][player_y-2].ID == BOX2_SPR || Map[player_x][player_y-2].ID == WALL_SPR)
			{
				return;
			}
			if(Map[player_x][player_y-2].ID == TARGET_SPR)
			{
				xPlaySound(Sounds[BOX_SND]);
				set_tile(player_x, player_y-2, BOX2_SPR);
				blocks++;
			}
			else
			{
				xPlaySound(Sounds[BOX_SND]);
				set_tile(player_x, player_y-2, BOX1_SPR);
			}

			if(player_on_target)
				set_tile(player_x, player_y, TARGET_SPR);
			else
				set_tile(player_x, player_y, FLOOR_SPR);

			player_on_target = false;
		}
		else if(Map[player_x][player_y-1].ID == BOX2_SPR)
		{
			if(Map[player_x][player_y-2].ID == BOX1_SPR || Map[player_x][player_y-2].ID == BOX2_SPR || Map[player_x][player_y-2].ID == WALL_SPR)
			{
				return;
			}
			if(Map[player_x][player_y-2].ID == TARGET_SPR)
			{
				xPlaySound(Sounds[BOX_SND]);
				set_tile(player_x, player_y-2, BOX2_SPR);
			}
			else
			{
				xPlaySound(Sounds[BOX_SND]);
				set_tile(player_x, player_y-2, BOX1_SPR);
				blocks--;
			}

			if(player_on_target)
				set_tile(player_x, player_y, TARGET_SPR);
			else
				set_tile(player_x, player_y, FLOOR_SPR);

			player_on_target = true;
		}
		else if(Map[player_x][player_y-1].ID == FLOOR_SPR)
		{
			xPlaySound(Sounds[WALK_SND]);
			if(player_on_target)
				set_tile(player_x, player_y, TARGET_SPR);
			else
				set_tile(player_x, player_y, FLOOR_SPR);

			player_on_target = false;
		}

		player_y--;
	}

///////////////////////////////// STRZALKA W DOL ////////////////////////////////////////////
	if (xKeyHit(xKEY_DOWN) || xKeyHit(xKEY_S))
	{
		if(Map[player_x][player_y+1].ID == TARGET_SPR)
		{
			xPlaySound(Sounds[WALK_SND]);
			if(player_on_target)
				set_tile(player_x, player_y, TARGET_SPR);
			else
				set_tile(player_x, player_y, FLOOR_SPR);

			player_on_target = true;
		}
		else if(Map[player_x][player_y+1].ID == WALL_SPR)
		{
			return;
		}
		else if(Map[player_x][player_y+1].ID == BOX1_SPR)
		{
			if(Map[player_x][player_y+2].ID == BOX1_SPR || Map[player_x][player_y+2].ID == BOX2_SPR || Map[player_x][player_y+2].ID == WALL_SPR)
			{
				return;
			}
			if(Map[player_x][player_y+2].ID == TARGET_SPR)
			{
				xPlaySound(Sounds[BOX_SND]);
				set_tile(player_x, player_y+2, BOX2_SPR);
				blocks++;
			}
			else
			{
				xPlaySound(Sounds[BOX_SND]);
				set_tile(player_x, player_y+2, BOX1_SPR);
			}
			
			if(player_on_target)
				set_tile(player_x, player_y, TARGET_SPR);
			else
				set_tile(player_x, player_y, FLOOR_SPR);

			player_on_target = false;
		}
		else if(Map[player_x][player_y+1].ID == BOX2_SPR)
		{
			if(Map[player_x][player_y+2].ID == BOX1_SPR || Map[player_x][player_y+2].ID == BOX2_SPR || Map[player_x][player_y+2].ID == WALL_SPR)
			{
				return;
			}
			if(Map[player_x][player_y+2].ID == TARGET_SPR)
			{
				xPlaySound(Sounds[BOX_SND]);
				set_tile(player_x, player_y+2, BOX2_SPR);
			}
			else
			{
				xPlaySound(Sounds[BOX_SND]);
				set_tile(player_x, player_y+2, BOX1_SPR);
				blocks--;
			}

			if(player_on_target)
				set_tile(player_x, player_y, TARGET_SPR);
			else
				set_tile(player_x, player_y, FLOOR_SPR);

			player_on_target = true;
		}
		else if(Map[player_x][player_y+1].ID == FLOOR_SPR)
		{
			xPlaySound(Sounds[WALK_SND]);
			if(player_on_target)
				set_tile(player_x, player_y, TARGET_SPR);
			else
				set_tile(player_x, player_y, FLOOR_SPR);

			player_on_target = false;
		}

		player_y++;
	}

///////////////////////////////// STRZALKA W LEWO ////////////////////////////////////////////
	if (xKeyHit(xKEY_LEFT) || xKeyHit(xKEY_A))
	{
		if(Map[player_x-1][player_y].ID == TARGET_SPR)
		{
			xPlaySound(Sounds[WALK_SND]);
			if(player_on_target)
				set_tile(player_x, player_y, TARGET_SPR);
			else
				set_tile(player_x, player_y, FLOOR_SPR);

			player_on_target = true;
		}
		else if(Map[player_x-1][player_y].ID == WALL_SPR)
		{
			return;
		}
		else if(Map[player_x-1][player_y].ID == BOX1_SPR)
		{
			if(Map[player_x-2][player_y].ID == BOX1_SPR || Map[player_x-2][player_y].ID == BOX2_SPR || Map[player_x-2][player_y].ID == WALL_SPR)
			{
				return;
			}
			if(Map[player_x-2][player_y].ID == TARGET_SPR)
			{
				xPlaySound(Sounds[BOX_SND]);
				set_tile(player_x-2, player_y, BOX2_SPR);
				blocks++;
			}
			else
			{
				xPlaySound(Sounds[BOX_SND]);
				set_tile(player_x-2, player_y, BOX1_SPR);
			}

			if(player_on_target)
				set_tile(player_x, player_y, TARGET_SPR);
			else
				set_tile(player_x, player_y, FLOOR_SPR);

			player_on_target = false;
		}
		else if(Map[player_x-1][player_y].ID == BOX2_SPR)
		{
			if(Map[player_x-2][player_y].ID == BOX1_SPR || Map[player_x-2][player_y].ID == BOX2_SPR || Map[player_x-2][player_y].ID == WALL_SPR)
			{
				return;
			}
			if(Map[player_x-2][player_y].ID == TARGET_SPR)
			{
				xPlaySound(Sounds[BOX_SND]);
				set_tile(player_x-2, player_y, BOX2_SPR);
			}
			else
			{
				xPlaySound(Sounds[BOX_SND]);
				set_tile(player_x-2, player_y, BOX1_SPR);
				blocks--;
			}

			if(player_on_target)
				set_tile(player_x, player_y, TARGET_SPR);
			else
				set_tile(player_x, player_y, FLOOR_SPR);

			player_on_target = true;
		}
		else if(Map[player_x-1][player_y].ID == FLOOR_SPR)
		{
			xPlaySound(Sounds[WALK_SND]);
			if(player_on_target)
				set_tile(player_x, player_y, TARGET_SPR);
			else
				set_tile(player_x, player_y, FLOOR_SPR);

			player_on_target = false;
		}

		player_x--;
	}

///////////////////////////////// STRZALKA W PRAWO ////////////////////////////////////////////
	if (xKeyHit(xKEY_RIGHT) || xKeyHit(xKEY_D))
	{
		if(Map[player_x+1][player_y].ID == TARGET_SPR)
		{
			xPlaySound(Sounds[WALK_SND]);
			if(player_on_target)
				set_tile(player_x, player_y, TARGET_SPR);
			else
				set_tile(player_x, player_y, FLOOR_SPR);

			player_on_target = true;
		}
		else if(Map[player_x+1][player_y].ID == WALL_SPR)
		{
			return;
		}
		else if(Map[player_x+1][player_y].ID == BOX1_SPR)
		{
			if(Map[player_x+2][player_y].ID == BOX1_SPR || Map[player_x+2][player_y].ID == BOX2_SPR || Map[player_x+2][player_y].ID == WALL_SPR)
			{
				return;
			}
			if(Map[player_x+2][player_y].ID == TARGET_SPR)
			{
				xPlaySound(Sounds[BOX_SND]);
				set_tile(player_x+2, player_y, BOX2_SPR);
				blocks++;
			}
			else
			{
				xPlaySound(Sounds[BOX_SND]);
				set_tile(player_x+2, player_y, BOX1_SPR);
			}

			if(player_on_target)
				set_tile(player_x, player_y, TARGET_SPR);
			else
				set_tile(player_x, player_y, FLOOR_SPR);

			player_on_target = false;
		}
		else if(Map[player_x+1][player_y].ID == BOX2_SPR)
		{
			if(Map[player_x+2][player_y].ID == BOX1_SPR || Map[player_x+2][player_y].ID == BOX2_SPR || Map[player_x+2][player_y].ID == WALL_SPR)
			{
				return;
			}
			if(Map[player_x+2][player_y].ID == TARGET_SPR)
			{
				xPlaySound(Sounds[BOX_SND]);
				set_tile(player_x+2, player_y, BOX2_SPR);
			}
			else
			{
				xPlaySound(Sounds[BOX_SND]);
				set_tile(player_x+2, player_y, BOX1_SPR);
				blocks--;
			}

			if(player_on_target)
				set_tile(player_x, player_y, TARGET_SPR);
			else
				set_tile(player_x, player_y, FLOOR_SPR);

			player_on_target = true;
		}
		else if(Map[player_x+1][player_y].ID == FLOOR_SPR)
		{
			xPlaySound(Sounds[WALK_SND]);
			if(player_on_target)
				set_tile(player_x, player_y, TARGET_SPR);
			else
				set_tile(player_x, player_y, FLOOR_SPR);

			player_on_target = false;
		}

		player_x++;
	}
///////////////////////////////////////////////////////////////////////////////////////////////

	set_tile(player_x, player_y, PLAYER_SPR);

}

void load_map_from_file()
{
	const char* level = ("Data\\Levels\\level" + int_to_string(current_level) + ".map").c_str();
	std::ifstream pFile(level, std::ifstream::binary);

	if(pFile.is_open())
	{
		pFile.read((char*)Map, sizeof(Map));
		pFile.close();
		find_elements();

		now = clock();
		load_best_score();
	}
	else
	{
		pFile.close();
		Handle font2 = xLoadFont("Arial", 32);
		xSetFont(font2);
		std::ifstream pFile2("Data\\Levels\\end.map", std::ifstream::binary);
		if(pFile2.is_open())
		{
			pFile2.read((char*)Map, sizeof(Map));
			is_finished = true;
		}
		else
		{
			exit(0);
		}

		pFile2.close();
	}
}

void load_game()
{
	std::ifstream pFile("Data\\Saves\\Save.sav", std::ifstream::binary);

	if(pFile.is_open())
	{
		pFile.read(reinterpret_cast<char*>(&player_on_target), sizeof(bool));
		clock_t temp = 0;
		pFile.read(reinterpret_cast<char*>(&temp), sizeof(clock_t));
		now = clock()-temp;
		pFile.read(reinterpret_cast<char*>(&current_level), sizeof(int));

		pFile.read((char*)Map, sizeof(Map));
		find_elements();
		load_best_score();
	}

	pFile.close();
}

void save_game()
{
	std::ofstream pFile("Data\\Saves\\Save.sav", std::ofstream::binary);

	if(pFile.is_open())
	{
		pFile.write(reinterpret_cast<const char*>(&player_on_target), sizeof(bool));
		clock_t temp = clock()-now;
		pFile.write(reinterpret_cast<const char*>(&temp), sizeof(clock_t));

		pFile.write(reinterpret_cast<const char*>(&current_level), sizeof(int));
		pFile.write((char*)Map, sizeof(Map));
	}

	pFile.close();
}

void load_best_score()
{
	const char* level = ("Data\\Scores\\level" + int_to_string(current_level) + ".tim").c_str();
	std::ifstream pFile(level, std::ifstream::binary);

	if(pFile.is_open())
	{
		pFile.read(reinterpret_cast<char*>(&best), sizeof(int));

		int m = best/60000;
		int s = best/1000-m*60;
		int ms = best-s*1000;

		best_min = m<10 ? "0" + int_to_string(m) : int_to_string(m);
		best_sec = s<10 ? "0" + int_to_string(s) : int_to_string(s);
		best_mili = ms<10 ? "00" + int_to_string(ms) : ms<100 ? "0" + int_to_string(ms) : int_to_string(ms);
	}

	pFile.close();
}

void find_elements()
{
	player_x = 0;
	player_y = 0;
	targets = 0;
	blocks = 0;

	for(int i=0; i<16; i++)
	{
		for(int j=0; j<12; j++)
		{
			if(Map[i][j].ID == PLAYER_SPR)
			{
				player_x = i;
				player_y = j;
			}
			else if(Map[i][j].ID == TARGET_SPR)
			{
				targets++;
			}
			else if(Map[i][j].ID == BOX2_SPR)
			{
				targets++;
				blocks++;
			}
		}
	}
}

void win_check()
{	
	if(targets==blocks && !is_finished)
	{
		if(best>(clock() - now) / (double)(CLOCKS_PER_SEC / 1000) || best == 0)
		{
		
			const char* level = ("Data\\Scores\\level" + int_to_string(current_level) + ".tim").c_str();
			std::ofstream pFile(level, std::ofstream::binary);

			if(pFile.is_open())
			{
				int time = (clock() - now) / (double)(CLOCKS_PER_SEC / 1000);
				pFile.write(reinterpret_cast<const char*>(&time), sizeof(int));
			}

			pFile.close();

		}
		
		xPlaySound(Sounds[APLAUZ_SND]);
		current_level++;
		best = 0;
		best_min = "00";
		best_sec = "00";
		best_mili = "0000";
		load_map_from_file();
	}
}


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
        char msgBuffer[129];
		xSetEngineSetting("Splash::TilingTime", "0"); // Wylacz animacje intra
		xSetEngineSetting("Splash::AfterTilingTime", "0"); // Wylacz intro
        xGraphics3D(1024, 768, 32, false, true);
		xAppTitle("Somblocks");

		Handle font = xLoadFont("Arial", 20);	// Laduj czcionke
		xSetFont(font); // Ustaw czcionke											

        int camera = xCreateCamera();

		LoadSprites();
		LoadSounds();
		load_map_from_file();
		find_elements();
		
        while(!(xKeyHit(KEY_ESCAPE) || xWinMessage("WM_CLOSE")))
        {
                xRenderWorld();
				
				win_check();
				xDrawImage(img_background, 0, 0);
				UpdateMap();


				if(!is_finished)
				{
					int time = ((clock() - now) / (CLOCKS_PER_SEC / 1000));
					int m = time / 60000;
					time -= m*60000;
					int s = time / 1000;
					time -= s*1000;
					int ms = time;

					std::string min = m<10 ? "0" + int_to_string(m) : int_to_string(m);
					std::string sec = s<10 ? "0" + int_to_string(s) : int_to_string(s);
					std::string mili = ms<10 ? "00" + int_to_string(ms) : ms<100 ? "0" + int_to_string(ms) : int_to_string(ms);
					
					sprintf(msgBuffer, (min+":"+sec+":"+mili).c_str());
					xText(10, 10, msgBuffer);

					sprintf(msgBuffer, "LEVEL %d", current_level);
					xText(460, 10, msgBuffer);

					sprintf(msgBuffer, ("BEST TIME: "+best_min+":"+best_sec+":"+best_mili).c_str());
					xText(700, 10, msgBuffer);

					xFlip();
					continue;
				}
				
				sprintf(msgBuffer, "Congratulations!");
				xText(350, 80, msgBuffer);
				sprintf(msgBuffer, "You've completed all of the levels.");
				xText(180, 310, msgBuffer);
				sprintf(msgBuffer, "If you want to you can try to");
				xText(180, 350, msgBuffer);
				sprintf(msgBuffer, "build your own in our level editor!");
				xText(180, 390, msgBuffer);

				xFlip();

                
        }

        return 0;
}
