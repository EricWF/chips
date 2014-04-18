#ifndef CHIPS_DRAW_HPP
#define CHIPS_DRAW_HPP

# include "chips/core/fwd.hpp"
# include <SFML/Graphics.hpp>

namespace chips
{
    // forward //
    class entity;
    
    unsigned top_left_window_coord(unsigned chip_dim);
    position top_left_window_position(level const & l);
    position position_in_window(position top_left, entity const & e);
    
    void draw_level(sf::RenderWindow &, level const &);
    
    void draw_playing_field(sf::RenderWindow &, level const &);
    void draw_chip(sf::RenderWindow &, position, level const &);
    void draw_entities(sf::RenderWindow &, position, level const &);
    
    void draw_scoreboard(sf::RenderWindow &, level const &);
    void draw_chip_count(sf::RenderWindow &, level const &);
    void draw_inventory(sf::RenderWindow &, level const & );
    
    void draw_helptext(sf::RenderWindow &, level const &);
    
    // return true if the entity is drawn
    bool draw_entity_at(sf::RenderWindow &, entity const &, position at);
    void draw_tile_at(sf::RenderWindow &, tile_id, position);
    void draw_tile_at(sf::RenderWindow &, entity_id, position);
}                                                           // namespace chips
#endif /* CHIPS_DRAW_HPP */
