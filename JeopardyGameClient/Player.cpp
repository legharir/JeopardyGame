#include "Player.h"

Player::Player()
    : m_balance(0)
    , m_responding(false)
    , m_picking(false)
{
}

Player::Player(const std::string& playerName)
    : m_name(playerName)
    , m_balance(0)
    , m_responding(false)
	, m_picking(false)
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

bool Player::isPicking() const
{
    return m_picking;
}

bool Player::isResponding() const
{
    return m_responding;
}

void Player::updateBalance(int amount)
{
    m_balance += amount;
}

void Player::setPicking(bool picking)
{
    m_picking = picking;
}

void Player::setResponding(bool responding)
{
    m_responding = responding;
}
