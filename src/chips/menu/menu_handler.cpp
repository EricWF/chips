#include "chips/menu/menu_handler.hpp"
#include "chips/core.hpp"
#include "chips/draw.hpp"
#include <elib/config.hpp>

namespace chips
{

	menu_handler::menu_handler(){
		m_title.setString("Chip's Challenge!");
		m_title.setFont(resource_manager::get()[menu_font_id]);
		m_title.setCharacterSize(100);
		m_title.setColor(sf::Color::Red);
		m_title.setPosition(10, 20);
	}
	
    void menu_handler::set_menu(sf::RenderWindow const & win, std::vector<button_info> const & blist)
    {
	
        m_buttons.clear();
        m_buttons.reserve(blist.size());
        
        for (auto & b :  blist)
        {
            text_button tmp(b.id, b.value);
            align(win, tmp, b.hpos, b.halign, b.vpos, b.valign);
            m_buttons.push_back(tmp);
        }
    }
    
#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wswitch-enum"
#elif defined(ELIB_CONFIG_GCC)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wswitch-enum"
#endif
    menu_item_id 
    menu_handler::handle_event(sf::RenderWindow & win)
    {
        sf::Event e;
        
        while (win.pollEvent(e))
        {
            switch (e.type)
            {
                case sf::Event::Closed:
                    return menu_item_id::quit;
                case sf::Event::MouseButtonReleased:
                    return m_handle_mouse_click(win, e);
                case sf::Event::MouseMoved:
                    m_handle_mouse_move(win, e);
                    break;
                default: break;
            }
        }
        return menu_item_id::none;
    }
#if defined(__clang__)
# pragma clang diagnostic pop
#elif defined(ELIB_CONFIG_GCC)
# pragma GCC diagnostic pop
#endif
    
    void menu_handler::draw(sf::RenderWindow & to)
    {
        for (auto & b : m_buttons)
            to.draw(b);

		to.draw(m_title);
    }
    
    menu_item_id 
    menu_handler::m_handle_mouse_click(
        sf::RenderWindow const & win
      , sf::Event const & e
    )
    {
        for (auto & b : m_buttons)
        {
            if (b.getGlobalBounds().contains(
               win.mapPixelToCoords(sf::Vector2i(e.mouseButton.x, e.mouseButton.y))
            ))
                return b.id();
        }
        return menu_item_id::none;
    }
    
    void 
    menu_handler::m_handle_mouse_move(
        sf::RenderWindow const & win
      , sf::Event const & e
    )
    {
        for (auto & b : m_buttons)
        {
            b.setHovered(
                b.getGlobalBounds().contains(
                    win.mapPixelToCoords(sf::Vector2i(e.mouseMove.x, e.mouseMove.y))
            ));
        }
    }
    
}                                                           // namespace chips
