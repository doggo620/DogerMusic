#pragma once

#include <vector>
#include <functional>
#include <iostream>
#include <Windows.h>
#include <ctime>

namespace dUI {
	struct cords {
		int x, y;
	};
	class UIElement {
	public:
		cords cords = { 0,0 };
		bool visible = true;
		bool toRender = true;

		std::string text;
		std::function<void()> Action;
	};
	class Button : public UIElement {
	public:
		int length;
		Button(std::string, dUI::cords, std::function<void()>);
		static std::shared_ptr<Button> Create(std::string, dUI::cords, std::function<void()>);
	};
	class Animable : public UIElement {
	public:
		Animable(std::string, dUI::cords, std::function<void()>);
	};

	class movingText {
	public:
		cords c1;
		cords c2;
		std::string originalText;
		std::string text;
		int lenght;
		int index = 0;

		void change(std::string);
		movingText(cords, cords, std::string);
		void render();
	};

	class slider {
	public:
		cords c1;
		cords c2;
		int progress = 0;

		bool dragging = false;

		std::function<void()> Action = NULL;
		std::function<void()> SliderAction = NULL;
		std::string pointer;
		std::string ch;

		slider(cords, cords, std::string, std::string);

		void setAction(std::function<void()>);
		void setSliderAction(std::function<void()>);
		void update();
		void draw(int);
	};

	class keyEvent {
	public:
		WORD key;
		std::function<void()> Action = NULL;

		keyEvent(WORD, std::function<void()>);
	};

	class Menu {
	public:
		cords c1;
		cords c2;
		
		bool toRender = true;

		std::function<void(cords)> action;
		std::function<void()> render;

		bool enabled = false;

		Menu(cords, cords, std::function<void(cords)>, std::function<void()>);
	};

	class UIManager {
	private:
		std::vector<std::shared_ptr<Button>> buttons;
		std::vector<std::shared_ptr<Animable>> animables;
		std::vector<std::shared_ptr<slider>> sliders;
		std::vector<std::shared_ptr<movingText>> movings;
		std::vector<std::shared_ptr<keyEvent>> keyEvents;
		std::vector<std::shared_ptr<Menu>> Menus;

		time_t lastFrame = 0;

		//windows nwm
		HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
		INPUT_RECORD InputRecord;
		DWORD Events;
	public:
		int frame = 0;
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		static UIManager& Instance();
		double frameTime = 0;
		UIManager();
		bool canClick = true;
		void addButton(std::shared_ptr<Button>);
		void addSlider(std::shared_ptr<slider>);
		void addAnimables(std::shared_ptr<Animable>);
		void addMoving(std::shared_ptr<movingText>);
		void addKeyEvent(std::shared_ptr<keyEvent>);
		void addMenu(std::shared_ptr<Menu>);
		void print(cords, std::string);
		void divide(cords, cords, std::string);
		void setTitle(LPCSTR);
		void events();
		void clear();
		void render();
		void fpsCap();
		void waitForInput();
		void fixInput();
		std::string input();
		cords getSize();
	};
}