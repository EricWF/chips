#ifndef CHIPS_CORE_FWD_HPP
#define CHIPS_CORE_FWD_HPP

namespace chips
{
    // chips_state.hpp //
    enum class chips_state;
    
    // direction.hpp //
    enum class direction : unsigned;
    
    // entity_locator.hpp //
    struct entity_locator;
    
    // error.hpp //
    class chips_error;
    
    // id.hpp //
    enum class entity_id;
    enum class entity_type;
    enum class tile_id;
    
    // inventory.hpp //
    class inventory;
    
    // level.hpp //
    class level;
    
    // load.hpp //
    struct tile_properties;
    struct parsed_level;
    enum class action_type;
    struct parsed_action;
    
    // position.hpp //
    struct position;
}                                                           // namespace chips
#endif /* CHIPS_CORE_FWD_HPP */