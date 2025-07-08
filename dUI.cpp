#include "dUI.hpp"
#include <vector>
#include <functional>
#include <iostream>
#include <string>
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
void dUI::UIManager::addSlider(std::shared_ptr<slider> sl) {
	sliders.push_back(sl);
	sl->update();
}
void dUI::UIManager::addAnimables(std::shared_ptr<dUI::Animable> animable) {
	animables.push_back(animable);
}
void dUI::UIManager::addMoving(std::shared_ptr<movingText> cc) {
	movings.push_back(cc);
}

void dUI::UIManager::addKeyEvent(std::shared_ptr<keyEvent> e) {
	keyEvents.push_back(e);
}

void dUI::UIManager::addMenu(std::shared_ptr<Menu> m) {
	Menus.push_back(m);
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
			SetConsoleCursorPosition(hStdout, { (SHORT)c1.x, (SHORT)i });
			std::cout << t;
		}

	}
}

void dUI::UIManager::setTitle(LPCSTR t) {
	SetConsoleTitleA(t);
}

void dUI::UIManager::events() {
	if (PeekConsoleInput(hin, &InputRecord, 1, &Events)) {
		if (Events > 0) {
			ReadConsoleInput(hin, &InputRecord, 1, &Events);
			switch (InputRecord.EventType) {
			case KEY_EVENT:
				if (InputRecord.Event.KeyEvent.bKeyDown) {
					for (auto keyEvent : keyEvents) {
						if (keyEvent->key == InputRecord.Event.KeyEvent.wVirtualKeyCode) keyEvent->Action();
					}
				}
				break;
			case MOUSE_EVENT:
				if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
					//sliders dragging
					for (auto& slider : sliders) {
						if (slider->dragging) {
							if (InputRecord.Event.MouseEvent.dwMousePosition.X >= slider->c1.x &&
								InputRecord.Event.MouseEvent.dwMousePosition.X < slider->c2.x) {
								int x = InputRecord.Event.MouseEvent.dwMousePosition.X - slider->c1.x;
								slider->draw(x);
								slider->progress = x;
							}
							else if (InputRecord.Event.MouseEvent.dwMousePosition.X > slider->c2.x) {
								int x = slider->c2.x - slider->c1.x;
								slider->draw(x);
								slider->progress = x;
							}
							else if (InputRecord.Event.MouseEvent.dwMousePosition.X < slider->c1.x) {
								int x = 0;
								slider->draw(x);
								slider->progress = x;
							}
						}
					}
					//click
					if (canClick) {
						//buttons check
						for (auto& button : buttons) {
							if (InputRecord.Event.MouseEvent.dwMousePosition.X >= button->cords.x &&
								InputRecord.Event.MouseEvent.dwMousePosition.X < button->cords.x + button->length && //button->text.length()
								InputRecord.Event.MouseEvent.dwMousePosition.Y == button->cords.y) {
								button->Action();
								canClick = false;
								return;
							}
						}
						//sliders check
						for (auto& slider : sliders) {
							if (InputRecord.Event.MouseEvent.dwMousePosition.X >= slider->c1.x &&
								InputRecord.Event.MouseEvent.dwMousePosition.X < slider->c2.x && //button->text.length()
								InputRecord.Event.MouseEvent.dwMousePosition.Y == slider->c1.y) {
								//button->Action();
								slider->dragging = true;
								canClick = false;
								return;
							}
						}
						//menus check
						for (auto& menu : Menus) {
							if (InputRecord.Event.MouseEvent.dwMousePosition.X >= menu->c1.x
								&& InputRecord.Event.MouseEvent.dwMousePosition.X <= menu->c2.x
								&& InputRecord.Event.MouseEvent.dwMousePosition.Y >= menu->c1.y
								&& InputRecord.Event.MouseEvent.dwMousePosition.Y <= menu->c2.y){
								menu->action({ InputRecord.Event.MouseEvent.dwMousePosition.X,InputRecord.Event.MouseEvent.dwMousePosition.Y });
								canClick = false;
								return;
								}
						}
					}
				}
				//release click
				else if (InputRecord.Event.MouseEvent.dwButtonState == 0) {
					for (auto& slider : sliders) {
						if (slider->dragging) {
							slider->dragging = false;
							if (slider->Action != NULL) {
								slider->Action();
							}
						}
					}


					canClick = true;
				}
				break;
			}
		}
	}
}

void dUI::UIManager::clear() {
	system("cls");
}



dUI::cords dUI::UIManager::getSize() {
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
	: UIElement{ c, true, true, txt,ac } {
}

dUI::Animable::Animable(std::string txt, dUI::cords c, std::function<void()> ac)
//ac is not defined
	: UIElement{ c, true, true, txt,ac } {
	//dUI::UIManager::Instance().addAnimables(shared_from_this());
}

void dUI::UIManager::render() {
	/*double seconds = difftime(time(NULL), lastFrame);
	///TODO: add setting for fps
	const double FPS_TIME = 1.0 / 60.0;
	frameTime = seconds;

	if (seconds < FPS_TIME) {
		std::this_thread::sleep_for(std::chrono::duration<double>(FPS_TIME - seconds));
	}
	*/
	events();

	for (auto& button : buttons) {
		if (button->visible && button->toRender) {
			button->toRender = false;
			print(button->cords, button->text);
		}
	}

	for (auto& menu : Menus) {
		if (menu->toRender) {
			menu->render();
		}
		else if (frame % 30 == 0) {//optimalisation esss?
			menu->render();
		}
	}
	/*if (frame % 30 == 0) { //optimalisation esss?
		for (auto& menu : Menus) {
			menu->render();
		}
	} 
	*/
	if (frame % 5 == 0) {
		for (auto& movable : movings) {
			movable->render();
		}
	}

	//std::cout.flush();
	frame = (frame + 1) % 60;
	//time(&lastFrame);
}

void dUI::UIManager::fpsCap(){
	double seconds = difftime(time(NULL), lastFrame);
	///TODO: add setting for fps
	const double FPS_TIME = 1.0 / 60.0;
	frameTime = seconds;

	if (seconds < FPS_TIME) {
		std::this_thread::sleep_for(std::chrono::duration<double>(FPS_TIME - seconds));
	}
	time(&lastFrame);
}

void dUI::UIManager::waitForInput() {
	ReadConsoleInput(hin, &InputRecord, 1, &Events);
}

void dUI::UIManager::fixInput() {
	SetConsoleOutputCP(CP_UTF8);
	DWORD mode = 0;
	//mode &= ~(ENABLE_QUICK_EDIT_MODE | ENABLE_LINE_INPUT);
	//mode |= ENABLE_EXTENDED_FLAGS;
	//mode |= ENABLE_MOUSE_INPUT;
	//mode |= ENABLE_WINDOW_INPUT;


	GetConsoleMode(hin, &mode);

	// Modify mode flags as needed
	mode |= ENABLE_EXTENDED_FLAGS;
	mode |= ENABLE_MOUSE_INPUT;
	mode |= ENABLE_WINDOW_INPUT;


	bool ConsoleModeBool = SetConsoleMode(hin, mode);
	if (!ConsoleModeBool) {
		std::cerr << "Error setting console mode: " << GetLastError() << std::endl;
	}
}

std::string dUI::UIManager::input()
{
	SetConsoleCursorPosition(hStdout, { (SHORT)0, (SHORT)(getSize().y-3)});
	std::string x;
	std::getline(std::cin, x);
	return x;
}

dUI::slider::slider(cords cc1, cords cc2, std::string poi, std::string cha) {
	c1 = cc1;
	c2 = cc2;
	ch = cha;
	pointer = poi;
}

void dUI::slider::setAction(std::function<void()> an) {
	Action = an;
}

void dUI::slider::setSliderAction(std::function<void()>an){
	SliderAction = an;
}

void dUI::slider::update() {
	auto ins = dUI::UIManager::Instance();
	SetConsoleCursorPosition(ins.hStdout, { (SHORT)c1.x, (SHORT)c1.y });

	for (int i = 0; i < c2.x - c1.x; i++) {
		if (i == progress) { //c1.x +
			std::cout << pointer;
		}
		else if (i < progress) {
			std::cout << ch;
		}
		else if (i > progress) {
			std::cout << "-";
		}
	}
}

void dUI::slider::draw(int newPosX) {
	auto ins = dUI::UIManager::Instance();

	bool RIGHT = (progress - newPosX) < 0;
	short distance = std::abs(progress - newPosX);
	if (RIGHT) {
		SetConsoleCursorPosition(ins.hStdout, { (SHORT)(c1.x + progress), (SHORT)c1.y });
		for (int i = progress; i <= progress + distance; i++) {
			if (i == newPosX) { //c1.x +
				std::cout << pointer;
			}
			else if (i < newPosX) {
				std::cout << ch;
			}
		}
	}
	else {
		SetConsoleCursorPosition(ins.hStdout, { (SHORT)(c1.x + newPosX), (SHORT)c1.y });
		for (int i = newPosX; i <= newPosX + distance; i++) {
			if (i == newPosX) { //c1.x +
				std::cout << pointer;
			}
			else if (i > newPosX) {
				std::cout << "-";
			}
		}
	}
	SliderAction();
}

dUI::movingText::movingText(cords cc, cords ccc, std::string tx) {
	c1 = cc;
	c2 = ccc;
	lenght = (int)tx.length();
	originalText = tx + tx.substr(0, c2.x - c1.x);
	text = originalText.substr(0, c2.x - c1.x);
}

void dUI::movingText::change(std::string tx) {
	lenght = (int)tx.length();
	originalText = tx + tx.substr(0, c2.x - c1.x);
	text = originalText.substr(0, c2.x - c1.x);
}
void dUI::movingText::render() {
	auto ins = dUI::UIManager::Instance();
	if (lenght > c2.x - c1.x) {
		text = originalText.substr(index, c2.x - c1.x);
		index = (index + 1) % lenght;
		ins.print(c1, text);
	}
	else {
		ins.print(c1, originalText.substr(0, lenght) + std::string(c2.x - c1.x - lenght, ' '));
	}
}

dUI::keyEvent::keyEvent(WORD k, std::function<void()> a){
	key = k;
	Action = a;
}

dUI::Menu::Menu(cords cc1, cords cc2, std::function<void(cords)> ac, std::function<void()> fps) : c1(cc1), c2(cc2), action(ac), render(fps) {}