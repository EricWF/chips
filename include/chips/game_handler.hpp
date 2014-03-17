#ifndef CHIPS_GAME_HANDLER_HPP
#define CHIPS_GAME_HANDLER_HPP

# include "chip/core.hpp"
# include "chips/draw.hpp"
# include "chips/entity.hpp"
# include "chips/game.hpp"
# include "chips/resource_manager.hpp"
# include <elib/aux.hpp>
# include <elib/fmt.hpp>
# include <string>
# include <vector>
# include <utility>
# include <cstddef>

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
        
        game_handler(level const & l)
          : m_level(l)
        {}
        
        ELIB_DEFAULT_COPY_MOVE(game_handler);
        
        game_event_id update();
        
        void draw(sf::RenderWindow & to);
        
    private:
        level m_level;
    };
}                                                           // namespace chips
#endif /* CHIPS_GAME_HANDLER_HPP */