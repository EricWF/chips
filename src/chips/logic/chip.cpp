#include "chips/logic/fwd.hpp"
#include "chips/logic/common.hpp"
#include "chips/core.hpp"
#include "chips/entity.hpp"
#include <elib/aux.hpp>
#include <elib/fmt.hpp>

namespace chips { namespace logic
{
    ////////////////////////////////////////////////////////////////////////////
    void init_chip(entity & e, level &)
    {
        ELIB_ASSERT(is_chip(e));
        e.remove<tile_id>();
        
        auto chip_update_ =
        [](entity & self, level & l)
        {
            auto end = l.entity_list.end();
            
            if (!self.get<inventory>().contains(entity_id::skates))
            {
                auto ice_pos = OnIce(self).find(l.entity_list);
                if (ice_pos != end)
                {
                    move_on_ice(self, *ice_pos, l);
                    return;
                }
            }
            
            if (!self.get<inventory>().contains(entity_id::suction_boots))
            {
                auto ff_pos = OnForceFloor(self).find(l.entity_list);
                if (ff_pos != end)
                {
                    move_on_force_floor(self, *ff_pos, l);
                    return;
                }
            }
        };
        
        e << inventory() 
          << method(update_, chip_update_)
          << method(move_, common::move_);
    }
    
    void process_chip(entity &, level &)
    {
    }
    
    void finalize_chip(entity &, level &)
    {
    }
}}                                                          // namespace chips