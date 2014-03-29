#ifndef CHIPS_LOGIC_ATTRIBUTES_HPP
#define CHIPS_LOGIC_ATTRIBUTES_HPP

# include "chips/logic/fwd.hpp"
# include "chips/core.hpp"
# include "chips/entity.hpp"
# include <vector>

namespace chips
{
////////////////////////////////////////////////////////////////////////////////
//                               MISC ATTRIBUTES
////////////////////////////////////////////////////////////////////////////////
    
    /// A list of entities to be called after a given event
    namespace logic_detail { struct entity_list_tag {}; } 
    using entity_list = any_attribute<std::vector<entity_locator>, logic_detail::entity_list_tag>;
    
    /// Used by socket to count the number of chips it needs
    namespace logic_detail { struct chip_lock_tag {}; }
    using chip_count_lock = any_attribute<unsigned, logic_detail::chip_lock_tag>;
    
    /// For use with traps. 
    namespace logic_detail { struct trapped_entity_tag {}; }
    using trapped_entity = any_attribute<entity*, logic_detail::trapped_entity_tag>;
    
    /// For use with traps and other things
    struct move_lock : attribute_base {};

}                                                           // namespace chips
#endif /* CHIPS_LOGIC_ATTRIBUTES_HPP */