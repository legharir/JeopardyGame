#pragma once

#include <string>

enum class GameEventType {
	PLAYER_JOIN,
};

// TODO: how to make this an "abstract" class
struct GameEvent
{
	GameEventType type;
};

// TODO: implementing this properly
struct PlayerJoinEvent : GameEvent
{
	std::string m_name;
	PlayerJoinEvent(std::string& name) : m_name(name)
	{
		// why cant this be defined in the initializer list?
		type = GameEventType::PLAYER_JOIN;
	}
};
