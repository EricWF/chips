#ifndef CHIPS_DRAW_HPP
#define CHIPS_DRAW_HPP

# include "chips/core.hpp"
# include "chips/entity.hpp"
# include <SFML/Graphics.hpp>

namespace chips
{
    // return true if the entity is drawn
    bool draw(sf::RenderWindow &, entity const &, position at);
    void draw(sf::RenderWindow &, position, texture_index);
}                                                           // namespace chips
#endif /* CHIPS_DRAW_HPP */