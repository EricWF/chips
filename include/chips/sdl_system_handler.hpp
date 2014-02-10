#ifndef CHIPS_SDL_SYSTEM_HANDLER_HPP
#define CHIPS_SDL_SYSTEM_HANDLER_HPP

# include <SDL2/SDL.h>

namespace chips
{
    /* rational: sdl_system_handler is a clever way to ensure that
     * SDL_Init is called only once AND that SDL_Quit() is called only
     * once. It creates a static object on the first call to init().
     */
    class sdl_system_handler
    {
    public:
        static void init(Uint32 flags = SDL_INIT_EVERYTHING);
    private:
        sdl_system_handler(Uint32 flags);
        ~sdl_system_handler();
    };
}                                                           // namespace chips
#endif /* CHIPS_SDL_SYSTEM_HANDLER_HPP */