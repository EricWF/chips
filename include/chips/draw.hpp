#ifndef CHIPS_DRAW_HPP
#define CHIPS_DRAW_HPP

# include <SFML/Graphics.hpp>

namespace chips
{
  class entity;
  struct position;
  enum class  tile_id;
    
  ////////////////////////////////////////////////////////////////////////////////
  //                               DRAW
  ////////////////////////////////////////////////////////////////////////////////

  // return true if the entity is drawn
  bool draw(sf::RenderWindow &, entity const &, position at);
  void draw(sf::RenderWindow &, position, tile_id);
}                                                           // namespace chips
#endif /* CHIPS_DRAW_HPP */
