#include "Question.h"

#include "ResourcePath.hpp"

Question::Question(
	std::string& category, const std::string& clue, const std::string& answer, const unsigned int value)
	: m_category(category)
	, m_clue(clue)
	, m_answer(answer)
	, m_value(value)
{
}

Question::Question() : m_value(0)
{
}

std::string Question::getCategory() const
{
	return m_category;
}

std::string Question::getClue() const
{
	return m_clue;
}

std::string Question::getAnswer() const
{
	return m_answer;
}

unsigned int Question::getValue() const
{
	return m_value;
}

std::ostream& operator<<(std::ostream& output, const Question& q)
{
	output << q.getCategory() << " for " << q.getValue() << ": "
		<< q.getClue() << " (" << q.getAnswer() << ")" << std::endl;;
	return output;
}

bool operator<(const Question& q1, const Question& q2)
{
	return q1.m_value < q2.m_value ? true : false;
}

void Question::draw(sf::RenderTarget& target, sf::RenderStates states, const sf::Vector2f& pos) const
{
	// the box
	sf::RectangleShape questionBox(sf::Vector2f(100.0, 100.0));
	questionBox.setPosition(pos);
	questionBox.setFillColor(sf::Color::Blue);

	// the text
	sf::Font font;
    if (!font.loadFromFile(resourcePath() + "KORIN.ttf")) {
		std::cout << "Failed to load font" << std::endl;
	}
	sf::Text text;
	text.setFont(font);
	text.setString("$" + std::to_string(m_value));
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(24);
	text.setPosition(pos);

	target.draw(questionBox);
	target.draw(text);
}
