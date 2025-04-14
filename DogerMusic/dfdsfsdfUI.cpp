#include "dUI.hpp"
#include <vector>
#include <functional>
#include <iostream>
#include <Windows.h>

class dUI::UIManager {
private:
	std::vector<std::shared_ptr<dUI::Button>> buttons;
	std::vector<std::shared_ptr<dUI::Animable>> animables;
	std::vector<std::shared_ptr<dUI::Animation>> animations;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
public:
	static dUI::UIManager& Instance(){
		static dUI::UIManager instance;
		return instance;
	}

	void addButton(std::shared_ptr<dUI::Button> button) {
		buttons.push_back(button);
	}
	void addAnimables(std::shared_ptr<dUI::Animable> animable) {
		animables.push_back(animable);
	}
	void addAnimation(std::shared_ptr<dUI::Animation> animation) {
		animations.push_back(animation);
	}
	
	void print(dUI::cords cd, std::string text) {
		SetConsoleCursorPosition(hStdout, { (SHORT)cd.x, (SHORT)cd.y });
		std::cout << text;
	}
};

class dUI::UIElement {
public:
	dUI::cords cords = { 0,0 };
	bool visible = true;
	bool toRender = false;

	std::string text;
	std::function<void()> Action;
};

class dUI::Button : public dUI::UIElement, public std::enable_shared_from_this<dUI::Button> {
public:
	Button(std::string txt,dUI::cords c, std::function<void()> ac)
		: UIElement{c, true, false, txt, ac} {
		cords = c;
		Action = ac;
		text = txt;
		dUI::UIManager::Instance().addButton(shared_from_this());
	}
};

class dUI::Animable : public dUI::UIElement, public std::enable_shared_from_this<dUI::Animable> {
public:
	Animable(std::string txt, dUI::cords c, std::function<void()> ac) {
		cords = c;
		Action = ac;
		text = txt;
		dUI::UIManager::Instance().addAnimables(shared_from_this());
	}
};