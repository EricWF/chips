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
		
        sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Chips");
        
        // initialize the tileset
        if (ts_name == "") ts_name = default_tileset_file;
        resource_manager::init(ts_name);
        
        // parse the tileset info
        auto prop_list = parse_tileset(tileset_info_file);
        
        // load the level using the tileset info
        auto l = create_level(lv_name, prop_list);
        game_handler gh(l);

        // initialize the music
        sf::Music music;
		if(!music.openFromFile(CHIPS_RESOURCE_ROOT "poc.wav")) {
		   fprintf(stderr, "Failed to open music: poc.wav\n");
        } else {
            music.setLoop(true);
            music.play();
        }
        
        // Enter the game loop.
        game_event_id last = game_event_id::none;
        while (gh.update(window) != game_event_id::closed)
        {
            if (last != gh.state()) {
                last = gh.state();
                music.stop();
            }
           
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
        std::string tileset_fname, lvl_name;

        opterr = 0;

        while ((c = getopt (argc, argv, "l:mt:")) != -1)
        {
            switch (c)
            {
            case 't':
                if (optarg) {
                    tileset_fname = optarg;
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

                return EXIT_FAILURE;
            }
        }

        if (not level_flag) {
            printf("No level flag given");
            return EXIT_FAILURE;
        }

        run_level(lvl_name, tileset_fname);
        
        return 0;
    }
#if defined(__GNUC__)
# pragma GCC diagnostic pop
#endif
}                                                           // namespace chips
