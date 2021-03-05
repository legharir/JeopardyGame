#include "Server.h"
#include "Game.h"
#include "GameMessage.h"

#include <iostream>

int main() {
	Game game;

	Server server(2223);
	server.setGame(&game);
	server.start();
}
