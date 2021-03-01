#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>

class Question
{
public:
	Question(std::string& category, const std::string& clue, const std::string& answer, const unsigned int value);
	Question();

	std::string getCategory() const;
	std::string getClue() const;
	std::string getAnswer() const;
	unsigned int getValue() const;

	friend std::ostream& operator << (std::ostream& output, const Question& q);
	friend bool operator< (const Question& q1, const Question& q2);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states, const sf::Vector2f& pos) const;

private:
	std::string m_category;
	std::string m_clue;
	std::string m_answer;
	unsigned int m_value;
};
