#include "dUI.hpp"
#include <vector>
#include <functional>
#include <iostream>
#include <Windows.h>

dUI::UIManager& dUI::UIManager::Instance() {
	SetConsoleOutputCP(CP_UTF8);
	static UIManager instance;
	return instance;
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

void dUI::UIManager::divideX(dUI::cords c, char t) {
	SetConsoleCursorPosition(hStdout, { 0, (SHORT)c.y });
	cords pos = dUI::UIManager::Instance().getSize();
	for (int i = 0; i < pos.x; i++) {
		std::cout << t;
	}
}

void dUI::UIManager::setTitle(LPCSTR t){
	SetConsoleTitleA(t);
}

dUI::cords dUI::UIManager::getSize(){
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;
	cords c;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	c.x = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	c.y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	return c;
}

dUI::Button::Button(std::string txt, dUI::cords c, std::function<void()> ac)
	: UIElement{ c, true, false, txt, ac } {
	cords = c;
	Action = ac;
	text = txt;
	dUI::UIManager::Instance().addButton(shared_from_this());
}

dUI::Animable::Animable(std::string txt, dUI::cords c, std::function<void()> ac) {
	cords = c;
	Action = ac;
	text = txt;
	dUI::UIManager::Instance().addAnimables(shared_from_this());
}