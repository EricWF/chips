#include "chips/chips_main.hpp"
#include "chips/core.hpp"
#include "chips/logic.hpp"
#include "chips/game_handler.hpp"
#include "chips/draw.hpp"
#include "chips/menu.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>
#include <stdlib.h> 
#include <chrono>
#include <unistd.h>

namespace chips
{ 
    void run_level(std::string const & lv_name, std::string ts_name);
    void run_level(std::string const & lv_name, std::string ts_name)
    {
		sf::Music music;
        sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Chips");
        
        if (ts_name == "") ts_name = "pirate_tileset.png";
        resource_manager::init(ts_name);
        
        auto prop_list = parse_tileset(CHIPS_RESOURCE_ROOT "/default_tileset.tsx");
        auto l = create_level(lv_name, prop_list);
        
        game_handler gh(l);

		if(!music.openFromFile(CHIPS_RESOURCE_ROOT "poc.wav"))
		   fprintf(stderr, "Failed to open music: poc.wav\n");
			
		   music.setLoop(true);
		   music.play();
    
        while (gh.update(window) != game_event_id::closed)
        {
            window.clear(sf::Color::Black);
            gh.draw(window);
            window.display();
        }
        window.close();
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
	
#if defined(__GNUC__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wswitch-default"
#endif
    int chips_main(int argc, char** argv, char**)
    {        
		bool level_flag = false;//, menu_flag = false;
		int c;
		std::string tileset_name, lvl_name;
		
		opterr = 0;

		while ((c = getopt (argc, argv, "l:mt:")) != -1)
		{
			switch (c)
			{
			case 't':
                if (optarg) {
                    tileset_name = optarg;
                }
				break;
            case 'l':
				level_flag = true;
				if (optarg) {
                    lvl_name = optarg;
                }
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
		
		if (not level_flag) {
            printf("SHIT NO LEVEL\n");
            return 1;
        }
		
		run_level(lvl_name, tileset_name);
		
        return 0;
    }
#if defined(__GNUC__)
# pragma GCC diagnostic pop
#endif
}                                                           // namespace chips
