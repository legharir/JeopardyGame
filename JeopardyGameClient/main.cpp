#include "Engine.h"

#include "GameMessage.h"
#include "IntroState.h"

#include <SFML/Network.hpp>
#include <iostream>

int main()
{
	Engine game;
	game.changeState(IntroState::getInstance());

	while (game.isRunning())
	{
		game.handleEvents();
		game.update();
		game.draw();
	}
}
