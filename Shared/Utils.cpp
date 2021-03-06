#include "Utils.h"

#include <algorithm>

std::string Utils::ltrim(const std::string& str)
{
    const auto begin = str.find_first_not_of(' ');
    if (begin == std::string::npos)
    {
        return "";
    }
    else
    {
        return str.substr(begin, str.size());
    }
}

std::string Utils::cleanToken(const std::string& token)
{
    auto cleaned = token;
    cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), '\\'), cleaned.end());
    return cleaned;
}

std::vector<std::string> Utils::split(const std::string& str, const std::string& delimiter)
{
    std::vector<std::string> tokens;
    auto start = 0;
    auto end = str.find(delimiter);

    while (end != std::string::npos)
    {
        const auto nextToken = str.substr(start, end - start);
        tokens.push_back(nextToken);

        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    const auto nextToken = str.substr(start, end - start);
    tokens.push_back(nextToken);

    return tokens;
}

unsigned int Utils::dollarStrToUInt(const std::string& dollarStr)
{
    if (dollarStr[0] != '$')
        throw;

    std::string numStr;
    for (const auto& character : dollarStr)
    {
        if (std::isdigit(character))
            numStr += character;
    }

    return std::stoi(numStr);
}

std::string Utils::intToDollarStr(int amount)
{
    std::string amountStr = std::to_string(std::abs(amount));

    for (int i = amountStr.size() - 3; i > 0; i = i - 3)
    {
        amountStr.insert(i, ",");
    }

    const auto& prefix = amount >= 0 ? "$" : "-$";
    return prefix + amountStr;
}

bool Utils::insideRect(int x, int y, const sf::RectangleShape& rect)
{
    sf::Vector2f rectPos(rect.getPosition());
    sf::Vector2f rectSize(rect.getSize());

    bool insideX = x > rectPos.x && x < rectPos.x + rectSize.x;
    bool insideY = y > rectPos.y && y < rectPos.y + rectSize.y;
    return insideX && insideY;
}

long long Utils::getMsSinceEpoch()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())
        .count();
}

bool Utils::contains(const std::string& s1, const std::string& s2, bool ignoreCase)
{
    if (s2.empty())
        return false;

    auto it = std::search(s1.begin(), s1.end(), s2.begin(), s2.end(), [ignoreCase](char ch1, char ch2) {
        if (ignoreCase)
            return std::tolower(ch1) == std::tolower(ch2);
        else
            return ch1 == ch2;
    });
    return (it != s1.end());
}

unsigned int Utils::nearestOdd(float x)
{
    return 2 * ( (int)(x / 2.0f) ) + 1;
}

