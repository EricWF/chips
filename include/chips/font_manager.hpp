#ifndef CHIPS_FONT_MANAGER_HPP
#define CHIPS_FONT_MANAGER_HPP

# include <SFML/Graphics.hpp>
# include <map>
# include <string>

namespace chips
{
    enum class font_id
    {
        none, 
        arial
    };
    
    const char* get_font_id_path(font_id id);
    
    class font_manager
    {
    public:
        static font_manager & get();
        
        font_manager(font_manager const &) = delete;
        font_manager & operator=(font_manager const &) = delete;
        
        // creates font if not found
        const sf::Font & operator[](font_id id);
        // throws if font not found
        const sf::Font & at(font_id id) const;
        
    private:
        font_manager() = default;
        std::map<font_id, sf::Font> m_font_map;
    };
}
#endif /* CHIPS_FONT_MANAGER_HPP */