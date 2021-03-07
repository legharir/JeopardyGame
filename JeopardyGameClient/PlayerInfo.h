#pragma once

#include "Player.h"

#include <vector>
#include <map>

class PlayerInfo
{
public:
    PlayerInfo();

    const Player* getSelf() const;
    void initSelf(const std::string& playerName);

    const std::vector<const Player*> getPlayers() const;
    void addPlayer(const std::string& playerName);

    int getPlayerBalance(const std::string& playerName) const;
    void updatePlayerBalance(const std::string& playerName, int amount);

    const Player* getCluePicker() const;
    const Player* getClueResponder() const;
    void setCluePicker(const std::string& playerName);
    void setClueResponder(const std::string& playerName);

private:
    Player* m_self;

    using PlayerName = std::string;
    std::map<PlayerName, Player> m_players;
};
