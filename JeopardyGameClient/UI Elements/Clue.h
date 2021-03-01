// A drawable Jeopardy! clue, this is basically just a thin wrapper of button
// that makes it bigger.

#pragma once

#include "Button.h"

#include <SFML/Graphics.hpp>

class Clue : public Button
{
public:
    Clue();

    void setAnswer(const std::string& answer);
    void setQuestion(const std::string& question);
};
