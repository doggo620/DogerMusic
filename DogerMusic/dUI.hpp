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
	class Animation;
	class UIManager {
	private:
		std::vector<std::shared_ptr<Button>> buttons;
		std::vector<std::shared_ptr<Animable>> animables;
		std::vector<std::shared_ptr<Animation>> animations;

		time_t lastFrame = 0;

		//windows nwm
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
		INPUT_RECORD InputRecord;
		DWORD Events;
	public:
		static UIManager& Instance();
		double frameTime = 0;
		UIManager();
		bool canClick = true;
		void addButton(std::shared_ptr<Button>);
		void addAnimables(std::shared_ptr<Animable>);
		void addAnimation(std::shared_ptr<Animation>);
		void print(cords, std::string);
		void divideX(cords, char);
		void setTitle(LPCSTR);
		void events();
		void render();
		cords getSize();
	};
}