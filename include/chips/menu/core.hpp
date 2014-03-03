#ifndef CHIPS_MENU_CORE_HPP
#define CHIPS_MENU_CORE_HPP

# include "chips/config.hpp"
# include "chips/font_manager.hpp"
# include <elib/enumeration.hpp>
# include <elib/fmt.hpp>
# include <SFML/Graphics.hpp>
# include <map>
# include <string>

namespace chips
{
    constexpr const int menu_button_width = 200;
    constexpr const int menu_button_height = 75;
    constexpr const int menu_button_border = 10;
    constexpr const int menu_button_outline_size = 3;
    
    constexpr const font_id menu_font_id = font_id::arial;
    constexpr const unsigned menu_font_size = 40;
    
# if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wglobal-constructors"
# endif
    static const sf::Color menu_background_color = sf::Color::Black;
    static const sf::Color menu_text_color = sf::Color::White;
    static const sf::Color menu_button_fill_color = sf::Color::Red;
    static const sf::Color menu_button_hovered_outline_color = sf::Color::Blue;
    static const sf::Color menu_button_default_outline_color = sf::Color::Yellow;
# if defined(__clang__)
#   pragma clang diagnostic pop
# endif
    
    enum class menu_item_id
    {
         none, begin, levels, highscore, quit
    };
    
    enum class align_h
    {
        left, center, right
    };
    
    enum class align_v
    {
        top, center, bottom
    };
}                                                           // namespace chips

namespace elib { namespace enumeration
{
    template <>
    struct basic_enum_traits<::chips::menu_item_id>
    {
        static const std::map<::chips::menu_item_id, std::string> name_map;
    };
    
      template <>
    struct basic_enum_traits<::chips::align_h>
    {
        static const std::map<::chips::align_h, std::string> name_map;
    };
    
    template <>
    struct basic_enum_traits<::chips::align_v>
    {
        static const std::map<::chips::align_v, std::string> name_map;
    };
}}                                               // namespace elib::enumeration

namespace chips
{
    struct button_info
    {
        menu_item_id id;
        std::string value;
        
        float hpos;
        align_h halign;
        
        float vpos;
        align_v valign;
    };
    
    inline std::string to_string(button_info const & i)
    {
        return elib::fmt(
            "button_info: id=%s hpos=%d vpos=%d halign=%s valign=%s"
          , elib::enumeration::enum_cast<std::string>(i.id)
          , i.hpos, i.vpos
          , elib::enumeration::enum_cast<std::string>(i.halign)
          , elib::enumeration::enum_cast<std::string>(i.valign)
        );
    }
    
    template <class Widget>
    void align_x(sf::RenderWindow const & win, Widget & widget, float xpercent
              , align_h with = align_h::left)
    {
        sf::Vector2u size = win.getSize();
        sf::Vector2f widget_size = widget.getSize();
        
        sf::Vector2f pos;
        pos.x = widget.getPosition().x;
        if (with == align_h::left)
            pos.x = size.x * xpercent;
        else if (with == align_h::center)
            pos.x = (size.x * xpercent) - (widget_size.x / 2.0f);
        else
            pos.x = (size.x * xpercent) - widget_size.x;
        widget.setPosition(pos);
    }
    
    template <class Widget>
    void align_y(sf::RenderWindow const & win, Widget & widget, float xpercent
                , align_v with = align_v::top)
    {
        sf::Vector2u size = win.getSize();
        sf::Vector2f widget_size = widget.getSize();
        
        sf::Vector2f pos;
        pos.x = widget.getPosition().x;
        if (with == align_v::top)
            pos.y = size.y * xpercent;
        else if (with == align_v::center)
            pos.y = (size.y * xpercent) - (widget_size.y / 2.0f);
        else
            pos.y = (size.y * xpercent) - widget_size.y;
            
        widget.setPosition(pos);
    }
    
    template <class Widget>
    void align(sf::RenderWindow const & win, Widget & widget
                , float percentx, align_h hor
                , float percenty, align_v ver)
    {
       align_x(win, widget, percentx, hor);
       align_y(win, widget, percenty, ver);
    }
    
}                                                           // namespace chips
#endif /* CHIPS_MENU_CORE_HPP */