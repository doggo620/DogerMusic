#include "dUI.hpp"
#include <vector>
#include <functional>
#include <iostream>
#include <Windows.h>

dUI::UIManager& dUI::UIManager::Instance() {
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