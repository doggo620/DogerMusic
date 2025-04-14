#pragma once

#include <vector>
#include <functional>
#include <iostream>
#include <Windows.h>

namespace dUI {
	struct cords {
		int x, y;
	};
	class UIElement {
	public:
		cords cords = { 0,0 };
		bool visible = true;
		bool toRender = false;

		std::string text;
		std::function<void()> Action;
	};
	class Button : public UIElement, public std::enable_shared_from_this<Button> {
	public:
		Button(std::string, dUI::cords, std::function<void()>);
	};
	class Animable : public UIElement, public std::enable_shared_from_this<Animable> {
	public:
		Animable(std::string, dUI::cords, std::function<void()>);
	};
	class Animation;
	class UIManager {
	private:
		std::vector<std::shared_ptr<Button>> buttons;
		std::vector<std::shared_ptr<Animable>> animables;
		std::vector<std::shared_ptr<Animation>> animations;
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	public:
		static UIManager& Instance();
		void addButton(std::shared_ptr<Button>);
		void addAnimables(std::shared_ptr<Animable>);
		void addAnimation(std::shared_ptr<Animation>);
		void print(cords cd, std::string);
	};
}