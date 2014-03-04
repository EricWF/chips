#ifndef CHIPS_TEXTURE_MANAGER_HPP
#define CHIPS_TEXTURE_MANAGER_HPP

# include "chips/texture_index.hpp"
# include <SFML/Graphics.hpp>
# include <map>

namespace chips
{
    enum class texture_uid
    {
        none, tiles, menu
    };
    
    class texture_manager
    {
    public:
        static texture_manager & get();
        
        sf::Texture const & operator[](texture_uid) const;
        sf::Sprite & operator[](texture_index index);
        
    private:
        texture_manager();
        ~texture_manager() = default;
        
        void create_sprite(texture_index index);
        
        std::map<texture_uid, sf::Texture> m_tex_map;
        std::map<texture_index, sf::Sprite> m_sprite_map;
    };
}                                                           // namespace chips
#endif /* CHIPS_TEXTURE_MANAGER_HPP */