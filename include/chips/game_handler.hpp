#ifndef CHIPS_GAME_HANDLER_HPP
#define CHIPS_GAME_HANDLER_HPP

# include "chips/core.hpp"
# include "chips/game.hpp"
# include <elib/aux.hpp>
# include <SFML/Graphics.hpp>

namespace chips
{
    enum class game_event_id
    {
        none,
        closed, 
        paused, 
        level_failed,
        level_passed
    };

    // TODO similar layout to menu/menu_handler
    class game_handler
    {
    public:
        game_handler() = default;
        
        game_handler(chips::level const & l)
          : m_level(l)
        {}
        
        ELIB_DEFAULT_COPY_MOVE(game_handler);
        
        game_event_id update(sf::RenderWindow & win);
        
        void draw(sf::RenderWindow & to) const;
        
        chips::level const & level() const { return m_level; }
        
    private:
        game_event_id m_handle_event(sf::RenderWindow & win);
        void m_move_chip_event(sf::Event const &);
        void m_move_chip(direction);
    private:
        chips::level m_level;
    };
    
    
}                                                           // namespace chips
#endif /* CHIPS_GAME_HANDLER_HPP */