#pragma once

#include <SFML/Network.hpp>

#include <string>
#include <queue>
#include <thread>

class Engine;

class Client
{
public:
	Client(Engine* engine, const std::string& host, const unsigned int port, std::queue<sf::Packet>& messageQueue);
	~Client();

	void connectToServer();
	
	void send(sf::Packet& packet);

private:
    Engine* m_game;
	std::string m_host;
	unsigned int m_port;

	sf::TcpSocket m_socket;

	// Queue to store all the game messages in.
	std::queue<sf::Packet>& m_messageQueue;

	// Thread that receives any messages from the game server.
	std::thread m_receiveThread;
    void receive();
};

