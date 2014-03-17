#ifndef CHIPS_GAME_HANDLER_HPP
#define CHIPS_GAME_HANDLER_HPP

# include "chip/core.hpp"
# include "chips/draw.hpp"
# include "chips/entity.hpp"
# include "chips/gome.hpp"
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

    class game_handler;
}                                                           // namespace chips
#endif /* CHIPS_GAME_HANDLER_HPP */