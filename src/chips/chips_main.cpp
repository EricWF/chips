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

#include "chips/sounds.hpp"

namespace chips
{
	struct cmdline_opts {
		bool level_flag;
        std::string tileset_fname;
		std::string lvl_name;
	};
	
    namespace { bool cheats = false; }

	level init_level(std::string const & lv_name, std::string ts_name);
	level init_level(std::string const & lv_name, std::string ts_name)
	{
		// initialize the tileset
        if (ts_name == "") ts_name = default_tileset_file;
        resource_manager::init(ts_name);
        
        // parse the tileset info
        auto prop_list = parse_tileset(tileset_info_file);
        
        // load the level using the tileset info
        auto l = create_level(lv_name, prop_list);
        
        if (cheats) {
            auto & inv = l.chip.get<inventory>();
            inv.add_item(entity_id::computer_chip, 100);
            inv.add_item(entity_id::red_key, 100);
            inv.add_item(entity_id::blue_key, 100);
            inv.add_item(entity_id::green_key, 100);
            inv.add_item(entity_id::yellow_key, 100);
            inv.add_item(entity_id::flippers);
            inv.add_item(entity_id::skates);
            inv.add_item(entity_id::fire_boots);
            inv.add_item(entity_id::suction_boots);
        }

		return l;
	}
	
	game_event_id run_level(level l, sf::RenderWindow & window);
    game_event_id run_level(level l, sf::RenderWindow & window)
    {
        
        game_handler gh(l);
		
        // // initialize the music
        // sf::Music music;
		// if(!music.openFromFile(CHIPS_RESOURCE_ROOT "sounds/poc.wav")) {
		// 	fprintf(stderr, "Failed to open music: poc.wav\n");
        // } else {
        //     music.setLoop(true);
        //     //music.play();
        // }
        
        // Enter the game loop.
        game_event_id last = game_event_id::none;
        while ( (last = gh.update(window)) == game_event_id::none ); // {}
		

		return last;

		// if(last == game_even_id::level_failed)
		// 	run_level(lv_name, ts_name, window);

		
		// if (last != gh.state()) {
		// 		last = gh.state();
		// 		music.stop();
		// }
		
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
	
	struct cmdline_opts parse_args(int argc, char** argv);
	struct cmdline_opts parse_args(int argc, char** argv)
	{
		struct cmdline_opts opts;
        int c;		
		
		opts.level_flag = false;
        opterr = 0;
		
        while ((c = getopt (argc, argv, "l:mt:c")) != -1)
        {
            switch (c)
            {
            case 't':
                if (optarg) {
                    opts.tileset_fname = optarg;
                }
                break;
            case 'l':
                opts.level_flag = true;
                if (optarg) {
                    opts.lvl_name = optarg;
                }
                break;
            case 'c':
                cheats = true;
                break;
            case 'm':
                //menu_flag = true;
                break;
            case '?':
			default:
                if (optopt == 'l'){
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                    opts.level_flag = false;
                } else if (isprint (optopt)) {
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
				} else {
                    fprintf(stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
				
					fprintf(stderr, "Usage: ./chips [-l [1 - 4] -m]\n");
					
					exit(EXIT_FAILURE);
				}
			}
		}

		return opts;
		
	}

	int chips_main(int argc, char** argv, char**)
	{
		sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Chips");		
		auto opts = parse_args(argc, argv);
		
		if (not opts.level_flag) {
			printf("No level flag given");
			return EXIT_FAILURE;
		}
		
		int i;
		for(i = 2; i < 5; i++)
		{
			auto l = init_level(std::to_string(i), opts.tileset_fname);
			auto id = run_level(l, window);
			
			while(id  == game_event_id::level_failed)
				id = run_level(l, window);

			if(id == game_event_id::closed){
				window.close();
				return EXIT_SUCCESS;
			}
			
		}
		
		return 0;
	}
	

					
// #if defined(__GNUC__)
// # pragma GCC diagnostic push
// # pragma GCC diagnostic ignored "-Wswitch-default"
// #endif	
// #if defined(__GNUC__)
// # pragma GCC diagnostic pop
// #endif
}                                                           // namespace chips
