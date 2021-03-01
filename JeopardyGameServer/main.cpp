#include "Server.h"
#include "Game.h"
#include "GameMessage.h"

#include <iostream>

int main() {
	Game game;

	Server server(2222);
	server.setGame(&game);
	server.start();
}