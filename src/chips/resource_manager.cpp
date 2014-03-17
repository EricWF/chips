#include "chips/resource_manager.hpp"
#include "chips/core.hpp"
#include <elib/aux.hpp>
#include <algorithm>
#include <utility>

namespace chips
{
    namespace detail { namespace 
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
        
#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wcovered-switch-default"
#endif
        const char* get_font_uid_path(font_uid id)
        {
            switch (id)
            {
                case font_uid::arial: return CHIPS_RESOURCE_ROOT "/arial.ttf";
                case font_uid::none:
                default: throw chips_error{"Invalid font_uid"};
            }
        }
#if defined(__clang__)
# pragma clang diagnostic pop
#endif

    }}                                                       // namespace detail
    
    
#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
    resource_manager & resource_manager::get()
    {
        static resource_manager tm;
        return tm;
    }
#if defined(__clang__)
# pragma clang diagnostic pop
#endif
    
    resource_manager::resource_manager()
    {
        init_tileset();
    }
    
    sf::Texture const & resource_manager::operator[](texture_uid id) const
    {
        return m_tex_map.at(id);
    }
    
    sf::Sprite & resource_manager::operator[](texture_index index)
    {
        if (m_sprite_map.count(index) == 0) create_sprite(index);
        return m_sprite_map.at(index);
    }
    
     const sf::Font & resource_manager::operator[](font_uid id)
    {
        auto found = m_font_map.find(id);
        if (found != m_font_map.end())
            return found->second;
        
        sf::Font tmp;
        if (!tmp.loadFromFile(detail::get_font_uid_path(id)))
            throw "TODO";
            
        auto pos = m_font_map.insert(std::make_pair(id, tmp));
        
        return pos.first->second;
    }
    
    const sf::Font & resource_manager::at(font_uid id) const
    {
        auto found = m_font_map.find(id);
        if (found == m_font_map.end())
            throw chips_error{"Failed to find font!"};
        return found->second;
    }
    
    void resource_manager::init_tileset()
    {
        sf::Image img;
        if (!img.loadFromFile(CHIPS_RESOURCE_ROOT "/tileset.gif"))
            throw "TODO";
        
        detail::mask_tileset(img);
       
        if (!m_tex_map[texture_uid::tiles].loadFromImage(img))
            throw "TODO";
    }
    
    void resource_manager::create_sprite(texture_index index)
    {
        position p = to_texture_position(index);
        
        sf::IntRect tex_rect(
            static_cast<int>(p.x)
          , static_cast<int>(p.y)
          , tile_width, tile_height
        );
        
        m_sprite_map[index] = sf::Sprite(m_tex_map[texture_uid::tiles], tex_rect);
    }
}                                                           // namespace chips