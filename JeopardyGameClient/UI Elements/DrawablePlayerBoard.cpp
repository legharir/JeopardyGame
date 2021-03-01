#include "DrawablePlayerBoard.h"

#include "Utils.h"

#include <algorithm>
#include <iostream>

DrawablePlayerBoard::DrawablePlayerBoard() = default;

DrawablePlayerBoard::DrawablePlayerBoard(const PlayerBoard& playerBoard)
    : PlayerBoard(playerBoard)
    , m_cellSize(160, 80)
    , m_cellMargin(20, 20)
{
    // TODO: remove this since m_font isn't being used in draw()?
    // Initialize the font.
    if (!m_font.loadFromFile("/System/Library/Fonts/SFNSMono.ttf"))
        throw;

    // Initialize each cell in the game board.
    sf::Vector2f cellPos(0, 0);
    for (const auto& cluesForCategory : getClues())
    {
        const auto& category = cluesForCategory.first;
        const auto& clues = cluesForCategory.second;

        // Draw the category.
        cellPos.y = 0;

        Button categoryButton(cellPos, category, m_font, m_cellSize);
        const auto categoryCell = Cell(CellType::Category, categoryButton, category);
        m_cells.push_back(categoryCell);

        // Draw each value.
        for (const auto& clue : clues)
        {
            cellPos.y += m_cellSize.y + m_cellMargin.y;
            Button valueButton(cellPos, std::to_string(clue.value), m_font, m_cellSize);
			const auto valueCell = Cell(CellType::Value, valueButton, category, clue.value);
			m_cells.push_back(valueCell);
        }

        cellPos.x += m_cellSize.x + m_cellMargin.x;
    }
}

bool DrawablePlayerBoard::clicked(const sf::Event::MouseButtonEvent& mouse) const
{
    for (const auto& cell : m_cells)
    {
        if (cell.button.clicked(mouse))
            return true;
    }

    return false;
}

const DrawablePlayerBoard::Cell* DrawablePlayerBoard::getValueCellAtPos(int x, int y) const
{
    for (const auto& cell : m_cells)
    {
        if (cell.type == CellType::Value && cell.button.areCoordsInside(x, y))
            return &cell;
    }
    return nullptr;
}

void DrawablePlayerBoard::setPicked(const std::string& category, unsigned int value)
{
    // Do not show text for picked cells to indicate they have already been picked.
    for (auto& cell : m_cells)
        if (cell.category == category && cell.value == value)
			cell.button.setText("");
}

void DrawablePlayerBoard::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto const& cell : m_cells)
        target.draw(cell.button);
}
