#pragma once

#include <SFML/Network.hpp>
#include <list>
#include <thread>
#include <queue>

class Game;

class Server
{
public:
	Server(unsigned int port);
	~Server();
	
	void start();

	void setGame(Game* game);
private:
	unsigned int m_port;
	std::list<sf::TcpSocket> m_clients;

	std::thread m_listenThread;
	void listen();

	std::queue<std::pair<sf::TcpSocket*, sf::Packet>> m_messageQueue;

	Game* m_game;
};
