#pragma once
#include <SDL3\SDL.h>
#include <SDL3_Image\SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include "string.h"
#include <set>
#include <fstream>
#include <thread>
#include <chrono>
#include <vector>
#include <sstream>

using namespace std;

//My Classes
#include "Utils.hpp"
#include "Player.hpp"



using namespace Utils;

extern SDL_Renderer* renderer;

extern SDL_Texture* pg_fgtex;
extern SDL_Texture* pg_bgtex;
extern SDL_Texture* player_tex;
extern SDL_Texture* hands_player_tex[4];

extern bool fishOnHook;
extern bool lineThrown;
extern bool isThrowingLine;
extern float timeThrowingLine;
extern float lineThrownAdd;
extern float fishReelAmount;

extern string fishList[128];
extern string caughtFish[128];
extern double caughtFishPrice[128];
extern double money;

extern float delta;
extern Uint16 tick;

class Audio {
public:
	static void playAudio();
};
