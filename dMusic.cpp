#include "dMusic.hpp"
#include <iostream>
#include <filesystem>
#include <random>

std::random_device rd;

dMusic::Player::Player(Playlist p ) : currentPlaylist(p){}

void dMusic::Player::LoadPlaylist(Playlist p){
	currentPlaylist = p;
}


void dMusic::Player::LoadMusic(std::string path) {
	currentPath = path;
	currentMusic = path.substr(path.find_last_of("/\\") + 1).substr(0, path.substr(path.find_last_of("/\\") + 1).find_last_of('.'));
	if (!music.openFromFile(path)) {
		std::cout << "Error while loading music";
	}
}

void dMusic::Playlist::reQueue() {  
   queue.insert(queue.begin(), paths.begin(), paths.end());   
}

dMusic::Playlist::Playlist(std::string path){
	for (auto p : std::filesystem::directory_iterator(path)) {
		if (p.path().extension() != ".flac") continue;
		paths.push_back(p.path().u8string());
	}

	name = path.substr(path.find_last_of("/\\") + 1);
}
