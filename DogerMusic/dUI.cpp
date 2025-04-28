#include "dUI.hpp"
#include <vector>
#include <functional>
#include <iostream>
#include <Windows.h>
#include <thread>
#include <ctime>

dUI::UIManager& dUI::UIManager::Instance() {
	static UIManager instance;
	return instance;
}

dUI::UIManager::UIManager()
{
	SetConsoleOutputCP(CP_UTF8);
	DWORD mode = 0;
	mode &= ~(ENABLE_QUICK_EDIT_MODE | ENABLE_LINE_INPUT);
	mode |= ENABLE_EXTENDED_FLAGS;
	mode |= ENABLE_MOUSE_INPUT;
	mode |= ENABLE_WINDOW_INPUT;
	bool ConsoleModeBool = SetConsoleMode(hin, mode);
	if (!ConsoleModeBool) {
		std::cerr << "Error setting console mode: " << GetLastError() << std::endl;
	}

	//std::thread eventThread([this]() {
	//	while (true) {
	//		events();
	//		//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	//	}
	//});

	//eventThread.detach();
}

void dUI::UIManager::addButton(std::shared_ptr<dUI::Button> button) {
	buttons.push_back(button);
}
void dUI::UIManager::addAnimables(std::shared_ptr<dUI::Animable> animable) {
	animables.push_back(animable);
}
void dUI::UIManager::addAnimation(std::shared_ptr<dUI::Animation> animation) {
	animations.push_back(animation);
}

void dUI::UIManager::print(dUI::cords cd, std::string text) {
	SetConsoleCursorPosition(hStdout, { (SHORT)cd.x, (SHORT)cd.y });
	std::cout << text;
}

void dUI::UIManager::divide(dUI::cords c1, dUI::cords c2, std::string t) {
	if (c1.y == c2.y) {
		SetConsoleCursorPosition(hStdout, { (SHORT)c1.x, (SHORT)c1.y });
		for (int i = 0; i < c2.x - c1.x; i++) {
			std::cout << t;
		}
	}
	else {
		//if (c1.x != c2.x) return;
		for (int i = c1.y; i < c2.y; i++) {
			SetConsoleCursorPosition(hStdout, { (SHORT)c1.x, (SHORT)i});
			std::cout << t;
		}

	}
}

void dUI::UIManager::setTitle(LPCSTR t){
	SetConsoleTitleA(t);
}

void dUI::UIManager::events(){
	if (PeekConsoleInput(hin, &InputRecord, 1, &Events)) {
		if (Events > 0) {
			ReadConsoleInput(hin, &InputRecord, 1, &Events);
			switch (InputRecord.EventType) {
			case MOUSE_EVENT:
				if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
					if (canClick) {
						for (auto& button : buttons) {
							if (InputRecord.Event.MouseEvent.dwMousePosition.X >= button->cords.x &&
								InputRecord.Event.MouseEvent.dwMousePosition.X < button->cords.x + button->length && //button->text.length()
								InputRecord.Event.MouseEvent.dwMousePosition.Y == button->cords.y) {
								button->Action();
								canClick = false;
							}
						}
					}
				}
				else if (InputRecord.Event.MouseEvent.dwButtonState == 0) {
					canClick = true;
				}
				break;
			}
		}
	}
}



dUI::cords dUI::UIManager::getSize(){
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	cords c{};
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	c.x = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	c.y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	return c;
}

std::shared_ptr<dUI::Button> dUI::Button::Create(std::string txt, dUI::cords c, std::function<void()> ac)
{
	auto button = std::make_shared<Button>(txt, c, ac);
	dUI::UIManager::Instance().addButton(button);
	return button;
}

dUI::Button::Button(std::string txt, dUI::cords c, std::function<void()> ac)
	: UIElement{ c, true, true, txt,ac} {}

dUI::Animable::Animable(std::string txt, dUI::cords c, std::function<void()> ac) 
//ac is not defined
	: UIElement{c, true, true, txt,ac} {
	//dUI::UIManager::Instance().addAnimables(shared_from_this());
}

void dUI::UIManager::render(){
	double seconds = difftime(time(NULL), lastFrame);
	///TODO: add setting for fps
	const double FPS_TIME = 1.0 / 60.0;
	frameTime = seconds;

	if (seconds < FPS_TIME) {
		std::this_thread::sleep_for(std::chrono::duration<double>(FPS_TIME - seconds));
	}

	events();

	for (auto& button : buttons) {
		if (button->visible && button->toRender) {
			button->toRender = false;
			print(button->cords, button->text);
		}
	}
	std::cout.flush();

	time(&lastFrame);
}