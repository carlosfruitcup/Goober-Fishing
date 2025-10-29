#include "GameManager.hpp"

class HUD
{

public:
	static void drawHUD();
	static void loadTextures();
	static void unloadTextures();
	static void displayText(string text);
	static void updateCatchList();
	static void updateMoney();

};

extern TTF_Font* font;
