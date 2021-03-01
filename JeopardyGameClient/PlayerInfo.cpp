#include "PlayerInfo.h"

PlayerInfo::PlayerInfo()
    : m_self(nullptr)
{
}

void PlayerInfo::initSelf(const std::string& playerName)
{
    m_players[playerName] = Player(playerName);
    m_self = &m_players.at(playerName);
}

const Player* PlayerInfo::getSelf() const
{
    return m_self;
}

const std::vector<const Player*> PlayerInfo::getPlayers() const
{
    std::vector<const Player*> players;
    for (const auto& player : m_players)
        players.push_back(&player.second);
    return players;
}

void PlayerInfo::addPlayer(const std::string& playerName)
{
    m_players[playerName] = Player(playerName);
}

void PlayerInfo::updatePlayerBalance(const std::string& playerName, int amount)
{
    m_players.at(playerName).updateBalance(amount);
}

const Player* PlayerInfo::getCluePicker() const
{
    for (const auto& namePlayerPair : m_players)
    {
        const auto& player = namePlayerPair.second;
        if (player.isPicking())
			return &player;
    }
    return nullptr;
}

const Player* PlayerInfo::getClueResponder() const
{
    for (const auto& namePlayerPair : m_players)
    {
        const auto& player = namePlayerPair.second;
        if (player.isResponding())
            return &player;
    }
    return nullptr;
}

void PlayerInfo::setCluePicker(const std::string& playerName)
{
    for (auto& namePlayerMapping : m_players)
    {
        auto& name = namePlayerMapping.first;
        auto& player = namePlayerMapping.second;
        if (name == playerName)
            player.setPicking(true);
        else
			player.setPicking(false);
    }
}

void PlayerInfo::setClueResponder(const std::string& playerName)
{
    for (auto& namePlayerMapping : m_players)
    {
        auto& name = namePlayerMapping.first;
        auto& player = namePlayerMapping.second;
        if (name == playerName)
            player.setResponding(true);
        else
			player.setResponding(false);
    }
}

