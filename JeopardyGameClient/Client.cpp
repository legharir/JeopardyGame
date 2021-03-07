#include "Client.h"

#include "Engine.h"

#include <iostream>

Client::Client(Engine* engine, const std::string& host, const unsigned int port, std::queue<sf::Packet>& messageQueue) : m_host(host), m_port(port), m_messageQueue(messageQueue)
{
    m_game = engine;
    connectToServer();
	m_receiveThread = std::thread([this] { this->receive(); });
}

Client::~Client()
{
    m_receiveThread.join();
}

void Client::connectToServer()
{
    std::cout << "Connecting to server on port " << m_port << std::endl;
    const auto status = m_socket.connect(m_host, m_port);
    switch (status) {
        case sf::Socket::Status::Done:
            std::cout << "Done" << std::endl;
            break;
        case sf::Socket::Status::Partial:
            std::cout << "Could not connect to server! socket status: Partial" << std::endl;
            throw;
        case sf::Socket::Status::NotReady:
            std::cout << "Could not connect to server! socket status: NotReady" << std::endl;
            throw;
        case sf::Socket::Status::Error:
            std::cout << "Could not connect to server! socket status: Error" << std::endl;
            throw;
        case sf::Socket::Status::Disconnected:
            std::cout << "Could not connect to server! socket status: Disconnected" << std::endl;
            throw;
        default:
            break;
    }
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
