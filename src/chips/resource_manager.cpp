#include "chips/resource_manager.hpp"
#include "chips/error.hpp"

namespace chips
{
    resource_manager & resource_manager::get()
    {
        static resource_manager rm;
        return rm;
    }
    
    void resource_manager::init(Uint32 flags)
    {
        if (is_init()) throw chips_error{ "SDL is already init" };
        
        auto ret = SDL_Init(flags);
        if (ret < 0) throw throw_sdl_error{"SDL failed to init with error: %s"};
        m_init = true;
    }
    
    void resource_manager::release()
    {
        if (!is_init()) return;
        release_window();
        m_init = false;
        SDL_Quit();
    }
    
    SDL_Window & resource_manager::init_window(window_info const & info)
    {
        if (has_window()) throw chips_error{"SDL already has window"};
        m_window = SDL_CreateWindow(info.name, info.xpos, info.ypos
                                  , info.width, info.height, info.flags);
        if (!m_window)
            throw_sdl_error{"SDL failed to create window with error: %s"};
        
        return *m_window;
    }
    
    void resource_manager::release_window()
    {
        if (!has_window()) return;
        release_renderer();
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    
    SDL_Renderer & resource_manager::init_renderer(int index, Uint32 flags)
    {
        if (has_renderer())
            throw chips_error{"SDL already has renderer"};
        if (!has_window())
            throw chips_error{"SDL Does not have window for renderer"};
        
        m_renderer = SDL_CreateRenderer(m_window, index, flags);
        if (!m_renderer)
            throw_sdl_error{"Failed to create renderer with error: %s"}
            
        return *m_renderer;
    }
    
    void resource_manager::release_renderer()
    {
        if (!has_renderer()) return;
        release_texture_map();
        SDL_DestroyRendeder(m_renderer);
        m_renderer = nullptr;
    }
    
    void resource_manager::release_texture_map()
    {
        for (auto & kv :  m_textures)
            SDL_DestroyTexture(kv->second);
        m_textures.clear();
    }
}                                                           // namespace chips