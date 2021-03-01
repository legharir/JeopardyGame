#pragma once

#include "Player.h"
#include "Clue.h"

class SubRound
{
public:
    SubRound();

    Clue* getClue() const;
    Player* getResponder() const;

    void setClue(Clue* clue);
    void setResponder(Player* responder);

private:
    Clue* m_clue;
    Player* m_responder;
};
