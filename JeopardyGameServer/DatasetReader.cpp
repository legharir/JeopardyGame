#include "DatasetReader.h"

#include "Utils.h"
#include "ResourcePath.hpp"

#include <fstream>
#include <iostream>

const std::string DatasetReader::m_basePath = resourcePath();
const unsigned int DatasetReader::m_cluesPerEpisode = 30 + 30 + 1;

std::vector<std::string> DatasetReader::getNextTokenizedLine(std::ifstream& dataset)
{
    std::string line;
    std::getline(dataset, line);
    return Utils::split(line, "\t");
}

Clue DatasetReader::getClueFromTokens(const std::vector<std::string>& tokens)
{
    const auto value = std::stoi(tokens[1]);
    const auto isDailyDouble = tokens[2] == "yes" ? true : false;
    const auto category = tokens[3];
    const auto answer = tokens[5];
    const auto question = tokens[6];
    return Clue(category, value, answer, question, isDailyDouble);
}

Round DatasetReader::getRoundFromTokens(const std::vector<std::string>& tokens)
{
    return (Round)std::stoi(tokens[0]);
}

std::string DatasetReader::getAirDateFromTokens(const std::vector<std::string>& tokens)
{
    return tokens[7];
}

std::unordered_map<Round, GameBoard, EnumClassHash> DatasetReader::getGameBoardsForShow(unsigned int season, unsigned int episode)
{
    std::ifstream dataset;
    const auto fileName = m_basePath + "season" + std::to_string(season) + ".tsv";
    dataset.open(fileName);

    if (dataset.fail())
    {
        std::cout << "Failed to open " << fileName << "!" << std::endl;
    }

    // The first line is always the header, so get rid of this.
    getNextTokenizedLine(dataset);

    unsigned int episodeCount = 0;
    std::string airDate;
    std::vector<std::string> tokens;

    // Advance to the point in the dataset where the clues for the specified
    // episode begin. Since the dataset doesn't contain episode #s, use the
    // air date of the episode to keep track of the current episode #.
    while (episodeCount < episode)
    {
        tokens = getNextTokenizedLine(dataset);
        const auto& curAirDate = getAirDateFromTokens(tokens);

        if (curAirDate != airDate)
            ++episodeCount;

        airDate = curAirDate;
    }

    std::unordered_map<Round, GameBoard, EnumClassHash> gameBoards;
    std::string curAirDate = airDate;

    // Read in all the clues for the episode and add each clue to its
    // corresponding game board.
    while (curAirDate == airDate)
    {
        const auto& clue = getClueFromTokens(tokens);
        const auto& round = getRoundFromTokens(tokens);
        gameBoards[round].addClue(clue);

        tokens = getNextTokenizedLine(dataset);
        curAirDate = getAirDateFromTokens(tokens);
    }

    return gameBoards;
}
