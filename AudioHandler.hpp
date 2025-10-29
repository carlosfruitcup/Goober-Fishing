#pragma once
#include "GameManager.hpp"

extern "C" {
#include "miniaudio.h"
}

#include "stb_vorbis.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

ma_result result;
ma_engine engine;

bool isSoundEngine_init = false;

void Audio::playAudio() {
	ma_sound sound = ma_sound();
}

class AudioHandler {
public:
	AudioHandler();
	static ma_result playSound(ma_sound* sound, string path, int loop, float vol);
	static ma_result stopSound(ma_sound* sound);
	static void freeAudio();
};

ma_sound sou;
AudioHandler::AudioHandler() {
	

	ma_engine_config engConfig = ma_engine_config_init();
	engConfig.listenerCount = 1;
	engConfig.channels = 2;
	engConfig.sampleRate = 48000;


	result = ma_engine_init(&engConfig, &engine);
	ma_engine_set_gain_db(&engine, 6.f);


	if (result != MA_SUCCESS) {
		std::cout << "MINIAUDIO FAILED TO LOAD, FUCK.";
	}
	else {

		std::cout << "miniaudio initialized! :)";
		isSoundEngine_init = true;
		
		AudioHandler::playSound(&sou,"res/music.ogg", 1, 0.35);

		
	}
}

ma_result AudioHandler::playSound(ma_sound* sound, string path, int loop, float vol) {
	if (!isSoundEngine_init) {
		return MA_ERROR;
	}

	ma_result result = ma_sound_init_from_file(&engine, path.c_str(), 0, NULL, NULL, sound);
	ma_sound_set_looping(sound, loop);
	ma_sound_set_volume(sound, vol); //volume is 0 - 1


	if (result != MA_SUCCESS) {
		return result;
	}

	ma_result result_sound = ma_sound_start(sound);

	return MA_SUCCESS;
}

ma_result AudioHandler::stopSound(ma_sound* sound) {
	ma_result result = ma_sound_stop(sound);
	return result;
}

void AudioHandler::freeAudio() {
	ma_engine_uninit(&engine);
}