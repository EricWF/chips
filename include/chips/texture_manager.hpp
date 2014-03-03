#ifndef CHIPS_TEXTURE_MANAGER_HPP
#define CHIPS_TEXTURE_MANAGER_HPP

# include <map>
# include <SFML/Graphics.hpp>

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
        
    private:
        texture_manager();
        ~texture_manager() = default;
        
        std::map<texture_uid, sf::Texture> m_tex_map;
    };
}                                                           // namespace chips
#endif /* CHIPS_TEXTURE_MANAGER_HPP */