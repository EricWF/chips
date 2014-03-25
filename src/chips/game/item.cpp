#include "chips/game/fwd.hpp"
#include "chips/game/common.hpp"
#include "chips/core.hpp"
#include "chips/entity.hpp"
#include "chips/game.hpp"
#include <elib/aux.hpp>

namespace chips 
{
    void init_item(entity & e, level &)
    {
        ELIB_ASSERT(is_item(e)); ((void)e);
        
        auto item_on_col =
        [](entity & self, entity & other, level &)
        {
            if (!is_chip(other)) return;
            REQUIRE_CONCEPT(other, EntityHas<inventory>);
            
            other.get<inventory>().add_item(self.id());
            self.kill();
        };
        
        e.on_death([](entity & self) { self.clear(); });
        
        e << method(collides_, common::collides_with_monster_)
          << method(on_collision_, item_on_col);
    }
    
    void process_item(entity &, level &)
    {
    }
    
    void finalize_item(entity &, level &)
    {
    }
}                                                           // namespace chips