// Knows how to read the Jeopardy dataset files to create game boards objects.

#pragma once

#include "Clue.h"
#include "Game.h"
#include "GameBoard.h"

#include <fstream>
#include <string>
#include <vector>

class DatasetReader
{
public:
    // Creates and returns the Jeopardy!, Double Jeopardy!, and Final Jeopardy!
    // game boards for the specified show.
    static std::unordered_map<Round, GameBoard, EnumClassHash> getGameBoardsForShow(unsigned int season, unsigned int episode);

private:
    // Returns the tokens resulting from splitting the next line by a \t
    // delimiter.
    static std::vector<std::string> getNextTokenizedLine(std::ifstream& dataset);

    static Clue getClueFromTokens(const std::vector<std::string>& tokens);
    static Round getRoundFromTokens(const std::vector<std::string>& tokens);
    static std::string getAirDateFromTokens(const std::vector<std::string>& tokens);

    static const unsigned int m_cluesPerEpisode;
    static const std::string m_basePath;
};
