#include "HUD.hpp"
#include "GameManager.hpp"

SDL_Color color = { 255,255,255,0 };
TTF_Font* font;

//tutorial text vars
unsigned long long tut_txtsize;
SDL_Surface* tut_surface;
SDL_Texture* tut_texture;
//

unsigned long long fish_txtsize;
SDL_Surface* fish_surface;
SDL_Texture* fish_texture;
SDL_FRect fish_rect;
int fish_DisplayTime = 0;

unsigned long long catch_txtsize;
SDL_Surface* catch_surface;
SDL_Texture* catch_texture;
SDL_FRect catch_rect;
bool displayCatchList = false;

unsigned long long money_txtsize;
SDL_Surface* money_surface;
SDL_Texture* money_texture;
SDL_FRect money_rect;
bool displayMoneyList = false;

//Rects
SDL_FRect progressbar_fg;
SDL_FRect progressbar_bg;
SDL_FRect tutRect;

void drawTutorialText() {
	if (!font) { return; }

	SDL_RenderTexture(renderer, tut_texture, NULL, &tutRect);
}

void drawThrowingPG() {
	if (!isThrowingLine) { return; }

	progressbar_fg.w = utility::max(pg_fgtex->w, timeThrowingLine);

	SDL_RenderTexture(renderer, pg_bgtex, NULL, &progressbar_bg);
	SDL_RenderTexture(renderer, pg_fgtex, NULL, &progressbar_fg);
}

void drawReelingPG() {
	if (!fishOnHook) { return; }

	progressbar_fg.w = utility::max(pg_fgtex->w, fishReelAmount);

	SDL_RenderTexture(renderer, pg_bgtex, NULL, &progressbar_bg);
	SDL_RenderTexture(renderer, pg_fgtex, NULL, &progressbar_fg);
	
}

void HUD::drawHUD() {
	drawThrowingPG();
	drawReelingPG();
	drawTutorialText();

	if (fish_DisplayTime > 0) {
		SDL_RenderTexture(renderer, fish_texture, NULL, &fish_rect);
		fish_DisplayTime -= 1;
	}else{
		if (fish_surface) {
			SDL_DestroySurface(fish_surface);
		}
		if (fish_texture) {
			SDL_DestroyTexture(fish_texture);
		}
	}
	
	if (displayCatchList){
		SDL_RenderTexture(renderer, catch_texture, NULL, &catch_rect);
	}
	if (displayMoneyList) {
		SDL_RenderTexture(renderer, money_texture, NULL, &money_rect);
	}
}

void HUD::unloadTextures() {
	SDL_DestroyTexture(pg_bgtex);
	SDL_DestroyTexture(pg_fgtex);
	SDL_DestroyTexture(tut_texture);
	SDL_DestroyTexture(money_texture);
	SDL_DestroyTexture(catch_texture);
	SDL_DestroyTexture(fish_texture);
}

void HUD::loadTextures() {
	//variables are in GameManager.hpp
	pg_fgtex = IMG_LoadTexture(renderer, "res/progressbar_fg.png");
	SDL_SetTextureScaleMode(pg_fgtex, SDL_SCALEMODE_NEAREST);

	pg_bgtex = IMG_LoadTexture(renderer, "res/progressbar_bg.png");
	SDL_SetTextureScaleMode(pg_bgtex, SDL_SCALEMODE_NEAREST);

	font = TTF_OpenFont("res/UniversCondensed.ttf", 24);

	if (!font) {
		std::cout << "failed to load font! error:" << SDL_GetError();
	}

	char tut_text[] = "Space (Hold) - Cast Rod\nLMB - Reel Fish\nG - Sell";
	tut_txtsize = strlen(tut_text);
	tut_surface = TTF_RenderText_Blended_Wrapped(font, tut_text, tut_txtsize, color, 0);
	tut_texture = SDL_CreateTextureFromSurface(renderer, tut_surface);

	tutRect.x = 0;
	tutRect.y = 0;
	tutRect.w = tut_surface->w;
	tutRect.h = tut_surface->h;

	progressbar_bg.h = pg_bgtex->h;
	progressbar_bg.w = pg_bgtex->w;
	progressbar_bg.x = 20;
	progressbar_bg.y = 125;

	progressbar_fg.h = pg_fgtex->h;
	progressbar_fg.x = 20;
	progressbar_fg.y = 125;

}

/*
wasted 18 mins on this stupid function
all i had to do was add * and &

eitherway, it sets up the surface and texture references up with the dynamic text.
*/
void setupDyanmicSurfaceTexture(string text, unsigned long long& size, SDL_Surface* &surface, SDL_Texture* &tex) {
	if (!font) { return; }

	size = text.size();

	surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), size, color, 0);
	tex = SDL_CreateTextureFromSurface(renderer, surface);
}

void HUD::updateCatchList() {
	string text = "Fish Caught:\n";

	for (int i = 0; i < caughtFish->size();i++) {
		if (caughtFish[i] == "\0") { continue; }
		ostringstream amount;

		amount << caughtFishPrice[i];

		text = text + caughtFish[i] + "($" + amount.str() + ")" + "\n";
	}

	//constantly destroy as we do have this get update a fair amount
	if (catch_surface) {
		SDL_DestroySurface(catch_surface);
	}

	if (catch_texture) {
		SDL_DestroyTexture(catch_texture);
	}

	setupDyanmicSurfaceTexture(text, catch_txtsize, catch_surface, catch_texture);

	catch_rect.w = catch_texture->w;
	catch_rect.h = catch_texture->h;
	catch_rect.x = 320;
	catch_rect.y = 10;

	displayCatchList = true;
}

void HUD::displayText(string text) {
	setupDyanmicSurfaceTexture(text, fish_txtsize, fish_surface, fish_texture);

	fish_rect.w = fish_texture->w;
	fish_rect.h = fish_texture->h;
	fish_rect.x = 20;
	fish_rect.y = 145;

	fish_DisplayTime = 100;
}

void HUD::updateMoney() {
	ostringstream convert;
	convert << money;

	string saved_amount;
	ifstream file("save_file.txt");

	file >> saved_amount;

	string text = "Money: $" + convert.str() + " (Highest Earned: $" + saved_amount + ")";

	//constantly destroy as we do have this get update a
	//update a? reminder to never code at 1am again
	if (money_surface) {
		SDL_DestroySurface(money_surface);
	}

	if (money_texture) {
		SDL_DestroyTexture(money_texture);
	}

	setupDyanmicSurfaceTexture(text, money_txtsize, money_surface, money_texture);

	money_rect.w = money_texture->w;
	money_rect.h = money_texture->h;
	money_rect.x = 140;
	money_rect.y = 450;

	displayMoneyList = true;
}
