#include "chips/draw.hpp"
#include "chips/core.hpp"
#include "chips/entity.hpp"
#include "chips/resource_manager.hpp"
#include <elib/aux.hpp>
#include <elib/fmt.hpp>

namespace chips
{

////////////////////////////////////////////////////////////////////////////////
//                               DRAW
////////////////////////////////////////////////////////////////////////////////
    
    bool draw(sf::RenderWindow & to, entity const & e, position at)
    {
        // Don't draw if the entity is dead
        if (!e) return false;
        
        tile_id tid;
        if (e.has<tile_id>()) {
            tid = e.get<tile_id>();
        }
        else {
            tid = static_cast<tile_id>(e.id());
        }
            
        if (e.has<direction>()){
            tid = directional_tile_id(tid, e.get<direction>());
        }

        draw(to, at, tid);
        return true;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    void draw(sf::RenderWindow & to, position p, tile_id index)
    {
        auto & rm = resource_manager::get();
        sf::Sprite & s = rm[index];
        s.setPosition(
            static_cast<float>(p.x)
          , static_cast<float>(p.y)
        );
        to.draw(s);
    }
}                                                           // namespace chips
