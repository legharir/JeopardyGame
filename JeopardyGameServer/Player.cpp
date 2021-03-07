#include "Player.h"

Player::Player()
    : m_socket(nullptr)
    , m_balance(0)
{
}

Player::Player(const std::string& name, sf::TcpSocket* socket)
    : m_name(name)
    , m_socket(socket)
    , m_balance(0)
{
}

std::string Player::getName() const
{
    return m_name;
}

void Player::setName(const std::string& name)
{
    m_name = name;
}

int Player::getBalance() const
{
    return m_balance;
}

void Player::updateBalance(int amount)
{
    m_balance += amount;
}

unsigned int Player::getWager() const
{
    return m_wager;
}

void Player::setWager(unsigned int wager)
{
    m_wager = wager;
}

const sf::TcpSocket* const Player::getSocket() const
{
    return m_socket;
}

void Player::send(sf::Packet& packet) const
{
    m_socket->send(packet);
}

bool Player::operator==(const Player& other) const
{
    return m_socket == other.getSocket();
}

bool Player::operator!=(const Player& other) const
{
    return m_socket != other.getSocket();
}

std::string Player::getFinalJeopardyResponse() const
{
    return m_finalJeopardyResponse;
}

void Player::setFinalJeopardyResponse(const std::string& response)
{
    m_finalJeopardyResponse = response;
}
