#include "chips/sdl_system_handler.hpp"
#include "chips/chips_error.hpp"
#include <SDL2/SDL.h>

namespace chips
{
    
#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wexit-time-destructors"
# endif
    void sdl_system_handler::init(Uint32 flags)
    {
        static sdl_system_handler h(flags);
    }
# if defined(__clang__)
#   pragma clang diagnostic pop
# endif
    
    sdl_system_handler::sdl_system_handler(Uint32 flags)
    {
        if (SDL_Init(flags) < 0)
            throw_sdl_error("SDL init failed! with: %s");
    }
    
    sdl_system_handler::~sdl_system_handler()
    {
        SDL_Quit();
    }
}