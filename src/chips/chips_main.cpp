#include "chips/chips_main.hpp"
#include "chips/config.hpp"
#include "chips/entity.hpp"
#include "chips/log.hpp"
#include "chips/error.hpp"
#include "chips/texture_manager.hpp"
#include "chips/menu/core.hpp"
#include "chips/menu/parse.hpp"
#include "chips/menu/menu_handler.hpp"
#include <elib/aux.hpp>
#include <elib/enumeration.hpp>
#include <elib/eprintf.hpp>

#include <SFML/Graphics.hpp>

#include <string>

namespace chips
{
    
    int chips_main(int, char**, char**)
    {
        /* Handle uncaught exceptions and unexpected termination */
        set_terminate_handler();
        
        log::level(level_e::debug);
        
        texture_manager::get();
        
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