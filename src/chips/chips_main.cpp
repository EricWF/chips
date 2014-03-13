#include "chips/chips_main.hpp"
#include "chips/config.hpp"
#include "chips/draw.hpp"
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
    inline void draw_tiles_test(sf::RenderWindow & win)
    {
        texture_manager & man = texture_manager::get();
        sf::Sprite s(man[texture_uid::tiles]);
        win.clear(sf::Color::Black);
        win.draw(s);
        win.display();
    }
    
    inline void entity_draw_test(sf::RenderWindow & win)
    {
        entity e(entity_id::bug);
        e << position{0, 0} << direction::N << texture_type::cutout;
          
        win.clear(sf::Color::Black);
        draw(win, e);
        win.display();
        
    }
    
    int chips_main(int, char**, char**)
    {        
        log::level(level_e::debug);

        auto prop_list = parse_tileset(CHIPS_RESOURCE_ROOT "tileset.tsx" );
        
        
		std::string level_file = CHIPS_RESOURCE_ROOT "level1.tmx";
		auto l = parse_level(level_file);
		
        
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
            //entity_draw_test(window);
            window.display();
        }
        
        window.close();
        return 0;
    }
  
}                                                           // namespace chips
