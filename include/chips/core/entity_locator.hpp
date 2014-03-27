#ifndef CHIPS_GAME_ENTITY_LOCATOR_HPP
#define CHIPS_GAME_ENTITY_LOCATOR_HPP

# include "chips/core/id.hpp"
# include "chips/core/position.hpp"

namespace chips
{
    // TODO this is a stupid class. remove it.
    struct entity_locator
    {
        entity_id id;
        position pos;
    };
}                                                           // namespace chips
#endif /* CHIPS_GAME_ENTITY_LOCATOR_HPP */