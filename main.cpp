#include "GameManager.hpp"
#include "HUD.hpp"
#include <wtypes.h>
#include "AudioHandler.hpp"


SDL_Texture* bgtex;
int timeoutTime = 0;
std::set<int> jackpot = { 0, 2, 4 };
int currentIDX = 0; //for caughtFish
bool hitMaxThreshold = false; //for throwing

static void render() {
	SDL_SetRenderDrawColor(renderer, 14, 144, 249, 255);
	SDL_RenderClear(renderer);
	/*
		reminder: dont load textures here
		unless you want the memory to skyrocket to 4gbs and kill your computer
	*/

	SDL_FRect background;
	background.x = 0;
	background.y = 0;
	background.h = bgtex->h;
	background.w = bgtex->w;
	SDL_RenderTexture(renderer, bgtex, NULL, &background);
	

	Player::drawPlayer();
	HUD::drawHUD();

	
	SDL_RenderPresent(renderer);
	SDL_Delay(32.333f);
}

static void loadTextures() {
	bgtex = IMG_LoadTexture(renderer, "res/bg.png");
	SDL_SetTextureScaleMode(bgtex, SDL_SCALEMODE_NEAREST);

	if (!bgtex) {
		std::cout << "Background failed to load!";
		exit(-1);
	}

	HUD::loadTextures();
	Player::loadTextures();
}

static void unloadTextures() {
	HUD::unloadTextures();
	Player::unloadTextures();
	SDL_DestroyTexture(bgtex);
}

static void chanceFishHooked() {
	if (!lineThrown || fishOnHook) { return; }

	if (tick % 16 == 0) {
		int current_chance = rand() % 16;
		bool isHooked = false;

		if (jackpot.count(current_chance)) {
			isHooked = true;
			fishOnHook = true;
		}
	}
}

static void resetVariables() {
	lineThrown = false;
	fishOnHook = false;
	hitMaxThreshold = false;
	isThrowingLine = false;
	timeThrowingLine = 0;
	lineThrownAdd = 0;
	timeoutTime = 0;
	fishReelAmount = 0;
}

static void handleTimeout() {
	if (!fishOnHook) { return; }

	if (timeoutTime < 100) {
		timeoutTime++;
	}else{
		HUD::displayText("The line snapped...");
		resetVariables();
	}

}

static void catchFish() {
	if (fishReelAmount < pg_bgtex->w) { return; }

	//seed the randomizer so we can get different fish each time
	srand(chrono::system_clock::to_time_t(chrono::system_clock::now()));

	//not sure why in the og game we add one?
	float size = (rand() % 20) + 1;
	float weight = (rand() % 2) + 1 * size;

	if (weight <= 0) { weight = 1; }
	
	int index = rand() % 98;
	std::cout << index;
	string fishCaught = fishList[index];
	HUD::displayText("You Caught a " + fishCaught + "!");

	if (fishCaught == "Shark") {
		size = 100;
		weight = 1000;
	}

	if (fishCaught == "Kraken") {
		size = 1000;
		weight = 2000;
	}

	caughtFish[currentIDX] = fishCaught;
	caughtFishPrice[currentIDX] = size * weight;
	currentIDX++;

	HUD::updateCatchList();
	resetVariables();

}

static void sellFish() {
	double amountEarned = 0;

	for (int i = 0; i < caughtFish->size(); i++) {
		if (caughtFish[i] == "\0") { continue; }

		amountEarned += caughtFishPrice[i];
	}

	money += amountEarned;

	ostringstream convert;
	convert << amountEarned;
	string display = convert.str();

	for (int i = 0; i < caughtFish->capacity(); i++) {
		if (caughtFish[i] == "\0") { continue; }
		
		caughtFish[i] = "\0";
		caughtFishPrice[i] = 0;
	}

	ifstream savedFile("save_file.txt");
	double savedAmount;

	savedFile >> savedAmount;

	if (savedAmount < money) {
		ofstream file;
		file.open("save_file.txt", ios::trunc);
		file << money;
		file.close();
	}

	

	HUD::updateMoney();
	HUD::displayText("You earned $" + display + "!");
	HUD::updateCatchList();
	resetVariables();

	//reset the current index or else nothing gets displayed
	currentIDX = 0;
}

static void loadFishList() {
	ifstream file("fish_list.txt");

	if (file.is_open()) {
		for (int i = 0; i < 98; i++) {
			file >> fishList[i];
		}
	}
	else {
		cout << "Failed to load 'fish_list.txt'!";
		exit(-1);
	}

}

static void kbDownInput(SDL_Event* key) {
	if (key->key.scancode == SDL_SCANCODE_SPACE) {
		if (timeThrowingLine < pg_bgtex->w && !hitMaxThreshold) {
			isThrowingLine = true;
			timeThrowingLine += 5;
			lineThrownAdd += 3;
		}
		else {
			if (timeThrowingLine < 0) {
				resetVariables();
				return;
			}

			hitMaxThreshold = true;
			timeThrowingLine -= 2;
			lineThrownAdd -= 3;
		}
	}

}

static void kbUpInput(SDL_Event* key) {
	if (key->key.scancode == SDL_SCANCODE_SPACE) {
		if (timeThrowingLine > 4) {
			isThrowingLine = false;
			lineThrown = true;
		}else{
			resetVariables();
		}
	}

	if (key->key.scancode == SDL_SCANCODE_G) {
		sellFish();
	}

}

static void mouseInput(SDL_Event* mouse) {
	if (mouse->button.button == SDL_BUTTON_LEFT) {
		if (!fishOnHook) { return; }

		fishReelAmount += 15;

		catchFish();
	}
}


int main() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "sdl failed to initialize jajaja %s\n" << SDL_GetError();
		return EXIT_FAILURE;
	}

	TTF_Init();

	Uint32 flags{};
	SDL_Window* window = SDL_CreateWindow("Goober Fishing", 640, 480, flags);

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	
	if (!window) {
		std::cout << "couldnt open window :( %s\n", SDL_GetError();
		SDL_Quit();
		return EXIT_FAILURE;
	}
	
	renderer = SDL_CreateRenderer(window, NULL);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_SetRenderDrawColor(renderer, 14, 144, 249, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	loadFishList();
	loadTextures();
	AudioHandler audio = AudioHandler::AudioHandler();
	
	bool is_running = true;
	while (is_running) {
		Uint64 start = SDL_GetPerformanceCounter();
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				is_running = false;
			}

			if (event.type == SDL_EVENT_KEY_DOWN) {
				kbDownInput(&event);
			}

			if (event.type == SDL_EVENT_KEY_UP) {
				kbUpInput(&event);
			}

			if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				mouseInput(&event);
			}

		}
		handleTimeout();
		chanceFishHooked();
		render();
		Uint64 end = SDL_GetPerformanceCounter();

		float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
		
		delta = elapsed;
		tick++;
	}
	
	audio.freeAudio();
	unloadTextures();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_Quit();
	SDL_Quit();
	return EXIT_SUCCESS;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	main();
}


