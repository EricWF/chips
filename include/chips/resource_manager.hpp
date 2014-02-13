#ifndef CHIPS_SDL_MANAGER_HPP
#define CHIPS_SDL_MANAGER_HPP

# include "chips/tiles/fwd.hpp"
# include <string>
# include <map>
# include <SDL2/SDL.h>

namespace chips
{
    
    
    ////////////////////////////////////////////////////////////////////////////
    //
    class resource_manager
    {
    public:
        static resource_manager & get();
        
        void init_all(Uint32 flags, window_info const &
                    , int index, Uint32 rend_flags);
                    
        void release_all();
        
        bool is_init() const noexcept { return m_init; }
        void init(Uint32 flags = SDL_INIT_EVERYTHING);
        void release();
        
        bool has_window() const noexcept { return m_window; }
        SDL_Window & init_window(window_info const &);
        SDL_Window & window() { return *m_window; }
        operator SDL_Window*(){ return m_window; }
        void release_window();
        
        bool has_renderer() const noexcept { return m_renderer; }
        SDL_Renderer & init_renderer(int index, Uint32 flags);
        SDL_Renderer & renderer() { return *m_renderer; }
        operator SDL_Renderer*() { return m_renderer; }
        void release_renderer();
        
        bool has_texture() const noexcept { return m_texture; }
        SDL_Texture & init_texture();
        SDL_Texture & texture() { return *m_texture; }
        operator SDL_Texture*() { return m_texture; }
        void release_texture();
        
        SDL_Rect & operator[](texture_index i) { return m_clip_map[i]; }
        
    private:
        resource_manager() {}
        
        resource_manager(resource_manager const &) = delete;
        resource_manager(resource_manager &&) = delete;
        resource_manager & operator=(resource_manager const &) = delete;
        resource_manager & operator=(resource_manager &&) = delete;
        
        ~resource_manager()
        { release(); }

        bool m_init{false};
        SDL_Window *m_window{nullptr};
        SDL_Renderer *m_renderer{nullptr};
        SDL_Texture *m_texture{nullptr};
        std::map<texture_index, SDL_Rect> m_clip_map;
    };
}                                                           // namespace chips
#endif /* CHIPS_SDL_MANAGER_HPP */