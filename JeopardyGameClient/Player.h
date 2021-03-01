#pragma once

#include <SFML/Network.hpp>
#include <string>

class Player
{
public:
    Player();
    Player(const std::string& playerName);

    std::string getName() const;
    int getBalance() const;
    bool isPicking() const;
    bool isResponding() const;

    void setName(const std::string& name);
    void updateBalance(int amount);
    void setPicking(bool picking);
    void setResponding(bool responding);

private:
    std::string m_name;
    int m_balance;
    // True if the player is the current responder, false otherwise.
    bool m_responding;
    // True if the player is the current clue picker, false otherwise.
    bool m_picking;
};
