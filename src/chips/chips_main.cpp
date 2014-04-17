#include "chips/chips_main.hpp"
#include "chips/core.hpp"
#include "chips/logic.hpp"
#include "chips/game_handler.hpp"
#include "chips/draw.hpp"
#include "chips/menu.hpp"
#include "chips/resource_manager.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>
#include <stdlib.h> 
#include <chrono>
#include <unistd.h>

#include "chips/sounds.hpp"

namespace chips { namespace 
{
    
    struct cmdline_opts {
        bool level_flag;
        bool cheats;
        std::string tileset_fname;
        std::string lvl_name;
    };
    
    ////////////////////////////////////////////////////////////////////////////
    cmdline_opts parse_args(int argc, char** argv)
    {
        cmdline_opts opts;
        int c;      
            
        opts.lvl_name = "1";
        opts.level_flag = false;
        opts.cheats = false;
        opterr = 0;
            
        while ((c = getopt (argc, argv, "l:mt:c")) != -1)
        {
            switch (c)
            {
            case 't':
                opts.tileset_fname = optarg ? optarg : "";
                break;
            case 'l':
                opts.level_flag = true;
                opts.lvl_name = optarg ? optarg : "";
                break;
            case 'c':
                opts.cheats = true;
                break;
            case 'm':
                //menu_flag = true;
                break;
            case '?':
            default:
                if (optopt == 'l') {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                    opts.level_flag = false;
                } else if (isprint (optopt)) {
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                } else {
                    fprintf(stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
                            
                    fprintf(stderr, "Usage: ./chips [-l [1 - 4] -m]\n");
                }
            }
        }

        return opts;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    inline void start_music()
    {
        auto & rm = resource_manager::get();
        rm[music_uid::theme].play();
    }
    
    inline void stop_music()
    {
        auto & rm = resource_manager::get();
        rm[music_uid::theme].stop();
    }
    
    ////////////////////////////////////////////////////////////////////////////
    level init_level(cmdline_opts opts)
    {
        // initialize the tileset
        if (opts.tileset_fname == "") opts.tileset_fname = default_tileset_file;
        resource_manager::init(opts.tileset_fname);
            
        // parse the tileset info
        auto prop_list = parse_tileset(tileset_info_file);
            
        // load the level using the tileset info
        auto l = create_level(opts.lvl_name, prop_list);
            
        if (opts.cheats) {
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
        
    ////////////////////////////////////////////////////////////////////////////
    game_event_id run_level(level l, sf::RenderWindow & window)
    {
        game_handler gh(l);

        // Enter the game loop.
        game_event_id last = game_event_id::none;
        while ((last = gh.update(window)) == game_event_id::none)
        {
        }
        
        if(last == game_event_id::level_passed) {
            sf::Event event;
            while(!window.pollEvent(event))
            {
            }
        }
        
        return last;
    }

    ////////////////////////////////////////////////////////////////////////////
    game_event_id run_level_repeat(cmdline_opts const & opts, sf::RenderWindow & window)
    {
        game_event_id id = game_event_id::level_failed;
            
        while(id == game_event_id::level_failed) 
        {
            id = run_level(init_level(opts), window);
        }
        
        return id;
    }
    
	
    
    
    ////////////////////////////////////////////////////////////////////////////
    int run_from_command_line(cmdline_opts const & opts, sf::RenderWindow & window)
    {
        auto l = init_level(opts);
        
        start_music();
        
        auto id = run_level(l, window);
        while(id  == game_event_id::level_failed)
                id = run_level(l, window);
                
        stop_music();
        return EXIT_SUCCESS;
    }
    
    ////////////////////////////////////////////////////////////////////////////
    bool run_menu(sf::RenderWindow & window)
    {
        resource_manager::init(default_tileset_file);
        menu_handler mh;

        auto blist = parse_menu(CHIPS_RESOURCE_ROOT "main_menu.xml");
        mh.set_menu(window, blist);
            
        while (window.isOpen())
        {
            window.clear(sf::Color::Black);
                
            auto bid = mh.handle_event(window);
            if (bid == menu_item_id::quit)
            return false;

            if (bid == menu_item_id::begin)
            return true;
                    
            mh.draw(window);
            window.display();
        }

        return false;
    }
    
    ///////////////////////////////////////////////////////////////////////
    int run_from_menu(cmdline_opts const & opts, sf::RenderWindow & window)
    {
        if (!run_menu(window)) return EXIT_SUCCESS;
            
        start_music();
            
        for(int i = 1 ; i < 8 ; i++) 
        {
            cmdline_opts lvl_opts = opts;
            lvl_opts.lvl_name = std::to_string(i);
            if (run_level_repeat(lvl_opts, window) == game_event_id::closed)
                break;
        }
        
        stop_music();
        return EXIT_SUCCESS;
    }
  
}}                                                          // namespace chips
namespace chips
{
        
    int chips_main(int argc, char** argv, char**)
    {
        sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Chips");		
        auto opts = parse_args(argc, argv);
        
        if (opts.level_flag) {
            return run_from_command_line(opts, window);
        } else {
            return run_from_menu(opts, window);
        }
    }
        
}                                                           // namespace chips
