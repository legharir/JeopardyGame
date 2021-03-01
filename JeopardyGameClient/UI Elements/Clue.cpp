#include "Clue.h"

#include <SFML/Graphics.hpp>

Clue::Clue()
{
    setSize(sf::Vector2f(500, 500));
    setCharacterSize(30);
}

void Clue::setAnswer(const std::string& answer)
{
    setText(answer);
}

void Clue::setQuestion(const std::string& question)
{
    setText(getText() + "\n\n"+question);
}
