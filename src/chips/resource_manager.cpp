#include "chips/resource_manager.hpp"
#include "chips/core.hpp"
#include <elib/aux.hpp>
#include <elib/fmt.hpp>

namespace chips
{
                   
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
                case sound_uid::bomb:
                    return CHIPS_RESOURCE_ROOT "sounds/bomb.wav";        
                case sound_uid::none:
                default:
                    ELIB_THROW_EXCEPTION(chips_error("Invalid sound_uid"));
            }
        }
      
        const char* get_music_uid_path(music_uid id)
        {
            switch (id)
            {
                case music_uid::theme:
                    return CHIPS_RESOURCE_ROOT "sounds/poc.wav";
                case music_uid::none:
                default:
                    ELIB_THROW_EXCEPTION(chips_error("Invalid music_uid"));
            }
        }

#if defined(__clang__)
# pragma clang diagnostic pop
#endif
    }                                                       // namespace
    
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
        init_scoreboard(tileset_name);
    }

    sf::Sound & resource_manager::operator[](sound_uid id)
    {
        auto found = m_sound_map.find(id);
        if (found != m_sound_map.end())
            return found->second;
        
        sf::SoundBuffer sb;
        sf::Sound sound;
        
        if (!sb.loadFromFile(get_sound_uid_path(id)))
            throw "TODO";
        
        auto buf_pos = m_sb_map.insert(std::make_pair(id, sb));
        auto sound_pos = m_sound_map.insert(std::make_pair(id, sound));

        sound_pos.first->second.setBuffer(buf_pos.first->second);
        
        return sound_pos.first->second;
    }
    
    
    sf::Music & resource_manager::operator[](music_uid id)
    {
        auto music_pos = m_music_map.find(id);
        if (music_pos != m_music_map.end()) return music_pos->second;
            
        auto & music = m_music_map[id];
        if (not music.openFromFile(get_music_uid_path(id))) {
            m_music_map.erase(id);
            ELIB_THROW_EXCEPTION(chips_error(elib::fmt(
                "Failed to get music file %s"
              , get_music_uid_path(id)
            )));
        }
        music.setLoop(true);
        return music;
    }

    void resource_manager::init_scoreboard(const char* tileset_name)
    {
        std::string fname;
        if (tileset_name == std::string{"default_tileset.png"}) {
            fname = CHIPS_RESOURCE_ROOT "/grey-scoreboard.png";
        } else  {
            fname = CHIPS_RESOURCE_ROOT"/scoreboard.png";
        }
        sf::Image scoreboard_img;
        if (not scoreboard_img.loadFromFile(fname.c_str()))
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
        if (!tmp.loadFromFile(get_font_uid_path(id)))
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
}                                                           // namespace chips