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
    // TODO similar layout to menu/menu_handler
    class game_handler
    {
    public:
        game_handler() = default;
        
        game_handler(chips::level const & l)
          : m_level(l)
        {}
        
        ELIB_DEFAULT_COPY_MOVE(game_handler);
        
        level_state update(sf::RenderWindow & win);
        
        void draw(sf::RenderWindow & to) const;
        
        chips::level const & level() const { return m_level; }
        
        level_state state() const noexcept
        { return m_level.state; }
        
        bool closed() const noexcept 
        { return m_level.state == level_state::exited; }
        
        bool in_play() const noexcept 
        { return m_level.state == level_state::in_play; }
        
        bool good() const noexcept
        { return m_level.state == level_state::in_play; }
        
        explicit operator bool() const noexcept 
        { return good(); }
        
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
    private:
        chips::level m_level;
        time_point m_last_tick;
        std::intmax_t m_tick_count = 0;
    };
}                                                           // namespace chips
#endif /* CHIPS_GAME_HANDLER_HPP */