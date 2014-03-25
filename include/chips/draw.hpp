#ifndef CHIPS_DRAW_HPP
#define CHIPS_DRAW_HPP

# include <SFML/Graphics.hpp>
# include <map>

namespace chips
{
    // forward //
    class entity;
    struct position;
    
////////////////////////////////////////////////////////////////////////////////
//                           RESOURCE MANAGER
////////////////////////////////////////////////////////////////////////////////
    
    enum class texture_index : unsigned {};
    
    enum class texture_uid
    {
        none, tiles, menu
    };
    
    enum class font_uid
    {
        none, arial
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
    
////////////////////////////////////////////////////////////////////////////////
//                               DRAW
////////////////////////////////////////////////////////////////////////////////

    // return true if the entity is drawn
    bool draw(sf::RenderWindow &, entity const &, position at);
    void draw(sf::RenderWindow &, position, texture_index);
}                                                           // namespace chips
#endif /* CHIPS_DRAW_HPP */