#include "chips/resource_manager.hpp"
#include "chips/error.hpp"
#include "chips/position.hpp"
#include "chips/tiles/support.hpp"
#include <SDL2/SDL_image.h>

namespace chips
{
#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
    resource_manager & resource_manager::get()
    {
        static resource_manager rm;
        return rm;
    }
#if defined(__clang__)
# pragma clang diagnostic pop
#endif

    void resource_manager::init_all(Uint32 flags, window_info const & info
                                  , int index,  Uint32 rend_flags)
    {
        init(flags);
        init_window(info);
        init_renderer(index, rend_flags);
        init_texture();
    }
    
    void resource_manager::release_all()
    {
        release();
    }
    
    void resource_manager::init(Uint32 flags)
    {
        if (is_init()) throw chips_error{ "SDL is already init" };
        
        auto ret = SDL_Init(flags);
        if (ret < 0) throw_sdl_error("SDL failed to init with error: %s");
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
        m_window = SDL_CreateWindow(info.name.c_str(), info.xpos, info.ypos
                                  , info.width, info.height, info.flags);
        if (!m_window)
            throw_sdl_error("SDL failed to create window with error: %s");
        
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
            throw_sdl_error("Failed to create renderer with error: %s");
            
        return *m_renderer;
    }
    
    void resource_manager::release_renderer()
    {
        if (!has_renderer()) return;
        release_texture();
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    
    SDL_Texture & resource_manager::init_texture()
    {
        if (!has_renderer()) 
            throw chips_error{"Cannot init texture without renderer"};
        if (has_texture())
            throw chips_error{"Tile texture is already init"};
            
        SDL_Surface *tmp = IMG_Load(tile_image_file);
        if (!tmp) throw_sdl_error("Failed to open tile image with error: %s");
        
        m_texture = SDL_CreateTextureFromSurface(m_renderer, tmp);
        SDL_FreeSurface(tmp);
        
        if (!m_texture) 
            throw_sdl_error("Failed to create texture from tiles with error: %s");
            
        for (int i=0; i < num_tiles; ++i)
        {
            texture_index id = static_cast<texture_index>(i);
            SDL_Rect & r = m_clip_map[id];
            auto pos = to_texture_position(id);
            r.x = pos.x;
            r.y = pos.y;
            r.h = tile_height;
            r.w = tile_width;
        }
        
        return *m_texture;
    }
    
    void resource_manager::release_texture()
    {
        if (!has_texture()) return;
        m_clip_map.clear();
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
    
    
}                                                           // namespace chips