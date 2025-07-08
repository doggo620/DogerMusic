#include <iostream>
#include "dUI.hpp"
#include "dMusic.hpp"
#include <string>
#include <thread>
#include <random>
#include <vector>

void EnableEcoQoS()
{
	// Ask Windows to throttle us at the CPU‐power level
	PROCESS_POWER_THROTTLING_STATE pts = {};
	pts.Version = PROCESS_POWER_THROTTLING_CURRENT_VERSION;
	pts.ControlMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED;
	pts.StateMask = PROCESS_POWER_THROTTLING_EXECUTION_SPEED;

	SetProcessInformation(
		GetCurrentProcess(),
		ProcessPowerThrottling,
		&pts,
		sizeof(pts)
	);
}

int main()
{
	//TEST SLEEP
	SetPriorityClass(GetCurrentProcess(), IDLE_PRIORITY_CLASS);
	EnableEcoQoS();

	//Setup
	auto& window = dUI::UIManager::Instance();
	window.clear();
	window.setTitle("PinkMusic");

	HWND name = GetForegroundWindow();

	//random engine
	std::random_device rd;



	window.print({ 0,0 }, "P I N K   M U S I C");
	window.print({ 0,1 }, "Welcome to the beta!");
	window.print({ 0,2 }, "Please enter path of your playlist!");
	dMusic::Playlist pl(window.input());
	dMusic::Player player(pl);

	window.clear();
	//set width and height
	if (window.getSize().x != 120 || window.getSize().y != 30) {
		window.print({ 0,0 }, "Warning! You need to resize your console window.");
		window.print({ 0,1 }, "Resize horizontaly: ");
		window.print({ 0,2 }, "Resize verticly: ");
		while (window.getSize().x != 120 || window.getSize().y != 30) {
			window.print({ 21,1 }, std::to_string(120 - window.getSize().x) + " ");
			window.print({ 17,2 }, std::to_string(30 - window.getSize().y) + " ");
			window.waitForInput();
			//std::to_string(120 - window.getSize().x)
		}
	}
	window.clear();

	//logo
	window.print({ window.getSize().x/2,window.getSize().y / 2 }, "P I N K");
	std::this_thread::sleep_for(std::chrono::seconds(1));
	window.clear();
	window.print({ window.getSize().x / 2,window.getSize().y / 2 }, "M U S I C");
	std::this_thread::sleep_for(std::chrono::seconds(1));
	window.clear();

	//timestamp
	std::string timestamp = u8"0 : 0 0";
	window.print({ 12,window.getSize().y - 1 }, timestamp);

	//volume
	std::string volumeP = u8"100%";
	window.print({ window.getSize().x -37,0}, volumeP);


	//Slider 
	dUI::slider musicSlider = dUI::slider({ 21,window.getSize().y - 1 }, { 86, window.getSize().y - 1 }, u8"🩷", "#");//79 // 19
	auto msP = std::make_shared<dUI::slider>(musicSlider);
	msP->setSliderAction([&player, &msP, &timestamp, &window]() {
		int newTime = player.music.getDuration().asSeconds() * (float(msP->progress) / 65);

		timestamp = std::to_string(int(newTime / 60)) + " : " + std::to_string(int((newTime - (int(newTime / 60) * 60)) / 10)) + " " + std::to_string((newTime - (int(newTime / 60) * 60)) % 10);
		window.print({ 12,window.getSize().y - 1 }, timestamp);
		});

	msP->setAction([&player, &msP, &timestamp, &window]() {
		//std::cout << (float(msP->progress) / 65);
		//std::cout << int(player.music.getDuration().asSeconds());
		//std::cout << player.music.getDuration().asSeconds() * (msP->progress / 65);
		int newTime = player.music.getDuration().asSeconds() * (float(msP->progress) / 65);

		player.music.setPlayingOffset(sf::seconds(newTime));

		timestamp = std::to_string(int(newTime / 60)) + " : " + std::to_string(int((newTime - (int(newTime / 60) * 60)) / 10)) + " " + std::to_string((newTime - (int(newTime / 60) * 60)) % 10);
		window.print({ 12,window.getSize().y - 1 }, timestamp);
		});

	window.addSlider(msP);


	dUI::slider volumeSlider = dUI::slider({ window.getSize().x - 32,0}, { window.getSize().x -3, 0}, u8"🔈", "#");
	auto vsP = std::make_shared<dUI::slider>(volumeSlider);
	vsP->setSliderAction([&player, &vsP, &volumeP, &window]() {
		int newVol = 100 * (float(vsP->progress) / 29);

		player.music.setVolume(newVol);

		volumeP = std::to_string(newVol) + "%";
		window.print({ window.getSize().x - 37,0 }, std::string(4, ' ')); // clear old
		window.print({ window.getSize().x - 37,0 }, volumeP);
		});

	vsP->setAction([&player, &vsP, &volumeP, &window]() {
		int newVol = 100 * (float(vsP->progress) / 29);

		player.music.setVolume(newVol);

		volumeP = std::to_string(newVol) + "%";
		window.print({ window.getSize().x - 37,0 }, std::string(4, ' ')); // clear old
		window.print({ window.getSize().x - 37,0 }, volumeP);
		});
	vsP->progress = 28;
	window.addSlider(vsP);

	//UI elements
	//💜 pink hearth
	window.print({ (window.getSize().x / 2) - 5, 0 }, u8"❥🔥PinkMusic <3");
	//window.print({ (window.getSize().x) - 13, 0 }, u8"🦅Version Uno");
	window.print({ 0, 0 }, u8"🩷Made by Doggo");
	window.divide({ 0, 1 }, { window.getSize().x / 2, 1 }, u8"🖤");
	window.divide({ 0, window.getSize().y - 2 }, { window.getSize().x / 2, window.getSize().y - 2 }, u8"🖤");

	//window.divide({ 0, 1 }, { 0,  window.getSize().y - 1 }, u8"💜"); // left horizontal
	//window.divide({ window.getSize().x - 2 , 1 }, { 0,  window.getSize().y - 1 }, u8"💜"); // right vertical

	//Music text
	window.print({ 88, window.getSize().y - 1 }, u8"🎵");
	dUI::movingText exampleMusicText = dUI::movingText({ 91, window.getSize().y - 1 }, { window.getSize().x - 2, window.getSize().y - 1 }, player.currentMusic); // 89
	auto musicTextP = std::make_shared<dUI::movingText>(exampleMusicText);
	window.addMoving(musicTextP);

	//MENUS :SOB:
	std::shared_ptr<dUI::Menu> playlistMenu;
	int index = 0;
	playlistMenu = std::make_shared<dUI::Menu>(dUI::Menu({ 1,5 }, { 41,27 }, [&player, &musicTextP, &index, &playlistMenu](dUI::cords cord) {
		//action

		if (cord.x >= 5 && cord.y >= 6 &&cord.y < ((player.currentPlaylist.paths.size() > 20) ? 20 + 6 : player.currentPlaylist.paths.size()+6)) {
			auto y = player.currentPlaylist.paths[(cord.y - 6) + ((index == 0) ? 0 : (index * 20))];
			player.LoadMusic(y);
			player.music.play();
			musicTextP->change(player.currentMusic + "     ");
			playlistMenu->toRender = true;
		}
		else if (cord.x <=2) {
			switch (cord.y) {
			case 5:
				//add playlist
				break;
			case 7:
				if (index != 0) {
					index--;
					playlistMenu->toRender = true;
				}
				break;
			case 24:
				if (player.currentPlaylist.paths.size() - (20 * index) > 20){
					index++;
					playlistMenu->toRender = true;
				}
				break;
			}
		}



		}, [&window, &playlistMenu, &player, &index]() {
			//buttons
			static std::string oldPlaylistName = "";
			static dUI::movingText playlistName({ 10,5 }, { 30,5 }, oldPlaylistName);

			if (playlistMenu->toRender) {
				//DEBUG
				//window.print({ 0,5 }, "#");
				//window.print({ 41,25 }, "#");

				//buttons

				window.print({ 1,5 }, u8"➕");
				window.print({ 1,7 }, u8"👆");
				window.print({ 1,24 }, u8"👇");
				window.print({ 32,5 }, u8"->");
				window.print({ 6,5 }, u8"<-");

				window.divide({ 1,8 }, { 1,24 }, " |");

				for (int x = 0; x < 20; x++) {
					//TODO: FIX CLEAR LINE
					window.print({ 5,6 + x }, std::string(100, ' '));
				}

				//text
				for (int x = 0; x < ((player.currentPlaylist.paths.size() - (20 * index) > 20)? 20 : player.currentPlaylist.paths.size() - (20 * index)); x++) {
					if (player.currentPlaylist.paths[(x) + ((index == 0) ? 0 : (index * 20))].substr(player.currentPlaylist.paths[(x) + ((index == 0) ? 0 : (index * 20))].find_last_of("/\\") + 1).substr(0, player.currentPlaylist.paths[(x) + ((index == 0) ? 0 : (index * 20))].substr(player.currentPlaylist.paths[(x) + ((index == 0) ? 0 : (index * 20))].find_last_of("/\\") + 1).find_last_of('.')) == player.currentMusic) {
						window.print({ 5,6 + x }, std::to_string((x + 1) + ((index == 0) ? 0 : (index * 20))) + ". " + "\033[1;37;35m" + player.currentPlaylist.paths[(x) + ((index == 0) ? 0 : (index * 20))].substr(player.currentPlaylist.paths[(x) + ((index == 0) ? 0 : (index * 20))].find_last_of("/\\") + 1).substr(0, player.currentPlaylist.paths[(x) + ((index == 0) ? 0 : (index * 20))].substr(player.currentPlaylist.paths[(x) + ((index == 0) ? 0 : (index * 20))].find_last_of("/\\") + 1).find_last_of('.')) + "\033[0m");//player.currentPlaylist.paths[(x + 1) * (index * 15)]
					}
					else {
						window.print({ 5,6 + x }, std::to_string((x + 1) + ((index == 0) ? 0 : (index * 20))) + ". " + player.currentPlaylist.paths[(x) + ((index == 0) ? 0 : (index * 20))].substr(player.currentPlaylist.paths[(x) + ((index == 0) ? 0 : (index * 20))].find_last_of("/\\") + 1).substr(0, player.currentPlaylist.paths[(x) + ((index == 0) ? 0 : (index * 20))].substr(player.currentPlaylist.paths[(x) + ((index == 0) ? 0 : (index * 20))].find_last_of("/\\") + 1).find_last_of('.')));//player.currentPlaylist.paths[(x + 1) * (index * 15)]
					}
				}//((x+1)*(index == 0)? 1: (index*15))


				if (!(playlistName.lenght > playlistName.c2.x - playlistName.c1.x)) {
					window.print(playlistName.c1, playlistName.originalText.substr(0, playlistName.lenght) + std::string(playlistName.c2.x - playlistName.c1.x - playlistName.lenght, ' '));
				}

				playlistMenu->toRender = false;
			}

			//moving texts
			if (window.frame % 5 == 0) {
				if (oldPlaylistName == "") {
					oldPlaylistName = player.currentPlaylist.name;
					playlistName.change(oldPlaylistName + "  ");
				}
				else {
					if (oldPlaylistName != player.currentPlaylist.name) {
						oldPlaylistName = player.currentPlaylist.name;
						playlistName.change(oldPlaylistName + "  ");
					}
				}
				if (playlistName.lenght > playlistName.c2.x - playlistName.c1.x) {
					playlistName.render();
				}

			}
			}));
		window.addMenu(playlistMenu);





	//buttons
	std::shared_ptr<dUI::Button> backButton;
	backButton = dUI::Button::Create(u8"⏮", dUI::cords{ 0,window.getSize().y - 1 }, [&player, &musicTextP, &playlistMenu]() {
		if (player.music.getPlayingOffset().asSeconds() > 3) {
			player.music.setPlayingOffset(sf::seconds(0));
			return;
		}
		if (player.index > 0) {
			player.index--;
			playlistMenu->toRender = true;
		}
		else {
			player.index = 0;
			playlistMenu->toRender = true;
		}
		auto x = player.currentPlaylist.queue[player.index];
		player.LoadMusic(x);
		player.music.play();
		musicTextP->change(player.currentMusic + "     ");

		});
	backButton->length = 2;

	std::shared_ptr<dUI::Button> playButton;
	playButton = dUI::Button::Create(u8"⏯️", dUI::cords{ 2, window.getSize().y - 1 }, [&playButton, &player]() {
		static bool isPlaying = false;
		if (isPlaying) {
			player.music.play();
			playButton->text = u8"⏯️"; 
		}
		else {
			player.music.pause();
			playButton->text = u8"⏹️";
		}
		isPlaying = !isPlaying;
		playButton->toRender = true;
		});
	playButton->length = 2;


	std::shared_ptr<dUI::Button> skipButton;
	skipButton = dUI::Button::Create(u8"⏭", dUI::cords{ 4, window.getSize().y - 1 }, [&player, &musicTextP, &playlistMenu]() {
		if (player.index < player.currentPlaylist.queue.size() - 1) {
			player.index++;
			playlistMenu->toRender = true;
		}
		else {
			player.index = 0;
			playlistMenu->toRender = true;
		}
		auto x = player.currentPlaylist.queue[player.index];
		player.LoadMusic(x);
		player.music.play();
		musicTextP->change(player.currentMusic + "     ");
		});
	skipButton->length = 2;

	std::shared_ptr<dUI::Button> shufleButton;
	shufleButton = dUI::Button::Create(u8"➡️", dUI::cords{ 6, window.getSize().y - 1 }, [&shufleButton, &player, &musicTextP, &rd, &playlistMenu]() {
		static bool isShuffling = true;
		if (isShuffling) {
			shufleButton->text = u8"🔀";
			player.shuffle = true;
			std::shuffle(player.currentPlaylist.queue.begin(), player.currentPlaylist.queue.end(), rd);
		}
		else {
			shufleButton->text = u8"➡️";
			player.shuffle = false;
			player.currentPlaylist.queue.clear();
			player.currentPlaylist.reQueue();
		}
		playlistMenu->toRender = true;
		//TEST
		//player.currentPlaylist.reQueue();
		player.index = 0;
		auto x = player.currentPlaylist.queue[player.index];
		player.LoadMusic(x);
		player.music.play();
		musicTextP->change(player.currentMusic + "     ");
		//END TEST
		isShuffling = !isShuffling;
		shufleButton->toRender = true;
		});
	shufleButton->length = 2;

	std::shared_ptr<dUI::Button> loopButton;
	loopButton = dUI::Button::Create(u8"🔁", dUI::cords{ 8, window.getSize().y - 1 }, [&loopButton, &player]() {
		static bool isLooping = true;
		if (isLooping) {
			loopButton->text = u8"🔂";
			player.loop = true;
		}
		else {
			loopButton->text = u8"🔁";
			player.loop = false;
		}
		isLooping = !isLooping;
		loopButton->toRender = true;
		});
	loopButton->length = 2;


	//spacebar play key
	dUI::keyEvent playKey(VK_SPACE, [&playButton]() {
		playButton->Action();
		});
	std::shared_ptr<dUI::keyEvent>playKeyPointer = std::make_shared<dUI::keyEvent>(playKey);
	window.addKeyEvent(playKeyPointer);

	//IMPORTANT :sob:
	window.fixInput();

	while (true) {
		if (player.music.getStatus() == sf::SoundSource::Status::Stopped ) {
			if (player.index == -1) {
				player.currentPlaylist.reQueue();
				if (player.shuffle) {
					std::shuffle(player.currentPlaylist.queue.begin(), player.currentPlaylist.queue.end(), rd);
				}
			}
			if (player.loop) {

			}
			else if (player.index < player.currentPlaylist.queue.size() - 1) {
				player.index++;
				playlistMenu->toRender = true;
			}
			else {
				player.index = 0;
				playlistMenu->toRender = true;
			}
			auto x = player.currentPlaylist.queue[player.index];
			player.LoadMusic(x);
			player.music.play();
			musicTextP->change(player.currentMusic + "     ");
		}
		if (name == GetForegroundWindow()) {
			int tsmp = std::floor(player.music.getPlayingOffset().asSeconds());
			if (tsmp != player.oldTimestamp && !msP->dragging) {
				player.oldTimestamp = tsmp;
				timestamp = std::to_string(int(tsmp / 60)) + " : " + std::to_string(int((tsmp - (int(tsmp / 60) * 60)) / 10)) + " " + std::to_string((tsmp - (int(tsmp / 60) * 60)) % 10);
				window.print({ 12,window.getSize().y - 1 }, timestamp);
				msP->progress = (tsmp / std::floor(player.music.getDuration().asSeconds())) * 65;
				msP->update();
			}
			window.render();
		}
		window.fpsCap();
	}
	return 0;
}