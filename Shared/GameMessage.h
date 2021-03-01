#pragma once

#include <SFML/Network.hpp>

#include <string>
#include <vector>

enum class MessageType
{
    PLAYER_JOINED,
    GAME_START,
    ROUND_START,
    CLUE_PICK_START,
    CLUE_PICKED,
    DAILY_DOUBLE,
    WAGER,
    BUZZER_ENABLED,
    BUZZ_IN,
    PARTIAL_CLUE_RESPONSE,
    CLUE_RESPONSE,
    RESPONSE_RECEIVED,
    SUB_ROUND_ENDED,
    FINAL_JEOPARDY
};

enum class Round
{
    JEOPARDY = 1,
    DOUBLE_JEOPARDY,
    FINAL_JEOPARDY
};

struct EnumClassHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};

// A PlayerBoard is a stripped down version of a GameBoard, meant only to be
// sent to players by the server. It is essentially a GameBoard "schema" that
// only contains information necessary for players to choose a clue (i.e.
// category and value). It doesn't contain answers, questions, or daily double
// clue locations.
struct PlayerBoard
{
    struct Clue
    {
        sf::Uint32 value;
        bool picked;
    };

    PlayerBoard() = default;

    std::map<std::string, std::vector<Clue>> getClues() const;
    void addClue(const std::string& category, sf::Uint32 value);

    unsigned int getNumClues() const;

    using Category = std::string;
    std::map<Category, std::vector<Clue>> m_clues;

    friend std::ostream& operator<<(std::ostream& output, const PlayerBoard& playerBoard);
};

///////////////////////////////////// Game Messages ////////////////////////////////////

// Base class for all game messages. This class is not meant to be instantiated
// and doesn't signify anything on its own. It is meant to be subclassed to
// create a message that can be sent to the server from clients, from clients to
// the server, or both.
struct GameMessage
{
protected:
    GameMessage(const MessageType& msgType);

public:
    MessageType type;
};

// When sent from a client to the server, this message notifies the server that
// the client wants to join the game. When sent from the server to client, it
// notifies them that a new player has joined the game.
struct PlayerJoinedMessage : public GameMessage
{
    PlayerJoinedMessage();
    PlayerJoinedMessage(const std::string& playerName);

    std::string playerName;
};

// When sent from a client to the server, this message acts as a request to
// start a game for the given season and episode. The server can then create,
// for each round, the correct PlayerBoards for that show. When sent from server
// to clients, it signals that a game has begun. In the latter case, this
// message is followed up by a RoundStartMessage for the first round of play.
struct GameStartMessage : public GameMessage
{
    GameStartMessage();
    GameStartMessage(sf::Uint32 season, sf::Uint32 episode);

    sf::Uint32 season;
    sf::Uint32 episode;
};

// This message is sent by the server to clients indicating that the specified
// round of play has begun. It also contains the PlayerBoard for that round.
struct RoundStartMessage : public GameMessage
{
    RoundStartMessage();
    RoundStartMessage(const Round& round, const PlayerBoard& playerBoard);

    Round round;
    PlayerBoard playerBoard;
};

// This message is sent by the server to clients indicating that it will now
// only accept subsequent CluePickedMessages from the specified player, up to the
// next CluePickingStartedMessage. In other words, it signals that it's now the turn
// of the specified player to pick a clue.
struct CluePickingStartedMessage : public GameMessage
{
    CluePickingStartedMessage();
    CluePickingStartedMessage(const std::string& picker);

    std::string picker;
};

// When sent from a client to the server, this message indicates that a user
// selected a clue. If not sent from the clue picker, this message is ignored.
// When sent from the server to clients, this message is sent in response to a
// CluePickedMessage from the current clue picker. In this case, the message
// also contains the clue answer and results in the answer being displayed on
// clients' screens. Note, the "answer" in Jeopardy! is NOT synonymous with
// "correct response".
struct CluePickedMessage : public GameMessage
{
    CluePickedMessage();
    // TODO: get rid of the isDailyDouble field if not being used?
    CluePickedMessage(const std::string& category, sf::Uint32 value, const std::string& answer = "",
                      bool isDailyDouble = false);

    std::string category;
    sf::Uint32 value;
    std::string answer;
    bool isDailyDouble;
};

// This message is sent from the server to a client after receiving a
// CluePickedMessage to indicate that the chosen clue is a daily double.
struct DailyDoubleMessage : public GameMessage
{
    DailyDoubleMessage();
    DailyDoubleMessage(const std::string& category, sf::Uint32 value);

    std::string category;
    // Value is needed so that the client known which clue to access.
    sf::Uint32 value;
};

// When sent from clients, this message indicates to the server the amount of
// money wagered for the daily double/final Jeopardy! round. When sent from the
// server to clients, this message indiciates that a WagerMessage was received,
// and that responses to the clue are now being accepted.
struct WagerMessage : public GameMessage
{
    WagerMessage();
    WagerMessage(sf::Uint32 wager);
    WagerMessage(sf::Uint32 wager, const std::string& answer, long long responseDeadline);

    sf::Uint32 wager;
    std::string answer;
    sf::Uint64 responseDeadline;
};

// This message is sent from the server to clients to inform them that it's
// ready for players to buzz in. The BuzzInMessage must be received before
// the deadline timestamp (specified in ms since epoch) for it to be counted
// as a valid buzz in.
struct BuzzerEnabledMessage : public GameMessage
{
    BuzzerEnabledMessage();
    BuzzerEnabledMessage(sf::Uint64 buzzInDeadline);

    sf::Uint64 buzzInDeadline;
};

// When sent from a client to the server, this message indicates that the player
// buzzed in. When sent from the server to clients, this message indicates that
// that the specified player has buzzed in. It also acts an indicator for the
// player who buzzed in to send a response to the clue.
struct BuzzInMessage : public GameMessage
{
    BuzzInMessage();
    BuzzInMessage(const std::string& playerName, sf::Uint64 responseDeadline);

    std::string playerName;
    sf::Uint64 responseDeadline;
};

// When sent the a client to the server, this message indicates that the
// player (who also has to be the current responder for this message to be
// processed) typed in a character into the response field. When sent from the
// server to clients, this message notifies them of the character the current
// responder typed in to their respond field, so that each player can see what
// the responder is currently typing.
struct PartialClueResponseMessage : public GameMessage
{
    PartialClueResponseMessage();
    PartialClueResponseMessage(sf::Uint32 character);

    sf::Uint32 character;
};

// This message is only sent from clients to the server. It indicates that the
// client responded to the clue with the specified response.
struct ClueResponseMessage : public GameMessage
{
    ClueResponseMessage();
    ClueResponseMessage(const std::string& response);

    std::string response;
};

// This message is sent from the server to clients to acknowledge that it
// received a clue response sent by the responder in the current subround. It
// also notifies players whether the response was correct, and how much money
// the responder gained/lost as a result.
struct ResponseReceivedMessage : public GameMessage
{
    ResponseReceivedMessage();
    ResponseReceivedMessage(const std::string& responder, bool responseAccepted, sf::Int32 balanceDelta);

    std::string responder;
    bool responseAccepted;
    sf::Int32 balanceDelta;
};

// This message, sent only to clients from the server, indicates the end of the current
// subround. This can occur either if a player responds correctly to the subround clue,
// or all players in the game have responded to the clue.
struct SubRoundEndedMessage : public GameMessage
{
    SubRoundEndedMessage();
    SubRoundEndedMessage(const std::string& question);

    std::string question;
};

// This message is sent from the server to a clients to inform them of the
// final Jeopardy! category.
struct FinalJeopardyMessage : public GameMessage
{
    FinalJeopardyMessage();
    FinalJeopardyMessage(const std::string& category, long long wagerDeadline);

    std::string category;
    long long wagerDeadline;
};

////////////////////////////////// Packet Operator Overloads ////////////////////////////////////

sf::Packet& operator<<(sf::Packet& packet, const MessageType& msg);
sf::Packet& operator>>(sf::Packet& packet, MessageType& msg);

sf::Packet& operator<<(sf::Packet& packet, const Round& msg);
sf::Packet& operator>>(sf::Packet& packet, Round& msg);

sf::Packet& operator<<(sf::Packet& packet, const PlayerBoard& playerBoard);
sf::Packet& operator>>(sf::Packet& packet, PlayerBoard& playerBoard);

sf::Packet& operator<<(sf::Packet& packet, const PlayerJoinedMessage& msg);
sf::Packet& operator>>(sf::Packet& packet, PlayerJoinedMessage& msg);

sf::Packet& operator<<(sf::Packet& packet, const GameStartMessage& msg);
sf::Packet& operator>>(sf::Packet& packet, GameStartMessage& msg);

sf::Packet& operator<<(sf::Packet& packet, const RoundStartMessage& msg);
sf::Packet& operator>>(sf::Packet& packet, RoundStartMessage& msg);

sf::Packet& operator<<(sf::Packet& packet, const CluePickingStartedMessage& msg);
sf::Packet& operator>>(sf::Packet& packet, CluePickingStartedMessage& msg);

sf::Packet& operator<<(sf::Packet& packet, const CluePickedMessage& msg);
sf::Packet& operator>>(sf::Packet& packet, CluePickedMessage& msg);

sf::Packet& operator<<(sf::Packet& packet, const BuzzerEnabledMessage& msg);
sf::Packet& operator>>(sf::Packet& packet, BuzzerEnabledMessage& msg);

sf::Packet& operator<<(sf::Packet& packet, const BuzzInMessage& msg);
sf::Packet& operator>>(sf::Packet& packet, BuzzInMessage& msg);

sf::Packet& operator<<(sf::Packet& packet, const PartialClueResponseMessage& msg);
sf::Packet& operator>>(sf::Packet& packet, PartialClueResponseMessage& msg);

sf::Packet& operator<<(sf::Packet& packet, const ClueResponseMessage& msg);
sf::Packet& operator>>(sf::Packet& packet, ClueResponseMessage& msg);

sf::Packet& operator<<(sf::Packet& packet, const ResponseReceivedMessage& msg);
sf::Packet& operator>>(sf::Packet& packet, ResponseReceivedMessage& msg);

sf::Packet& operator<<(sf::Packet& packet, const SubRoundEndedMessage& msg);
sf::Packet& operator>>(sf::Packet& packet, SubRoundEndedMessage& msg);

sf::Packet& operator<<(sf::Packet& packet, const DailyDoubleMessage& msg);
sf::Packet& operator>>(sf::Packet& packet, DailyDoubleMessage& msg);

sf::Packet& operator<<(sf::Packet& packet, const WagerMessage& msg);
sf::Packet& operator>>(sf::Packet& packet, WagerMessage& msg);

sf::Packet& operator<<(sf::Packet& packet, const FinalJeopardyMessage& msg);
sf::Packet& operator>>(sf::Packet& packet, FinalJeopardyMessage& msg);
