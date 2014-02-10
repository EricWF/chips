#ifndef CHIPS_SDL_MANAGER_HPP
#define CHIPS_SDL_MANAGER_HPP

# include "chips/tiles/fwd.hpp"
# include <string>
# include <map>
# include <SDL2/SDL.h>

namespace chips
{

    using texture_map_type = std::map<texture_id, SDL_Texture*>;
    
    struct window_info
    {
        std::string name;
        int xpos, ypos, width, height;
        Uint32 flags;
    };
    
    ////////////////////////////////////////////////////////////////////////////
    //
    class resource_manager
    {
    public:
        static resource_manager & get();
        
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
        
        texture_map_type & texture_map() noexcept { return m_textures; }
        void release_texture_map();
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
        texture_map_type m_textures;
    };
}                                                           // namespace chips
#endif /* CHIPS_SDL_MANAGER_HPP */