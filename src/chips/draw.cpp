#include "chips/draw.hpp"
#include "chips/core.hpp"
#include "chips/entity.hpp"
#include "chips/error.hpp"
#include "chips/texture_manager.hpp"
#include <elib/aux.hpp>

namespace chips
{
    bool draw(sf::RenderWindow & to, entity const & e)
    {
        position p = e.get_attribute<position>();
        
        texture_id tex_id;
        if (e.has_attribute<texture_id>())
            tex_id = e.get_attribute<texture_id>();
        else
            tex_id = static_cast<texture_id>(e.id());
            
        texture_index index;
        if (e.has_attribute<direction>())
        {
            if (e.has_attribute<texture_type>())
            {
                index = to_texture_index(
                    tex_id
                  , e.get_attribute<direction>()
                  , e.get_attribute<texture_type>()
                  );
            }
            else
            {
                index = to_texture_index(
                    tex_id
                  , e.get_attribute<direction>()
                  );
            }
        }
        else
        {
            index = to_texture_index(tex_id);
        }
        
        draw(to, p, index);
        return true;
    }
    
    void draw(sf::RenderWindow & to, position p, texture_index index)
    {
        texture_manager & tex_man = texture_manager::get();
        sf::Sprite & s = tex_man[index];
        s.setPosition(
            static_cast<float>(p.x)
          , static_cast<float>(p.y)
        );
        to.draw(s);
    }
}                                                           // namespace chips