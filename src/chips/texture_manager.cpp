#include "chips/texture_manager.hpp"
#include "chips/config.hpp"
#include "chips/core.hpp"

namespace chips
{
    namespace detail
    {
        inline void mask_tileset(sf::Image & img)
        {
            const unsigned cutout_start_col = 7;
            const unsigned cutout_start_xpos = cutout_start_col * tile_width;
            const unsigned mask_start_col = 10;
            const unsigned mask_start_xpos = mask_start_col * tile_width;
            
            const unsigned mask_width = tile_width * 3;
            const unsigned mask_height = texture_height;
            
            for (unsigned x=0; x < mask_width; ++x)
            {
                for (unsigned y=0; y < mask_height; ++y)
                {
                    sf::Color mask_c = img.getPixel(x + mask_start_xpos, y);
                    if (mask_c == sf::Color::Black)
                        img.setPixel(x + cutout_start_xpos, y, sf::Color::Transparent);
                }
            }
        }
    }                                                       // namespace detail
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
        sf::Image img;
        if (!img.loadFromFile(CHIPS_RESOURCE_ROOT "/tileset.gif"))
            throw "TODO";
        
        detail::mask_tileset(img);
       
        if (!m_tex_map[texture_uid::tiles].loadFromImage(img))
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