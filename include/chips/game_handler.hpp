#ifndef CHIPS_GAME_HANDLER_HPP
#define CHIPS_GAME_HANDLER_HPP

# include "chips/core.hpp"
# include "chips/logic.hpp"
# include <elib/aux.hpp>
# include <SFML/Graphics.hpp>
# include <chrono>

namespace chips
{
    enum class game_event_id
    {
        none,
        closed, 
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
        
        game_event_id state() const { return m_event; }
        
    private:
        void m_update_logic();
        void m_handle_event(sf::RenderWindow & win);
        void m_move_chip_event(sf::Event const &);
        void m_move_chip(direction);
        
        void m_draw_chip(sf::RenderWindow &, position win_pos) const;
        void m_draw_scoreboard(sf::RenderWindow &) const;
        void m_draw_helptext(sf::RenderWindow &) const;
        void m_draw_chip_count(sf::RenderWindow &) const;
        void m_draw_inventory(sf::RenderWindow &) const;
    private:
        game_event_id m_event{game_event_id::none};
        chips::level m_level;
        std::chrono::high_resolution_clock::time_point m_tick;
    };
    
    
}                                                           // namespace chips
#endif /* CHIPS_GAME_HANDLER_HPP */