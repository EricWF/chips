#ifndef CHIPS_GAME_ENTITY_LOCATOR_HPP
#define CHIPS_GAME_ENTITY_LOCATOR_HPP

# include "chips/core/entity_id.hpp"
# include "chips/game/position.hpp"

namespace chips
{
    struct entity_locator
    {
        entity_id id;
        position pos;
    };
}                                                           // namespace chips
#endif /* CHIPS_GAME_ENTITY_LOCATOR_HPP */