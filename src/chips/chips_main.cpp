#include "chips/chips_main.hpp"
#include "chips/core.hpp"
#include "chips/game.hpp"
#include "chips/resource_manager.hpp"
#include "chips/menu.hpp"
#include <SFML/Graphics.hpp>

namespace chips
{    
    int chips_main(int, char**, char**)
    {        
        log::level(level_e::debug);
        
        sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Chips");
        
        // initalize resources
        resource_manager::get();
        
        auto prop_list = parse_tileset(CHIPS_RESOURCE_ROOT "tileset.tsx" );
        auto l = create_level(1, prop_list);
        
        menu_handler mh;
        auto blist = parse_menu(CHIPS_RESOURCE_ROOT "main_menu.xml");
        mh.set_menu(window, blist);
        
        while (window.isOpen())
        {
            window.clear(sf::Color::Black);
            
            auto bid = mh.handle_event(window);
            if (bid == menu_item_id::quit) break;
                
            mh.draw(window);
            window.display();
        }
        
        window.close();
        return 0;
    }
  
}                                                           // namespace chips