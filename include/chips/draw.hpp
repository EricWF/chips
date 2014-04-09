#ifndef CHIPS_DRAW_HPP
#define CHIPS_DRAW_HPP

# include <SFML/Graphics.hpp>
# include <map>

namespace chips
{
    // forward //
    class entity;
    struct position;
    enum class tile_id;
    
////////////////////////////////////////////////////////////////////////////////
//                           RESOURCE MANAGER
////////////////////////////////////////////////////////////////////////////////
    
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
    private:
        static resource_manager & get_impl(const char* tileset_str = nullptr);

    public:
        static resource_manager & init(std::string const &);
        static resource_manager & get();
        
        
        // creates font if not found
        const sf::Font & operator[](font_uid);
        // throws if font not found
        const sf::Font & at(font_uid id) const;
        
        sf::Texture const & operator[](texture_uid) const;
        sf::Sprite & operator[](tile_id);
        
    private:
        resource_manager(const char* tileset_name);
        ~resource_manager() = default;
        
        void init_tileset(const char* tileset_name);
        void init_fonts();
        void create_sprite(tile_id);
        
        std::map<font_uid, sf::Font> m_font_map;
        std::map<texture_uid, sf::Texture> m_tex_map;
        std::map<tile_id, sf::Sprite> m_sprite_map;
    };
    
////////////////////////////////////////////////////////////////////////////////
//                               DRAW
////////////////////////////////////////////////////////////////////////////////

    // return true if the entity is drawn
    bool draw(sf::RenderWindow &, entity const &, position at);
    void draw(sf::RenderWindow &, position, tile_id);
}                                                           // namespace chips
#endif /* CHIPS_DRAW_HPP */