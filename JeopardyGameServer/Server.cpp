#include "Server.h"
#include "Game.h"
#include "GameMessage.h"

#include <SFML/Network.hpp>

#include <iostream>

Server::Server(const unsigned int port)
    : m_port(port)
    , m_game(nullptr)
{
}

Server::~Server()
{
    m_listenThread.join();
}

void Server::setGame(Game* game)
{
    m_game = game;
}

void Server::start()
{
    m_game->init(this);
    m_listenThread = std::thread([this] { this->listen(); });

    while (true)
    {
        // If there are any messages in the queue, get the game to handle them.
        if (!m_messageQueue.empty())
        {
            auto& clientMessage = m_messageQueue.front();
            const auto& socket = clientMessage.first;
            auto& packet = clientMessage.second;
            m_game->handleMessage(socket, packet);
            m_messageQueue.pop();
        }

        m_game->update();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void Server::listen()
{
    std::cout << "Listening for client requests..." << std::endl;

    sf::TcpListener listener;
    listener.listen(m_port);

    sf::SocketSelector selector;
    selector.add(listener);

    // Wait until a client connection/message is received.
    while (selector.wait())
    {
        // Check for new client connections.
        if (selector.isReady(listener))
        {
            std::cout << "New client connection received" << std::endl;
            m_clients.emplace_back();

            if (listener.accept(m_clients.back()) != sf::Socket::Done)
            {
                m_clients.pop_back();
            }
            else
            {
                std::cout << "A client has connected: " << m_clients.back().getRemoteAddress() << ":"
                          << m_clients.back().getRemotePort() << std::endl;
                selector.add(m_clients.back());
            }
        }

        // Check for messages from connected clients.
        for (auto& client : m_clients)
        {
            if (selector.isReady(client))
            {
                // The selector is ready, we can safely receive the message without blocking.
                sf::Packet packet;
                client.receive(packet);
                m_messageQueue.push({ &client, packet });
            }
        }
    }
}
