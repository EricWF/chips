#ifndef CHIPS_MENU_MENU_HANDLER_HPP
#define CHIPS_MENU_MENU_HANDLER_HPP

# include "chips/menu/core.hpp"
# include "chips/menu/text_button.hpp"
# include <elib/aux.hpp>
# include <SFML/Graphics.hpp>

# include <vector>

namespace chips
{
    class menu_handler
    {
    public:
        //ELIB_DEFAULT_CLASS(menu_handler);
		menu_handler();
        
        void set_menu(sf::RenderWindow const &, std::vector<button_info> const &);
        menu_item_id handle_event(sf::RenderWindow &);
        void draw(sf::RenderWindow & to);
        
    private:
        menu_item_id m_handle_mouse_click(
            sf::RenderWindow const &
          , sf::Event const &
        );
        
        void m_handle_mouse_move(
            sf::RenderWindow const &
          , sf::Event const &
        );
        
        std::vector<text_button> m_buttons;
		sf::Text m_title;
    };
}                                                           // namespace chips
#endif /* CHIPS_MENU_MENU_HANDLER_HPP */
