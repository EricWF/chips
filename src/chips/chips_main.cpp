#include "chips/chips_main.hpp"
#include "chips/error.hpp"
#include "chips/resource_manager.hpp"
#include "chips/game.hpp"
#include <elib/aux.hpp>

namespace chips
{
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
        
        resource_manager & rh = resource_manager::get();
        rh.init_all(SDL_INIT_EVERYTHING, info, -1, 0);

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
