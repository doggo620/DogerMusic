#include <iostream>
#include "dUI.hpp"
#include <string>

int main()
{
	auto& window = dUI::UIManager::Instance();
	window.clear();
	window.setTitle("PinkMusic");

	if (window.getSize().x != 120 || window.getSize().y != 30) {
		window.print({ 0,0 }, "Warning! You need to resize your console window.");
		window.print({ 0,1 }, "Resize horizontaly: ");
		window.print({ 0,2 }, "Resize verticly: ");
		while (window.getSize().x != 120 || window.getSize().y != 30) {
			window.print({ 21,1 }, std::to_string(120 - window.getSize().x) + " ");
			window.print({ 17,2 }, std::to_string(30 - window.getSize().y) +" ");
			window.waitForInput();
			//std::to_string(120 - window.getSize().x)
		}
	}
	window.clear();


	std::string timestamp = u8"0 : 0 0";
	window.print({ 10,window.getSize().y - 1 }, timestamp);
	dUI::slider musicSlider = dUI::slider({ 19,window.getSize().y - 1 }, { 84, window.getSize().y - 1 }, u8"🩷", "#");//79
	musicSlider.setAction([&timestamp, &musicSlider, &window]() {

		});
	window.addSlider(std::make_shared<dUI::slider>(musicSlider));

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

	std::shared_ptr<dUI::Button> shufleButton;
	shufleButton = dUI::Button::Create(u8"➡️", dUI::cords{ 6, window.getSize().y - 1 }, [&shufleButton]() {
		static bool isShuffling = false;
		if (isShuffling) {
			shufleButton->text = u8"🔀";
		}
		else {
			shufleButton->text = u8"➡️";
		}
		isShuffling = !isShuffling;
		shufleButton->toRender = true;
		});
	shufleButton->length = 2;

	int frame = 0;

	dUI::movingText exampleMusicText = dUI::movingText({ 89, window.getSize().y - 1 }, { window.getSize().x - 2, window.getSize().y - 1 }, u8"Kizo x Bletka ft.Szpaku - DOLCE VITA       ");
	window.addMoving(std::make_shared<dUI::movingText>(exampleMusicText));

	dUI::movingText ttile = dUI::movingText({ 60, 20 }, { 80, 20 }, u8"MOVING TEXT IS AWESOME <3333333     ");
	window.addMoving(std::make_shared<dUI::movingText>(ttile));
	dUI::movingText ss = dUI::movingText({ 30, 6 }, { 40, 6 }, u8"<33333333       ");
	window.addMoving(std::make_shared<dUI::movingText>(ss));

	window.print({ 4,20 }, "Size X: " + std::to_string(window.getSize().x));
	window.print({ 4,21 }, "Size Y: " + std::to_string(window.getSize().y));

	//IMPORTANT :sob:
	window.fixInput();

	while(true){
		window.print({ 10,10 }, "Frames: " + std::to_string(frame));
		window.print({ 10,11 }, "FrameTime: " + std::to_string(window.frameTime));
		window.print({ 50,5 }, "FrameCap: " + std::to_string(window.frame));
		window.print({ 20,20 }, "Frames: " + std::to_string(frame));
		frame++;
		window.render();
	}
	return 0;
}