#include "GameBoard.h"
#include "Utils.h"

#include <algorithm>
#include <iostream>

void GameBoard::addClue(const Clue& clue)
{
    const auto category = clue.getCategory();
    const auto value = clue.getValue();
    m_clues[category][value] = clue;
}

const Clue& GameBoard::getClue(const std::string& category, unsigned int value) const
{
    return m_clues.at(category).at(value);
}

std::map<std::string, std::map<unsigned int, Clue>> GameBoard::getClues() const
{
    return m_clues;
}

Clue GameBoard::pickClue(const std::string& category, unsigned int value)
{
    auto& clue = m_clues.at(category).at(value);
    clue.pick();
    return clue;
}

unsigned int GameBoard::numCluesUnpicked() const
{
    unsigned int numCluesUnpicked = 0;
    for (const auto& categoryValueClueMapping : m_clues)
    {
        const auto& valueClueMapping = categoryValueClueMapping.second;
        for (const auto& valueCluePair : valueClueMapping)
        {
            const auto& clue = valueCluePair.second;
            if (!clue.isPicked())
                ++numCluesUnpicked;
        }
    }

    return numCluesUnpicked;
}

std::ostream& operator<<(std::ostream& out, const GameBoard& gameBoard)
{
    for (const auto& cluesForCategory : gameBoard.m_clues)
    {
        for (const auto& valueClueMapping : cluesForCategory.second)
        {
            const auto& clue = valueClueMapping.second;
            out << clue;
        }
        out << std::endl;
    }
    return out;
}
