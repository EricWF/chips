#ifndef CHIPS_RENDERER_HPP
#define CHIPS_RENDERER_HPP

# include "chips/window.hpp"
# include <SDL2/SDL.h>

namespace chips
{
    class renderer
    {
    public:
        static renderer & init(window &);
        static renderer & instance();
        
        renderer(renderer const &) = delete;
        renderer(renderer &&) = delete;
        renderer & operator=(renderer const &) = delete;
        renderer & operator=(renderer &&) = delete;
        
        ~renderer() noexcept;
        
        SDL_Renderer & get() noexcept 
        { return *m_renderer; }
        
        SDL_Renderer const & get() const noexcept 
        { return *m_renderer; } 
        
        operator SDL_Renderer*() noexcept 
        { return m_renderer; }
        
        operator SDL_Renderer const*() const noexcept 
        { return m_renderer; }
        
    private:
        static renderer & m_instance(window* = nullptr);
        
        renderer(window &);
        
        SDL_Renderer *m_renderer;
    };
}                                                           // namespace chips
#endif /* CHIPS_RENDERER_HPP */