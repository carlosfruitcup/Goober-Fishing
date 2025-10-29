#include "Player.hpp"

int hand_idx = 0;

void Player::drawPlayer() {
	SDL_FRect player;
	player.x = 90;
	player.y = 225;
	player.h = player_tex->h;
	player.w = player_tex->w;
	SDL_RenderTexture(renderer, player_tex, NULL, &player);

	drawHands();
	drawLine();
}

void Player::drawHands() {
	SDL_FRect hand;
	hand.x = 120;
	hand.y = 225;

	if (isThrowingLine || fishOnHook) {
		if (tick % 2 == 0) { hand_idx = utility::wrap(hand_idx + 1, 0, 3); }
	}
	else {
		hand_idx = 0;
	}

	hand.h = hands_player_tex[hand_idx]->h;
	hand.w = hands_player_tex[hand_idx]->w;
	SDL_RenderTexture(renderer, hands_player_tex[hand_idx], NULL, &hand);
}

void Player::drawLine() {
	if (!lineThrown) { return; }

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderLine(renderer, 145, 234, 300 + lineThrownAdd, 400);
}

void Player::unloadTextures() {
	for (int i = 0; i < 4; i++) {
		SDL_DestroyTexture(hands_player_tex[i]);
	}

	SDL_DestroyTexture(player_tex);
}

void Player::loadTextures() {
	player_tex = IMG_LoadTexture(renderer, "res/player.png");
	SDL_SetTextureScaleMode(player_tex, SDL_SCALEMODE_NEAREST);


	hands_player_tex[0] = IMG_LoadTexture(renderer, "res/fishingrod0.png");
	hands_player_tex[1] = IMG_LoadTexture(renderer, "res/fishingrod1.png");
	hands_player_tex[2] = IMG_LoadTexture(renderer, "res/fishingrod2.png");
	hands_player_tex[3] = IMG_LoadTexture(renderer, "res/fishingrod3.png");
}
