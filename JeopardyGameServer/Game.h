#pragma once

#include "GameMessage.h"

#include "GameBoard.h"
#include "Player.h"
#include "Server.h"

#include <SFML/Network.hpp>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>

class SGameState;

class Game
{
public:
    Game();

    bool isRunning() const;

    void init(Server* server);
    void update();
    void changeState(SGameState* state);

    void handleMessage(sf::TcpSocket* socket, sf::Packet& packet);

    std::vector<std::string> getPlayerNames();
    void addPlayer(sf::TcpSocket* socket, const std::string& playerName);

    void broadcast(sf::Packet& packet) const;

    bool hasPicker() const;
    std::string getPickerName() const;
    std::string getResponderName() const;

    const std::set<std::string>& getResponded() const;

    const Clue& getClue(const std::string& category, unsigned int value) const;
    const Clue& getCurClue() const;
    Round getCurRound() const;

    unsigned int numCluesUnpicked() const;

    long long getBuzzInDeadline() const;
    unsigned int getBuzzerReadyDelay() const;
    long long getWagerDeadline() const;
    long long getResponseDeadline() const;

    bool allPlayersResponded() const;

    void startGame(unsigned int season, unsigned int episode);
    void endGame();
    void startRound(Round round);
    void startCluePickingPhase();
    void endSubRound();
    void onCluePicked(const std::string& category, unsigned int value);
    void onDailyDoublePicked(const std::string& playerName, const std::string& category, unsigned int value);
    void onWagerSubmitted(const std::string& playerName, unsigned int wager);
    void enableBuzzers();
    void onBuzzIn(const std::string& responder);
    void onClueResponse(bool isResponseCorrect);
    void onResponseDeadlineExceeded();
    void onPartialClueResponse(const std::string& playerName, int partialResponse);
    void onFinalJeopardyStart();
    void onFinalJeopardyPlayStart();
    void onFinalJeopardyResponse(const std::string& playerName, const std::string& response);
    void onFinalJeopardyEnd();

private:
    Server* m_server;
    bool m_running;
    SGameState* m_curState;

    std::unordered_map<sf::TcpSocket*, Player> m_players;
    std::unordered_map<Round, GameBoard, EnumClassHash> m_gameBoards;
    std::unordered_map<Round, PlayerBoard, EnumClassHash> m_playerBoards;

    Round m_curRound;

    Clue m_curClue;
    Player* m_picker;
    Player* m_responder;

    const Player& getPlayer(sf::TcpSocket* socket) const;
    Player* getPlayer(const std::string& playerName);
    std::set<std::string> m_responded;

    long long m_buzzInDeadline;
    long long m_responseDeadline;
    long long m_wagerDeadline;

    const unsigned m_buzzerReadyDelay;
    const unsigned m_buzzInWindowLength;
    const unsigned m_responseWindowLength;
    const unsigned m_dailyDoubleResponseWindowLength;
    const unsigned m_finalJeopardyWagerWindowLength;
    const unsigned m_finalJeopardyResponseWindowLength;

    void createPlayerBoards(const std::unordered_map<Round, GameBoard, EnumClassHash>& gameBoards);
};
