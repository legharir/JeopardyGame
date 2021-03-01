#include "SubRound.h"

SubRound::SubRound()
    : m_clue(nullptr)
    , m_responder(nullptr)
{
}

Clue* SubRound::getClue() const
{
    return m_clue;
}

Player* SubRound::getResponder() const
{
    return m_responder;
}

void SubRound::setClue(Clue* clue)
{
    m_clue = clue;
}

void SubRound::setResponder(Player* responder)
{
    m_responder = responder;
}
