#ifndef CHIPS_SDL_MANAGER_HPP
#define CHIPS_SDL_MANAGER_HPP

# include <SDL2/SDL.h>

namespace chips
{
    struct window_info
    {
        std::string name;
        int xpos, ypos, width, height;
        Uint32 flags;
    };
    
    struct renderer_info
    {
        int index;
        Uint32 flags;
    };
    
    class resource_manager
    {
    public:
        static resource_manager & instance();
        
        SDL_Window & init_window(window_info const &);
        SDL_Window & window() const;
        
        SDL_Renderer & init_renderer(renderer_info const &);
        SDL_Renderer & renderer() const;
        
    private:
        resource_manager();
        
        resource_manager(resource_manager const &) = delete;
        resource_manager(resource_manager &&) = delete;
        resource_manager & operator=(resource_manager const &) = delete;
        resource_manager & operator=(resource_manager &&) = delete;
        
        ~resource_manager();
        
        SDL_Window *m_window;
        SDL_Renderer *m_renderer;
    };
}                                                           // namespace chips
#endif /* CHIPS_SDL_MANAGER_HPP */