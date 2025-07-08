#pragma once

#include <SFML/Audio.hpp>

namespace dMusic {
	class Playlist {
	public:
		std::vector<std::string> queue;
		std::vector<std::string> paths;

		std::string name;

		void reQueue();
		Playlist(std::string);

	};
	class Player {
	public:
		std::string currentMusic = "";
		std::string currentPath = "";
		
		bool shuffle = false;
		bool loop = false;

		Player(Playlist);
		Playlist currentPlaylist;


		sf::Music music;
		int oldTimestamp = 0;

		int index = -1;

		void LoadPlaylist(Playlist);
		void LoadMusic(std::string);
	};
}