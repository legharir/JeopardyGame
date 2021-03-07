#include "GameMessage.h"
#include <iostream>

std::map<std::string, std::vector<PlayerBoard::Clue>> PlayerBoard::getClues() const
{
    return m_clues;
}

void PlayerBoard::addClue(const std::string& category, sf::Uint32 value)
{
    m_clues[category].push_back({ value, false });
}

unsigned int PlayerBoard::getNumClues() const
{
    unsigned int numClues = 0;
    for (const auto& categoryCluesMapping : m_clues)
        numClues += categoryCluesMapping.second.size();

    return numClues;
}

std::ostream& operator<<(std::ostream& output, const PlayerBoard& playerBoard)
{
    for (const auto& categoryCluesMapping : playerBoard.m_clues)
    {
        const auto& category = categoryCluesMapping.first;
        const auto& clues = categoryCluesMapping.second;

        for (const auto& clue : clues)
            output << category << ": " << clue.value << std::endl;
    }

    return output;
}

GameMessage::GameMessage(const MessageType& msgType)
    : type(msgType)
{
}

PlayerJoinedMessage::PlayerJoinedMessage()
    : GameMessage(MessageType::PLAYER_JOINED)
{
}

PlayerJoinedMessage::PlayerJoinedMessage(const std::string& playerName)
    : GameMessage(MessageType::PLAYER_JOINED)
    , playerName(playerName)
{
}

GameStartMessage::GameStartMessage()
    : GameMessage(MessageType::GAME_START)
    , season(0)
    , episode(0)
{
}

GameStartMessage::GameStartMessage(sf::Uint32 season, sf::Uint32 episode)
    : GameMessage(MessageType::GAME_START)
    , season(season)
    , episode(episode)
{
}

RoundStartMessage::RoundStartMessage()
    : GameMessage(MessageType::ROUND_START)
    , round(Round::JEOPARDY)
{
}

RoundStartMessage::RoundStartMessage(const Round& round, const PlayerBoard& playerBoard)
    : GameMessage(MessageType::ROUND_START)
    , round(round)
    , playerBoard(playerBoard)
{
}

CluePickingStartedMessage::CluePickingStartedMessage()
    : GameMessage(MessageType::CLUE_PICK_START)
{
}

CluePickingStartedMessage::CluePickingStartedMessage(const std::string& picker)
    : GameMessage(MessageType::CLUE_PICK_START)
    , picker(picker)
{
}

CluePickedMessage::CluePickedMessage()
    : GameMessage(MessageType::CLUE_PICKED)
    , value(0)
    , isDailyDouble(false)
{
}

CluePickedMessage::CluePickedMessage(const std::string& category, sf::Uint32 value, const std::string& answer,
                                     bool isDailyDouble)
    : GameMessage(MessageType::CLUE_PICKED)
    , category(category)
    , value(value)
    , answer(answer)
    , isDailyDouble(isDailyDouble)
{
}

DailyDoubleMessage::DailyDoubleMessage()
    : GameMessage(MessageType::DAILY_DOUBLE)
    , value(0)
{
}

DailyDoubleMessage::DailyDoubleMessage(const std::string& category, sf::Uint32 value)
    : GameMessage(MessageType::DAILY_DOUBLE)
    , category(category)
    , value(value)
{
}

WagerMessage::WagerMessage()
    : GameMessage(MessageType::WAGER)
    , wager(0)
    , responseDeadline(0)
{
}

WagerMessage::WagerMessage(sf::Uint32 wager)
    : GameMessage(MessageType::WAGER)
    , wager(wager)
    , responseDeadline(0)
{
}

WagerMessage::WagerMessage(sf::Uint32 wager, const std::string& answer, long long responseDeadline)
    : GameMessage(MessageType::WAGER)
    , wager(wager)
    , answer(answer)
    , responseDeadline(0)
{
}

BuzzerEnabledMessage::BuzzerEnabledMessage()
    : GameMessage(MessageType::BUZZER_ENABLED)
    , buzzInDeadline(0)
{
}

BuzzerEnabledMessage::BuzzerEnabledMessage(sf::Uint64 buzzInDeadline)
    : GameMessage(MessageType::BUZZER_ENABLED)
    , buzzInDeadline(buzzInDeadline)
{
}

BuzzInMessage::BuzzInMessage()
    : GameMessage(MessageType::BUZZ_IN)
    , responseDeadline(0)
{
}

BuzzInMessage::BuzzInMessage(const std::string& playerName, sf::Uint64 responseDeadline)
    : GameMessage(MessageType::BUZZ_IN)
    , playerName(playerName)
    , responseDeadline(responseDeadline)
{
}

PartialClueResponseMessage::PartialClueResponseMessage()
    : GameMessage(MessageType::PARTIAL_CLUE_RESPONSE)
    , character(0)
{
}

PartialClueResponseMessage::PartialClueResponseMessage(sf::Uint32 character)
    : GameMessage(MessageType::PARTIAL_CLUE_RESPONSE)
    , character(character)
{
}

ClueResponseMessage::ClueResponseMessage()
    : GameMessage(MessageType::CLUE_RESPONSE)
{
}

ClueResponseMessage::ClueResponseMessage(const std::string& response)
    : GameMessage(MessageType::CLUE_RESPONSE)
    , response(response)
{
}

ResponseReceivedMessage::ResponseReceivedMessage()
    : GameMessage(MessageType::RESPONSE_RECEIVED)
    , responseAccepted(false)
    , balanceDelta(0)
{
}

ResponseReceivedMessage::ResponseReceivedMessage(const std::string& responder, bool responseAccepted,
                                                 sf::Int32 balanceDelta)
    : GameMessage(MessageType::RESPONSE_RECEIVED)
    , responder(responder)
    , responseAccepted(responseAccepted)
    , balanceDelta(balanceDelta)
{
}

SubRoundEndedMessage::SubRoundEndedMessage()
    : GameMessage(MessageType::SUB_ROUND_ENDED)
{
}

SubRoundEndedMessage::SubRoundEndedMessage(const std::string& question)
    : GameMessage(MessageType::SUB_ROUND_ENDED)
    , question(question)
{
}

FinalJeopardyMessage::FinalJeopardyMessage()
    : GameMessage(MessageType::FINAL_JEOPARDY)
    , wagerDeadline(0)
{
}

FinalJeopardyMessage::FinalJeopardyMessage(const std::string& category, long long wagerDeadline)
    : GameMessage(MessageType::FINAL_JEOPARDY)
    , category(category)
    , wagerDeadline(wagerDeadline)
{
}

FinalJeopardyEndMessage::FinalJeopardyEndMessage()
    : GameMessage(MessageType::FINAL_JEOPARDY_END)
{
}

FinalJeopardyResultsMessage::FinalJeopardyResultsMessage()
    : GameMessage(MessageType::FINAL_JEOPARDY_RESULTS)
{
}

sf::Packet& operator<<(sf::Packet& packet, const MessageType& type)
{
    return packet << (sf::Uint32)type;
}

sf::Packet& operator>>(sf::Packet& packet, MessageType& type)
{
    int msgType;
    packet >> msgType;
    type = (MessageType)msgType;
    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const Round& round)
{
    return packet << (sf::Uint32)round;
}

sf::Packet& operator>>(sf::Packet& packet, Round& round)
{
    sf::Uint32 roundValue;
    packet >> roundValue;
    round = (Round)roundValue;
    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const PlayerBoard& playerBoard)
{
    packet << playerBoard.getNumClues();

    for (const auto& categoryCluesMapping : playerBoard.getClues())
    {
        const auto& category = categoryCluesMapping.first;
        const auto& clues = categoryCluesMapping.second;

        for (const auto& clue : clues)
            packet << category << clue.value;
    }

    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, PlayerBoard& playerBoard)
{
    sf::Uint32 numClues;
    packet >> numClues;

    for (sf::Uint32 i = 0; i < numClues; ++i)
    {
        std::string category;
        sf::Uint32 value;
        packet >> category >> value;
        playerBoard.addClue(category, value);
    }

    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const PlayerJoinedMessage& msg)
{
    return packet << msg.type << msg.playerName;
}

sf::Packet& operator>>(sf::Packet& packet, PlayerJoinedMessage& msg)
{
    return packet >> msg.playerName;
}

sf::Packet& operator<<(sf::Packet& packet, const GameStartMessage& msg)
{
    return packet << msg.type << msg.season << msg.episode;
}

sf::Packet& operator>>(sf::Packet& packet, GameStartMessage& msg)
{
    return packet >> msg.season >> msg.episode;
}

sf::Packet& operator<<(sf::Packet& packet, const RoundStartMessage& msg)
{
    return packet << msg.type << msg.round << msg.playerBoard;
}

sf::Packet& operator>>(sf::Packet& packet, RoundStartMessage& msg)
{
    return packet >> msg.round >> msg.playerBoard;
}

sf::Packet& operator<<(sf::Packet& packet, const CluePickingStartedMessage& msg)
{
    return packet << msg.type << msg.picker;
}

sf::Packet& operator>>(sf::Packet& packet, CluePickingStartedMessage& msg)
{
    return packet >> msg.picker;
}

sf::Packet& operator<<(sf::Packet& packet, const CluePickedMessage& msg)
{
    return packet << msg.type << msg.category << msg.value << msg.answer << msg.isDailyDouble;
}

sf::Packet& operator>>(sf::Packet& packet, CluePickedMessage& msg)
{
    return packet >> msg.category >> msg.value >> msg.answer >> msg.isDailyDouble;
}

sf::Packet& operator<<(sf::Packet& packet, const BuzzerEnabledMessage& msg)
{
    return packet << msg.type << msg.buzzInDeadline;
}

sf::Packet& operator>>(sf::Packet& packet, BuzzerEnabledMessage& msg)
{
    return packet >> msg.buzzInDeadline;
}

sf::Packet& operator<<(sf::Packet& packet, const BuzzInMessage& msg)
{
    return packet << msg.type << msg.playerName << msg.responseDeadline;
}

sf::Packet& operator>>(sf::Packet& packet, BuzzInMessage& msg)
{
    return packet >> msg.playerName >> msg.responseDeadline;
}

sf::Packet& operator<<(sf::Packet& packet, const PartialClueResponseMessage& msg)
{
    return packet << msg.type << msg.character;
}

sf::Packet& operator>>(sf::Packet& packet, PartialClueResponseMessage& msg)
{
    return packet >> msg.character;
}

sf::Packet& operator<<(sf::Packet& packet, const ClueResponseMessage& msg)
{
    return packet << msg.type << msg.response;
}

sf::Packet& operator>>(sf::Packet& packet, ClueResponseMessage& msg)
{
    return packet >> msg.response;
}

sf::Packet& operator<<(sf::Packet& packet, const ResponseReceivedMessage& msg)
{
    return packet << msg.type << msg.responder << msg.responseAccepted << msg.balanceDelta;
}

sf::Packet& operator>>(sf::Packet& packet, ResponseReceivedMessage& msg)
{
    return packet >> msg.responder >> msg.responseAccepted >> msg.balanceDelta;
}

sf::Packet& operator<<(sf::Packet& packet, const SubRoundEndedMessage& msg)
{
    return packet << msg.type << msg.question;
}

sf::Packet& operator>>(sf::Packet& packet, SubRoundEndedMessage& msg)
{
    return packet >> msg.question;
}

sf::Packet& operator<<(sf::Packet& packet, const DailyDoubleMessage& msg)
{
    return packet << msg.type << msg.category << msg.value;
}

sf::Packet& operator>>(sf::Packet& packet, DailyDoubleMessage& msg)
{
    return packet >> msg.category >> msg.value;
}

sf::Packet& operator<<(sf::Packet& packet, const WagerMessage& msg)
{
    return packet << msg.type << msg.wager << msg.answer << msg.responseDeadline;
}

sf::Packet& operator>>(sf::Packet& packet, WagerMessage& msg)
{
    return packet >> msg.wager >> msg.answer >> msg.responseDeadline;
}

sf::Packet& operator<<(sf::Packet& packet, const FinalJeopardyMessage& msg)
{
    return packet << msg.type << msg.category << msg.wagerDeadline;
}

sf::Packet& operator>>(sf::Packet& packet, FinalJeopardyMessage& msg)
{
    return packet >> msg.category >> msg.wagerDeadline;
}

sf::Packet& operator<<(sf::Packet& packet, const FinalJeopardyEndMessage& msg)
{
    return packet << msg.type;
}

sf::Packet& operator>>(sf::Packet& packet, FinalJeopardyEndMessage& msg)
{
    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const FinalJeopardyResult& msg)
{
    return packet << msg.playerName << msg.response << msg.correct << msg.wager << msg.finalBalance;
}

sf::Packet& operator>>(sf::Packet& packet, FinalJeopardyResult& msg)
{
    return packet >> msg.playerName >> msg.response >> msg.correct >> msg.wager >> msg.finalBalance;
}

sf::Packet& operator<<(sf::Packet& packet, const FinalJeopardyResultsMessage& msg)
{
    packet << msg.type << msg.question;
    packet << (unsigned int)msg.results.size();
    for (const auto result : msg.results)
        packet << result;
    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, FinalJeopardyResultsMessage& msg)
{
    std::string question;
    packet >> msg.question;

    sf::Uint32 numResults;
    packet >> numResults;
    
    for (int i = 0u; i < numResults; ++i)
    {
        FinalJeopardyResult result;
        packet >> result;
        msg.results.push_back(result);
    }
    
    return packet;
}
