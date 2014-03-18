#include "chips/chips_main.hpp"
#include "chips/core.hpp"
#include "chips/game.hpp"
#include "chips/resource_manager.hpp"
#include "chips/menu.hpp"
#include <SFML/Graphics.hpp>
#include <stdlib.h> 
#include <unistd.h>

namespace chips
{ 
	void level_test(const unsigned lvl_pick)
	{
		log::level(level_e::debug);
		auto prop_list = parse_tileset(CHIPS_RESOURCE_ROOT "tileset.tsx" );
        auto l = create_level(lvl_pick, prop_list);
	}

	void menu_test()
	{
		sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Chips");
		resource_manager::get();


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
		
	}

    int chips_main(int argc, char** argv, char**)
    {        
		bool level_flag = false;//, menu_flag = false;
		int lvl_arg = 1, c;
		opterr = 0;

		while ((c = getopt (argc, argv, "l:m")) != -1)
		{
			switch (c)
			{
            case 'l':
				level_flag = true;
				lvl_arg = atoi(optarg);
				break;
			case 'm':
				//menu_flag = true;
				break;
			case '?':
				if (optopt == 'l')
				{
					fprintf(stderr, "Option -%c requires an argument.\n", optopt);
					level_flag = false;
				}
				else if (isprint (optopt))
					fprintf(stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf(stderr,
							"Unknown option character `\\x%x'.\n",
							optopt);

				fprintf(stderr, "Usage: ./chips [-l [1 - 4] -m]\n");

				exit(EXIT_FAILURE);
			}
		}

		if(level_flag)
			level_test((unsigned)lvl_arg);
		else
			menu_test();
		
        return 0;
    }
  
}                                                           // namespace chips
