#include "GameManager.hpp"

SDL_Renderer* renderer;

SDL_Texture* pg_fgtex;
SDL_Texture* pg_bgtex;
SDL_Texture* player_tex;
SDL_Texture* hands_player_tex[4];

bool fishOnHook = false;
bool isThrowingLine = false;
bool lineThrown = false;
float timeThrowingLine = 0.0;
float lineThrownAdd = 0.0;
float fishReelAmount = 0.0;
string fishList[128]; //not sure why this is limited to 128?
string caughtFish[128];
double caughtFishPrice[128];
double money = 0;

float delta = 0.033f;

Uint16 tick = 0;
