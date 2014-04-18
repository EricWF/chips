#ifndef CHIPS_LOGIC_ATTRIBUTES_HPP
#define CHIPS_LOGIC_ATTRIBUTES_HPP

# include "chips/logic/fwd.hpp"
# include "chips/core.hpp"
# include "chips/entity.hpp"
# include <vector>


namespace chips
{    
    /// A list of entities to be called after a given event
    namespace logic_detail { struct entity_list_tag {}; } 
    using entity_list = any_attribute<std::vector<entity_locator>, logic_detail::entity_list_tag>;
    
    /// For use with entities that need to draw multiple tiles
    namespace logic_detail { struct draw_list_tag {}; }
    using draw_list = any_attribute<std::vector<std::pair<tile_id, position>>, logic_detail::draw_list_tag>;
    
    /// Used by socket to count the number of chips it needs
    namespace logic_detail { struct chip_lock_tag {}; }
    using chip_count_lock = any_attribute<unsigned, logic_detail::chip_lock_tag>;
    
    /// For use with traps. 
    namespace logic_detail { struct trapped_entity_tag {}; }
    using trapped_entity = any_attribute<entity_locator, logic_detail::trapped_entity_tag>;
    
    /// For use with clone machines
    namespace logic_detail { struct clone_target_tag {}; }
    using clone_target = any_attribute<entity, logic_detail::clone_target_tag>;
    
    /// For use with traps and other things
    struct move_lock : attribute_base {};
    struct update_lock : attribute_base {};
    
    /// For use with chips weapons
    namespace logic_detail { struct ammo_tag {}; }
    using ammo_t = any_attribute<int, logic_detail::ammo_tag>;
    
    /// For use with determining what ways chips is allowed to move when
    /// he is on force floor.
    namespace logic_detail { struct ff_move_tag {}; }
    using force_floor_move = any_attribute<direction, logic_detail::ff_move_tag>;
    
    

}                                                           // namespace chips
#endif /* CHIPS_LOGIC_ATTRIBUTES_HPP */