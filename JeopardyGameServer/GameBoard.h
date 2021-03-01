#pragma once

#include "Clue.h"

#include <set>
#include <string>
#include <map>
#include <vector>

class GameBoard
{
public:
    GameBoard() = default;

    void addClue(const Clue& clue);

    using Category = std::string;
    using Value = unsigned int;

    const Clue& getClue(const std::string& category, unsigned int value) const;
    std::map<Category, std::map<Value, Clue>> getClues() const;

    Clue pickClue(const std::string& category, unsigned int value);
    unsigned int numCluesUnpicked() const;

    friend std::ostream& operator<<(std::ostream& out, const GameBoard& gameBoard);

private:
    // Allows using syntax like m_clues["harry potter"][400] to access
    // the clue corresponding to the "harry potter" category valued at $400.
    std::map<Category, std::map<Value, Clue>> m_clues;
};
