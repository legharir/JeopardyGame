// This is the class representing the main game board UI players see when a game
// is started. In theory, it should be the same 6x5 table of cells that you
// would see if you were a contestant on Jeopardy! for the particular
// season/episode specified prior to the start of the game. The board has two
// types of cells:
//
// 1. Category Cells (headers cells)
// 2. Value Cells (body cells)
//
// Unsurprisingly, category cells contain the category for the clues in that
// column whereas value cells contain a dollar amount corresponding to the worth
// of the clue. Note that some cells may be missing, due to incomplete datasets.

#pragma once

#include "GameMessage.h"

#include "Button.h"

#include <SFML/Graphics.hpp>
#include <utility>

class DrawablePlayerBoard : public PlayerBoard, public sf::Drawable
{
public:
    enum class CellType
    {
        Category,
        Value
    };

    // This struct is needed so that the getValueCellAtPos function can be implemented cleanly.
    // Basically, if we didn't have this struct (and instead only stored the buttons themselves),
    // then we would need to ensure that we iterate through the cells in the same way in both draw()
    // and getValueCellAtPos().
    struct Cell
    {
        Cell(CellType type, Button button, const std::string& category, unsigned value = 0)
            : button(button)
            , type(type)
            , category(category)
            , value(value)
        {
        }

        Button button;    
        CellType type;
        std::string category;
        unsigned value;
    };

    DrawablePlayerBoard();
    DrawablePlayerBoard(const PlayerBoard& playerBoard);

    // Returns true only if a value cell is clicked.
    bool clicked(const sf::Event::MouseButtonEvent& mouse) const;

    // TODO: Not sure what to make the return type here. I would normally just
    // return Cell, however what happens if the call the method at a position
    // not occupied by a value cell? In that case, it would be nice to be able
    // to check for 'null' values.
    // Returns the category and value of the value cell at the specified position.
    // This is useful when a player clicks on a value cell during their turn (we
    // can inform the server to send us the clue).
    const Cell* getValueCellAtPos(int x, int y) const;
    void setPicked(const std::string& category, unsigned int value);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

    sf::Font m_font;

    sf::Vector2f m_cellSize;
    sf::Vector2f m_cellMargin;

    // Represents a category or value rectangle in the player board.
    std::vector<Cell> m_cells;
};
