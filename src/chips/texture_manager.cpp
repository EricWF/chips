#include "chips/texture_manager.hpp"
#include "chips/config.hpp"

namespace chips
{
#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
    texture_manager & texture_manager::get()
    {
        static texture_manager tm;
        return tm;
    }
#if defined(__clang__)
# pragma clang diagnostic pop
#endif
    
    texture_manager::texture_manager()
    {
        if (!m_tex_map[texture_uid::tiles].loadFromFile(
                CHIPS_RESOURCE_ROOT "/tileset.gif"))
            throw "TODO";
    }
    
    sf::Texture const & texture_manager::operator[](texture_uid id) const
    {
        return m_tex_map.at(id);
    }
    
    sf::Sprite & texture_manager::operator[](texture_index index)
    {
        if (m_sprite_map.count(index) == 0) create_sprite(index);
        return m_sprite_map.at(index);
    }
    
    void texture_manager::create_sprite(texture_index index)
    {
        position p = to_texture_position(index);
        sf::IntRect tex_rect(p.x, p.y, tile_width, tile_height);
        m_sprite_map[index] = sf::Sprite(m_tex_map[texture_uid::tiles], tex_rect);
    }
}                                                           // namespace chips