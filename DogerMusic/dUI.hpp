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

		std::string pointer;
		std::string ch;

		slider(cords,cords, std::string, std::string);

		void setAction(std::function<void()>);

		void update();
		void draw(int);
	};

	class Animation;
	class UIManager {
	private:
		std::vector<std::shared_ptr<Button>> buttons;
		std::vector<std::shared_ptr<Animable>> animables;
		std::vector<std::shared_ptr<Animation>> animations;
		std::vector<std::shared_ptr<slider>> sliders;
		std::vector<std::shared_ptr<movingText>> movings;

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
		void addAnimation(std::shared_ptr<Animation>);
		void addMoving(std::shared_ptr<movingText>);
		void print(cords, std::string);
		void divide(cords, cords, std::string);
		void setTitle(LPCSTR);
		void events();
		void clear();
		void render();
		void waitForInput();
		void fixInput();
		cords getSize();
	};
}