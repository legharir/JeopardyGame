#pragma once

#include <string>
#include <SFML/Network.hpp>

class Player
{
public:
    Player();
	Player(const std::string& name, sf::TcpSocket* socket);

	std::string getName() const;
	void setName(const std::string& name);

	int getBalance() const;
	void updateBalance(int amount);

	unsigned int getWager() const;
	void setWager(unsigned int wager);

	const sf::TcpSocket* const getSocket() const;

	void send(sf::Packet& packet) const;

	bool operator==(const Player& other) const;
	bool operator!=(const Player& other) const;

private:
	std::string m_name;
	int m_balance;
	unsigned int m_wager;

	// Socket used to communicate with the player.
	sf::TcpSocket* m_socket;
};
