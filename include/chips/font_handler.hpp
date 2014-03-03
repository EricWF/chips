#ifndef CHIPS_FONT_HANDLER_HPP
#define CHIPS_FONT_HANDLER_HPP

# include <string>
# include <map>
# include <SFML/Graphics.hpp>

namespace chips
{
    enum class font_id
    {
        none, 
        arial
    };
    
    const char* get_font_id_path(font_id id);
    
    class font_handler
    {
    public:
        static font_handler & get();
        
        font_handler(font_handler const &) = delete;
        font_handler & operator=(font_handler const &) = delete;
        
        // creates font if not found
        const sf::Font & operator[](font_id id);
        // throws if font not found
        const sf::Font & at(font_id id) const;
        
    private:
        font_handler() = default;
        std::map<font_id, sf::Font> m_font_map;
    };
}
#endif /* CHIPS_FONT_HANDLER_HPP */