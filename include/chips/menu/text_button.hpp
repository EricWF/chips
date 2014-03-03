#ifndef CHIPS_MENU_TEXT_BUTTON_HPP
#define CHIPS_MENU_TEXT_BUTTON_HPP

# include "chips/menu/core.hpp"
# include <string>
# include <vector>
# include <SFML/Graphics.hpp>

namespace chips
{
    
    class text_button final : public sf::Drawable
    {
    public:
        text_button();
        
        text_button(menu_item_id id, std::string const & s);
        
        text_button(menu_item_id id, std::string const & s, int xpos, int ypos);
        
        text_button(text_button const &) = default;
        text_button & operator=(text_button const &) = default;
        
        ~text_button() = default;
        
        menu_item_id id() const noexcept { return m_id; }
        void id(menu_item_id nid) noexcept { m_id = nid; }
        
        void draw(sf::RenderTarget & rt,  sf::RenderStates) const;
        
        void create(std::string const & s, int xpos, int ypos);
        
        void setShow(bool v = true) noexcept { m_show = v; }
        bool getShow() const noexcept { return m_show; }
        
        void setHovered(bool v = true) noexcept 
        { 
            m_hovered = v;
            m_rect.setOutlineColor(m_hovered ? m_hovered_color : m_default_color);
        }
        
        bool getHovered() const noexcept { return m_hovered; }
        
        void setHoveredOutlineColor(sf::Color const & c) { m_hovered_color = c; }
        sf::Color const & getHoveredOutlineColor() const noexcept { return m_hovered_color; }
        
        void setDefaultOutlineColor(sf::Color const & c) { m_default_color = c; }
        sf::Color const & getDefaultOutlineColor() const noexcept { return m_default_color; }
        
        ////////////////////////////////////////////////////////////////////////
        // sf::Text
        void setString(std::string const & s) { m_text.setString(s); }
        sf::String const & getString() const { return m_text.getString(); }
        
        void setCharacterSize(unsigned v) { m_text.setCharacterSize(v); }
        unsigned getCharacterSize() const { return m_text.getCharacterSize(); }
        
        void setFont(sf::Font const & f) { m_text.setFont(f); }
        sf::Font const * getFont() const { return m_text.getFont(); }
        
        void setStyle(sf::Uint32 s) { m_text.setStyle(s); }
        sf::Uint32 getStyle() const { return m_text.getStyle(); }
        
        void setTextColor(sf::Color const & c) { m_text.setColor(c); }
        sf::Color const & getTextColor() const { return m_text.getColor(); }
        
        void centerText();
        void updateButtonSizeToFit();
        void updateStringSizeToFit();
        
        ////////////////////////////////////////////////////////////////////////
        // sf::RectangleShape
        void setSize(sf::Vector2f const &); /* TODO */
        sf::Vector2f const & getSize() const { return m_rect.getSize(); }
        
        void setTexture(sf::Texture const* t, bool resetRect = false) 
        { m_rect.setTexture(t, resetRect); }
        sf::Texture const * getTexture() const { return m_rect.getTexture(); }
        
        void setTextureRect(sf::IntRect const & r) { m_rect.setTextureRect(r); }
        sf::IntRect const & getTextureRect() const { return m_rect.getTextureRect(); }
        
        void setFillColor(sf::Color const & c) { m_rect.setFillColor(c); }
        sf::Color const & getFillColor() const { return m_rect.getFillColor(); }
        
        void setOutlineThickness(float t) { m_rect.setOutlineThickness(t); }
        float getOutlineThickness() const { return m_rect.getOutlineThickness(); }
        
        sf::FloatRect getLocalBounds() const { return m_rect.getLocalBounds(); }
        sf::FloatRect getGlobalBounds() const { return m_rect.getGlobalBounds(); }
        
        ////////////////////////////////////////////////////////////////////////
        // Transformations
        void setPosition(float x, float y)
        {
            m_rect.setPosition(x, y);
            centerText();
        }
        
        void setPosition(sf::Vector2f const & v) 
        {
            m_rect.setPosition(v);
            centerText();
        }
        
        void setRotation(float d)
        {
            m_rect.setRotation(d);
            m_text.setRotation(d);
        }
        
        void setScale(float dx, float dy)
        {
            m_rect.setScale(dx, dy);
            m_text.setScale(dx, dy);
        }
        
        void setScale(sf::Vector2f const & v)
        {
            m_rect.setScale(v);
            m_text.setScale(v);
        }
        
        void setOrigin(float x, float y);
        void setOrigin(sf::Vector2f const & v);
        
        sf::Vector2f const & getPosition() const { return m_rect.getPosition(); }
        sf::Vector2f const & getScale()    const { return m_rect.getScale();    }
        sf::Vector2f const & getOrigin()   const { return m_rect.getOrigin();   }
        
        float getRotation() const { return m_rect.getRotation(); }
        
        void move(float dx, float dy)
        {
            m_text.move(dx, dy);
            m_rect.move(dx, dy);
        }
        
        void move(sf::Vector2f const & v) 
        {
            m_text.move(v);
            m_rect.move(v);
        }
        
        void scale(float dx, float dy)
        {
            m_text.scale(dx, dy);
            m_rect.scale(dx, dy);
        }
        
        void scale(sf::Vector2f const & v)
        {
            m_text.scale(v);
            m_rect.scale(v);
        }
        
    private:
        menu_item_id m_id;
        sf::Text m_text;
        sf::RectangleShape m_rect;
        sf::Color m_hovered_color, m_default_color;
        bool m_hovered{false}, m_show{false};
    };
    
}                                                           // namespace chips
#endif /* CHIPS_MENU_TEXT_BUTTON_HPP */