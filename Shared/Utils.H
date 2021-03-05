#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <SFML/Graphics.hpp>

namespace Utils
{
	// Returns a copy of the given string with any leading space characters trimmed off.
	std::string ltrim(const std::string& str);

	// Clues/answers/categories in the dataset are strings that may contain quotes.
	// For some reason, these quotes are wrapped by an extra set of quotes themselves.
	// (e.g. LORD OF THE ""RING""s). This function takes in such an entity, and returns a
	// "cleaned" version in which extra inner quotes are removed (e.g. LORD OF THE "RING"s).
	std::string cleanToken(const std::string& token);

	// Returns the tokens obtained by splitting the given string with the provided delimiter.
	std::vector<std::string> split(const std::string& str, const std::string& delimiter);

	// Converts a "dollar string" to an unsigned int, e.g "$420,000" => 420,000.
	unsigned int dollarStrToUInt(const std::string& dollarStr);

	// Converts a unsigned int to a "dollar string" with thousands separators e.g -32332 => "-$32,332".
	std::string intToDollarStr(int amount);

	// Checks if the given coordinates are inside the rectangle.
	bool insideRect(int x, int y, const sf::RectangleShape& rect);

	// Gets the number of milliseconds since the unix epoch.
	long long getMsSinceEpoch();

	// Returns true if s2 is contained in s1, and false otherwise.
	bool contains(const std::string& s1, const std::string& s2, bool ignoreCase = true);

    // Returns the nearest odd number to x.
    unsigned int nearestOdd(float x);
}
