#include <iostream>
#include "dUI.hpp"

int main()
{
	dUI::UIManager& window = dUI::UIManager::Instance();

	window.setTitle("PinkMusic");


	window.print({(window.getSize().x / 2) - 5, 0}, u8"❥🔥PinkMusic <3");
	window.print({ (window.getSize().x ) - 13, 0 }, u8"🦅Version Uno");
	window.print({0, 0 }, u8"🍻Made by Doggo");
	window.divideX({ 0, 1 }, '-');

	return 0;
}