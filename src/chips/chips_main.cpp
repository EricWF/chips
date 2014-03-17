#include "chips/chips_main.hpp"
#include "chips/core.hpp"
#include "chips/draw.hpp"
#include "chips/entity.hpp"
#include "chips/game.hpp"
#include "chips/resource_manager.hpp"
#include "chips/menu.hpp"
#include <elib/aux.hpp>
#include <elib/enumeration.hpp>

#include <SFML/Graphics.hpp>

#include <string>

namespace chips
{    
    int chips_main(int, char**, char**)
    {        
        log::level(level_e::debug);

        auto prop_list = parse_tileset(CHIPS_RESOURCE_ROOT "tileset.tsx" );
        
		auto l = create_level(1, prop_list);
		
        // initalize resources
        resource_manager::get();
        
        sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Chips");
        
        std::string test = CHIPS_RESOURCE_ROOT "main_menu.xml";
        auto blist = parse_menu(test);
        
        menu_handler mh;
        
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