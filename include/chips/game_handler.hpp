#ifndef CHIPS_GAME_HANDLER_HPP
#define CHIPS_GAME_HANDLER_HPP

# include "chips/core.hpp"
# include "chips/logic.hpp"
# include <elib/aux.hpp>
# include <SFML/Graphics.hpp>
# include <chrono>
# include <cstddef>

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
        bool m_try_tick();
        void m_tick(sf::RenderWindow &);
        /// Returns true if any entity was moved
        bool m_slide();
        bool m_slide_chip();
        void m_update();
        bool m_check_success();
        bool m_check_failure();
        
        // returns true if the window should be redrawn
        bool m_handle_event(sf::RenderWindow & win);
        // returns true if it chips was moved
        bool m_move_chip_event(sf::Event const &);
        void m_move_chip(direction);
        
        void m_draw(sf::RenderWindow &) const;
        void m_draw_chip(sf::RenderWindow &, position win_pos) const;
        void m_draw_scoreboard(sf::RenderWindow &) const;
        void m_draw_helptext(sf::RenderWindow &) const;
        void m_draw_chip_count(sf::RenderWindow &) const;
        void m_draw_inventory(sf::RenderWindow &) const;
    private:
        game_event_id m_event{game_event_id::none};
        chips::level m_level;
        time_point m_last_tick;
        std::intmax_t m_tick_count = 0;
         
    };
    
    
}                                                           // namespace chips
#endif /* CHIPS_GAME_HANDLER_HPP */