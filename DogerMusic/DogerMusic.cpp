#include <iostream>
#include "dUI.hpp"
#include <string>

int main()
{
	auto& window = dUI::UIManager::Instance();

	window.setTitle("PinkMusic");


	window.print({(window.getSize().x / 2) - 5, 0}, u8"❥🔥PinkMusic <3");
	window.print({ (window.getSize().x ) - 13, 0 }, u8"🦅Version Uno");
	window.print({0, 0 }, u8"🍻Made by Doggo");
	window.divide({ 0, 1 }, { window.getSize().x/2, 1}, u8"💜");
	window.divide({ 0, window.getSize().y - 2 }, { window.getSize().x / 2, window.getSize().y - 2 }, u8"💜");

	window.divide({ 0, 1 }, { 0,  window.getSize().y - 1}, u8"💜");
	window.divide({ window.getSize().x - 2 , 1 }, { 0,  window.getSize().y - 1}, u8"💜");

	//buttons
	std::shared_ptr<dUI::Button> backButton;
	backButton = dUI::Button::Create(u8"⏮", dUI::cords{ 0,window.getSize().y - 1 }, [&backButton]() {

		});
	backButton->length = 2;

	std::shared_ptr<dUI::Button> playButton;
    playButton = dUI::Button::Create(u8"⏯️", dUI::cords{ 2, window.getSize().y - 1 }, [&playButton]() {
					static bool isPlaying = false;
					if (isPlaying){
						playButton->text = u8"⏯️";
					}else{
						playButton->text = u8"⏹️";
					}
					isPlaying = !isPlaying;
					playButton->toRender = true;
    });
	playButton->length = 2;

	std::shared_ptr<dUI::Button> skipButton;
	skipButton = dUI::Button::Create(u8"⏭", dUI::cords{ 4, window.getSize().y - 1 }, [&skipButton]() {
		
		});
	skipButton->length = 2;

	int frame = 0;

	while(true){
		window.print({ 10,10 }, std::to_string(frame));
		window.print({ 10,11 }, std::to_string(window.frameTime));

		window.print({ 20,20 }, std::to_string(frame));

		frame++;
		window.render();
	}
	return 0;
}