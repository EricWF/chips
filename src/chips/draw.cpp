#include "chips/draw.hpp"
#include "chips/core.hpp"
#include "chips/entity.hpp"
#include <elib/aux.hpp>
#include <elib/fmt.hpp>

namespace chips
{
////////////////////////////////////////////////////////////////////////////////
//                            TEXTURE_INDEX
////////////////////////////////////////////////////////////////////////////////

    namespace 
    {
        ////////////////////////////////////////////////////////////////////////
        constexpr unsigned texture_index_row(tile_id i) noexcept
        {
            return static_cast<unsigned>(i) % texture_rows;
        }
        
        ////////////////////////////////////////////////////////////////////////
        constexpr unsigned texture_index_col(tile_id i) noexcept
        {
            return static_cast<unsigned>(i) / texture_rows;
        }

        ////////////////////////////////////////////////////////////////////////
        constexpr position to_texture_position(tile_id i) noexcept
        {
            return position{
                texture_index_col(i) * tile_width
              , texture_index_row(i) * tile_height
            };
        }
    }                                                       // namespace 
    
////////////////////////////////////////////////////////////////////////////////
//                           RESOURCE MANAGER
////////////////////////////////////////////////////////////////////////////////
        
    namespace detail { namespace
    {
#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wcovered-switch-default"
#endif
        const char* get_font_uid_path(font_uid id)
        {
            switch (id)
            {
                case font_uid::arial: return CHIPS_RESOURCE_ROOT "/Treamd.ttf";
                case font_uid::none:
                default: ELIB_THROW_EXCEPTION(chips_error("Invalid font_uid"));
            }
        }

      const char* get_sound_uid_path(sound_uid id)
      {
        switch (id)
          {
          case sound_uid::bug_die: 
            return CHIPS_RESOURCE_ROOT "sounds/bug.wav";        
          case sound_uid::teeth_die: 
            return CHIPS_RESOURCE_ROOT "sounds/teeth.wav";
          case sound_uid::chip_die: 
            return CHIPS_RESOURCE_ROOT "sounds/chip_death.wav";
          case sound_uid::generic_die:
            return CHIPS_RESOURCE_ROOT "sounds/generic_death.wav";
          case sound_uid::add_inventory:
            return CHIPS_RESOURCE_ROOT "sounds/add_inventory.wav";
          case sound_uid::pickup_chip:
            return CHIPS_RESOURCE_ROOT "sounds/pickup_chip.wav";
          case sound_uid::pistol:
            return CHIPS_RESOURCE_ROOT "sounds/pistol.wav";        
          case sound_uid::none:
          default:
            ELIB_THROW_EXCEPTION(chips_error("Invalid sound_uid"));
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
    resource_manager & resource_manager::get_impl(const char* opt_str)
    {
        static resource_manager tm(opt_str);
        return tm;
    }
#if defined(__clang__)
# pragma clang diagnostic pop
#endif

    ////////////////////////////////////////////////////////////////////////////
    resource_manager & resource_manager::init(std::string const & s)
    {
        return resource_manager::get_impl(s.c_str());
    }
    
    ////////////////////////////////////////////////////////////////////////////
    resource_manager & resource_manager::get()
    {
        return resource_manager::get_impl();
    }
    
    ////////////////////////////////////////////////////////////////////////////
    resource_manager::resource_manager(const char *tileset_name)
    {
        ELIB_ASSERT(tileset_name);
        init_tileset(tileset_name);
        init_scoreboard();
       
    }

  sf::Sound & resource_manager::operator[](sound_uid id)
  {
    auto found = m_sound_map.find(id);
    if (found != m_sound_map.end())
      return found->second;
    
    sf::SoundBuffer sb;
    sf::Sound sound;
    
    if (!sb.loadFromFile(detail::get_sound_uid_path(id)))
      throw "TODO";
    
    m_sb_map.insert(std::make_pair(id, sb));
    sound.setBuffer(sb);
    auto pos = m_sound_map.insert(std::make_pair(id, sound));
       
    return pos.first->second;
  }

    void resource_manager::init_scoreboard()
    {
         sf::Image scoreboard_img;
        if (not scoreboard_img.loadFromFile(CHIPS_RESOURCE_ROOT "/scoreboard.png"))
            throw "TODO";
            
        sf::Texture scoreboard_tex;
        if (not scoreboard_tex.loadFromImage(scoreboard_img))
            throw "TODO";
            
        m_tex_map[texture_uid::scoreboard] = scoreboard_tex;
        
        m_scoreboard.setTexture(m_tex_map[texture_uid::scoreboard]);
        
        m_scoreboard.setPosition(
            sf::Vector2f((float)scoreboard_xpos, (float)scoreboard_ypos)
          );
    }
    
    ////////////////////////////////////////////////////////////////////////////
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
    
    ////////////////////////////////////////////////////////////////////////////
    const sf::Font & resource_manager::at(font_uid id) const
    {
        auto found = m_font_map.find(id);
        if (found == m_font_map.end())
            throw chips_error{"Failed to find font!"};
        return found->second;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    sf::Texture const & resource_manager::operator[](texture_uid id) const
    {
        return m_tex_map.at(id);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    void resource_manager::init_tileset(const char *tileset_name)
    {
        std::string fname = elib::fmt(CHIPS_RESOURCE_ROOT "/%s", tileset_name);
        sf::Image img;
        if (!img.loadFromFile(fname.c_str()))
            throw "TODO";
       
        if (!m_tex_map[texture_uid::tiles].loadFromImage(img))
            throw "TODO";
    }
    
    ////////////////////////////////////////////////////////////////////////////
    void resource_manager::create_sprite(tile_id tid)
    {
        position p = to_texture_position(tid);
        
        sf::IntRect tex_rect(
            static_cast<int>(p.x)
          , static_cast<int>(p.y)
          , tile_width, tile_height
        );
        
        m_sprite_map[tid] = sf::Sprite(m_tex_map[texture_uid::tiles], tex_rect);
    }
    
    ////////////////////////////////////////////////////////////////////////////
    sf::Sprite & resource_manager::operator[](tile_id index)
    {
        if (m_sprite_map.count(index) == 0) create_sprite(index);
        return m_sprite_map.at(index);
    }
      
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
