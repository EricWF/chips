#include "chips/menu/menu_handler.hpp"
#include "chips/error.hpp"
#include "chips/log.hpp"
#include <elib/config.hpp>

namespace chips
{
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
                    return m_handle_mouse_click(e);
                case sf::Event::MouseMoved:
                    m_handle_mouse_move(e);
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
    }
    
    menu_item_id menu_handler::m_handle_mouse_click(sf::Event const & e)
    {
        for (auto & b : m_buttons)
        {
            if (b.getGlobalBounds().contains(
                static_cast<float>(e.mouseButton.x)
              , static_cast<float>(e.mouseButton.y)
            ))
                return b.id();
        }
        return menu_item_id::none;
    }
    
    void menu_handler::m_handle_mouse_move(sf::Event const & e)
    {
        for (auto & b : m_buttons)
        {
            b.setHovered(
                b.getGlobalBounds().contains(
                    static_cast<float>(e.mouseMove.x)
                  , static_cast<float>(e.mouseMove.y)
            ));
        }
    }
    
}                                                           // namespace chips