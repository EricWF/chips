#include "chips/logic/attributes.hpp"

namespace chips
{
    ENTITY_ATTRIBUTE_INSTANTIATE( direction );
    ENTITY_ATTRIBUTE_INSTANTIATE( position );
    ENTITY_ATTRIBUTE_INSTANTIATE( tile_id );
    ENTITY_ATTRIBUTE_INSTANTIATE( entity_list );
   // ENTITY_ATTRIBUTE_INSTANTIATE( chip_count_lock );
    ENTITY_ATTRIBUTE_INSTANTIATE( trapped_entity );
    ENTITY_ATTRIBUTE_INSTANTIATE( move_lock );
}                                                           // namespace chips