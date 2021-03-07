#include "Engine.h"

#include "GameMessage.h"
#include "IntroState.h"

#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <chrono>


int main()
{
	Engine game;
	game.changeState(IntroState::getInstance());

	while (game.isRunning())
	{
		game.handleEvents();
		game.update();
		game.draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}
