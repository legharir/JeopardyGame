#include "Clue.h"

Clue::Clue()
    : m_value(0)
    , m_isDailyDouble(false)
    , m_picked(false)
{
}

Clue::Clue(const std::string& category, unsigned value, const std::string& answer,
           const std::string& question, bool isDailyDouble)
    : m_category(category)
    , m_value(value)
    , m_answer(answer)
    , m_question(question)
    , m_isDailyDouble(isDailyDouble)
    , m_picked(false)
{
}

std::string Clue::getCategory() const
{
    return m_category;
}

unsigned int Clue::getValue() const
{
    return m_value;
}

std::string Clue::getAnswer() const
{
    return m_answer;
}

std::string Clue::getQuestion() const
{
    return m_question;
}

bool Clue::isDailyDouble() const
{
    return m_isDailyDouble;
}

bool Clue::isPicked() const
{
    return m_picked;
}

void Clue::pick()
{
    m_picked = true;
}

std::ostream& operator<<(std::ostream& out, const Clue& clue)
{
    out << clue.getCategory() << " for "
        << "$" << clue.getValue() << ": " << clue.getAnswer() << " (" << clue.getQuestion() << ")" << std::endl;
    return out;
}
