#include "chips/renderer.hpp"
#include "chips/chips_error.hpp"

namespace chips
{
    renderer & renderer::init(window & win)
    {
        return renderer::m_instance(&win);
    }
    
    renderer & renderer::instance()
    {
        return renderer::m_instance();
    }
    
    renderer & renderer::m_instance(window *win_ptr)
    {
        static renderer *rend_ptr = nullptr;
        if (win_ptr)
        {
            ELIB_ASSERT(!rend_ptr);
            static renderer rend(*win_ptr);
            rend_ptr = &rend;
        }
        else
        {
            ELIB_ASSERT(rend_ptr);
        }
        return *rend_ptr;
    }
    
    renderer::renderer(window & win)
      : m_renderer{nullptr}
    {
        m_renderer = SDL_CreateRenderer(win, -1, 0);
        if (!m_renderer)
            throw_and_clear_sdl_error("Rendeder failed to create with error %s");
    }
    
    renderer::~renderer() noexcept
    {
        SDL_DestroyRenderer(m_renderer);
    }
}                                                           // namespace chips