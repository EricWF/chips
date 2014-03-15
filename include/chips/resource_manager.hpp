#ifndef CHIPS_RESOURCE_MANAGER_HPP
#define CHIPS_RESOURCE_MANAGER_HPP

# include "chips/core.hpp"
# include <SFML/Graphics.hpp>
# include <map>

namespace chips
{
    enum class texture_uid
    {
        none, tiles, menu
    };
    
    enum class font_uid
    {
        none, 
        arial
    };
    
    class resource_manager
    {
    public:
        static resource_manager & get();
        
        // creates font if not found
        const sf::Font & operator[](font_uid);
        // throws if font not found
        const sf::Font & at(font_uid id) const;
        
        sf::Texture const & operator[](texture_uid) const;
        sf::Sprite & operator[](texture_index);
        
    private:
        resource_manager();
        ~resource_manager() = default;
        
        void init_tileset();
        void init_fonts();
        void create_sprite(texture_index index);
        
        std::map<font_uid, sf::Font> m_font_map;
        std::map<texture_uid, sf::Texture> m_tex_map;
        std::map<texture_index, sf::Sprite> m_sprite_map;
    };
}                                                           // namespace chips
#endif /* CHIPS_RESOURCE_MANAGER_HPP */