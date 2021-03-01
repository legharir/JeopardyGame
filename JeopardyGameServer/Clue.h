#pragma once

#include <iostream>
#include <string>

class Clue
{
public:
    Clue();
    Clue(const std::string& category, unsigned value, const std::string& answer, const std::string& question,
         bool isDailyDouble);

    std::string getCategory() const;
    unsigned int getValue() const;
    std::string getAnswer() const;
    std::string getQuestion() const;
    bool isDailyDouble() const;
    bool isPicked() const;

    void pick();

    friend std::ostream& operator<<(std::ostream& out, const Clue& clue);

private:
    std::string m_category;
    unsigned int m_value;
    std::string m_answer;
    std::string m_question;
    bool m_isDailyDouble;
    bool m_picked;
};
