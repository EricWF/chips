#include "chips/chips_main.hpp"
#include "chips/error.hpp"
#include "chips/sdl_system_handler.hpp"
#include "chips/window.hpp"
#include "chips/renderer.hpp"
#include "chips/game.hpp"
#include <elib/aux.hpp>

namespace chips
{
    constexpr const char window_name[] = "chips";
    constexpr const int window_xpos = 100;
    constexpr const int window_ypos = 100;
    constexpr const int window_width = 640;
    constexpr const int window_height = 480;
    constexpr const Uint32 sdl_flags = 0;
    
    int chips_main(int, char**, char**)
    {
        /* Handle uncaught exceptions and unexpected termination */
        set_terminate_handler();
        
        const window_info info{
            window_name
          , window_xpos, window_ypos
          , window_width, window_height
          , sdl_flags
        };
        
        /* WARNING: The order in which this is done is very important.
         * each of these calls creates a static instance of an object
         * who's job it is to destroy the resources on exit. If this
         * order is changed BAD THINGS will happen */
        sdl_system_handler::init();
        window::init(info);
        renderer::init(window::instance());

        game m_game{};
        while(m_game.running())
        {
            m_game.handleEvents();
            m_game.update();
            m_game.render();
            SDL_Delay(10);
        }

        return 0;
    }
}                                                           // namespace chips
