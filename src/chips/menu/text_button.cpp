#include "chips/menu/text_button.hpp"
#include "chips/font_handler.hpp"
#include "chips/error.hpp"
#include <algorithm>
#include <cstddef>

namespace chips
{
    text_button::text_button()
      : m_id(menu_item_id::none), m_text(), m_rect()
      , m_hovered_color(menu_button_hovered_outline_color)
      , m_default_color(menu_button_default_outline_color)
    {
        m_text.setColor(menu_text_color);
        m_text.setCharacterSize(menu_font_size);
        m_text.setFont(font_handler::get()[menu_font_id]);
        
        m_rect.setFillColor(menu_button_fill_color);
        m_rect.setOutlineColor(m_default_color);
        m_rect.setOutlineThickness(menu_button_outline_size);
        m_rect.setSize(sf::Vector2f(menu_button_width, menu_button_height));
        
        updateButtonSizeToFit();
        centerText();
    }
    
    text_button::text_button(menu_item_id xid, std::string const & s)
      : text_button()
    {
        create(s, 0, 0);
        m_id = xid;
    }
    
    text_button::text_button(menu_item_id xid, std::string const & s, int xpos, int ypos)
      : text_button()
    {
        create(s, xpos, ypos);
         m_id = xid;
    }
    
    void text_button::draw(sf::RenderTarget & rt, sf::RenderStates rs) const
    {
        if (!m_show) return;
        rt.draw(m_rect, rs);
        rt.draw(m_text, rs);
    }
    
    void text_button::create(std::string const & s, int xpos, int ypos)
    {
        *this = text_button();
        m_text.setString(s);
        setPosition(static_cast<float>(xpos), static_cast<float>(ypos));
        updateButtonSizeToFit();
        centerText();
        m_show = true;
    }
    
    void text_button::centerText()
    {
        auto rect_b = m_rect.getGlobalBounds();
        auto text_b = m_text.getGlobalBounds();
        
        // amount that characters like g, and j hang below the bounds
        // just a good aproximation.
        float underhanging = m_text.getCharacterSize() / 5.0f;
        
        m_text.setPosition(
            rect_b.left + ((rect_b.width - text_b.width) / 2.0f)
          , rect_b.top  + ((rect_b.height - text_b.height) / 2.0f) - underhanging
        );
    }
    
    void text_button::updateButtonSizeToFit()
    {
        auto text_b = m_text.getGlobalBounds();
        
        sf::Vector2f size{
            text_b.width + menu_button_border * 2
          , text_b.height + menu_button_border * 2
        };
        
        m_rect.setSize(size);
    }
    
    void text_button::updateStringSizeToFit()
    {
        unsigned char_count = static_cast<unsigned>(m_text.getString().getSize());
        
        auto rect_b = m_rect.getGlobalBounds();
        unsigned opt_char_size = std::min( 
            static_cast<unsigned>(rect_b.height) - (menu_button_border * 2)
          , (static_cast<unsigned>(rect_b.width) - (menu_button_border * 2)) / char_count
          );
        
        m_text.setCharacterSize(opt_char_size);
    }
}                                                           // namespace chips