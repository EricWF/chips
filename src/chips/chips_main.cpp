#include "chips/chips_main.hpp"
#include "chips/config.hpp"
#include "chips/draw.hpp"
#include "chips/entity_id.hpp"
#include "chips/entity.hpp"
#include "chips/log.hpp"
#include "chips/error.hpp"
#include "chips/parse.hpp"
#include "chips/texture_manager.hpp"
#include "chips/texture_id.hpp"
#include "chips/texture_index.hpp"
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
    inline void entity_draw_test(sf::RenderWindow & win)
    {
        entity e(entity_id::bug);
        e << texture_id::bug_N << position{0, 0} << direction::N 
          << texture_type::cutout;
        win.clear(sf::Color::Black);
        draw(win, e);
        win.display();
        
    }
    
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
            entity_draw_test(window);
            window.display();
        }
        
        window.close();
        return 0;
    }
  
}                                                           // namespace chips