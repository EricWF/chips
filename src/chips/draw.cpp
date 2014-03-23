#include "chips/draw.hpp"
#include "chips/core.hpp"
#include "chips/entity.hpp"
#include "chips/resource_manager.hpp"
#include <elib/aux.hpp>

namespace chips
{
    bool draw(sf::RenderWindow & to, entity const & e, position at)
    {
        // Don't draw if the entity is dead
        if (!e) return false;
        
        tile_id tex_id;
        if (e.has<tile_id>())
            tex_id = e.get<tile_id>();
        else
            tex_id = static_cast<tile_id>(e.id());
            
        texture_index index;
        if (e.has<direction>())
        {
            if (e.has<texture_type>())
            {
                index = to_texture_index(
                    tex_id
                  , e.get<direction>()
                  , e.get<texture_type>()
                );
            }
            else
            {
                index = to_texture_index(
                    tex_id
                  , e.get<direction>()
                );
            }
        }
        else
        {
            index = to_texture_index(tex_id);
        }
        
        draw(to, at, index);
        return true;
    }
    
    void draw(sf::RenderWindow & to, position p, texture_index index)
    {
        resource_manager & res_man = resource_manager::get();
        sf::Sprite & s = res_man[index];
        s.setPosition(
            static_cast<float>(p.x)
          , static_cast<float>(p.y)
        );
        to.draw(s);
    }
}                                                           // namespace chips