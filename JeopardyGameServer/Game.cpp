#include "Game.h"

#include "DatasetReader.h"
#include "SGameState.h"
#include "SIntroState.h"
#include "Utils.h"

#include <SFML/Network.hpp>
#include <future>
#include <iostream>
#include <thread>

Game::Game()
    : m_running(true)
    , m_server(nullptr)
    , m_curRound(Round::JEOPARDY)
    , m_buzzInDeadline(0LL)
    , m_responseDeadline(0LL)
    , m_wagerDeadline(0LL)
    , m_buzzerReadyDelay(2000)
    , m_buzzInWindowLength(4000)
    , m_responseWindowLength(8000)
    , m_finalJeopardyWagerWindowLength(20000)
    , m_curState(nullptr)
    , m_picker(nullptr)
    , m_responder(nullptr)
{
}

void Game::init(Server* server)
{
    m_server = server;
    const auto& introState = SIntroState::getInstance();
    changeState(introState);
}

void Game::update()
{
    m_curState->update();
}

bool Game::isRunning() const
{
    return m_running;
}

void Game::handleMessage(sf::TcpSocket* socket, sf::Packet& packet)
{
    MessageType type;
    packet >> type;

    // Depending on the type of message, send it to the correct handler.
    switch (type)
    {
        case MessageType::PLAYER_JOINED:
        {
            PlayerJoinedMessage msg;
            packet >> msg;
            std::cout << "Received PLAYER_JOINED msg: " << msg.playerName << std::endl;
            m_curState->handlePlayerJoined(msg, socket);
            break;
        }
        case MessageType::GAME_START:
        {
            GameStartMessage msg;
            packet >> msg;
            std::cout << "Received GAME_START msg: " << msg.season << " " << msg.episode << std::endl;
            m_curState->handleGameStart(msg);
            break;
        }
        case MessageType::CLUE_PICKED:
        {
            CluePickedMessage msg;
            packet >> msg;
            std::cout << "Received CLUE_PICKED msg: " << msg.category << " for " << msg.value << std::endl;
            m_curState->handleCluePicked(msg, getPlayer(socket));
            break;
        }
        case MessageType::WAGER:
        {
            WagerMessage msg;
            packet >> msg;
            std::cout << "Received WAGER msg: " << msg.wager << std::endl;
            m_curState->handleWager(msg, getPlayer(socket));
            break;
        }
        case MessageType::BUZZ_IN:
        {
            BuzzInMessage msg;
            packet >> msg;
            std::cout << "Received BUZZ_IN msg: " << msg.playerName << std::endl;
            m_curState->handleBuzzIn(getPlayer(socket));
            break;
        }
        case MessageType::PARTIAL_CLUE_RESPONSE:
        {
            PartialClueResponseMessage partialClueResponseMessage;
            packet >> partialClueResponseMessage;
            m_curState->handlePartialClueResponse(partialClueResponseMessage, getPlayer(socket));
            break;
        }
        case MessageType::CLUE_RESPONSE:
        {
            ClueResponseMessage clueResponseMessage;
            packet >> clueResponseMessage;
            m_curState->handleClueResponse(clueResponseMessage, getPlayer(socket));
            break;
        }
        default:
            break;
    }
}

std::vector<std::string> Game::getPlayerNames()
{
    std::vector<std::string> playerNames;
    for (const auto& player : m_players)
        playerNames.push_back(player.second.getName());
    return playerNames;
}

void Game::addPlayer(sf::TcpSocket* socket, const std::string& newPlayerName)
{
    const auto& existingNames = getPlayerNames();
    m_players[socket] = Player(newPlayerName, socket);

    // Give the new player the list of existing players.
    for (const auto& playerName : existingNames)
    {
        sf::Packet packet;
        PlayerJoinedMessage existingPlayerMessage(playerName);
        packet << existingPlayerMessage;
        socket->send(packet);
    }

    // Notify all players that a new player joined.
    sf::Packet packet;
    PlayerJoinedMessage newPlayerMessage(newPlayerName);
    packet << newPlayerMessage;
    broadcast(packet);
}

void Game::broadcast(sf::Packet& packet) const
{
    for (const auto& player : m_players)
        player.first->send(packet);
}

bool Game::hasPicker() const
{
    return m_picker;
}

std::string Game::getPickerName() const
{
    return m_picker->getName();
}

std::string Game::getResponderName() const
{
    return m_responder->getName();
}

const std::set<std::string>& Game::getResponded() const
{
    return m_responded;
}

const Clue& Game::getClue(const std::string& category, unsigned int value) const
{
    return m_gameBoards.at(m_curRound).getClue(category, value);
}

const Clue& Game::getCurClue() const
{
    return m_curClue;
}

Round Game::getCurRound() const
{
    return m_curRound;
}

unsigned int Game::numCluesUnpicked() const
{
    return m_gameBoards.at(m_curRound).numCluesUnpicked();
}

long long Game::getBuzzInDeadline() const
{
    return m_buzzInDeadline;
}

unsigned int Game::getBuzzerReadyDelay() const
{
    return m_buzzerReadyDelay;
}

long long Game::getResponseDeadline() const
{
    return m_responseDeadline;
}

bool Game::allPlayersResponded() const
{
    return m_responded.size() == m_players.size();
}

void Game::startGame(unsigned int season, unsigned int episode)
{
    m_gameBoards = DatasetReader::getGameBoardsForShow(season, episode);
    createPlayerBoards(m_gameBoards);

    GameStartMessage gameStartMessage(season, episode);
    sf::Packet packet;
    packet << gameStartMessage;
    broadcast(packet);
}

void Game::endGame()
{
}

void Game::startRound(Round round)
{
    m_curRound = round;

    sf::Packet packet;
    RoundStartMessage roundStartMessage(round, m_playerBoards.at(round));
    packet << roundStartMessage;
    broadcast(packet);
}

void Game::startCluePickingPhase()
{
    // Assign a random player to be the picker if there isn't one already assigned.
    // This case should only occur at the beginning of the game.
    if (!m_picker)
        m_picker = &m_players.begin()->second;

    sf::Packet packet;
    CluePickingStartedMessage cluePickingStartedMessage(m_picker->getName());
    packet << cluePickingStartedMessage;
    broadcast(packet);
}

void Game::endSubRound()
{
    m_responded.clear();

    sf::Packet packet;
    SubRoundEndedMessage subRoundEndedMessage(m_curClue.getQuestion());
    packet << subRoundEndedMessage;
    broadcast(packet);
}

void Game::assignResponder(const std::string& responder)
{
}

void Game::onCluePicked(const std::string& category, unsigned int value)
{
    m_curClue = m_gameBoards.at(m_curRound).pickClue(category, value);

    sf::Packet packet;
    CluePickedMessage cluePickedMessage(m_curClue.getCategory(), m_curClue.getValue(), m_curClue.getAnswer(),
                                        m_curClue.isDailyDouble());
    packet << cluePickedMessage;
    broadcast(packet);
}

void Game::onDailyDoublePicked(const std::string& playerName, const std::string& category, unsigned int value)
{
    m_responder = getPlayer(playerName);
    m_curClue = m_gameBoards.at(m_curRound).pickClue(category, value);

    sf::Packet packet;
    DailyDoubleMessage dailyDoubleMessage(category, value);
    packet << dailyDoubleMessage;
    broadcast(packet);
}

void Game::onWagerSubmitted(const std::string& playerName, unsigned int wager)
{
    auto player = getPlayer(playerName);
    player->setWager(wager);
    m_responseDeadline = Utils::getMsSinceEpoch() + 15000;

    sf::Packet packet;
    WagerMessage wagerMessage;
    wagerMessage.answer = m_curClue.getAnswer();
    wagerMessage.responseDeadline = m_responseDeadline;
    packet << wagerMessage;
    broadcast(packet);
}

void Game::enableBuzzers()
{
    m_buzzInDeadline = Utils::getMsSinceEpoch() + m_buzzInWindowLength;

    // Let the players know they can buzz in now.
    sf::Packet packet;
    BuzzerEnabledMessage buzzerEnabledMessage(m_buzzInDeadline);
    packet << buzzerEnabledMessage;
    broadcast(packet);
}

void Game::onBuzzIn(const std::string& playerName)
{
    // Set the responder as the player who buzzed in.
    m_responded.insert(playerName);
    m_responder = getPlayer(playerName);
    m_responseDeadline = Utils::getMsSinceEpoch() + m_responseWindowLength;

    // Let all players know that a player buzzed in and now has until the deadline to respond.
    BuzzInMessage buzzInMessage(playerName, m_responseDeadline);
    sf::Packet packet;
    packet << buzzInMessage;
    broadcast(packet);
}

void Game::onClueResponse(bool isResponseCorrect)
{
    unsigned int value = m_curClue.isDailyDouble() ? m_responder->getWager() : m_curClue.getValue();
    ResponseReceivedMessage responseReceivedMessage;
    responseReceivedMessage.responder = m_responder->getName();

    // Depending on whether the response was correct, update the player balance
    // accordingly and notify the players of the response result and the delta
    // applied to the responder's balance.
    if (isResponseCorrect)
    {
        m_responder->updateBalance(value);
        responseReceivedMessage.responseAccepted = true;
        responseReceivedMessage.balanceDelta = value;
        // If they responded correctly, they get to pick the next clue.
        m_picker = m_responder;
    }
    else
    {
        m_responder->updateBalance(value * -1);
        responseReceivedMessage.responseAccepted = false;
        responseReceivedMessage.balanceDelta = value * -1;
    }

    sf::Packet packet;
    packet << responseReceivedMessage;
    broadcast(packet);
}

void Game::onResponseDeadlineExceeded()
{
    // Decrement the responder's point by the value of the current clue.
    const auto value = m_curClue.getValue();
    m_responder->updateBalance(value * -1);

    // Let the players know about the incorrect response.
    ResponseReceivedMessage responseReceivedMessage(m_responder->getName(), false, value);
    sf::Packet packet;
    packet << responseReceivedMessage;
    broadcast(packet);
}

void Game::onPartialClueResponse(const std::string& playerName, int partialResponse)
{
    // Ignore this message if it wasn't sent by the player currently responding to the clue.
    if (playerName != m_responder->getName())
        return;

    // Let all the players know that the responder just typed in a character
    // into the response field.
    PartialClueResponseMessage partialClueResponseMessage(partialResponse);
    sf::Packet packet;
    packet << partialClueResponseMessage;
    broadcast(packet);
}

void Game::onFinalJeopardyStart()
{
    // Get the category of the final Jeopardy! round and send it to the players.
    const auto& clues = m_gameBoards.at(m_curRound).getClues();
    const auto& category = clues.begin()->first;

    m_wagerDeadline = Utils::getMsSinceEpoch() + m_finalJeopardyWagerWindowLength;

    FinalJeopardyMessage finalJeopardyMessage;
    finalJeopardyMessage.category = category;
    sf::Packet packet;
    packet << finalJeopardyMessage;
    broadcast(packet);
}

void Game::createPlayerBoards(const std::unordered_map<Round, GameBoard, EnumClassHash>& gameBoards)
{
    // Create the player boards for each game board.
    for (const auto& roundGameBoardMapping : gameBoards)
    {
        const auto& round = roundGameBoardMapping.first;
        const auto& gameBoard = roundGameBoardMapping.second;

        for (const auto& cluesForCategory : gameBoard.getClues())
        {
            const auto& category = cluesForCategory.first;

            for (const auto& valueClue : cluesForCategory.second)
            {
                const auto& value = valueClue.first;
                m_playerBoards[round].addClue(category, value);
            }
        }
        std::cout << m_playerBoards[round] << std::endl;
    }
}

void Game::changeState(SGameState* state)
{
    m_curState = state;
    m_curState->init(this);
}

const Player& Game::getPlayer(sf::TcpSocket* socket) const
{
    return m_players.at(socket);
}

Player* Game::getPlayer(const std::string& playerName)
{
    for (auto& player : m_players)
    {
        if (player.second.getName() == playerName)
            return &player.second;
    }
    return nullptr;
}
