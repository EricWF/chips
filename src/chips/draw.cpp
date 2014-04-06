#include "chips/draw.hpp"
#include "chips/core.hpp"
#include "chips/entity.hpp"
#include <elib/aux.hpp>

namespace chips
{
////////////////////////////////////////////////////////////////////////////////
//                            TEXTURE_INDEX
////////////////////////////////////////////////////////////////////////////////

    namespace {

        constexpr unsigned texture_index_row(texture_index i) noexcept
        {
            return static_cast<unsigned>(i) % texture_rows;
        }
        
        constexpr unsigned texture_index_col(texture_index i) noexcept
        {
            return static_cast<unsigned>(i) / texture_rows;
        }
        
//         constexpr bool is_tile_texture(texture_index i) noexcept
//         {
//             return texture_index_col(i) <= 6;
//         }
//         
//         constexpr bool is_cutout_texture(texture_index i) noexcept
//         {
//             return texture_index_col(i) >= 7 && texture_index_col(i) <= 9;
//         }
//         
//         constexpr bool is_outline_texture(texture_index i) noexcept
//         {
//             return texture_index_col(i) >= 10 && texture_index_col(i) <= 12;
//         }

        constexpr tile_id directional_tile_id(tile_id id, direction dir)
        {
            return (is_directional_texture(id)
                ? static_cast<tile_id>(
                    static_cast<unsigned>(id) + static_cast<unsigned>(dir)
                )
                : throw chips_error("not a directional texture")
            );
        }
        
        constexpr texture_index to_texture_index(tile_id id) noexcept
        {
            return static_cast<texture_index>(id);
        }
        
        constexpr texture_index to_texture_index(tile_id id, direction dir)
        {
            return static_cast<texture_index>(
                directional_tile_id(id, dir)
            );
        }
        
        constexpr texture_index 
        to_texture_index(tile_id id, direction dir, texture_type t)
        {
            return static_cast<texture_index>(
                static_cast<unsigned>(directional_tile_id(id, dir))
                + (static_cast<unsigned>(t) * 3 * texture_rows)
            );
        }
        
        constexpr position to_texture_position(texture_index i) noexcept
        {
            return position{
                texture_index_col(i) * tile_width
            , texture_index_row(i) * tile_height
            };
        }
        
//         constexpr position to_texture_position(tile_id id) noexcept
//         {
//             return to_texture_position( to_texture_index(id) );
//         }
//         
//         constexpr position to_texture_position(tile_id id, direction dir)
//         {
//             return to_texture_position( to_texture_index(id, dir) );
//         }
//         
//         constexpr position 
//         to_texture_position(tile_id id, direction dir, texture_type t)
//         {
//             return to_texture_position( to_texture_index(id, dir, t) );
//         }  
    }                                                       // namespace 
    
////////////////////////////////////////////////////////////////////////////////
//                           RESOURCE MANAGER
////////////////////////////////////////////////////////////////////////////////
    
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
                    {
                        img.setPixel(
                            x + cutout_start_xpos, y
                          , sf::Color::Transparent);
                    }
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
                default: 
                    ELIB_THROW_EXCEPTION(chips_error("Invalid font_uid"));
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
        if (!img.loadFromFile(CHIPS_RESOURCE_ROOT "/tileset.png"))
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
    
////////////////////////////////////////////////////////////////////////////////
//                               DRAW
////////////////////////////////////////////////////////////////////////////////
    
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
