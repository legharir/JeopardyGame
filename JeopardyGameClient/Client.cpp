#include "Client.h"

#include "Engine.h"

#include <iostream>

Client::Client(Engine* engine, const std::string& host, const unsigned int port, std::queue<sf::Packet>& messageQueue) : m_host(host), m_port(port), m_messageQueue(messageQueue)
{
    m_game = engine;
    if (!connectToServer())
    {
        std::cout << "Could not establish a connection to the server!" << std::endl;
        throw;
    }
	m_receiveThread = std::thread([this] { this->receive(); });
}

Client::~Client()
{
    m_receiveThread.join();
}

bool Client::connectToServer()
{
    return m_socket.connect(m_host, m_port) == sf::Socket::Done;
}

void Client::send(sf::Packet& packet)
{
    m_socket.send(packet);
}

void Client::receive()
{
    std::cout << "Listening for server messages..." << std::endl;

    sf::SocketSelector selector;
    selector.add(m_socket);
    
    sf::Packet packet;
    while(m_game->isRunning())
    {
        if (selector.wait(sf::milliseconds(20)))
        {
            if (selector.isReady(m_socket) && m_socket.receive(packet) == sf::Socket::Done)
            {
                m_messageQueue.push(packet);
                packet.clear();
            }
        }
    }
}
