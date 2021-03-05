#include "Engine.h"
#include "GameState.h"

#include <algorithm>
#include <iostream>

Engine::Engine()
    : m_running(true)
    , m_window(sf::VideoMode(1280, 720), "Jeopardy!")
    , m_client(this, "localhost", 2223, m_messageQueue)
    , m_event()
{
}

bool Engine::isRunning()
{
    return m_running;
}

void Engine::handleEvents()
{
    // In general, the handling of events is delegated to the "current" state.
    // This corresponds to the state on the top of the state stack.

    // Check for SFML events (mouse clicks, key presses, etc).
    if (m_window.pollEvent(m_event))
    {
        if (m_event.type == sf::Event::Closed)
        {
            m_window.close();
            m_running = false;
        }
        else
        {
            m_states.back()->handleEvent(m_event);
        }
    }

    // Check for messages from the game server.
    if (!m_messageQueue.empty())
    {
        sf::Packet packet = m_messageQueue.front();
        MessageType type;
        packet >> type;

        switch (type)
        {
            case MessageType::PLAYER_JOINED:
            {
                PlayerJoinedMessage message;
                packet >> message;
                m_states.back()->handlePlayerJoined(message);
                break;
            }
            case MessageType::GAME_START:
            {
                GameStartMessage message;
                packet >> message;
                m_states.back()->handleGameStart(message);
                break;
            }
            case MessageType::ROUND_START:
            {
                RoundStartMessage message;
                packet >> message;
                m_states.back()->handleRoundStart(message);
                break;
            }
            case MessageType::CLUE_PICK_START:
            {
                CluePickingStartedMessage message;
                packet >> message;
                m_states.back()->handleCluePickingStarted(message);
                break;
            }
            case MessageType::CLUE_PICKED:
            {
                CluePickedMessage message;
                packet >> message;
                m_states.back()->handleCluePicked(message);
                break;
            }
            case MessageType::WAGER:
            {
                WagerMessage message;
                packet >> message;
                m_states.back()->handleWagerReceived(message);
                break;
            }
            case MessageType::DAILY_DOUBLE:
            {
                DailyDoubleMessage message;
                packet >> message;
                m_states.back()->handleDailyDouble(message);
                break;
            }
            case MessageType::BUZZER_ENABLED:
            {
                BuzzerEnabledMessage message;
                packet >> message;
                m_states.back()->handleBuzzerEnabled(message);
                break;
            }
            case MessageType::BUZZ_IN:
            {
                BuzzInMessage message;
                packet >> message;
                m_states.back()->handleBuzzIn(message);
                break;
            }
            case MessageType::PARTIAL_CLUE_RESPONSE:
            {
                PartialClueResponseMessage message;
                packet >> message;
                m_states.back()->handlePartialClueResponse(message);
                break;
            }
            case MessageType::RESPONSE_RECEIVED:
            {
                ResponseReceivedMessage message;
                packet >> message;
                m_states.back()->handleResponseReceived(message);
                break;
            }
            case MessageType::SUB_ROUND_ENDED:
            {
                SubRoundEndedMessage message;
                packet >> message;
                m_states.back()->handleSubRoundEnded(message);
                break;
            }
            case MessageType::FINAL_JEOPARDY:
            {
                FinalJeopardyMessage message;
                packet >> message;
                m_states.back()->handleFinalJeopardy(message);
                break;
            }
        }

        m_messageQueue.pop();
    }
}

void Engine::update()
{
    m_dt = m_clock.restart();
    m_states.back()->update();
}

void Engine::draw()
{
    m_window.clear();
    m_states.back()->draw(m_window);
    m_window.display();
}

void Engine::changeState(GameState* state)
{
    // Remove the current state from the state stack
    if (!m_states.empty())
    {
        m_states.pop_back();
    }

    // Put the new state at the top of the stack
    m_states.push_back(state);
    m_states.back()->init(this);
}

void Engine::initSelf(const std::string& playerName)
{
    m_playerInfo.initSelf(playerName);
}

std::vector<const Player*> Engine::getPlayers() const
{
    return m_playerInfo.getPlayers();
}

const Player* Engine::getCluePicker() const
{
    return m_playerInfo.getCluePicker();
}

const Player* Engine::getClueResponder() const
{
    return m_playerInfo.getClueResponder();
}

void Engine::addPlayer(const std::string& playerName)
{
    m_playerInfo.addPlayer(playerName);
}

void Engine::setCluePicker(const std::string& playerName)
{
    m_playerInfo.setCluePicker(playerName);
}

void Engine::setClueResponder(const std::string& playerName)
{
    m_playerInfo.setClueResponder(playerName);
}

void Engine::updatePlayerBalance(const std::string& playerName, int delta)
{
    m_playerInfo.updatePlayerBalance(playerName, delta);
}

PlayerInfo* Engine::getPlayerInfo()
{
    return &m_playerInfo;
}

const Player* Engine::getSelf() const
{
    return m_playerInfo.getSelf();
}


sf::Time Engine::getDeltaTime() const
{
    return m_dt;
}

sf::Vector2u Engine::getWindowSize() const
{
    return m_window.getSize();
}

Client* Engine::getClient()
{
    return &m_client;
}
